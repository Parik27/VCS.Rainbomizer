#pragma once

#include <cstddef>
#include <cstdint>

class CRunningScript
{
public:
    CRunningScript *m_pNext;
    CRunningScript *m_pPrev;
    uint32_t        id;
    int             m_pBaseIP;
    uint32_t        m_pCurrentIP;
    int             m_apStack[16];     /* Created by retype action */
    int             m_aLocalVars[104]; /* Created by retype action */
    int             m_aTimers[2];
    uint32_t        field8_0x1fc;
    uint32_t        m_nWakeTime;
    uint16_t        m_nSP;
    uint16_t        field11_0x206;
    bool            m_bIsActive;          /* Created by retype action */
    bool            m_bCondResult;        /* Created by retype action */
    uint8_t         m_bUseMissionCleanup; /* Created by retype action */
    uint8_t         field15_0x20b;
    uint8_t         field16_0x20c;
    uint8_t         field17_0x20d;
    uint8_t         field18_0x20e;
    char            m_szName[8];  /* Created by retype action */
    bool            m_bIsMission; /* Created by retype action */
};

static_assert (offsetof (CRunningScript, m_bIsMission) == 0x217);
