// Copyright [2025] [maggu2810]
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

#include "suc/cmn/runtimeerror_errno.hxx"

#include "suc/cmn/to_string.hxx"
#include <cstring>
#include <format>

namespace {
    std::string genmsg(const std::string_view msg, const int errnum) {
        return std::format("{} [{}]", msg, suc::cmn::strerrnum(errnum));
    }
} // namespace

namespace suc::cmn {
    runtimeerror_errno::runtimeerror_errno(const std::string_view msg, const int errnum)
        : std::runtime_error{genmsg(msg, errnum)} {}
} // namespace suc::cmn
