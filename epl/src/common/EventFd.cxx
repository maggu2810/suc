// Copyright [2026] [maggu2810]
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
// Created by maggu2810 on 2/1/26.
//

#include "suc/epl/common/EventFd.hxx"

#include "suc/cmn/runtimeerror_errno.hxx"
#include <format>
#include <sys/eventfd.h>

namespace suc::epl {
    EventFd::EventFd(const bool semaphore, EventQueue& eventQueue)
        : m_fd(Fd{suc::cmn::fd::make_or_rteeno(
                      eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK | (semaphore ? EFD_SEMAPHORE : 0))),
              eventQueue}) {}

    void EventFd::onShot(std::function<void(std::uint64_t)> func) const {
        if (!func) {
            m_fd.onInputAvailable({});
            return;
        }

        m_fd.onInputAvailable([func = std::move(func), this]() {
            std::uint64_t value;
            const auto rvRead = ::read(m_fd, &value, sizeof(value));
            if (rvRead != sizeof(value)) {
                throw cmn::runtimeerror_errno(std::format("unexpected read rv: {}", rvRead));
            }
            func(value);
        });
    }

    void EventFd::add(const std::uint64_t value) const {
        const auto rvWrite = ::write(m_fd, &value, sizeof(value));
        if (rvWrite != sizeof(value)) {
            throw cmn::runtimeerror_errno(std::format("unexpected write rv: {}", rvWrite));
        }
    }

} // namespace suc::epl
