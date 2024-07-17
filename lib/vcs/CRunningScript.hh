#pragma once

#include "core/Logger.hh"
#include "memory/GameAddress.hh"
#include <cstddef>
#include <cstdint>

class CRunningScript;

struct ScriptCommandHandler
{
public:
    uint32_t unknown;
    int (*handler) (CRunningScript*);
};

class CTheScripts
{
public:
    static constexpr GameVariable<CRunningScript*, 0x08bb4050> CurrentScript{};
    static constexpr GameVariable<unsigned char *, 0x8baaf44> ScriptSpace{};
    static constexpr GameVariable<int[20], 0x8bc77c8>         ScriptParams{};
    static constexpr GameVariable<int, 0x8bb4084>             MainScriptSize{};

    template <typename T>
    static T &
    GetGlobal (size_t index)
    {
        return *reinterpret_cast<T *> (&ScriptSpace[index * 4]);
    }

    static constexpr GameVariable<ScriptCommandHandler[1371], 0x08b84ab0>
        ScriptCommands{};
};

class CRunningScript
{
public:
    CRunningScript *m_pNext   = nullptr;
    CRunningScript *m_pPrev   = nullptr;
    uint32_t        id        = 0;
    int             m_pBaseIP = 0;
    union
    {
        uint32_t       m_pCurrentIP = 0;
        unsigned char *m_pCurrentIpPtr;
    };
    int      m_apStack[16]        = {}; /* Created by retype action */
    int      m_aLocalVars[104]    = {}; /* Created by retype action */
    int      m_aTimers[2]         = {};
    uint32_t m_nLocalsBase         = 0;
    uint32_t m_nWakeTime          = 0;
    uint16_t m_nSP                = 0;
    uint16_t m_nAndOrState        = 0;
    bool     m_bIsActive          = 0; /* Created by retype action */
    bool     m_bCondResult        = 0; /* Created by retype action */
    uint8_t  m_bUseMissionCleanup = 0; /* Created by retype action */
    uint8_t  field15_0x20b        = 0;
    uint8_t  m_bNotFlag           = 0;
    uint8_t  field17_0x20d        = 0;
    uint8_t  field18_0x20e        = 0;
    char     m_szName[8]          = {};    /* Created by retype action */
    bool     m_bIsMission         = false; /* Created by retype action */

    int
    ProcessOneCommand ()
    {
        return GameFunction<0x88625e8, int (CRunningScript*)>::Call (this);
    }

    template <typename T = int32_t>
    T &
    GetLocalVariable (int index)
    {
        return *reinterpret_cast<T *> (&m_aLocalVars[m_nLocalsBase + index]);
    }

    int
    CollectParams (uint32_t *ip, int numParams, int *output)
    {
        return GameFunction<0x0886589c, int (CRunningScript *, uint32_t *, int,
                                             int *)>::Call (this, ip, numParams,
                                                            output);
    }

    uint16_t
    GetCurrentOpcode ()
    {
        return CTheScripts::ScriptSpace[m_pCurrentIP]
               | (CTheScripts::ScriptSpace[m_pCurrentIP + 1] << 8);
    }

    /* Nops a certain range. Make sure it is multiple of 2 */
    void
    Nop (size_t at, size_t size)
    {
        for (size_t i = 0; i < size; i++)
            CTheScripts::ScriptSpace[at+i] = 0x0;
    }
};

static_assert (offsetof (CRunningScript, m_bIsMission) == 0x217);
