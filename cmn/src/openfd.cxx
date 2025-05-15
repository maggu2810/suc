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

#include "suc/cmn/openfd.hxx"

#include <fcntl.h>
#include <format>
#include <stdexcept>
#include <unistd.h>
#include <utility>

namespace {
    int fdcheck(int fd) {
        if (fd < 0) {
            throw std::runtime_error("not an open file descriptor");
        }
        return fd;
    }
} // namespace

namespace suc::cmn {
    openfd::openfd(int fd) : m_fd{fdcheck(fd)} {}

    openfd::openfd(const char* pathname, int flags) : m_fd{fdcheck(open(pathname, flags))} {}

    openfd::~openfd() {
        if (m_fd >= 0) {
            close(m_fd);
            m_fd = -1;
        }
    }

    openfd::openfd(openfd&& other) noexcept : m_fd{std::exchange(other.m_fd, -1)} {}

    openfd& openfd::operator=(openfd&& other) noexcept {
        std::swap(m_fd, other.m_fd);
        return *this;
    }

    int openfd::fd() const {
        return m_fd;
    }
} // namespace suc::cmn
