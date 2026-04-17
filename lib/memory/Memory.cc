#include "Memory.hh"
#include "core/Logger.hh"
#include "memory/Cache.hh"
#include "memory/GameAddress.hh"
#include "memory/MemorySignature.hh"
#include "memory/Pattern.hh"
#include "memory/ThreadManager.hh"
#include <cstring>
#include <injector.h>
#include <pspsdk.h>
#include <string_view>
#include <utility>
#include <array>

#define GAME_INTERNAL_NAME "GTA3"

struct SolidByteInfo
{
    uint8_t byte_val;
    uint8_t offset;
    bool    found;
};

consteval SolidByteInfo
GetFirstSolidByteAndOffset (std::string_view str)
{
    size_t offset = 0;
    size_t i      = 0;
    while (i < str.size ())
        {
            while (i < str.size () && str[i] == ' ')
                i++;
            if (i >= str.size ())
                break;

            size_t start = i;
            while (i < str.size () && str[i] != ' ')
                i++;
            size_t end = i;

            auto [val, mask] = MemorySignature::NibbleMaskFromString (
                str.substr (start, end - start));
            if (mask == 0)
                { // Fully solid byte
                    return {val, static_cast<uint8_t> (offset), true};
                }
            offset++;
        }
    return {0, 0, false};
}

consteval size_t
GetMaxSigSize ()
{
    size_t max_s = 0;
    for (size_t i = 0; i < std::size (s_Patterns); ++i)
        {
            size_t s = MemorySignature::GetMemorySignatureSize (
                s_Patterns[i].pattern_str);
            if (s > max_s)
                max_s = s;
        }
    return max_s;
}

template <std::size_t NUM>
bool
CheckAndResolvePattern (MemoryManager &manager, uintptr_t address)
{
    using namespace MemorySignature;
    constexpr const auto &currentPattern = s_Patterns[NUM];

    if (GameAddress<currentPattern.address>::IsResolved ())
        return false;

    constexpr auto sig_size
        = MemorySignature::GetMemorySignatureSize (currentPattern.pattern_str);
    static constinit const MemorySignature::Signature<sig_size> sig{
        currentPattern.pattern_str};

    for (size_t j = 0; j < sig_size; j++)
        {
            if ((manager.ReadMemory<uint8_t> (address + j) & ~sig.masks[j])
                != sig.bytes[j])
                {
                    return false;
                }
        }

    auto addr = address + currentPattern.offset;

    if constexpr (currentPattern.resolver == Pattern::BRANCH)
        {
            addr = injector.GetBranchDestination (addr);
        }
    else if constexpr (currentPattern.resolver == Pattern::GPOFFSET)
        {
            addr = manager.GetGpAddress () + manager.ReadMemory<int16_t> (addr);
        }
    else if constexpr (currentPattern.resolver == Pattern::UPPER_AND_ADD)
        {
            addr = (manager.ReadMemory<uint16_t> (addr) << 16)
                   + manager.ReadMemory<int16_t> (
                       addr + (currentPattern.offset2 - currentPattern.offset));
        }

    GameAddress<currentPattern.address>::SetResolvedAddress (addr);

    Rainbomizer::Logger::LogMessage (
        "[PATTERN] Resolved address %x to %x", currentPattern.address,
        GameAddress<currentPattern.address>::Get ());

    return true;
}

using ByteCheckFunc = void (*) (MemoryManager &, uintptr_t, size_t &);

template <uint8_t ByteValue, size_t... PatternIndices>
void
CheckByteImpl (MemoryManager &manager, uintptr_t ptr, size_t &unresolved_count)
{
    (
        [&] () {
            constexpr auto info = GetFirstSolidByteAndOffset (
                s_Patterns[PatternIndices].pattern_str);
            if constexpr (info.found && info.byte_val == ByteValue)
                {
                    if (unresolved_count == 0)
                        return;

                    if (ptr >= manager.m_gameTextAddress + info.offset)
                        {
                            uintptr_t check_addr = ptr - info.offset;
                            if (CheckAndResolvePattern<PatternIndices> (
                                    manager, check_addr))
                                {
                                    unresolved_count--;
                                }
                        }
                }
            else if (!info.found)
                {
                    if (unresolved_count == 0)
                        return;

                    if (CheckAndResolvePattern<PatternIndices> (manager, ptr))
                        {
                            unresolved_count--;
                        }
                }
        }(),
        ...);
}

template <std::size_t... ByteValues, std::size_t... PatternIndices>
consteval auto
BuildJumpTableImpl (std::index_sequence<ByteValues...>,
                    std::index_sequence<PatternIndices...>)
{
    return std::array<ByteCheckFunc, 256>{
        &CheckByteImpl<static_cast<uint8_t> (ByteValues),
                       PatternIndices...>...};
}

consteval auto
BuildJumpTable ()
{
    return BuildJumpTableImpl (
        std::make_index_sequence<256>{},
        std::make_index_sequence<std::size (s_Patterns)>{});
}

std::string
MemoryManager::GetCacheFileName ()
{
    // Some random addresses that should hopefully be unique enough
    auto identifier = injector.ReadMemory32 (0x088f45b4)
                      + injector.ReadMemory32 (0x8b6f6bc);

    return std::move ("SignatureCache_" + std::to_string (identifier) + ".bin");
}

void
MemoryManager::InitialiseAllPatterns ()
{
    Rainbomizer::Logger::FunctionBenchmark benchmark;

    MemorySignatureCache::ReadFromFile (GetCacheFileName ());

    size_t unresolved_count =
        [this]<std::size_t... I> (std::index_sequence<I...>) {
            return ((GameAddress<s_Patterns[I].address>::IsResolved () ? 0 : 1)
                    + ...);
        }(std::make_index_sequence<std::size (s_Patterns)>{});

    if (unresolved_count == 0)
        {
            MemorySignatureCache::WriteToFile (GetCacheFileName ());
            return;
        }

    constexpr static auto s_JumpTable  = BuildJumpTable ();
    constexpr size_t      max_sig_size = GetMaxSigSize ();

    uintptr_t end_address = m_gameTextAddress + m_gameTextSize - max_sig_size;

    for (uintptr_t ptr = m_gameTextAddress; ptr < end_address; ++ptr)
        {
            uint8_t val = ReadMemory<uint8_t> (ptr);

            s_JumpTable[val](*this, ptr, unresolved_count);

            if (unresolved_count == 0)
                break;
        }

    MemorySignatureCache::WriteToFile (GetCacheFileName ());
}

void
MemoryManager::Init ()
{
    if (std::exchange (m_initialised, true))
        return;

    ThreadManager::Get ().BlockThread ("stupidthread");

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
