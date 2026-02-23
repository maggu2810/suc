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
#include <cstring>

namespace suc::cmn {
    namespace impl {
        // We handle little and bit endian only.
        // Currently, there does not exist other endian enumeration values
        // (except native that fits to one of them).
        template <std::endian endian>
        concept ValidEndian = requires(std::endian&& e) { e == std::endian::little || e == std::endian::big; };

        // Ensure that the native one is one of the handled ones.
        static_assert(ValidEndian<std::endian::native>);

        // Directions to use for copy memory. Copy forward or copy reverse.
        enum class Direction { Forward, Reverse };

        // Evaluate directory to use for desired endianness.
        template <std::endian endian>
            requires impl::ValidEndian<endian>
        constexpr Direction direction() {
            if constexpr (std::endian::native == endian) {
                return Direction::Forward;
            } else {
                return Direction::Reverse;
            }
        }

        /**
         * Copy memory from one location to another.
         *
         * @tparam dir the direction. Forward will copy the memory in the same order, reverse will copy the
         * memory in reverse order (0x12 0x34 0x56 will end up to 0x56 0x34 0x12).
         * @param dst the memory location the data will be written to
         * @param src the memory location the data will be read from
         * @param length the number of bytes to copy
         */
        template <Direction dir>
            requires (dir == Direction::Forward || dir == Direction::Reverse)
        constexpr void copy(void* dst, const void* src, std::size_t length) {
            if constexpr (dir == Direction::Forward) {
                // std::copy_n(static_cast<const char*>(src), length, static_cast<char*>(dst));
                std::memcpy(dst, src, length);
            } else {
                std::reverse_copy(
                    static_cast<const char*>(src), static_cast<const char*>(src) + length, static_cast<char*>(dst));
            }
        }

    } // namespace impl

    /**
     * Convert between native and another endianness.
     *
     * @tparam endian the non-native endian
     * @tparam T the type to convert
     * @param dst memory location the converted data will be written to
     * @param src memory location the data will be read
     */
    template <std::endian endian, class T>
        requires (std::integral<T> && impl::ValidEndian<endian>)
    constexpr void convert(void* dst, const void* src) {
        impl::copy<impl::direction<endian>()>(static_cast<char*>(dst), static_cast<const char*>(src), sizeof(T));
    }

    /**
     * Convert between native and another endianness.
     *
     * @tparam endian the non-native endian
     * @tparam T the type to convert
     * @param src the value that should be converted
     */
    template <std::endian endian, class T>
        requires (std::integral<T> && impl::ValidEndian<endian>)
    constexpr T convert(T src) {
        if constexpr (endian == std::endian::native) {
            return src;
        } else {
            return std::byteswap(src);
        }
    }

    /**
     * Convert between native and another endianness.
     *
     * @tparam endian the non-native endian
     * @tparam T the type to convert
     * @param src memory location the data will be read
     * @return the converted data
     */
    template <std::endian endian, class T>
        requires (std::integral<T> && impl::ValidEndian<endian>)
    constexpr T convert(const void* src) {
        T dst;
        impl::copy<impl::direction<endian>()>(&dst, src, sizeof(T));
        return dst;
    }
} // namespace suc::cmn

#endif // SUC_CMN_ENDIAN_HXX
