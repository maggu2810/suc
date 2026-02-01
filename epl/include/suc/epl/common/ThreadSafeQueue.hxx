// Copyright [2026] [maggu2810]
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
// Created by maggu2810 on 2/1/26.
//
#ifndef SUC_EPL_THREADSAFEQUEUE_HXX
#define SUC_EPL_THREADSAFEQUEUE_HXX

#include "EventFd.hxx"
#include <deque>
#include <mutex>

namespace suc::epl {
    template <class T>
    class ThreadSafeQueue {
    public:
        explicit ThreadSafeQueue(EventQueue& eventQueue = EventQueue::coreInstance())
            : m_eventFd(false, eventQueue) {}

        void enqueue(T&& ele) {
            std::lock_guard lock(m_mutex);
            m_deque.push_back(std::move(ele));
            m_eventFd.add(1);
        }

        void onElement(std::function<void(T&&)> func) {
            if (!func) {
                m_eventFd.onShot({});
                return;
            }

            m_eventFd.onShot([func = std::move(func), this](std::uint64_t value) {
                for (std::uint64_t i = value; i > 0; --i) {
                    func(dequeue());
                }
            });
        }

    private:
        [[nodiscard]] T dequeue() {
            std::lock_guard lock(m_mutex);
            T ele{std::move(m_deque.front())};
            m_deque.pop_front();
            return ele;
        }

        [[nodiscard]] bool empty() const {
            std::lock_guard lock(m_mutex);
            return m_deque.empty();
        }

    private:
        EventFd m_eventFd;
        mutable std::mutex m_mutex;
        std::deque<T> m_deque;
    };
} // namespace suc::epl

#endif // SUC_EPL_THREADSAFEQUEUE_HXX
