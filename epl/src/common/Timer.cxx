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

#include "../../../gpio/include/suc/gpio/event.hxx"
#include <format>
#include <memory>
#include <suc/cmn/logging.hxx>
#include <suc/cmn/runtimeerror_errno.hxx>
#include <suc/cmn/to_string.hxx>
#include <sys/timerfd.h>
#include <utility>

namespace suc::epl {
    Timer::Timer(EventQueue& eventQueue)
        : m_fd(Fd{
              suc::cmn::fd::make_or_rteeno(timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC)), eventQueue}) {}

    Timer::Timer(std::function<void(uint64_t)> func, EventQueue& eventQueue)
        : Timer(eventQueue) {
        onShot(std::move(func));
    }

    Timer::Timer(const itimerspec& value, std::function<void(uint64_t)> func, EventQueue& eventQueue)
        : Timer(std::move(func), eventQueue) {
        setTime(value);
    }

    void Timer::setTime(const itimerspec& value) const {
        lowLevelSetTime(value.it_value.tv_sec != 0 || value.it_value.tv_nsec != 0
                            ? value
                            : itimerspec{.it_interval = value.it_interval, .it_value = {0, 1}});
    }

    void Timer::cancel() const {
        // If there are unread expirations, this will not lead to a read anymore and on a new set the expirations starts
        // from zero. So set time to zeroes is enough, we do not need to read the expirations.
        lowLevelSetTime({});
    }

    void Timer::onShot(std::function<void(uint64_t)> func) const {
        m_fd.onInputAvailable([func = std::move(func), this]() { func(readNumberOfExpirations()); });
    }
    std::uint64_t Timer::readNumberOfExpirations() const {
        uint64_t numberOfExpirations;
        errno         = 0;
        const auto rv = ::read(m_fd, &numberOfExpirations, sizeof(numberOfExpirations));
        if (rv == sizeof(numberOfExpirations)) {
            return numberOfExpirations;
        } else {
            const auto errnum = errno;
            // As this function is mostly called inside the event handler, this will break the event handler.
            // But as it is a general error, there is nothing we can do.
            throw cmn::runtimeerror_errno(std::format("unexpected read return value: {}", rv), errnum);
        }
    }
    void Timer::lowLevelSetTime(const itimerspec& value) const {
        if (timerfd_settime(m_fd, 0, &value, nullptr) != 0) {
            throw cmn::runtimeerror_errno("timerfd_settime", errno);
        }
    }
} // namespace suc::epl
