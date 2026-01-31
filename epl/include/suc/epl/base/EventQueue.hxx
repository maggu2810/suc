// Copyright [2025-2026] [maggu2810]
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

//
// Created by maggu2810 on 11/2/25.
//
#ifndef SUC_EPL_EVENTQUEUE_HXX
#define SUC_EPL_EVENTQUEUE_HXX

#include "suc/cmn/fd.hxx"
#include <atomic>
#include <functional>
#include <map>

/* epoll */
namespace suc::epl {
    struct cb {
        std::function<void()> inputAvailable{}; /**< The associated file is available for read(2) operations. */
        std::function<void()> outputPossible{}; /**< The associated file is available for write(2) operations. */
        std::function<void()> priorityData{}; /**< There is an exceptional condition on the file descriptor. */
        std::function<void()>
            readSideHangUp{}; /**< Stream socket peer closed connection, or shut down writing half of connection. */
        std::function<void()> hangUp{}; /**< Hang up happened on the associated file descriptor. */
        std::function<void()> errorOccurred{}; /**< Error condition happened on the associated file descriptor. This
                event is also reported for the write end of a pipe when the read end has been closed. */
    };

    /** epoll based event queue */
    class EventQueue {

        struct Private {
            explicit Private() = default;
        };

    public:
        static EventQueue& coreInstance();

        static EventQueue create();

        explicit EventQueue(Private);

        int exec();
        void stop(int value = 0);

        [[nodiscard]] bool running() const;

        void add(int fd, std::function<void(cb&)> reg = {});
        void mod(int fd, std::function<void(cb&)> reg);
        void del(int fd);

    private:
        enum class State {
            Idle, /**< event queue is not running because it has not been started (exec not been called) */
            Running, /**< event queue is running and no stop is triggered */
            Stopping, /**< event queue is running and a stop is triggered */
            Stopped, /**< event queue is not running because it has been stopped */
            Error /**< event queue is not running because it of an error */
        };

        cmn::fd m_epfd;
        cmn::fd m_evtfd;
        std::map<int, cb> m_fds;
        std::atomic<State> m_state;
        std::atomic_int m_exitCode{1};
    };
} // namespace suc::epl

#endif // SUC_EPL_EVENTQUEUE_HXX
