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

#include <iostream>

#include <suc/net/iface.hxx>
#include <suc/cmn/logging.hxx>
#include <suc/net/sock.hxx>
#include <suc/net/to_string.hxx>
#include <suc/net/inaddr_storage.hxx>
#include <suc/cmn/to_string.hxx>

int main(int argc, char *argv[]) {
    int tcp_client = suc::net::create_connected_socket(suc::net::socket_type::tcp, "google.com", 80, false);
    int udp_client = suc::net::create_connected_socket(suc::net::socket_type::udp, "google.com", 80, false);

    int sockfd = suc::net::create_bound_socket(suc::net::socket_type::udp, 13400, false, true);
    if (sockfd < 0) { return EXIT_FAILURE; };
    if (auto [ipv4, ipv6] = suc::net::prepare_for_recv_info(sockfd); !ipv4 && !ipv6) {
        return EXIT_FAILURE;
    } {
        sockaddr_storage addr_sock{};
        socklen_t addr_sock_len = sizeof(addr_sock);
        sockaddr_storage addr_peer{};
        socklen_t addr_peer_len = sizeof(addr_peer);
        if (getsockname(sockfd, reinterpret_cast<sockaddr *>(&addr_sock),
                        &addr_sock_len) == 0) {
            LOGD("getsockname: {}", suc::net::to_string(reinterpret_cast<sockaddr&>(addr_sock), addr_sock_len));
        } else {
            const auto errnum = errno;
            LOGW("getsockname failed: {}", suc::cmn::strerrnum(errnum));
        }
        if (getpeername(sockfd, reinterpret_cast<sockaddr *>(&addr_peer),
                        &addr_peer_len) == 0) {
            LOGD("getpeername: {}", suc::net::to_string(reinterpret_cast<sockaddr&>(addr_sock), addr_sock_len));
        } else {
            const auto errnum = errno;
            LOGW("getpeername failed: {}", suc::cmn::strerrnum(errnum));
        }
    }

    while (true) {
        std::uint8_t buf[1500];
        suc::net::inaddr_storage host_addr;
        sockaddr_storage ss_peer; // NOLINT(*-pro-type-member-init)
        if (suc::net::recvfromadv(sockfd, buf, sizeof(buf), &host_addr, &ss_peer) == -1) {
            break;
        }
        const auto iface = suc::net::get_ifacename(host_addr);
        LOGD("host address: {}, peer: {}, iface: {}", suc::net::to_string(host_addr), suc::net::to_string(ss_peer),
             iface ? *iface: "unknown");
    }
    return EXIT_SUCCESS;
}
