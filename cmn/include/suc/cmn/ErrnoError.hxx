// Copyright [2026] [maggu2810]
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
// Created by maggu2810 on 3/4/26.
//
#ifndef SUC_CMN_ERRNOERROR_HXX
#define SUC_CMN_ERRNOERROR_HXX

#include <stdexcept>
#include <string_view>

namespace suc::cmn
{
class ErrnoError : public std::runtime_error
{
public:
    explicit ErrnoError(std::string_view msg = {}, int errorNumber = errno);

    [[nodiscard]] int errorNumber() const;

private:
    int         m_errorNumber;
    std::string m_what;
};
} // namespace suc::cmn

#endif // SUC_CMN_ERRNOERROR_HXX
