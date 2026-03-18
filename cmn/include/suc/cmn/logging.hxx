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

#define FWD(x) std::forward<decltype(x)>(x)

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
struct FormatStringWithSourceLocation
{
    template<typename T>
    requires std::constructible_from<std::format_string<Args...>, const T&>
    consteval FormatStringWithSourceLocation(
        const T&             fmt,
        std::source_location sloc = std::source_location::current())
        : fmt {fmt}, sloc {sloc}
    {
    }

    std::format_string<Args...> fmt;
    std::source_location        sloc;
};

void logf(Level level, const std::string& message, const std::optional<std::source_location>& sloc);

template<Level level = Level::Info, class... Args>
constexpr void logf(FormatStringWithSourceLocation<Args...>&& fmt, Args&&... args)
{
    logf(level, std::format(FWD(fmt.fmt), FWD(args)...), FWD(fmt.sloc));
}

template<Level level = Level::Info, class... Args>
constexpr void logf(std::format_string<Args...>&& fmt, Args&&... args)
{
    logf(level, std::format(FWD(fmt), FWD(args)...), std::nullopt);
}
} // namespace impl

template<typename... Args>
using FormatStringWithSourceLocation =
    impl::FormatStringWithSourceLocation<std::type_identity_t<Args>...>;
template<typename... Args>
using FormatStringWithoutSourceLocation = std::format_string<std::type_identity_t<Args>...>;

template<typename... Args>
using FormatString =
#if SUC_LOGGING_SLOC
    FormatStringWithSourceLocation
#else
    FormatStringWithoutSourceLocation
#endif
    <std::type_identity_t<Args>...>;

template<Level level = Level::Info, class... Args>
constexpr void logf(FormatString<Args...>&& fmt, Args&&... args)
{
    impl::logf(FWD(fmt), FWD(args)...);
}
} // namespace suc::logging

#define LOGFD(...) suc::logging::logf<suc::logging::Level::Debug>(__VA_ARGS__)
#define LOGFI(...) suc::logging::logf<suc::logging::Level::Info>(__VA_ARGS__)
#define LOGFN(...) suc::logging::logf<suc::logging::Level::Notice>(__VA_ARGS__)
#define LOGFW(...) suc::logging::logf<suc::logging::Level::Warning>(__VA_ARGS__)
#define LOGFE(...) suc::logging::logf<suc::logging::Level::Error>(__VA_ARGS__)
#define LOGFC(...) suc::logging::logf<suc::logging::Level::Critical>(__VA_ARGS__)
#define LOGFA(...) suc::logging::logf<suc::logging::Level::Alert>(__VA_ARGS__)
#define LOGFU(...) suc::logging::logf<suc::logging::Level::Emergency>(__VA_ARGS__)

#undef FWD

#endif // SUC_LOGGING_HXX
