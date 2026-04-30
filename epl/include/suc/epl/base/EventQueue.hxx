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

#include "suc/cmn/Fd.hxx"
#include <atomic>
#include <functional>
#include <map>

/* epoll */
namespace suc::epl
{
struct Callbacks
{
    std::function<void()>
        inputAvailable {}; /**< The associated file is available for read(2) operations. */
    std::function<void()>
        outputPossible {}; /**< The associated file is available for write(2) operations. */
    std::function<void()>
        priorityData {}; /**< There is an exceptional condition on the file descriptor. */
    std::function<void()> readSideHangUp {}; /**< Stream socket peer closed connection, or shut down
                                                writing half of connection. */
    std::function<void()> hangUp {}; /**< Hang up happened on the associated file descriptor. */
    std::function<void()>
        errorOccurred {}; /**< Error condition happened on the associated file descriptor. This
event is also reported for the write end of a pipe when the read end has been closed. */
};

/** epoll based event queue */
class EventQueue
{

    struct Private
    {
        explicit Private() = default;
    };

public:
    /**
     * @brief Returns the process-wide singleton EventQueue.
     *
     * The instance is a Meyers singleton (function-local static) and is destroyed when the
     * program exits. Callers must ensure that no other static-lifetime object (i.e. objects
     * with static storage duration) calls this function from their destructor, as the singleton
     * may already have been destroyed by the time those destructors run, leading to
     * undefined behaviour.
     */
    static EventQueue& coreInstance();

    static EventQueue create();

    explicit EventQueue(Private);

    int  exec();
    void stop(int value = 0);

    [[nodiscard]] bool running() const;

    void add(int fd, const std::function<void(Callbacks&)>& callbacks = {});
    void mod(int fd, const std::function<void(Callbacks&)>& callbacks);

    /**
     * @brief Removes a file descriptor from the event queue.
     *
     * If there are pending events, they will not be processed anymore.
     *
     * @param fd the file descriptor that should not be used for event handling anymore
     */
    void del(int fd);

    void delSafe(int fd) noexcept;

private:
    void handleEpollFilledEvent(uint32_t events, int fd);

    enum class State
    {
        Idle, /**< event queue is not running because it has not been started (exec not been called)
               */
        Running,  /**< event queue is running and no stop is triggered */
        Stopping, /**< event queue is running and a stop is triggered */
        Stopped,  /**< event queue is not running because it has been stopped */
        Error     /**< event queue is not running because it of an error */
    };

    cmn::Fd                  m_epfd;
    cmn::Fd                  m_evtfd;
    std::map<int, Callbacks> m_fds;
    std::atomic<State>       m_state;
    std::atomic_int          m_exitCode {1};
};
} // namespace suc::epl

#endif // SUC_EPL_EVENTQUEUE_HXX
