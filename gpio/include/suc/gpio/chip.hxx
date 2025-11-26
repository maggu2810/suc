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

#ifndef SUC_GPIO_CHIP_HXX
#define SUC_GPIO_CHIP_HXX

#include "event.hxx"
#include "input.hxx"
#include "output.hxx"
#include <cstdint>
#include <optional>
#include <string>
#include <suc/cmn/fd.hxx>

namespace suc::gpio {
    struct line_args {
        std::string_view consumer;
        bool active_low{false};
    };

    struct input_line_args {
        std::optional<std::uint32_t> debounce_period_us{};
    };

    class chip {
    public:
        explicit chip(int chip_number);

        [[nodiscard]] input get_input(
            std::uint32_t line, const line_args& line_args = {}, const input_line_args& input_line_args = {}) const;
        [[nodiscard]] event get_event(
            std::uint32_t line, const line_args& line_args = {}, const input_line_args& input_line_args = {}) const;
        [[nodiscard]] output get_output(std::uint32_t line, const line_args& line_args = {}) const;

    private:
        suc::cmn::fd m_fd;
    };
} // namespace suc::gpio


#endif // SUC_GPIO_CHIP_HXX
