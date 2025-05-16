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
#include <suc/cmn/openfd.hxx>

namespace suc::gpio {
    class chip {
    public:
        explicit chip(int chip_number);

        input get_input(std::uint32_t line, std::uint32_t debounce_period_us = 0, bool activeLow = false);
        event get_event(std::uint32_t line, std::uint32_t debounce_period_us = 0, bool activeLow = false);
        output get_output(std::uint32_t line, bool activeLow = false);

    private:
        suc::cmn::openfd m_fd;
    };
} // namespace suc::gpio


#endif // SUC_GPIO_CHIP_HXX
