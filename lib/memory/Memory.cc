#include "Memory.hh"
#include "core/Logger.hh"
#include "memory/GameAddress.hh"
#include "memory/MemorySignature.hh"
#include "memory/Pattern.hh"
#include <cstring>
#include <injector.h>
#include <patterns.h>
#include <pspsdk.h>
#include <string_view>

#include <chrono>
#include <regex>

#define GAME_INTERNAL_NAME "GTA3"

template <std::size_t NUM = 0>
inline void
InitialisePattern (MemoryManager& manager)
{
    using namespace MemorySignature;

    constexpr const auto &currentPattern = s_Patterns[NUM];

    auto t1 = std::chrono::high_resolution_clock::now ();

    static constinit const Signature<GetMemorySignatureSize (
        currentPattern.pattern_str)>
        sig{currentPattern.pattern_str};

    auto addr = manager.SignatureSearch (sig) + currentPattern.offset;

    auto t2 = std::chrono::high_resolution_clock::now ();

    Rainbomizer::Logger::LogMessage (
        "[PATTERN] Found address %lu in %lu microseconds", currentPattern.address,
        std::chrono::duration_cast<std::chrono::microseconds> (t2 - t1).count ());

    if constexpr (currentPattern.resolver == Pattern::BRANCH)
        {
            addr = injector.GetBranchDestination (addr);
        }
    if constexpr (currentPattern.resolver == Pattern::GPOFFSET)
        {
            addr = manager.GetGpAddress () + manager.ReadMemory<int16_t> (addr);
        }

    GameAddress<currentPattern.address>::SetResolvedAddress (addr);
    Rainbomizer::Logger::LogMessage (
        "[PATTERN] Resolved address %x to %x", currentPattern.address,
        GameAddress<currentPattern.address>::Get ());

    if constexpr (NUM < std::size (s_Patterns) - 1)
        return InitialisePattern<NUM + 1> (manager);
}

void
MemoryManager::Init ()
{
    if (std::exchange(m_initialised, true))
        return;

    SceUID modules[10];
    int    count  = 0;
    int    result = 0;

    if (sceKernelGetModuleIdList (modules, sizeof (modules), &count) >= 0)
        {
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

                            InitialisePattern (*this);
                        }
                }
        }
}
