#pragma once

// define this if you want to get logs if a pattern is used before
// initialisation
#define DEBUG_GAME_ADDRESSES

#ifdef DEBUG_GAME_ADDRESSES
#include <utility>
#include <core/Logger.hh>
#endif

#include <cstdint>
#include <injector.h>

#include "PatternList.hh"

template <uintptr_t pointer>
consteval bool
DoesPatternExist ()
{
    for (auto i : s_Patterns)
        if (i.address == pointer)
            return true;

    return false;
}

/* A class that will store the addresses after pattern resolution and contains
 * helper function to interact with the said addresses */
template <uint32_t Address> class GameAddress
{
    inline static uintptr_t resolvedAddress = Address;

#ifdef DEBUG_GAME_ADDRESSES
    inline static bool resolved                 = false;
    inline static bool unresolvedMessagePrinted = false;
#endif

public:
    static uintptr_t
    Get () requires (DoesPatternExist<Address> ())
    {
#ifdef DEBUG_GAME_ADDRESSES
        if (!resolved && !std::exchange (unresolvedMessagePrinted, true))
            Rainbomizer::Logger::LogMessage (
                "Address %08x used before resolution", Address);
#endif
        return resolvedAddress;
    }

    [[deprecated("Pattern does not exist for address")]]
    static uintptr_t
    Get () requires (!DoesPatternExist<Address> ())
    {
        return resolvedAddress;
    }

    template <typename T>
    static T &
    Read ()
    {
        return *reinterpret_cast<T *> (Get ());
    }

    template <typename T>
    static void
    Write (const T &value)
    {
        Read <T> () = value;
    }

    static void
    SetResolvedAddress (uintptr_t addr)
    {
        resolvedAddress = addr;
#ifdef DEBUG_GAME_ADDRESSES
        resolved = true;
#endif
    }

    static void
    Nop ()
    {
        injector.MakeNOP (Get ());
    }

    static void
    LuiOri (RegisterID reg, float val)
    {
        const uint32_t thing_ieee   = std::bit_cast<uint32_t> (val);
        const uint32_t lower_bytes  = (thing_ieee >> 16) & 0xFFFF;
        const uint32_t higher_bytes = thing_ieee & 0xFFFF;

        injector.WriteMemory32 (Get (), lui (reg, lower_bytes));
        injector.WriteMemory32 (Get () + 4, ori (reg, reg, higher_bytes));
    }

    template<typename ... Args>
    static void
    WriteInstructions (Args ...instructions)
    {
        size_t i = 0;
        (..., (injector.WriteMemory32 (Get () + i, instructions), i += 4));
    }

    GameAddress () = delete;
};

template <uint32_t Address, typename Prototype> class GameFunction;

template <uint32_t Address, typename Ret, typename... Args>
class GameFunction<Address, Ret (Args...)>
{
public:
    inline static Ret
    Call (Args... args)
    {
        return ((Ret (*) (Args...)) (GameAddress<Address>::Get ())) (args...);
    }

    // This can be made static in C++23 for guaranteed inlining
    inline Ret operator () (Args ...args) const
    {
        return Call (args...);
    }
};

template <typename T, uint32_t Address> class GameVariable
{
public:
    static T &
    Get ()
    {
        return GameAddress<Address>::template Read<T> ();
    }

    operator T & () const { return GameAddress<Address>::template Read<T> (); }

    auto operator -> () const {
        return Get();
    }
};

#define GAMEADDR(x) GameAddress<x>::Get ()
