// Copyright [2024] [maggu2810]
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

#ifndef SUC_NET_TO_STRING_HXX
#define SUC_NET_TO_STRING_HXX

#include "inaddr_storage.hxx"

#include <string>

namespace suc::net {
    std::string strerrnum(int errnum);

    std::string to_string(const inaddr_storage &address);

    std::string to_string(const in_addr &address);

    std::string to_string(const in6_addr &address);

    std::string to_string(const sockaddr &sa, socklen_t addrlen);

    inline std::string to_string(const sockaddr_storage &sa) {
        return to_string(reinterpret_cast<const sockaddr &>(sa), sizeof(sa));
    }

    inline std::string to_string(const sockaddr_in &sa) {
        return to_string(reinterpret_cast<const sockaddr &>(sa), sizeof(sa));
    }

    inline std::string to_string(const sockaddr_in6 &sa) {
        return to_string(reinterpret_cast<const sockaddr &>(sa), sizeof(sa));
    }
}

#endif //SUC_NET_TO_STRING_HXX
