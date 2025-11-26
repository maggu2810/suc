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

#include "../include/suc/epl/epleq.hxx"

#include <set>
#include <suc/cmn/runtimeerror_errno.hxx>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

namespace {
    uint32_t calc_events(const suc::epl::cb& cb, const std::set<suc::epl::input_flags> iflags) {
        uint32_t events = //
            (cb.in ? EPOLLIN : 0U) | //
            (cb.out ? EPOLLOUT : 0U) | //
            (cb.rdhup ? EPOLLRDHUP : 0U) | //
            (cb.pri ? EPOLLPRI : 0U) | //
            (cb.err ? EPOLLERR : 0U) | //
            (cb.hup ? EPOLLHUP : 0U) | //
            (iflags.contains(suc::epl::input_flags::et) ? EPOLLET : 0U) | //
            (iflags.contains(suc::epl::input_flags::oneshop) ? EPOLLONESHOT : 0U) | //
            (iflags.contains(suc::epl::input_flags::wakeup) ? EPOLLWAKEUP : 0U) | //
            (iflags.contains(suc::epl::input_flags::exclusive) ? EPOLLEXCLUSIVE : 0U); //
        return events;
    }
} // namespace

namespace suc::epl {
    epleq::epleq()
        : m_epfd{cmn::fd::make_or_rteeno(epoll_create1(EPOLL_CLOEXEC), "creation of epoll fd failed")},
          m_evtfd(cmn::fd::make_or_rteeno(eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK))), m_running{false} {
        auto [it, inserted] = m_fds.emplace(*m_evtfd, cb{});
        if (!inserted) {
            throw std::runtime_error("adding event file descriptor failed");
        }
        it->second.in = [&] -> void { m_running.store(false); };
        epoll_event evt{.events = calc_events(it->second, {}), .data{.fd = *m_evtfd}};
        if (epoll_ctl(*m_epfd, EPOLL_CTL_ADD, evt.data.fd, &evt) != 0) {
            throw cmn::runtimeerror_errno("epoll_ctl add failed");
        }
    }

    void epleq::start() {
        bool expected{false};
        if (!m_running.compare_exchange_strong(expected, true)) {
            return;
        }
        uint64_t cnt;
        read(*m_evtfd, &cnt, sizeof(cnt));

        constexpr int max_events = 20;
        epoll_event events[max_events];
        while (m_running) {
            const int rv = epoll_wait(*m_epfd, events, std::size(events), 0);
            if (rv == -1) {
                int errnum = errno;
                m_running.store(false);
                throw cmn::runtimeerror_errno("epoll_wait failed", errnum);
            }
            for (int i = 0; i < rv; ++i) {
                const epoll_event& event = events[i];
                if (const auto it = m_fds.find(event.data.fd); it != m_fds.end()) {
                    if (event.events & EPOLLIN && it->second.in) {
                        it->second.in();
                    }
                    if (events->events & EPOLLOUT && it->second.out) {
                        it->second.out();
                    }
                    if (events->events & EPOLLRDHUP && it->second.rdhup) {
                        it->second.rdhup();
                    }
                    if (events->events & EPOLLPRI && it->second.pri) {
                        it->second.pri();
                    }
                    if (events->events & EPOLLERR && it->second.err) {
                        it->second.err();
                    }
                    if (events->events & EPOLLHUP && it->second.hup) {
                        it->second.hup();
                    }
                }
            }
        }
    }
    void epleq::stop() const {
        if (m_running) {
            uint64_t incr = 1;
            write(*m_evtfd, &incr, sizeof(incr));
        }
    }

    void epleq::add(int fd, std::function<void(cb&)> reg) {
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

        epoll_event evt{.events = calc_events(it->second, {}), .data{.fd = fd}};
        if (epoll_ctl(*m_epfd, EPOLL_CTL_ADD, evt.data.fd, &evt) != 0) {
            throw cmn::runtimeerror_errno("epoll_ctl add failed");
        }
    }

    void epleq::mod(int fd, std::function<void(cb&)> reg) {
        if (!reg) {
            throw std::runtime_error("reg is mandatory");
        }
        if (auto it = m_fds.find(fd); it != m_fds.end()) {
            reg(it->second);
            epoll_event evt{.events = calc_events(it->second, {}), .data{.fd = fd}};
            if (epoll_ctl(*m_epfd, EPOLL_CTL_MOD, evt.data.fd, &evt) != 0) {
                throw cmn::runtimeerror_errno("epoll_ctl mod failed");
            }
        } else {
            throw std::runtime_error("unknown file descriptor");
        }
    }

    void epleq::del(int fd) {
        if (m_fds.erase(fd) == 0) {
            throw std::runtime_error("unknown fd");
        }
        if (epoll_ctl(*m_epfd, EPOLL_CTL_DEL, fd, nullptr) != 0) {
            throw cmn::runtimeerror_errno("epoll_ctl del failed");
        }
    }
} // namespace suc::epl
