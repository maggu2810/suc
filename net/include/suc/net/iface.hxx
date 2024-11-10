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

#ifndef SUC_NET_IFACE_HXX
#define SUC_NET_IFACE_HXX

#include "inaddr_storage.hxx"

#include <optional>
#include <string>

namespace suc::net {
    // get interface name by address

    std::optional<std::string> get_ifacename(const inaddr_storage &address);

    std::optional<std::string> get_ifacename(const in_addr &address);

    std::optional<std::string> get_ifacename(const in6_addr &address);

    // get interface name by sockaddr

    std::optional<std::string> get_ifacename(const sockaddr &sa);

    inline std::optional<std::string> get_ifacename(const sockaddr_storage &sa) {
        return get_ifacename(reinterpret_cast<const sockaddr &>(sa));
    }

    inline std::optional<std::string> get_ifacename(const sockaddr_in &sa) {
        return get_ifacename(reinterpret_cast<const sockaddr &>(sa));
    }

    inline std::optional<std::string> get_ifacename(const sockaddr_in6 &sa) {
        return get_ifacename(reinterpret_cast<const sockaddr &>(sa));
    }

    // get interface name by socket

    std::optional<std::string> get_ifacename(int sock);
}

#endif //SUC_NET_IFACE_HXX
