// Copyright [2025-2026] [maggu2810]
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
// Created by maggu2810 on 11/4/25.
//
#ifndef SUC_EPL_FD_HXX
#define SUC_EPL_FD_HXX

#include "EventQueue.hxx"
#include <functional>
#include <suc/cmn/fd.hxx>

namespace suc::epl {
    class Fd {
    public:
        explicit Fd(cmn::fd&& fd, EPL_EQ_CA_DEF);
        ~Fd();

        Fd(const Fd&)            = delete;
        Fd& operator=(const Fd&) = delete;

        Fd(Fd&&)            = delete;
        Fd& operator=(Fd&&) = delete;

        void onInputAvailable(std::function<void()> cb) const;
        void onOutputPossible(std::function<void()> cb) const;
        void onPriorityData(std::function<void()> cb) const;
        void onReadSideHangUp(std::function<void()> cb) const;
        void onHangUp(std::function<void()> cb) const;
        void onError(std::function<void()> cb) const;

        [[nodiscard]] operator int() const {
            return m_fd;
        }

    private:
        cmn::fd m_fd;
        std::shared_ptr<EventQueue> m_eventQueue;
    };
} // namespace suc::epl

#endif // SUC_EPL_FD_HXX
