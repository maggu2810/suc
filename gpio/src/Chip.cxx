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

#include "suc/gpio/Chip.hxx"

#include <suc/cmn/ErrnoError.hxx>

#include <cstring>
#include <fcntl.h>
#include <format>
#include <linux/gpio.h>
#include <sys/ioctl.h>

namespace
{
suc::cmn::Fd getLine(const int                                      chipFd,
                     const std::uint32_t                            line,
                     const std::uint64_t                            flags,
                     const suc::gpio::LineArgs&                     lineArgs,
                     const std::optional<suc::gpio::InputLineArgs>& inputLineArgs)
{
    std::uint32_t        num_attrs = 0;
    gpio_v2_line_request line_request {
        .offsets           = {line},
        .consumer          = {},
        .config            = {.flags = flags, .num_attrs = num_attrs, .attrs = {}},
        .num_lines         = 1,
        .event_buffer_size = 0,
        .padding           = {},
        .fd                = -1
    };

    if (lineArgs.activeLow)
    {
        line_request.config.flags |= GPIO_V2_LINE_FLAG_ACTIVE_LOW;
    }
    else
    {
        line_request.config.flags &= ~GPIO_V2_LINE_FLAG_ACTIVE_LOW;
    }

    const std::string_view consumer =
        lineArgs.consumer.empty() ? program_invocation_short_name : lineArgs.consumer;
    const auto consumer_len = std::min(std::size(line_request.consumer) - 1, consumer.size());
    std::memcpy(line_request.consumer, consumer.data(), consumer_len);
    line_request.consumer[consumer_len] = '\0';

    if (inputLineArgs)
    {
        if (inputLineArgs->debounce_period_us)
        {
            gpio_v2_line_config_attribute& cfg = line_request.config.attrs[num_attrs];
            cfg.attr.id                        = GPIO_V2_LINE_ATTR_ID_DEBOUNCE;
            cfg.attr.debounce_period_us        = *inputLineArgs->debounce_period_us;
            cfg.mask                           = 1;
            line_request.config.num_attrs      = ++num_attrs;
        }
    }
    const int iotctl_rv = ioctl(chipFd, GPIO_V2_GET_LINE_IOCTL, &line_request);
    if (iotctl_rv == -1)
    {
        throw suc::cmn::ErrnoError(std::format("get line {} failed", line));
    }
    return suc::cmn::Fd::make(line_request.fd);
}
} // namespace

namespace suc::gpio
{
Chip::Chip(const int chipNumber)
    : m_fd {suc::cmn::Fd::make(
          open(std::format("/dev/gpiochip{}", chipNumber).c_str(), O_RDONLY | O_NONBLOCK),
          "open")}
{
}

Input Chip::getInput(const std::uint32_t  line,
                     const LineArgs&      lineArgs,
                     const InputLineArgs& inputLineArgs) const
{
    return Input {getLine(m_fd, line, GPIO_V2_LINE_FLAG_INPUT, lineArgs, inputLineArgs)};
}

Event Chip::getEvent(const std::uint32_t  line,
                     const LineArgs&      lineArgs,
                     const InputLineArgs& inputLineArgs) const
{
    return Event {getLine(m_fd,
                          line,
                          GPIO_V2_LINE_FLAG_INPUT | GPIO_V2_LINE_FLAG_EDGE_FALLING |
                              GPIO_V2_LINE_FLAG_EDGE_RISING,
                          lineArgs,
                          inputLineArgs)};
}

Output Chip::getOutput(const std::uint32_t line, const LineArgs& lineArgs) const
{
    return Output {getLine(m_fd, line, GPIO_V2_LINE_FLAG_OUTPUT, lineArgs, {})};
}
} // namespace suc::gpio
