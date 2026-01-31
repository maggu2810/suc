// Copyright [2025] [maggu2810]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by maggu2810 on 11/2/25.
//

#include "suc/epl/base/EventQueue.hxx"

#include "suc/epl/base/InputFlags.hxx"
#include <set>
#include <suc/cmn/runtimeerror_errno.hxx>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#if 0
enum class event_type {
    in, /**< The associated file is available for read(2) operations. */
    out, /**< The associated file is available for write(2) operations. */
    rdhub, /**< Stream socket peer closed connection, or shut down writing half of connection.  (This flag is
              especially useful for writing simple code to detect peer shutdown when using edge-triggered
              monitoring.) */
    pri, /**< There is an exceptional condition on the file descriptor.
    Possibilities include:
        - There is out-of-band data on a TCP socket (see tcp(7)).
        - A pseudoterminal master in packet mode has seen a state change on the slave (see ioctl_tty(2)).
        - A cgroup.events file has been modified (see cgroups(7)).
          */
    err, /**< Error condition happened on the associated file descriptor.  This event is also reported for the write
            end of a pipe when the read end has been closed.

    epoll_wait(2) will always report for this event; it is not necessary to set it in events when calling
    epoll_ctl().*/
    hup /**< Hang up happened on the associated file descriptor.

    epoll_wait(2) will always wait for this event; it is not necessary to set it in events when calling epoll_ctl().

    Note  that  when reading from a channel such as a pipe or a stream socket, this event merely indicates
    that the peer closed its end of the channel.  Subsequent reads from the channel will return 0 (end of file) only
    after all outstanding data in the channel has been consumed.
    */
};
#endif


namespace {
    uint32_t calculateEvents(const suc::epl::cb& cb, const std::set<suc::epl::InputFlags> iflags) {
        uint32_t events = //
            (cb.inputAvailable ? EPOLLIN : 0U) | //
            (cb.outputPossible ? EPOLLOUT : 0U) | //
            (cb.readSideHangUp ? EPOLLRDHUP : 0U) | //
            (cb.priorityData ? EPOLLPRI : 0U) | //
            (cb.errorOccurred ? EPOLLERR : 0U) | //
            (cb.hangUp ? EPOLLHUP : 0U) | //
            (iflags.contains(suc::epl::InputFlags::EdgeTriggered) ? EPOLLET : 0U) | //
            (iflags.contains(suc::epl::InputFlags::OneShot) ? EPOLLONESHOT : 0U) | //
            (iflags.contains(suc::epl::InputFlags::WakeUp) ? EPOLLWAKEUP : 0U) | //
            (iflags.contains(suc::epl::InputFlags::Exclusive) ? EPOLLEXCLUSIVE : 0U); //
        return events;
    }
} // namespace

namespace suc::epl {
    EventQueue& EventQueue::coreInstance() {
        static auto instance = create();
        return instance;
    }

    EventQueue EventQueue::create() {
        return EventQueue(Private());
    }

    EventQueue::EventQueue(Private)
        : m_epfd{cmn::fd::make_or_rteeno(epoll_create1(EPOLL_CLOEXEC), "creation of epoll fd failed")},
          m_evtfd(cmn::fd::make_or_rteeno(eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK))),
          m_state{State::Idle} {
        auto [it, inserted] = m_fds.emplace(*m_evtfd, cb{.inputAvailable = [&] -> void {
            // We do no special handling here. We just want to be sure to handle e.g. changed state variable.
        }});
        if (!inserted) {
            throw std::runtime_error("adding event file descriptor failed");
        }
        epoll_event evt{.events = calculateEvents(it->second, {}), .data{.fd = *m_evtfd}};
        if (epoll_ctl(*m_epfd, EPOLL_CTL_ADD, evt.data.fd, &evt) != 0) {
            throw cmn::runtimeerror_errno("epoll_ctl add failed");
        }
    }

    int EventQueue::exec() {
        State expected{State::Idle};
        if (!m_state.compare_exchange_strong(expected, State::Running)) {
            throw std::runtime_error("event queue not in idle state");
        }
        uint64_t cnt;
        read(*m_evtfd, &cnt, sizeof(cnt));

        constexpr int max_events = 20;
        epoll_event eplEvents[max_events];
        while (running()) {
            const int rv = epoll_wait(*m_epfd, eplEvents, std::size(eplEvents), 0);
            if (rv == -1) {
                int errnum = errno;
                m_state.store(State::Error);
                throw cmn::runtimeerror_errno("epoll_wait failed", errnum);
            }
            expected = State::Stopping;
            if (m_state.compare_exchange_strong(expected, State::Stopped)) {
                break;
            }
            for (int i = 0; running() && i < rv; ++i) {
                const auto& [events, data] = eplEvents[i];
                if (const auto it = m_fds.find(data.fd); it != m_fds.end()) {
                    if (running() && events & EPOLLIN && it->second.inputAvailable) {
                        it->second.inputAvailable();
                    }
                    if (running() && events & EPOLLOUT && it->second.outputPossible) {
                        it->second.outputPossible();
                    }
                    if (running() && events & EPOLLRDHUP && it->second.readSideHangUp) {
                        it->second.readSideHangUp();
                    }
                    if (running() && events & EPOLLPRI && it->second.priorityData) {
                        it->second.priorityData();
                    }
                    if (running() && events & EPOLLERR && it->second.errorOccurred) {
                        it->second.errorOccurred();
                    }
                    if (running() && events & EPOLLHUP && it->second.hangUp) {
                        it->second.hangUp();
                    }
                }
            }
        }
        return m_exitCode;
    }

    void EventQueue::stop(int value) {
        State expected{State::Running};
        if (!m_state.compare_exchange_strong(expected, State::Stopping)) {
            return;
        }
        m_exitCode = value;
        uint64_t incr = 1;
        write(*m_evtfd, &incr, sizeof(incr));
    }

    bool EventQueue::running() const {
        return m_state == State::Running;
    }

    void EventQueue::add(int fd, std::function<void(cb&)> reg) {
        if (m_fds.contains(fd)) {
            throw std::runtime_error("file descriptor already added");
        }

        auto [it, inserted] = m_fds.emplace(fd, cb{});
        if (!inserted) {
            throw std::runtime_error("adding file descriptor failed");
        }

        if (reg) {
            reg(it->second);
        }

        epoll_event evt{.events = calculateEvents(it->second, {}), .data{.fd = fd}};
        if (epoll_ctl(*m_epfd, EPOLL_CTL_ADD, evt.data.fd, &evt) != 0) {
            throw cmn::runtimeerror_errno("epoll_ctl add failed");
        }
    }

    void EventQueue::mod(int fd, std::function<void(cb&)> reg) {
        if (!reg) {
            throw std::runtime_error("reg is mandatory");
        }
        if (auto it = m_fds.find(fd); it != m_fds.end()) {
            reg(it->second);
            epoll_event evt{.events = calculateEvents(it->second, {}), .data{.fd = fd}};
            if (epoll_ctl(*m_epfd, EPOLL_CTL_MOD, evt.data.fd, &evt) != 0) {
                throw cmn::runtimeerror_errno("epoll_ctl mod failed");
            }
        } else {
            throw std::runtime_error("unknown file descriptor");
        }
    }

    void EventQueue::del(int fd) {
        if (m_fds.erase(fd) == 0) {
            throw std::runtime_error("unknown fd");
        }
        if (const int rv = epoll_ctl(*m_epfd, EPOLL_CTL_DEL, fd, nullptr); rv != 0) {
            if (errno == EBADF) {
                // see "man 7 epoll" answer to question:
                // "Will closing a file descriptor cause it to be removed from all epoll interest lists?"
                // ATM we require to deregister before closing it.
                throw cmn::runtimeerror_errno("epoll_ctl del failed");
            } else {
                throw cmn::runtimeerror_errno("epoll_ctl del failed");
            }
        }
    }
} // namespace suc::epl
