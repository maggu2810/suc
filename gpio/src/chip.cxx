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

#include <cstring>
#include <fcntl.h>
#include <format>
#include <linux/gpio.h>
#include <suc/cmn/runtimeerror_errno.hxx>
#include <sys/ioctl.h>


namespace {
    suc::cmn::fd get_line(int chip_fd, std::uint32_t line, std::uint64_t flags, const suc::gpio::line_args& line_args,
        const std::optional<suc::gpio::input_line_args>& input_line_args) {
        std::uint32_t num_attrs = 0;
        gpio_v2_line_request line_request{.offsets = {line},
            .consumer                              = {},
            .config                                = {.flags = flags, .num_attrs = num_attrs, .attrs = {}},
            .num_lines                             = 1,
            .event_buffer_size                     = 0,
            .padding                               = {},
            .fd                                    = -1};

        if (line_args.active_low) {
            line_request.config.flags |= GPIO_V2_LINE_FLAG_ACTIVE_LOW;
        } else {
            line_request.config.flags &= ~GPIO_V2_LINE_FLAG_ACTIVE_LOW;
        }

        const std::string_view consumer = line_args.consumer.empty() ? program_invocation_short_name : line_args.consumer;
        const auto consumer_len = std::min(std::size(line_request.consumer) - 1, consumer.size());
        std::memcpy(line_request.consumer, consumer.data(), consumer_len);
        line_request.consumer[consumer_len] = '\0';

        if (input_line_args) {
            if (input_line_args->debounce_period_us) {
                gpio_v2_line_config_attribute& cfg = line_request.config.attrs[num_attrs];
                cfg.attr.id                        = GPIO_V2_LINE_ATTR_ID_DEBOUNCE;
                cfg.attr.debounce_period_us        = *input_line_args->debounce_period_us;
                cfg.mask                           = 1;
                line_request.config.num_attrs      = ++num_attrs;
            }
        }
        const int iotctl_rv = ioctl(chip_fd, GPIO_V2_GET_LINE_IOCTL, &line_request);
        if (iotctl_rv == -1) {
            throw suc::cmn::runtimeerror_errno(std::format("get line {} failed", line));
        }
        return suc::cmn::fd::make(line_request.fd);
    }
} // namespace

namespace suc::gpio {
    chip::chip(int chip_number)
        : m_fd{suc::cmn::fd::make_or_rteeno(
              open(std::format("/dev/gpiochip{}", chip_number).c_str(), O_RDONLY | O_NONBLOCK))} {}

    input chip::get_input(
        std::uint32_t line, const line_args& line_args, const input_line_args& input_line_args) const {
        return input{get_line(m_fd, line, GPIO_V2_LINE_FLAG_INPUT, line_args, input_line_args)};
    }

    event chip::get_event(
        std::uint32_t line, const line_args& line_args, const input_line_args& input_line_args) const {
        return event{get_line(m_fd, line,
            GPIO_V2_LINE_FLAG_INPUT | GPIO_V2_LINE_FLAG_EDGE_FALLING | GPIO_V2_LINE_FLAG_EDGE_RISING, line_args,
            input_line_args)};
    }

    output chip::get_output(std::uint32_t line, const line_args& line_args) const {
        return output{get_line(m_fd, line, GPIO_V2_LINE_FLAG_OUTPUT, line_args, {})};
    }
} // namespace suc::gpio
