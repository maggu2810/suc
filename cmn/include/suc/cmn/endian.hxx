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

//
// Created by maggu2810 on 11/26/24.
//
#ifndef SUC_CMN_ENDIAN_HXX
#define SUC_CMN_ENDIAN_HXX

#include <algorithm>
#include <bit>

namespace suc::cmn {
    namespace impl {
        static_assert(std::endian::native == std::endian::little || std::endian::native == std::endian::big);

        template <typename T>
        constexpr T fcopy(T value) {
            return value;
        }

        template <typename T>
        constexpr T rcopy(T value) {
            return std::byteswap(value);
        }

        template <typename T>
        constexpr T fcopy(const void* in) {
            T out{};
            auto i = static_cast<const std::uint8_t*>(in);
            auto o = reinterpret_cast<std::uint8_t*>(&out);
            std::copy_n(i, sizeof(T), o);
            return out;
        }

        template <typename T>
        constexpr T rcopy(const void* in) {
            T out{};
            auto i = static_cast<const std::uint8_t*>(in);
            auto o = reinterpret_cast<std::uint8_t*>(&out);
            std::reverse_copy(i, i + sizeof(T), o);
            return out;
        }

        template <typename T>
        constexpr void fcopy(const void* in, void* out) {
            auto i = static_cast<const std::uint8_t*>(in);
            auto o = static_cast<std::uint8_t*>(out);
            std::copy_n(i, sizeof(T), o);
        }

        template <typename T>
        constexpr void rcopy(const void* in, void* out) {
            auto i = static_cast<const std::uint8_t*>(in);
            auto o = static_cast<std::uint8_t*>(out);
            std::reverse_copy(i, i + sizeof(T), o);
        }
    } // namespace impl

    template <typename T>
        requires std::integral<T>
    constexpr T bigendian(T value) {
        if constexpr (std::endian::native == std::endian::little) {
            return impl::rcopy<T>(value);
        } else {
            return impl::fcopy<T>(value);
        }
    }

    template <typename T>
        requires std::integral<T>
    constexpr T bigendian(const void* in) {
        if constexpr (std::endian::native == std::endian::little) {
            return impl::rcopy<T>(in);
        } else {
            return impl::fcopy<T>(in);
        }
    }

    template <typename T>
        requires std::integral<T>
    constexpr void bigendian(const void* in, void* out) {
        if constexpr (std::endian::native == std::endian::little) {
            impl::rcopy<T>(in, out);
        } else {
            impl::fcopy<T>(in, out);
        }
    }

    template <typename T>
        requires std::integral<T>
    constexpr T littleendian(T value) {
        if constexpr (std::endian::native == std::endian::little) {
            return impl::fcopy<T>(value);
        } else {
            return impl::rcopy<T>(value);
        }
    }

    template <typename T>
        requires std::integral<T>
    constexpr T littleendian(const void* in) {
        if constexpr (std::endian::native == std::endian::little) {
            return impl::fcopy<T>(in);
        } else {
            return impl::rcopy<T>(in);
        }
    }

    template <typename T>
    constexpr void littleendian(const void* in, void* out) {
        if constexpr (std::endian::native == std::endian::little) {
            impl::fcopy<T>(in, out);
        } else {
            impl::rcopy<T>(in, out);
        }
    }

} // namespace suc::cmn

#endif // SUC_CMN_ENDIAN_HXX
