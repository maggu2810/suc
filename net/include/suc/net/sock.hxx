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

#ifndef SUC_NET_SOCK_HXX
#define SUC_NET_SOCK_HXX

#include "inaddr_storage.hxx"

#include <cstdint>
#include <string>
#include <utility>
#include <sys/types.h>
#include <sys/socket.h>

#include "socket_type.hxx"

namespace suc::net {
    int create_bound_socket(socket_type socket_type, std::uint16_t port, bool non_blocking = false, bool reuse_addr = false);

    int create_connected_socket(socket_type socket_type, const std::string& host, std::uint16_t port, bool non_blocking = false);

    std::pair<bool, bool> prepare_for_recv_info(int sock);

    ssize_t recvfromadv(int sockfd, void *buf, size_t len,
                        inaddr_storage *host_addr,
                        sockaddr_storage *peer_addr);
}

#endif //SUC_NET_SOCK_HXX
