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

#include <chrono>
#include <fcntl.h>
#include <iostream>
#include <suc/cmn/runtimeerror_errno.hxx>
#include <suc/epl/base/EventQueue.hxx>
#include <suc/epl/base/Fd.hxx>
#include <suc/epl/common/Timer.hxx>

int main(int argc, char* argv[]) {
    suc::epl::EventQueue& eventQueue = suc::epl::EventQueue::coreInstance();

#if 0
    int p[2];
    if (pipe2(p, O_NONBLOCK | O_CLOEXEC) == -1) {
        throw suc::cmn::runtimeerror_errno("pipe2 failed");
    }

    suc::epl::Fd testfd0{suc::cmn::fd::make(p[0]), eventQueue};
    suc::epl::Fd testfd1{suc::cmn::fd::make(p[1]), eventQueue};

    int cnt = 0;

    testfd0.onInputAvailable([](int fd) {
        std::cout << "on in (fd0): ";
        char buffer[512];
        int rv = read(fd, buffer, sizeof(buffer));
        std::cout << rv << " read\n";
    });
    testfd1.onOutputPossible([&](int fd) {
        std::cout << "on out (fd1): ";
        int rv = write(fd, "hello", 5);
        std::cout << rv << " written\n";
        if (++cnt >= 100) {
            eventQueue->stop();
        }
    });
#endif

    suc::epl::Timer timer {};
    timer.onShot([](auto numberOfExpirations) {
        std::print(std::cout, "[{}] number of expirations: {}\n",std::chrono::system_clock::now(), numberOfExpirations);
    });
    timer.setTime({.it_interval = {.tv_sec = 1, .tv_nsec=0}, .it_value = {.tv_sec = 3, .tv_nsec=0}});

    suc::epl::Timer timerShutdown{};
    timerShutdown.onShot([&](auto _) {
        eventQueue.stop();
    });
    timerShutdown.setTime({.it_value = {.tv_sec = 10}});

    std::print(std::cout, "[{}] start event loop\n",std::chrono::system_clock::now());
    return eventQueue.exec();
}
