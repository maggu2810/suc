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

#include "input.hxx"
#include "line.hxx"
#include <cstdint>
#include <functional>
#include <poll.h>

namespace suc::gpio {
    class event : protected input {
    public:
        enum class edge_event { rising, falling };

        explicit event(suc::cmn::openfd&& fd);
        using input::get;

        void poll_setup(pollfd& pfd) const;
        void poll_inspect(
            pollfd& pfd, const std::function<void(std::uint64_t ts_mon_ns, edge_event event)>& handler) const;
    };
} // namespace suc::gpio

#endif // SUC_GPIO_EVENT_HXX
