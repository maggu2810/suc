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
// Created by maggu2810 on 2/23/26.
//
#ifndef SUC_CMN_STRICTALIASING_HXX
#define SUC_CMN_STRICTALIASING_HXX

#include <concepts>
#include <cstddef>

namespace utils {
    // Define a concept for the types that pointer can hold other pointers without breaking strict
    // aliasing.
    template <typename T>
    concept StrictAliasCharTypes = std::same_as<std::decay_t<T>, char> || std::same_as<std::decay_t<T>, unsigned char>
                                || std::same_as<std::decay_t<T>, std::byte>;
} // namespace utils

#endif // SUC_CMN_STRICTALIASING_HXX
