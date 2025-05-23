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

#ifndef SUC_NET_GETADDRINFO_HXX
#define SUC_NET_GETADDRINFO_HXX

#include <expected>
#include <memory>
#include <netdb.h>

namespace suc::net {
    std::expected<std::shared_ptr<addrinfo>, const char *> getaddrinfo(const char *node,
                                                                       const char *service,
                                                                       const addrinfo &hints);
}

#endif //SUC_NET_GETADDRINFO_HXX
