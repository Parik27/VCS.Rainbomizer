#pragma once

#include "injector.h"
#include "patterns.h"
#include "core/Logger.hh"
#include <utility>

// Replaces an entire function with another
namespace InternalHooks {

template <typename prototype> struct func_pointer_type_t;

template <typename Ret, typename... Args>
struct func_pointer_type_t<Ret (Args...)>
{
    using type = Ret (*) (Args...);
};

template <typename... Args>
using func_pointer_type_v = func_pointer_type_t<Args...>::type;

namespace Jmp {
template <int **OrigAddr, int *OrigBytes, int *HookedBytes, auto func>
struct wrapper_t;

template <int **OrigAddr, int *OrigBytes, int *HookedBytes, typename Ret,
          typename... Ts, Ret (*func) (Ts...)>
struct wrapper_t<OrigAddr, OrigBytes, HookedBytes, func>
{
    static Ret
    functor (Ts... args)
    {
        **OrigAddr = *OrigBytes;
        if constexpr (std::is_void_v<Ret>)
            {
                func (std::forward<Ts> (args)...);
            }
        else
            {
                auto ret            = func (std::forward<Ts> (args)...);
                **(int **) OrigAddr = *HookedBytes;
                return ret;
            }

        **OrigAddr = *HookedBytes;
    }
};

template <auto hookedFunc, typename O>
void
RegisterHook (void *addr, O &originalFunc)
{
    static int *OrigAddr;
    static int  OrigBytes;
    static int  HookedBytes;
    originalFunc = (O) addr;

    OrigAddr  = (int *) originalFunc;
    OrigBytes = *OrigAddr;

    injector.MakeJMP (uintptr_t (addr),
                      uintptr_t (wrapper_t<&OrigAddr, &OrigBytes, &HookedBytes,
                                           hookedFunc>::functor));

    HookedBytes = *OrigAddr;
}
}; // namespace Jmp

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

inline void *
ResolveAddress (uintptr_t addr)
{
    return (void *) addr;
}

inline void *
ResolveAddress (const char *pattern_s, int offset)
{
    return (void *) pattern.get_first (pattern_s, offset);
}

#define HOOK(type, address, function, ...)                                     \
    {                                                                          \
        static InternalHooks::func_pointer_type_v<__VA_ARGS__> OrigFunc;       \
        InternalHooks::type::RegisterHook<function<OrigFunc>> (                \
            ResolveAddress address, OrigFunc);                                 \
    }

#define HOOK_MEMBER(type, address, function, ...)                              \
    {                                                                          \
        static InternalHooks::func_pointer_type_v<__VA_ARGS__> OrigFunc;       \
        InternalHooks::type::RegisterHook                                      \
            < (decltype (OrigFunc)) []<typename... Args> (Args... args)        \
        {                                                                      \
            return Get ().function<OrigFunc> (                                 \
                std::forward<decltype (args)> (args)...);                      \
        }                                                                      \
        > (ResolveAddress address, OrigFunc);                                  \
    }
