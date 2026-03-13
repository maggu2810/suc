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

/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */

#include "suc/gpio/Event.hxx"

#include <suc/cmn/ErrnoError.hxx>

#include <algorithm>
#include <format>
#include <linux/gpio.h>
#include <unistd.h>

namespace
{
using namespace suc::gpio;
namespace c = std::chrono;

Event::TimePoint timepoint(const uint64_t& nanoSeconds)
{
    return Event::TimePoint {c::duration_cast<Event::Duration>(c::nanoseconds(nanoSeconds))};
}
} // namespace

namespace suc::gpio
{
Event::Event(cmn::Fd&& fd) : Input(std::move(fd))
{
}

int Event::getFdForInputEvent() const
{
    return m_fd;
}

bool Event::inspectInput(const EdgeHandler& handler) const
{
    gpio_v2_line_event event {};
    if (const ssize_t rvRead = read(m_fd, &event, sizeof(event)); rvRead == sizeof(event))
    {
        handler(timepoint(event.timestamp_ns),
                [](const std::uint32_t id) -> Edge
                {
                    if (id == GPIOEVENT_EVENT_RISING_EDGE)
                    {
                        return Edge::Rising;
                    }
                    if (id == GPIOEVENT_EVENT_FALLING_EDGE)
                    {
                        return Edge::Falling;
                    }
                    throw std::runtime_error("unknown edge event");
                }(event.id));
        return true;
    }
    else if (rvRead == -1)
    {
        const int errorNumber = errno;
        if (errorNumber == EAGAIN)
        {
            return false;
        }
        throw cmn::ErrnoError("read event", errorNumber);
    }
    else
    {
        throw std::runtime_error(
            std::format("unexpected read size: got={}, expected={}", rvRead, sizeof(event)));
    }
}
} // namespace suc::gpio
