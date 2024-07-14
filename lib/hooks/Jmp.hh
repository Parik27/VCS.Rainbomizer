#pragma once

#include <core/Logger.hh>
#include <memory/Memory.hh>

#include <injector.h>
#include <mips.h>

#include <cstdint>
#include <utility>

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
template <uint64_t **OrigAddr, uint64_t *OrigBytes, uint64_t *HookedBytes,
          auto func>
struct wrapper_t;

template <uint64_t **OrigAddr, uint64_t *OrigBytes, uint64_t *HookedBytes,
          typename Ret, typename... Ts, Ret (*func) (Ts...)>
struct wrapper_t<OrigAddr, OrigBytes, HookedBytes, func>
{
#ifdef USE_MUTEX_IN_JMP_HOOKS
    inline static std::mutex m_mutex;
#endif

    static Ret
    functor (Ts... args)
    {
#ifdef USE_MUTEX_IN_JMP_HOOKS
        std::lock_guard lock (m_mutex);
#endif

        **OrigAddr = *OrigBytes;

        if constexpr (std::is_void_v<Ret>)
            {
                func (std::forward<Ts> (args)...);
            }
        else
            {
                auto ret                 = func (std::forward<Ts> (args)...);
                **(uint64_t **) OrigAddr = *HookedBytes;
                return ret;
            }

        **OrigAddr = *HookedBytes;
    }
};

template <auto hookedFunc, typename O>
void
RegisterHook (void *addr, O &originalFunc)
{
#ifdef OLD_JMP_METHOD
    static uint64_t *OrigAddr;
    static uint64_t  OrigBytes;
    static uint64_t  HookedBytes;

    originalFunc = (O) addr;

    OrigAddr  = (uint64_t *) originalFunc;
    OrigBytes = *OrigAddr;

    // need to nop because of delay slot nonsense
    injector.MakeJMPwNOP (uintptr_t (addr),
                          uintptr_t (
                              wrapper_t<&OrigAddr, &OrigBytes, &HookedBytes,
                                        hookedFunc>::functor));

    HookedBytes = *OrigAddr;
    Rainbomizer::Logger::LogMessage ("%p %llx %llx -> %p", OrigAddr, OrigBytes,
                                     HookedBytes);
#else

    originalFunc = (O) addr;
    MemoryManager::Get ().FindInstruction (
        jal (uint32_t (addr)), [] (uint32_t addr) {
            Rainbomizer::Logger::LogMessage ("[JMP] Hooking %p -> %p", addr,
                                             hookedFunc);
            injector.MakeJAL (addr, uintptr_t (hookedFunc));
        });
#endif
}

template <auto... Args> using AddressT = GameAddress<Args...>;

}; // namespace Jmp

}; // namespace InternalHooks
