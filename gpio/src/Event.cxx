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
#include <linux/gpio.h>
#include <unistd.h>

namespace suc::gpio
{
Event::Event(suc::cmn::Fd&& fd) : Input(std::move(fd))
{
}

void Event::pollSetup(pollfd& pfd) const
{
    pfd.fd      = m_fd;
    pfd.events  = POLLIN;
    pfd.revents = 0;
}

void Event::pollInspect(pollfd& pfd, const EdgeHandler& handler) const
{
    if (pfd.revents & POLLIN)
    {
        gpio_v2_line_event event {};
        if (read(m_fd, &event, sizeof(event)) != sizeof(event))
        {
            throw suc::cmn::ErrnoError("read event");
        };
        handler(event.timestamp_ns,
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
    }
}
} // namespace suc::gpio
