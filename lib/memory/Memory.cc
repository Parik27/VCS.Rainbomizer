#include "Memory.hh"
#include "core/Logger.hh"
#include <cstring>
#include <injector.h>
#include <patterns.h>
#include <pspsdk.h>
#include <string_view>

#define GAME_INTERNAL_NAME "GTA3"

template <std::size_t NUM = 0>
inline void
InitialisePattern ()
{
    constexpr const auto &currentPattern = s_Patterns[NUM];

    GameAddress<currentPattern.address>::SetResolvedAddress (
        pattern.get_first (currentPattern.pattern_str, currentPattern.offset));

    Rainbomizer::Logger::LogMessage (
        "[PATTERN] Address %08lx resolved to %08lx", currentPattern.address,
        GameAddress<currentPattern.address>::Get ());

    if constexpr (NUM < std::size (s_Patterns) - 1)
        return InitialisePattern<NUM + 1> ();
}

void
MemoryManager::Init ()
{
    if (m_initialised)
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
                                "Found game base address: %x (size: %x)",
                                info.text_addr, info.text_size);

                            m_gameTextAddress = info.text_addr;
                            m_gameTextSize    = info.text_size;

                            injector.SetGameBaseAddress (info.text_addr,
                                                         info.text_size);
                            pattern.SetGameBaseAddress (info.text_addr,
                                                        info.text_size);

                            InitialisePattern ();
                        }
                }
        }
}
