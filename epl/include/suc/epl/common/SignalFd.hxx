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
// Created by maggu2810 on 1/30/26.
//
#ifndef SUC_EPL_SIGNALFD_HXX
#define SUC_EPL_SIGNALFD_HXX

#include "suc/epl/base/Fd.hxx"
#include <functional>
#include <sys/signalfd.h>

// #error "Never include <bits/signum-generic.h> directly; use <signal.h> instead."
// signals: <bits/signum-generic.h>
// signals: <bits/signum-arch.h>

namespace suc::epl {
    class SignalFd {
    private:
        [[nodiscard]] static sigset_t createSigSet(std::initializer_list<int> signals);

        static void blockSignals(const sigset_t& sigset);

        explicit SignalFd(const sigset_t& sigset, EventQueue& eventQueue = EventQueue::coreInstance());

        explicit SignalFd(const sigset_t& sigset, bool block = true, EventQueue& eventQueue = EventQueue::coreInstance())
            : SignalFd(sigset, eventQueue) {
            if (block) {
                blockSignals(sigset);
            }
        }

    public:
        explicit SignalFd(
            std::initializer_list<int> signals, bool block = true, EventQueue& eventQueue = EventQueue::coreInstance())
            : SignalFd(createSigSet(signals), block, eventQueue) {}

        void onSignal(std::function<void(signalfd_siginfo&&)> func) const;

    private:
        Fd m_fd;
    };
} // namespace suc::epl

#endif // SUC_EPL_SIGNALFD_HXX
