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
#ifndef SUC_EPL_FD_HXX
#define SUC_EPL_FD_HXX

#include "epleq.hxx"

#include <suc/cmn/fd.hxx>

#include <functional>

namespace suc::epl {
    class fd {
    public:
        explicit fd(cmn::fd&& fd, epleq& eq);
        ~fd();
        void on_in(const std::function<void(int)>& cb) const;
        void on_out(const std::function<void(int)>& cb) const;
        void on_rdhup(const std::function<void(int)>& cb) const;
        void on_pri(const std::function<void(int)>& cb) const;
        void on_err(const std::function<void(int)>& cb) const;
        void on_hup(const std::function<void(int)>& cb) const;

    private:
        cmn::fd m_fd;
        epleq& m_eq;
    };
} // namespace suc::epl

#endif // SUC_EPL_FD_HXX
