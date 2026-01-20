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
// Created by maggu2810 on 1/18/26.
//

#include "suc/epl/common/Timer.hxx"

#include <format>
#include <suc/cmn/logging.hxx>
#include <suc/cmn/runtimeerror_errno.hxx>
#include <suc/cmn/to_string.hxx>
#include <sys/timerfd.h>
#include <utility>

namespace suc::epl {
    Timer::Timer(EventQueue& eventQueue)
        : m_fd(Fd{
              suc::cmn::fd::make_or_rteeno(timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC)), eventQueue}) {}

    void Timer::setTime(const itimerspec& value) const {
        if (timerfd_settime(m_fd, 0, &value, nullptr) != 0) {
            throw cmn::runtimeerror_errno("timerfd_settime", errno);
        }
    }

    void Timer::onShot(std::function<void(uint64_t)> func) const {
        m_fd.onInputAvailable([func = std::move(func), this]() {
            uint64_t numberOfExpirations;
            errno         = 0;
            const auto rv = ::read(m_fd, &numberOfExpirations, sizeof(numberOfExpirations));
            if (rv != sizeof(numberOfExpirations)) {
                LOGFW("unexpected read return value: {} (errno: {})", rv, suc::cmn::strerrnum(errno));
                return;
            }
            func(numberOfExpirations);
        });
    }
} // namespace suc::epl
