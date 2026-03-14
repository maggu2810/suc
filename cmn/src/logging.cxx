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
// Created by maggu2810 on 3/13/26.
//

#if LINK_SYSTEMD
#define SD_JOURNAL_SUPPRESS_LOCATION
#include <systemd/sd-journal.h>
#endif

#include "suc/cmn/logging.hxx"

#include <string>
#include <sys/uio.h>
#include <vector>

#if not LINK_SYSTEMD
#include <dlfcn.h>

namespace
{
using sd_journal_sendv_t = int (*)(const struct iovec* iov, int n);

int sd_journal_sendv_fallback(const iovec* iov, int n)
{
    printf("[\n");
    for (int i = 0; i < n; i++)
    {
        const auto& io = iov[i];
        printf("  %.*s\n", static_cast<int>(io.iov_len), static_cast<const char*>(io.iov_base));
    }
    printf("]\n");
    return 0;
}

sd_journal_sendv_t sd_journal_sendv_resolve()
{
    void* handle {nullptr};
    for (const char* libname : std::initializer_list {"libsystemd.so.0", "libsystemd.so"})
    {
        handle = dlopen(libname, RTLD_NOW | RTLD_LOCAL);
        if (handle)
        {
            break;
        }
    }
    if (!handle)
    {
        return &sd_journal_sendv_fallback;
    }

    dlerror(); // clear
    auto        sym = dlsym(handle, "sd_journal_sendv");
    const char* err = dlerror();
    if (err)
    {
        dlclose(handle);
        return &sd_journal_sendv_fallback;
    }

    auto sym_sd_journal_sendv = reinterpret_cast<sd_journal_sendv_t>(sym);
    return sym_sd_journal_sendv;
    // dlclose(handle); // handle must not be closed
}

int sd_journal_sendv(const iovec* iov, int n)
{
    static sd_journal_sendv_t targetFunc = sd_journal_sendv_resolve();
    return targetFunc(iov, n);
}
} // namespace
#endif

namespace suc::logging::impl
{
void log(Level                                      level,
         const std::string&                         message,
         const std::optional<std::source_location>& slocOpt)
{
    std::vector<std::string> fields;
    fields.reserve(slocOpt ? 6 : 2);
    fields.emplace_back(std::format("MESSAGE={}", message));
    fields.emplace_back(std::format("PRIORITY={}", std::to_underlying(level)));
    if (slocOpt)
    {
        const std::source_location& sloc = *slocOpt;
        fields.emplace_back(std::format("CODE_COLUMN={}", sloc.column()));
        fields.emplace_back(std::format("CODE_LINE={}", sloc.line()));
        fields.emplace_back(std::format("CODE_FUNC={}", sloc.function_name()));
        fields.emplace_back(std::format("CODE_FILE={}", sloc.file_name()));
    }

    std::vector<iovec> iov;
    iov.reserve(fields.size());
    for (auto& s : fields)
    {
        iov.emplace_back(s.data(), s.size());
    }

    std::ignore = sd_journal_sendv(iov.data(), static_cast<int>(iov.size()));
}

} // namespace suc::logging::impl