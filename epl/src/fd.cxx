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
#include "../include/suc/epl/fd.hxx"

#define EQ_MOD(MEMBER)                          \
    m_eq.mod(*m_fd, [&](epl::cb& cb) {          \
        if (func) {                             \
            cb.MEMBER = [&]() { func(*m_fd); }; \
        } else {                                \
            cb.MEMBER = {};                     \
        }                                       \
    })

namespace suc::epl {
    fd::fd(cmn::fd&& fd, epleq& eq) : m_fd(std::move(fd)), m_eq(eq) {
        eq.add(*m_fd);
    }

    fd::~fd() {
        m_eq.del(*m_fd);
    }

    void fd::on_in(const std::function<void(int)>& func) const {
        EQ_MOD(in);
    }

    void fd::on_out(const std::function<void(int)>& func) const {
        EQ_MOD(out);
    }

    void fd::on_rdhup(const std::function<void(int)>& func) const {
        EQ_MOD(rdhup);
    }

    void fd::on_pri(const std::function<void(int)>& func) const {
        EQ_MOD(pri);
    }

    void fd::on_err(const std::function<void(int)>& func) const {
        EQ_MOD(err);
    }

    void fd::on_hup(const std::function<void(int)>& func) const {
        EQ_MOD(hup);
    }

} // namespace suc::epl
