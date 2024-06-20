#pragma once

#include <injector.h>

#include <core/Logger.hh>

namespace InternalHooks {

namespace Jal {

template <typename Func, typename Addr>
inline void
ReadCall (Addr address, Func &func)
{
    func = (Func) injector.GetBranchDestination (uintptr_t (address));
}

template <bool Jmp = false, typename F>
inline void
RegisterHook (void *addr, F hookedFunc)
{
    if constexpr (Jmp)
        injector.MakeJMP (uintptr_t (addr), uintptr_t (hookedFunc));
    else
        injector.MakeJAL (uintptr_t (addr), uintptr_t (hookedFunc));
}

template <auto hookedFunc, bool Jmp = false, typename O>
inline void
RegisterHook (void *addr, O &originalFunc)
{
    ReadCall (addr, originalFunc);
    RegisterHook<Jmp> (addr, hookedFunc);

    Rainbomizer::Logger::LogMessage ("Hooking function: %p -> %p at address %p",
                                     originalFunc, hookedFunc, addr);
}
}; // namespace Jal
}; // namespace InternalHooks
