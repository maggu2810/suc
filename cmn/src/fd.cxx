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

#include "suc/cmn/fd.hxx"

#include "suc/cmn/runtimeerror_errno.hxx"

#include <unistd.h>
#include <utility>

namespace suc::cmn {
    fd fd::make_or_rteeno(int fd, std::string_view msg) {
        if (fd < 0) {
            throw runtimeerror_errno(msg, errno);
        }
        return {fd};
    }
    fd fd::make(int fd) {
        if (fd < 0) {
            throw std::runtime_error("not a valid file descriptor");
        }
        return {fd};
    }

    fd::fd(int fd) : m_fd{fd} {}

    fd::~fd() {
        if (m_fd >= 0) {
            close(m_fd);
            m_fd = -1;
        }
    }

    fd::fd(fd&& other) noexcept : m_fd{std::exchange(other.m_fd, -1)} {}

    fd& fd::operator=(fd&& other) noexcept {
        std::swap(m_fd, other.m_fd);
        return *this;
    }

    const int& fd::operator*() const {
        return m_fd;
    }

    fd::operator int() const {
        return m_fd;
    }

} // namespace suc::cmn
