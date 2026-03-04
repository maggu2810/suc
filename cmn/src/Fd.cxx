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

#include "suc/cmn/Fd.hxx"

#include "suc/cmn/ErrnoError.hxx"

#include <sstream>
#include <unistd.h>
#include <utility>

using namespace std::literals;

namespace suc::cmn
{
Fd Fd::make(const int fd, const std::string_view msg, const bool withErrno)
{
    if (fd < 0)
    {
        const int         errorNumber = errno;
        std::stringstream ss {"invalid file descriptor"};
        if (!msg.empty())
        {
            ss << "; " << msg;
        }
        if (withErrno)
        {
            throw ErrnoError(ss.str(), errorNumber);
        }
        else
        {
            throw std::runtime_error(ss.str());
        }
    }
    return Fd {fd};
}

Fd Fd::make(const std::function<int()>& fdFunc, const std::string_view msg, const bool withErrno)
{
    if (withErrno)
    {
        errno = 0;
    }
    return make(fdFunc ? fdFunc() : -1, msg, withErrno);
}

Fd::Fd(const int fd) : m_fd {fd}
{
}

Fd::~Fd()
{
    if (m_fd >= 0)
    {
        close(m_fd);
        m_fd = -1;
    }
}

Fd::Fd(Fd&& other) noexcept : m_fd {std::exchange(other.m_fd, -1)}
{
}

Fd& Fd::operator=(Fd&& other) noexcept
{
    std::swap(m_fd, other.m_fd);
    return *this;
}

const int& Fd::operator*() const
{
    return m_fd;
}

Fd::operator int() const
{
    return m_fd;
}

} // namespace suc::cmn
