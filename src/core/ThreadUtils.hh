#pragma once

#include "vcs/CRunningScript.hh"

class ThreadUtils
{
    inline static uint32_t currentMissionId = -1;
    inline static bool     initialised      = false;

    template <auto &CRunningScript__CollectParams>
    static int StoreCurrentMission (class CRunningScript *, int *, int, int *);

public:
    static void Initialise ();

    static uint32_t
    GetMissionIdFromThread (CRunningScript *script)
    {
        return script->m_bIsMission ? currentMissionId : -1;
    }

    static void
    OverrideCurrentMission (uint32_t missionId)
    {
        currentMissionId = missionId;
    }
};
