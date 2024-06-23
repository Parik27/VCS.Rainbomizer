#pragma once

#include <cstdint>

#include "GameAddress.hh"

class MemoryManager
{
    uint32_t m_gameTextAddress = 0;
    uint32_t m_gameTextSize = 0;
    uint32_t m_gameGpAddress = 0;
    bool     m_initialised = false;

    void Init ();

public:
    //constexpr MemoryManager () { Init (); }

    uint32_t
    GetGpAddress ()
    {
        return m_gameGpAddress;
    }

    static MemoryManager &
    Get ()
    {
        static constinit MemoryManager s_memoryMan;
        s_memoryMan.Init ();
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
