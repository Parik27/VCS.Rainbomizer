#pragma once

#include <cstdint>
#include <array>
#include <utility>

#include "GameAddress.hh"

class MemoryManager
{
    uint32_t m_gameTextAddress;
    uint32_t m_gameTextSize;
    bool     m_initialised = false;

    void Init ();

    std::array<std::pair<uint32_t, uint32_t>, 16> m_jmpHooks;
    int                                           m_numJmpHooks;

public:
    MemoryManager () { Init (); }

    static MemoryManager &
    Get ()
    {
        static MemoryManager s_memoryMan;
        return s_memoryMan;
    }

    template <typename Func>
    inline void
    FindInstruction (uint32_t instr, Func functor)
    {
        for (std::size_t i = m_gameTextAddress;
             i < m_gameTextAddress + m_gameTextSize; i += 4)
            {
                if (ReadMemory<uint32_t> (i) == instr)
                    functor (i);
            }
    }

    template <typename Ret, typename T>
    static Ret &
    ReadMemory (T addr)
    {
        return *reinterpret_cast<Ret *> (addr);
    }
};
