#pragma once

#include "vcs/CRunningScript.hh"
#include <injector.h>

#include <core/Logger.hh>
#include <scm/Opcodes.hh>

namespace InternalHooks {

namespace Opcode {

template <eCommandId Opcode> class OpcodeAddress
{
public:
    static constexpr eCommandId
    Get ()
    {
        return Opcode;
    }
};

template <auto hookedFunc, typename O>
inline void
RegisterHook (void *addr, O &originalFunc)
{
    Rainbomizer::Logger::LogMessage ("%p", &CTheScripts::ScriptCommands);
    Rainbomizer::Logger::LogMessage ("%p",
                                     &CTheScripts::ScriptCommands[int (addr)]);
    auto &command   = CTheScripts::ScriptCommands[int (addr)];
    originalFunc    = (O) command.handler;
    command.handler = hookedFunc;
}

template <auto... Args> using AddressT = OpcodeAddress<Args...>;

}; // namespace Opcode
}; // namespace InternalHooks
