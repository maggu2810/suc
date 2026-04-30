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

#include "Event.hxx"
#include "Input.hxx"
#include "Output.hxx"

#include <suc/cmn/Fd.hxx>

#include <cstdint>
#include <optional>
#include <string>

namespace suc::gpio
{
struct LineArgs
{
    std::string_view consumer;
    bool             activeLow {false};
};

struct InputLineArgs
{
    std::optional<std::uint32_t> debounce_period_us {};
};

struct EventLineArgs : public InputLineArgs
{
    enum class Edge
    {
        Rising,
        Falling,
        Both
    };
    Edge edge {Edge::Both};
};

class Chip
{
public:
    explicit Chip(int chipNumber);

    [[nodiscard]] Input  getInput(std::uint32_t          line,
                                   const LineArgs&       lineArgs       = {},
                                   const InputLineArgs& inputLineArgs = {}) const;
    [[nodiscard]] Event  getEvent(std::uint32_t          line,
                                   const LineArgs&       lineArgs      = {},
                                   const EventLineArgs& eventLineArgs = {}) const;
    [[nodiscard]] Output getOutput(std::uint32_t line, const LineArgs& lineArgs = {}) const;

private:
    suc::cmn::Fd m_fd;
};
} // namespace suc::gpio

#endif // SUC_GPIO_CHIP_HXX
