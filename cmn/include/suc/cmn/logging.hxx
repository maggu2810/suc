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

#ifndef SUC_CMN_LOGGING_HXX
#define SUC_CMN_LOGGING_HXX

#include <iostream>
#include <ostream>
#include <format>
#include <valarray>

/*
 * | 0 | EMERGENGY | System is unusable               |
 * | 1 | ALERT     | Action must be taken immediately |
 * | 2 | CRITICAL  | Critical conditions              |
 * | 3 | ERROR     | Error conditions                 |
 * | 4 | WARNING   | Warning conditions               |
 * | 5 | NOTICE    | Normal but significant condition |
 * | 6 | INFO      | Informational                    |
 * | 7 | DEBUG     | Debug-level messages             |
 */

#define LOGD(...) suc::logging::debug(__VA_ARGS__)
#define LOGI(...) suc::logging::info(__VA_ARGS__)
#define LOGN(...) suc::logging::notice(__VA_ARGS__)
#define LOGW(...) suc::logging::warn(__VA_ARGS__)
#define LOGE(...) suc::logging::err(__VA_ARGS__)
#define LOGC(...) suc::logging::crit(__VA_ARGS__)
#define LOGA(...) suc::logging::alert(__VA_ARGS__)
#define LOGU(...) suc::logging::emerg(__VA_ARGS__)

namespace suc::logging {
    enum class level {
        emergency, alert, critical, error, warning, notice, info, debug
    };

    namespace impl {
        inline void log(level lvl, const std::string &str) {
            std::cout << str << std::endl;
        }

        template<class... Args>
        constexpr void log(level lvl, std::format_string<Args...> fmt,
                           Args &&... args) {
            log(lvl, std::format(fmt, std::forward<Args>(args)...));
        }
    }

    template<class... Args>
    constexpr void log(level lvl, std::format_string<Args...> fmt, Args &&... args) {
        impl::log(lvl, fmt, std::forward<Args>(args)...);
    }

    template<class... Args>
    constexpr void emerg(std::format_string<Args...> fmt, Args &&... args) {
        impl::log(level::emergency, fmt, std::forward<Args>(args)...);
    }

    template<class... Args>
    constexpr void alert(std::format_string<Args...> fmt, Args &&... args) {
        impl::log(level::alert, fmt, std::forward<Args>(args)...);
    }

    template<class... Args>
    constexpr void crit(std::format_string<Args...> fmt, Args &&... args) {
        impl::log(level::critical, fmt, std::forward<Args>(args)...);
    }

    template<class... Args>
    constexpr void err(std::format_string<Args...> fmt, Args &&... args) {
        impl::log(level::error, fmt, std::forward<Args>(args)...);
    }

    template<class... Args>
    constexpr void warn(std::format_string<Args...> fmt, Args &&... args) {
        impl::log(level::warning, fmt, std::forward<Args>(args)...);
    }

    template<class... Args>
    constexpr void notice(std::format_string<Args...> fmt, Args &&... args) {
        impl::log(level::notice, fmt, std::forward<Args>(args)...);
    }

    template<class... Args>
    constexpr void info(std::format_string<Args...> fmt, Args &&... args) {
        impl::log(level::info, fmt, std::forward<Args>(args)...);
    }

    template<class... Args>
    constexpr void debug(std::format_string<Args...> fmt, Args &&... args) {
        impl::log(level::debug, fmt, std::forward<Args>(args)...);
    }
}

#endif //SUC_CMN_LOGGING_HXX
