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

#include "suc/cmn/ErrnoError.hxx"

#include "suc/cmn/logging.hxx"

#include <cstring>
#include <sstream>

namespace
{
std::string generateWhat(const std::string_view msg, const int errorNumber)
{
    std::stringstream str;
    if (!msg.empty())
    {
        str << msg;
        str << "; ";
    }
#if 0
    char buffer[128];
    const char *str = ::strerror_r(errnum, buffer, sizeof(buffer));
    str << std::format("[{}] {}", errnum, str);
#else
    str << std::format("{} ({}): {}",
                       strerrorname_np(errorNumber),
                       errorNumber,
                       strerrordesc_np(errorNumber));
#endif
    return str.str();
}
} // namespace

namespace suc::cmn
{
ErrnoError::ErrnoError(const std::string_view msg, const int errorNumber)
    : runtime_error(generateWhat(msg, errorNumber)), m_errorNumber {errorNumber}
{
}

int ErrnoError::errorNumber() const
{
    return m_errorNumber;
}

} // namespace suc::cmn
