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

#include "suc/cmn/runtimeerror_errno.hxx"
#include <fcntl.h>
#include <iostream>
#include <suc/epl/epleq.hxx>
#include <suc/epl/fd.hxx>

int main(int argc, char* argv[]) {
    suc::epl::epleq epleq;

    int p[2];
    if (pipe2(p, O_NONBLOCK | O_CLOEXEC) == -1) {
        throw suc::cmn::runtimeerror_errno("pipe2 failed");
    }

    suc::epl::fd testfd0{suc::cmn::fd::make(p[0]), epleq};
    suc::epl::fd testfd1{suc::cmn::fd::make(p[1]), epleq};

    int cnt = 0;

#if 0
    testfd0.on_out([](int fd) {
        std::cout << "on out (fd0): ";
        int rv =write(fd, "hello", 5);
        std::cout << rv << " written\n";
    });
#endif
    testfd0.on_in([](int fd) {
        std::cout << "on in (fd0): ";
        char buffer[512];
        int rv = read(fd, buffer, sizeof(buffer));
        std::cout << rv << " read\n";
    });
    testfd1.on_out([&](int fd) {
        std::cout << "on out (fd1): ";
        int rv =write(fd, "hello", 5);
        std::cout << rv << " written\n";
        if (++cnt >= 100) {
            epleq.stop();
        }
    });
#if 0
    testfd1.on_in([](int fd) {
        std::cout << "on in (fd1): ";
        char buffer[512];
        read(fd, buffer, sizeof(buffer));
        int rv = read(fd, buffer, sizeof(buffer));
        std::cout << rv << " read\n";
    });
#endif

    epleq.start();

    return EXIT_SUCCESS;
}
