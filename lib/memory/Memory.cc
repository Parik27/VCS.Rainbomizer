#include "Memory.hh"
#include "core/Logger.hh"
#include "memory/GameAddress.hh"
#include "memory/MemorySignature.hh"
#include "memory/Pattern.hh"
#include <cstring>
#include <injector.h>
#include <pspsdk.h>
#include <string_view>
#include <utility>

#define GAME_INTERNAL_NAME "GTA3"

template <std::size_t NUM = 0>
inline void
InitialisePattern (MemoryManager& manager)
{
    using namespace MemorySignature;

    constexpr const auto &currentPattern = s_Patterns[NUM];

    static constinit const Signature<GetMemorySignatureSize (
        currentPattern.pattern_str)>
        sig{currentPattern.pattern_str};

    auto addr = manager.SignatureSearch (sig) + currentPattern.offset;

    if constexpr (currentPattern.resolver == Pattern::BRANCH)
        {
            addr = injector.GetBranchDestination (addr);
        }
    if constexpr (currentPattern.resolver == Pattern::GPOFFSET)
        {
            addr = manager.GetGpAddress () + manager.ReadMemory<int16_t> (addr);
        }
    if constexpr (currentPattern.resolver == Pattern::UPPER_AND_ADD)
        {
            addr = (manager.ReadMemory<uint16_t> (addr) << 16)
                   + manager.ReadMemory<int16_t> (
                       addr + (currentPattern.offset2 - currentPattern.offset));
        }

    GameAddress<currentPattern.address>::SetResolvedAddress (addr);

    Rainbomizer::Logger::LogMessage (
        "[PATTERN] Resolved address %x to %x", currentPattern.address,
        GameAddress<currentPattern.address>::Get ());
}

void
MemoryManager::InitialiseAllPatterns ()
{
    Rainbomizer::Logger::FunctionBenchmark benchmark;

    [this]<std::size_t... I> (std::index_sequence<I...>) {
        (..., InitialisePattern<I> (*this));
    }(std::make_index_sequence<std::size (s_Patterns)>{});
}

void
MemoryManager::Init ()
{
    if (std::exchange (m_initialised, true))
        return;

    SceUID modules[10];
    int    count  = 0;
    int    result = 0;

    if (sceKernelGetModuleIdList (modules, sizeof (modules), &count) < 0)
        return;

    SceKernelModuleInfo info;
    for (int i = 0; i < count; ++i)
        {
            info.size = sizeof (SceKernelModuleInfo);
            if (sceKernelQueryModuleInfo (modules[i], &info) < 0)
                {
                    continue;
                }

            if (strcmp (info.name, GAME_INTERNAL_NAME) == 0)
                {
                    Rainbomizer::Logger::LogMessage (
                        "Found game base address: %x (size: %x), gp: %u",
                        info.text_addr, info.text_size, info.gp_value);

                    m_gameTextAddress = info.text_addr;
                    m_gameTextSize    = info.text_size;
                    m_gameGpAddress   = info.gp_value;

                    injector.SetGameBaseAddress (info.text_addr,
                                                 info.text_size);

                    InitialiseAllPatterns ();
                }
        }
}
