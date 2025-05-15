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

#ifndef SUC_GPIO_LINE_HXX
#define SUC_GPIO_LINE_HXX

#include <suc/cmn/openfd.hxx>

namespace suc::gpio {
    class line {
    public:
        explicit line(suc::cmn::openfd&& fd);

    protected:
        suc::cmn::openfd m_fd;
    };
} // namespace suc::gpio

#endif // SUC_GPIO_LINE_HXX
