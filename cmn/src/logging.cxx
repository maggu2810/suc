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

#include "suc/cmn/logging.hxx"

#define SD_JOURNAL_SUPPRESS_LOCATION
#include <systemd/sd-journal.h>

namespace suc::logging::impl
{
void log(Level level, const std::string& message)
{
    sd_journal_send("MESSAGE=%s", message.c_str(), "PRIORITY=%i", std::to_underlying(level), NULL);
}

void log(Level level, const std::string& message, const std::source_location& sloc)
{
    sd_journal_send("MESSAGE=%s",
                    message.c_str(),
                    "PRIORITY=%i",
                    std::to_underlying(level),
                    "CODE_COLUMN=%" PRIuLEAST32,
                    sloc.column(),
                    "CODE_LINE=%" PRIuLEAST32,
                    sloc.line(),
                    "CODE_FUNC=%s",
                    sloc.function_name(),
                    "CODE_FILE=%s",
                    sloc.file_name(),
                    NULL);
}

} // namespace suc::cmn::logging::impl