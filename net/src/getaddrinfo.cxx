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

#include <suc/net/getaddrinfo.hxx>

namespace suc::net {
    std::expected<std::shared_ptr<addrinfo>, const char *> getaddrinfo(const char *node,
                                                                       const char *service,
                                                                       const addrinfo &hints) {
        addrinfo *servinfo{};
        if (int rv = getaddrinfo(node, service, &hints, &servinfo); rv != 0) {
            return std::unexpected(gai_strerror(rv));
        }
        return std::shared_ptr<addrinfo>{servinfo, freeaddrinfo};
    }
}
