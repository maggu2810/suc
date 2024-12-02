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

#include <iostream>

#include <suc/cmn/logging.hxx>
#include <suc/cmn/endian.hxx>

int main(int argc, char *argv[]) {
    std::uint32_t value = 0x12345678;
    std::uint32_t le = suc::cmn::littleendian(value);
    std::uint32_t be = suc::cmn::bigendian(value);
    std::cout << std::format("value: {:#x}, le: {:#x}, be: {:#x}\n", value, le, be);

    value = 0x89ABCDEF;
    suc::cmn::littleendian<std::uint32_t>(&value, &le);
    suc::cmn::bigendian<std::uint32_t>(&value, &be);
    std::cout << std::format("value: {:#x}, le: {:#x}, be: {:#x}\n", value, le, be);

    value = 0xDEADBEEF;
    le = suc::cmn::littleendian<decltype(le)>(&value);
    be = suc::cmn::bigendian<decltype(le)>(&value);
    std::cout << std::format("value: {:#x}, le: {:#x}, be: {:#x}\n", value, le, be);

    value = 0xDEADBEEF;
    const std::uint32_t le2 = suc::cmn::littleendian<std::decay_t<decltype(le2)>>(&value);
    const auto le3 = suc::cmn::littleendian<std::uint32_t>(&value);
    be = suc::cmn::bigendian<decltype(le)>(&value);
    std::cout << std::format("value: {:#x}, le2: {:#x}, le3: {:#x}\n", value, le2, le3);

    return EXIT_SUCCESS;
}
