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
// Created by maggu2810 on 11/4/25.
//
#include "../../include/suc/epl/base/Fd.hxx"

#include "../../../gpio/include/suc/gpio/event.hxx"

#define EQ_MOD(MEMBER) m_eventQueue->mod(m_fd, [&](auto& cb) { cb.MEMBER = std::move(func); })

namespace suc::epl {
    Fd::Fd(cmn::fd&& fd, EPL_EQ_CA_DEC)
        : m_fd(std::move(fd)),
          m_eventQueue(std::move(eventQueue)) {
        m_eventQueue->add(m_fd);
    }

    Fd::~Fd() {
        m_eventQueue->del(m_fd);
    }

    void Fd::onInputAvailable(std::function<void()> func) const {
        EQ_MOD(inputAvailable);
    }

    void Fd::onOutputPossible(std::function<void()> func) const {
        EQ_MOD(outputPossible);
    }

    void Fd::onPriorityData(std::function<void()> func) const {
        EQ_MOD(priorityData);
    }

    void Fd::onReadSideHangUp(std::function<void()> func) const {
        EQ_MOD(readSideHangUp);
    }

    void Fd::onHangUp(std::function<void()> func) const {
        EQ_MOD(hangUp);
    }

    void Fd::onError(std::function<void()> func) const {
        EQ_MOD(errorOccurred);
    }

} // namespace suc::epl
