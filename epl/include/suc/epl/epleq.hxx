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

//
// Created by maggu2810 on 11/2/25.
//
#ifndef SUC_EPL_EPLEQ_HXX
#define SUC_EPL_EPLEQ_HXX

#include "suc/cmn/fd.hxx"
#include <atomic>
#include <functional>
#include <map>

namespace suc::epl {
    enum class event_type { in, out, rdhub, pri, err, hup };

    enum class input_flags { et, oneshop, wakeup, exclusive };

    struct cb {
        std::function<void()> in{};
        std::function<void()> out{};
        std::function<void()> rdhup{};
        std::function<void()> pri{};
        std::function<void()> err{};
        std::function<void()> hup{};
    };

    class epleq {
    public:
        explicit epleq();

        void start();
        void stop() const;

        void add(int fd, std::function<void(cb&)> reg = {});
        void mod(int fd, std::function<void(cb&)> reg);
        void del(int fd);

    private:
        cmn::fd m_epfd;
        cmn::fd m_evtfd;
        std::atomic_bool m_running;
        std::map<int, cb> m_fds;
    };
} // namespace suc::epl

#endif // SUC_EPL_EPLEQ_HXX
