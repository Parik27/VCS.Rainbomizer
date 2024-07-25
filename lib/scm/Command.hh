#pragma once

#include "core/Logger.hh"
#include "memory/GameAddress.hh"
#include "scm/Opcodes.hh"
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

template <size_t N>
constexpr auto
EncodeArgument (const std::array<uint8_t, N> &arr)
{
    return arr;
}

constexpr auto
EncodeArgument (Global global)
{
    return std::array<uint8_t, 2>{uint8_t ((global.idx >> 8) + 0xCD),
                                  uint8_t (global.idx & 0xFF)};
}

constexpr auto
EncodeArgument (Local local)
{
    return std::array<uint8_t, 2>{uint8_t ((local.idx >> 8) + 0x6D),
                                  uint8_t (local.idx & 0xFF)};
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

/* This class is not thread safe. Only use it from the game script thread */
class CommandCaller
{
    inline static constinit CRunningScript callerThread;

public:
    template <uint16_t Opcode, typename... Args>
    static void
    Call (Args &&...args)
    {
        auto buffer
            = GenerateCommandBuffer<Opcode> (std::forward<Args> (args)...);
        for (auto i : buffer)
            {
                if (i == 0)
                    break;
            }

        // Smart compiler optimizes away assignment to an integer if I just
        // store it in integer current ip.
        callerThread.m_pCurrentIpPtr = buffer.data ();
        callerThread.m_pCurrentIP -= int32_t (CTheScripts::ScriptSpace.Get ());

        callerThread.ProcessOneCommand ();
    }

    template <typename T>
    static T &
    GetLocal (Local local)
    {
        return *(T*) &callerThread.GetLocalVariable (local.idx);
    }

    static bool
    GetResult ()
    {
        return callerThread.m_bCondResult;
    }

    template <int32_t Size> class BackedUpScriptArgs
    {
        std::array<uint32_t, Size> m_backup;

    public:
        BackedUpScriptArgs ()
        {
            for (size_t i = 0; i < Size; i++)
                m_backup[i] = CTheScripts::ScriptParams[i];
        }

        ~BackedUpScriptArgs ()
        {
            for (size_t i = 0; i < Size; i++)
                CTheScripts::ScriptParams[i] = m_backup[i];
        }
    };
};

template <uint16_t Opcode, bool BackupScriptArgs = false, typename... Args>
void
CallCommand (Args &&...args)
{
    if constexpr (BackupScriptArgs)
        {
            CommandCaller::BackedUpScriptArgs<sizeof...(Args)> backup;
            CommandCaller::Call<Opcode> (std::forward<Args> (args)...);
        }
    else
        CommandCaller::Call<Opcode> (std::forward<Args> (args)...);
}

template <size_t NumReturn, typename Class, typename... Params>
class ScriptCommandHook
{
    inline static int (*OriginalCommand) (CRunningScript *);
public:
    static void
    Call (Params... params)
    {
        auto script           = CTheScripts::CurrentScript;
        auto buffer           = concatenate (GenerateArgumentBuffer (params)...,
                                             std::array<uint8_t, NumReturn * 3>{0});
        auto returnParamsAddr = buffer.data () + buffer.size () - NumReturn * 3;

        memcpy (returnParamsAddr,
                &CTheScripts::ScriptSpace[script->m_pCurrentIP], NumReturn * 3);

        auto prevIp             = script->m_pCurrentIP;
        script->m_pCurrentIpPtr = buffer.data ();
        script->m_pCurrentIP -= int32_t (CTheScripts::ScriptSpace.Get ());

        auto ipBeforeCall = script->m_pCurrentIP;
        OriginalCommand (script);
        auto bytesToSkip = script->m_pCurrentIP - ipBeforeCall;

        script->m_pCurrentIP
            = prevIp + (bytesToSkip - (buffer.size () - NumReturn * 3));
    }

    template <typename... Args>
    static void
    Return (Args... args)
    {
        auto                               script = CTheScripts::CurrentScript;
        std::array<uint8_t, NumReturn * 3> buffer;

        [args...]<std::size_t... I> (std::index_sequence<I...>) {
            ((GetParam<I, Args> () = args), ...);
        }(std::make_index_sequence<sizeof...(Args)> {});

        memcpy (buffer.data (), &CTheScripts::ScriptSpace[script->m_pCurrentIP],
                NumReturn * 3);

        auto prevIp             = script->m_pCurrentIP;
        script->m_pCurrentIpPtr = buffer.data ();
        script->m_pCurrentIP -= int32_t (CTheScripts::ScriptSpace.Get ());

        auto ipBeforeCall = script->m_pCurrentIP;
        script->StoreParams (&script->m_pCurrentIP, NumReturn);
        auto bytesToSkip = script->m_pCurrentIP - ipBeforeCall;

        script->m_pCurrentIP = prevIp + bytesToSkip;
    }

    template<std::size_t I, typename T>
    static T&
    GetParam ()
    {
        return *reinterpret_cast<T*>(&CTheScripts::ScriptParams[I]);
    }

    template <auto &OriginalCommand>
    static int
    Hook (CRunningScript *script)
    {
        script->CollectParams (&script->m_pCurrentIP, sizeof...(Params),
                               CTheScripts::ScriptParams);

        ScriptCommandHook::OriginalCommand = OriginalCommand;

        []<std::size_t... I> (std::index_sequence<I...>) {
            Class::Impl (GetParam<I, Params> ()...);
        }(std::make_index_sequence<sizeof...(Params)>{});

        return 0;
    }
};

template <typename Class, typename... Params>
class ScriptCommandHook<0, Class, Params...>;
