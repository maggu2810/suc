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
#ifndef SUC_CMN_BYTEBUFFER_HXX
#define SUC_CMN_BYTEBUFFER_HXX

#include "StrictAliasing.hxx"
#include "endian.hxx"
#include <bit>
#include <span>
#include <stdexcept>
#include <utility>

template <std::endian endian, bool readOnly>
class ByteBuffer {
    template <typename T>
    struct delegate_type;

    template <typename T>
        requires (readOnly)
    struct delegate_type<T> {
        using type = std::add_const_t<T>;
    };

    template <typename T>
        requires (!readOnly)
    struct delegate_type<T> {
        using type = std::remove_const_t<T>;
    };

    template <typename T>
    using delegate_type_t = delegate_type<T>::type;

    using VoidType         = delegate_type_t<void>;
    using CharType         = delegate_type_t<char>;
    using UnsignedCharType = delegate_type_t<unsigned char>;
    using ByteType         = delegate_type_t<std::byte>;
    using InternalType     = CharType;

public:
    template <std::size_t startPos = 0>
        requires readOnly
    explicit ByteBuffer(const void* data, std::size_t size)
        : m_data{static_cast<InternalType*>(data)},
          m_size{size},
          m_pos{startPos} {}

    template <std::size_t startPos = 0>
        requires (!readOnly)
    explicit ByteBuffer(void* data, std::size_t size)
        : m_data{static_cast<InternalType*>(data)},
          m_size{size},
          m_pos{startPos} {}

    [[nodiscard]] std::size_t position() const {
        return m_pos;
    }

    void setPosition(std::size_t position) {
        if (position > m_size) {
            throw std::runtime_error("buffer overflow exception");
        }
        m_pos = position;
    }

    template <typename T>
        requires std::is_integral_v<T>
    T get() {
        const auto p = increasePos(sizeof(T));
        return suc::cmn::convert<endian, T>(p);
    }

    template <typename DataType = void>
        requires (utils::StrictAliasCharTypes<DataType> || std::same_as<std::decay_t<DataType>, void>)
    delegate_type_t<DataType>* getDataPtr(std::size_t length) {
        using Type = delegate_type_t<DataType>*;
        return reinterpret_cast<Type>(increasePos(length));
    }

    template <std::size_t length, typename DataType = void>
        requires (utils::StrictAliasCharTypes<DataType> || std::same_as<std::decay_t<DataType>, void>)
    std::span<delegate_type_t<DataType>, length> getData() {
        using Type = delegate_type_t<DataType>;
        return std::span<Type, length>(getDataPtr<Type>(length), length);
    }

    template <typename T>
        requires (!readOnly && std::is_integral_v<T>)
    void set(T value) {
        const auto p = increasePos(sizeof(T));
        suc::cmn::convert<endian, T>(p, &value);
    }

    template <typename>
        requires (!readOnly)
    void setData(const void* buffer, std::size_t length) {
        auto p = getData(length);
        std::memcpy(p, buffer, length);
    }

private:
    void requiresRemaining(std::size_t size) const {
        if (m_size - m_pos < size) {
            throw std::runtime_error("buffer overflow exception");
        }
    }

    InternalType* increasePos(std::size_t size) {
        requiresRemaining(size);
        return m_data + std::exchange(m_pos, m_pos + size);
    }

    InternalType* m_data;
    std::size_t m_size;
    std::size_t m_pos{};
};

#endif // SUC_CMN_BYTEBUFFER_HXX
