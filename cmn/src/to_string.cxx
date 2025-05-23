// Copyright [2024-2025] [maggu2810]
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

#include <suc/cmn/to_string.hxx>

#include <cstring>
#include <format>

namespace suc::cmn {
    std::string strerrnum(int errnum) {
#if 0
        char buffer[128];
        const char *str = ::strerror_r(errnum, buffer, sizeof(buffer));
        return std::format("[{}] {}", errnum, str);
#endif
        return std::format("{} ({}): {}", strerrorname_np(errnum), errnum, strerrordesc_np(errnum));
    }
}
