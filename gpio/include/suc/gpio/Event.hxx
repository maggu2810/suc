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

#ifndef SUC_GPIO_EVENT_HXX
#define SUC_GPIO_EVENT_HXX

#include "Input.hxx"
#include "Line.hxx"

#include <cstdint>
#include <functional>
#include <poll.h>

namespace suc::gpio
{
class Event : protected Input
{
public:
    enum class Edge
    {
        Rising,
        Falling
    };

    explicit Event(suc::cmn::Fd&& fd);
    using Input::get;

    void pollSetup(pollfd& pfd) const;

    using EdgeHandler = std::function<void(std::uint64_t ts_mon_ns, Edge event)>;
    void pollInspect(pollfd& pfd, const EdgeHandler& handler) const;
};
} // namespace suc::gpio

#endif // SUC_GPIO_EVENT_HXX
