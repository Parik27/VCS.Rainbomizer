#include "injector.h"
#include "patterns.h"
#include "log.h"
#include <utility>

//

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
      injector.MakeJMP (uintptr_t(addr), uintptr_t (hookedFunc));
    else
      injector.MakeJAL (uintptr_t(addr), uintptr_t (hookedFunc));
}

template <bool Jmp = false, typename F, typename O>
inline void
RegisterHook (void *addr, O &originalFunc, F hookedFunc)
{
    ReadCall (addr, originalFunc);
    RegisterHook<Jmp> (addr, hookedFunc);

    logger.WriteF("Hooking function: %p -> %p at address %p", originalFunc, hookedFunc, addr);
}

template <bool Jmp = false, typename F, typename O>
inline void
RegisterHook (const char *patternStr, int offset, O &originalFunc, F hookedFunc)
{
    RegisterHook<Jmp> ((void *) pattern.get_first (patternStr, offset),
                       originalFunc, hookedFunc);
}

/*******************************************************/
#define REGISTER_HOOK_ADDR(address, function, ret, ...)                        \
    {                                                                          \
        static ret (*OrigFunc) (__VA_ARGS__);                                  \
        RegisterHook ((void *) address, OrigFunc, function<OrigFunc>);         \
    }

#define REGISTER_HOOK(pattern, offset, function, ret, ...)                     \
    {                                                                          \
        static ret (*OrigFunc) (__VA_ARGS__);                                  \
        RegisterHook (pattern, offset, OrigFunc, function<OrigFunc>);          \
    }

#define REGISTER_HOOK_MEMBER(pattern, offset, function, ret, ...)              \
    {                                                                          \
        static ret (*OrigFunc) (__VA_ARGS__);                                  \
        RegisterHook (                                                         \
            pattern, offset, OrigFunc,                                         \
            (decltype (OrigFunc)) []<typename... Args> (Args... args) {        \
                return Get ().function<OrigFunc> (                             \
                    std::forward<decltype (args)> (args)...);                  \
            });                                                                \
    }

#define REGISTER_HOOK_MEMBER_ADDR(address, function, ret, ...)                 \
    {                                                                          \
        static ret (*OrigFunc) (__VA_ARGS__);                                  \
        RegisterHook ((void *) address, OrigFunc,                              \
                      (decltype (OrigFunc)) []<typename... Args> (             \
                          Args... args) {                                      \
                          return Get ().function<OrigFunc> (                   \
                              std::forward<decltype (args)> (args)...);        \
                      });                                                      \
    }
