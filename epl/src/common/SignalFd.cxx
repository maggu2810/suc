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

#include "suc/cmn/runtimeerror_errno.hxx"
#include "suc/epl/common/SignalFd.hxx"
#include <csignal>

namespace suc::epl {
    [[nodiscard]] sigset_t SignalFd::createSigSet(std::initializer_list<int> signals) {
        sigset_t sigset;
        if (sigemptyset(&sigset) != 0) {
            throw suc::cmn::runtimeerror_errno("sigemptyset failed");
        }
        for (const auto& sig : signals) {
            if (sigaddset(&sigset, sig) != 0) {
                throw suc::cmn::runtimeerror_errno("sigaddset failed");
            }
        }
        return sigset;
    }

    void SignalFd::blockSignals(const sigset_t& sigset) {
        if (sigprocmask(SIG_BLOCK, &sigset, nullptr) != 0) {
            throw suc::cmn::runtimeerror_errno("sigprocmask failed");
        }
    }

    SignalFd::SignalFd(const sigset_t& sigset, EventQueue& eventQueue)
        : m_fd{suc::cmn::fd::make_or_rteeno(signalfd(-1, &sigset, SFD_NONBLOCK | SFD_CLOEXEC)), eventQueue} {}

    void SignalFd::onSignal(std::function<void(signalfd_siginfo&&)> func) const {
        if (!func) {
            m_fd.onInputAvailable({});
            return;
        }

        m_fd.onInputAvailable([func = std::move(func), this]() {
            signalfd_siginfo info[32];
            const auto rv_read = read(m_fd, info, sizeof(info));
            if (rv_read > 0) {
                for (int i = 0; i < rv_read / sizeof(*info); ++i) {
                    func(std::move(info[i]));
                }
            } else {
                if (rv_read == 0) {
                    throw suc::cmn::runtimeerror_errno("read of signalfd returns 0");
                } else {
                    throw suc::cmn::runtimeerror_errno("read of signalfd failed");
                }
            }
        });
    }
} // namespace suc::epl
