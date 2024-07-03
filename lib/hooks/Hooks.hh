#pragma once

#include <cstdint>

#include <core/Common.hh>
#include <memory/Memory.hh>

// Internal hook types
#include "Jal.hh"
#include "Jmp.hh"
#include "Opcode.hh"

#define HOOK(type, address, function, ...)                                     \
    {                                                                          \
        static InternalHooks::func_pointer_type_v<__VA_ARGS__> OrigFunc;       \
        InternalHooks::type::RegisterHook<function<OrigFunc>> (                \
            (void *) GameAddress<address>::Get (), OrigFunc);                  \
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
        > ((void *) GameAddress<address>::Get (), OrigFunc);                   \
    }
