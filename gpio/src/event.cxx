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

#include "suc/gpio/event.hxx"

#include <algorithm>
#include <linux/gpio.h>
#include <suc/cmn/runtimeerror_errno.hxx>
#include <unistd.h>

namespace suc::gpio {
    event::event(suc::cmn::openfd&& fd) : input(std::move(fd)) {}

    void event::poll_setup(pollfd& pfd) const {
        pfd.fd      = m_fd.fd();
        pfd.events  = POLLIN;
        pfd.revents = 0;
    }
    void event::poll_inspect(
        pollfd& pfd, const std::function<void(std::uint64_t ts_mon_ns, edge_event event)>& handler) const {
        if (pfd.revents & POLLIN) {
            gpio_v2_line_event event{};
            if (read(m_fd.fd(), &event, sizeof(event)) != sizeof(event)) {
                throw suc::cmn::runtimeerror_errno("read event");
            };
            handler(event.timestamp_ns, [](const std::uint32_t id) -> edge_event {
                if (id == GPIOEVENT_EVENT_RISING_EDGE) {
                    return edge_event::rising;
                }
                if (id == GPIOEVENT_EVENT_FALLING_EDGE) {
                    return edge_event::falling;
                }
                throw std::runtime_error("unknown edge event");
            }(event.id));
        }
    }
} // namespace suc::gpio
