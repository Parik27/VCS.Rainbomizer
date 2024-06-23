#pragma once

#include "core/Logger.hh"
#include "memory/GameAddress.hh"
#include "vcs/CRunningScript.hh"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

struct Global
{
    uint16_t idx;
};

struct Local
{
    uint16_t idx;
};

template <typename Type, std::size_t... sizes>
constexpr auto
concatenate (const std::array<Type, sizes> &...arrays)
{
    std::array<Type, (sizes + ...)> result;
    std::size_t                     index{};

    ((std::copy_n (arrays.begin (), sizes, result.begin () + index),
      index += sizes),
     ...);

    return result;
}

template <uint8_t Opcode, typename T> auto constexpr Encode (const T &arg)
{
    return concatenate (std::array<uint8_t, 1>{Opcode},
                        std::bit_cast<std::array<uint8_t, sizeof (T)>> (arg));
}

template <typename T>
    requires (std::is_arithmetic_v<T> && sizeof (T) <= 4)
constexpr auto
EncodeArgument (T arg)
{
    /* Opcodes:
       7 - 8 bit integer (signed)
       8 - 16 bit integer (signed)
       9 - 32 bit float

       The script engine doesn't care about this, so we just encode all numeric
       type with these opcodes regardless of whether they are actually integer
       or floats.
     */
    return Encode<7 + sizeof (T) / 2> (arg);
}

template <size_t N>
constexpr auto
EncodeArgument (const char (&str)[N])
{
    /* Inline string */

    std::array<uint8_t, std::size (str) + 1> arr;
    arr[0] = 0xA; // opcode
    std::copy_n (str, std::size (str), arr.begin () + 1);
    return arr;
}

constexpr auto
EncodeArgument (Global global)
{
    return std::array<uint8_t, 2>{uint8_t ((global.idx >> 8) + 0xCD),
                                  uint8_t (global.idx & 0xFF)};
}

template <size_t N>
constexpr auto
EncodeArgument (const char *str)
{
    return EncodeArgument (uint32_t (str));
}

template <typename T>
concept IsNonTrivialType = requires { T::Size (); };

template <typename... Args>
constexpr auto
GenerateArgumentBuffer (Args &&...args)
{
    return concatenate (EncodeArgument (args)...);
}

template <uint16_t Opcode, typename... Args>
constexpr auto
GenerateCommandBuffer (Args &&...args)
{
    return concatenate (std::bit_cast<std::array<uint8_t, 2>> (Opcode),
                        EncodeArgument (args)...);
}

template <uint16_t Opcode, typename... Args>
void
CallCommand (Args &&...args)
{
    static constinit CRunningScript caller;
    auto buffer = GenerateCommandBuffer<Opcode> (std::forward<Args> (args)...);
    caller.m_pCurrentIP
        = int32_t (buffer.data () - CTheScripts::ScriptSpace);
    caller.ProcessOneCommand ();
}
