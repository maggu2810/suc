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

#ifndef SUC_CMN_OPENFD_HXX
#define SUC_CMN_OPENFD_HXX

namespace suc::cmn {
    class openfd {
    public:
        explicit openfd(int fd);
        explicit openfd(const char* pathname, int flags);
        ~openfd();

        openfd(const openfd&)            = delete;
        openfd& operator=(const openfd&) = delete;

        openfd(openfd&&) noexcept;
        openfd& operator=(openfd&&) noexcept;

        [[nodiscard]] int fd() const;

    private:
        int m_fd{-1};
    };
} // namespace suc::cmn

#endif // SUC_CMN_OPENFD_HXX
