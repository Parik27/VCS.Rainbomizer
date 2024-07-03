#pragma once

#include "core/Logger.hh"
#include "memory/GameAddress.hh"
#include <cstddef>
#include <cstdint>

class CRunningScript
{
public:
    CRunningScript *m_pNext              = nullptr;
    CRunningScript *m_pPrev              = nullptr;
    uint32_t        id                   = 0;
    int             m_pBaseIP            = 0;
    uint32_t        m_pCurrentIP         = 0;
    int             m_apStack[16]        = {}; /* Created by retype action */
    int             m_aLocalVars[104]    = {}; /* Created by retype action */
    int             m_aTimers[2]         = {};
    uint32_t        field8_0x1fc         = 0;
    uint32_t        m_nWakeTime          = 0;
    uint16_t        m_nSP                = 0;
    uint16_t        field11_0x206        = 0;
    bool            m_bIsActive          = 0; /* Created by retype action */
    bool            m_bCondResult        = 0; /* Created by retype action */
    uint8_t         m_bUseMissionCleanup = 0; /* Created by retype action */
    uint8_t         field15_0x20b        = 0;
    uint8_t         m_bNotFlag           = 0;
    uint8_t         field17_0x20d        = 0;
    uint8_t         field18_0x20e        = 0;
    char            m_szName[8]          = {};    /* Created by retype action */
    bool            m_bIsMission         = false; /* Created by retype action */

    int
    ProcessOneCommand ()
    {
        return GameFunction<0x88625e8, int (CRunningScript*)>::Call (this);
    }

    int
    CollectParams (uint32_t *ip, int numParams, int *output)
    {
        return GameFunction<0x0886589c, int (CRunningScript *, uint32_t *, int,
                                             int *)>::Call (this, ip, numParams,
                                                            output);
    }
};

struct ScriptCommandHandler
{
public:
    uint32_t unknown;
    int (*handler) (CRunningScript*);
};

class CTheScripts
{
public:
    static constexpr GameVariable<unsigned char *, 0x8baaf44> ScriptSpace{};

    template <typename T>
    static T &
    GetGlobal (size_t index)
    {
        return *reinterpret_cast<T *> (&ScriptSpace[index * 4]);
    }

    static constexpr GameVariable<ScriptCommandHandler[1371], 0x08b84ab0>
        ScriptCommands{};
};

static_assert (offsetof (CRunningScript, m_bIsMission) == 0x217);
