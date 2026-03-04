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

#ifndef SUC_CMN_FD_HXX
#define SUC_CMN_FD_HXX

#include <functional>
#include <string_view>

namespace suc::cmn
{
class Fd
{
public:
    static Fd make(int fd, std::string_view msg = {}, bool withErrno = true);
    static Fd make(const std::function<int()>& fdFunc, std::string_view msg = {}, bool withErrno = true);

private:
    explicit Fd(int fd);

public:
    ~Fd();

    Fd(const Fd&)            = delete;
    Fd& operator=(const Fd&) = delete;

    Fd(Fd&&) noexcept;
    Fd& operator=(Fd&&) noexcept;

    [[nodiscard]] const int& operator*() const;
    [[nodiscard]]            operator int() const;

private:
    int m_fd {-1};
};
} // namespace suc::cmn

#endif // SUC_CMN_FD_HXX
