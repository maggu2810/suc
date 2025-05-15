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

#include "suc/gpio/chip.hxx"

#include <fcntl.h>
#include <format>
#include <linux/gpio.h>
#include <suc/cmn/runtimeerror_errno.hxx>
#include <sys/ioctl.h>


namespace {
    suc::cmn::openfd get_line(int chip_fd, std::uint32_t line, std::uint64_t flags) {
        gpio_v2_line_request line_request{.offsets = {line},
            .consumer                              = "tbd",
            .config                                = {.flags = flags, .num_attrs = 0, .attrs = {}},
            .num_lines                             = 1,
            .event_buffer_size                     = 0,
            .padding                               = {},
            .fd                                    = -1};
        const int iotctl_rv = ioctl(chip_fd, GPIO_V2_GET_LINE_IOCTL, &line_request);
        if (iotctl_rv == -1) {
            throw suc::cmn::runtimeerror_errno(std::format("get line {} failed", line));
        }
        return suc::cmn::openfd{line_request.fd};
    }
} // namespace

namespace suc::gpio {
    chip::chip(int chip_number) : m_fd{std::format("/dev/gpiochip{}", chip_number).c_str(), O_RDONLY | O_NONBLOCK} {}

    input chip::get_input(std::uint32_t line, bool activeLow) {
        return input{
            get_line(m_fd.fd(), line, GPIO_V2_LINE_FLAG_INPUT | (activeLow ? GPIO_V2_LINE_FLAG_ACTIVE_LOW : 0))};
    }
    output chip::get_output(std::uint32_t line, bool activeLow) {
        return output{
            get_line(m_fd.fd(), line, GPIO_V2_LINE_FLAG_OUTPUT | (activeLow ? GPIO_V2_LINE_FLAG_ACTIVE_LOW : 0))};
    }

    event chip::get_event(std::uint32_t line, bool activeLow) {
        return event{get_line(m_fd.fd(), line,
            GPIO_V2_LINE_FLAG_INPUT | GPIO_V2_LINE_FLAG_EDGE_FALLING | GPIO_V2_LINE_FLAG_EDGE_RISING
                | (activeLow ? GPIO_V2_LINE_FLAG_ACTIVE_LOW : 0))};
    }
} // namespace suc::gpio
