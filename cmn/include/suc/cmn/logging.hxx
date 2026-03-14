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

#ifndef SUC_LOGGING_HXX
#define SUC_LOGGING_HXX

#include <print>
#include <source_location>
#include <syslog.h>
#include <utility>

#ifndef SUC_LOGGING_SLOC
#define SUC_LOGGING_SLOC 0
#else
#if SUC_LOGGING_SLOC
#undef SUC_LOGGING_SLOC
#define SUC_LOGGING_SLOC 1
#else
#undef SUC_LOGGING_SLOC
#define SUC_LOGGING_SLOC 0
#endif
#endif

namespace suc::logging
{
enum class Level : decltype(LOG_INFO)
{
    Emergency = /* 0 | EMERGENGY | System is unusable               */ LOG_EMERG,
    Alert     = /* 1 | ALERT     | Action must be taken immediately */ LOG_ALERT,
    Critical  = /* 2 | CRITICAL  | Critical conditions              */ LOG_CRIT,
    Error     = /* 3 | ERROR     | Error conditions                 */ LOG_ERR,
    Warning   = /* 4 | WARNING   | Warning conditions               */ LOG_WARNING,
    Notice    = /* 5 | NOTICE    | Normal but significant condition */ LOG_NOTICE,
    Info      = /* 6 | INFO      | Informational                    */ LOG_INFO,
    Debug     = /* 7 | DEBUG     | Debug-level messages             */ LOG_DEBUG
};

namespace impl
{
template<typename... Args>
struct FormatStringWithOptSourceLocation
{
    template<typename T>
    requires std::constructible_from<std::format_string<Args...>, const T&>
    consteval FormatStringWithOptSourceLocation(const T&                            fmt,
                                                std::optional<std::source_location> sloc =
#if SUC_LOGGING_SLOC
                                                    std::source_location::current()
#else
                                                    std::nullopt
#endif
                                                    )
        : fmt {fmt}, sloc {sloc}
    {
    }

    std::format_string<Args...>         fmt;
    std::optional<std::source_location> sloc;
};

void log(Level level, const std::string& message, const std::optional<std::source_location>& sloc);

template<Level level = Level::Info, class... Args>
constexpr void log(std::optional<std::source_location>&& sloc,
                   std::format_string<Args...>&&         fmt,
                   Args&&... args)
{
    impl::log(
        level,
        std::format(std::forward<std::format_string<Args...>>(fmt), std::forward<Args>(args)...),
        sloc);
}
} // namespace impl

template<typename... Args>
using FormatStringWithOptSourceLocation =
    impl::FormatStringWithOptSourceLocation<std::type_identity_t<Args>...>;

template<Level level = Level::Info, class... Args>
constexpr void log(FormatStringWithOptSourceLocation<Args...>&& fmt, Args&&... args)
{
    impl::log(std::forward<std::optional<std::source_location>>(fmt.sloc),
              std::forward<std::format_string<Args...>>(fmt.fmt),
              std::forward<Args>(args)...);
}

} // namespace suc::logging

#define LOGFD(...) suc::logging::log<suc::logging::Level::Debug>(__VA_ARGS__)
#define LOGFI(...) suc::logging::log<suc::logging::Level::Info>(__VA_ARGS__)
#define LOGFN(...) suc::logging::log<suc::logging::Level::Notice>(__VA_ARGS__)
#define LOGFW(...) suc::logging::log<suc::logging::Level::Warning>(__VA_ARGS__)
#define LOGFE(...) suc::logging::log<suc::logging::Level::Error>(__VA_ARGS__)
#define LOGFC(...) suc::logging::log<suc::logging::Level::Critical>(__VA_ARGS__)
#define LOGFA(...) suc::logging::log<suc::logging::Level::Alert>(__VA_ARGS__)
#define LOGFU(...) suc::logging::log<suc::logging::Level::Emergency>(__VA_ARGS__)

#endif // SUC_LOGGING_HXX
