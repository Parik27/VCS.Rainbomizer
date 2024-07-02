#include "core/Common.hh"
#include "core/Logger.hh"
#include "core/ThreadUtils.hh"
#include "memory/GameAddress.hh"
#include "scm/Command.hh"
#include "scm/Opcodes.hh"
#include "vcs/CMatrix.hh"
#include "vcs/CRunningScript.hh"
#include "vcs/CPlayer.hh"
#include "vcs/CPed.hh"
#include "vcs/CVector.hh"
#include <array>

#include <core/Randomizer.hh>
#include <core/Config.hh>

#include <cstdio>
#include <random>
#include <ranges>
#include <utils/ContainerUtils.hh>
#include <utils/Utils.hh>
#include <vector>
#include <map>

class MissionRandomizer : public Randomizer<MissionRandomizer>
{
    struct MissionInfo
    {
        int8_t id;
        CVector startPos;
        CVector endPos;
    };

    std::vector<MissionInfo> Missions;
    std::map<int8_t, int8_t> MissionMap;

    MissionInfo* OriginalMission;
    MissionInfo* RandomMission;

    MissionInfo *
    GetMissionInfoForId (uint8_t id)
    {
        for (auto &mission : Missions)
            {
                if (mission.id == id)
                    return &mission;
            }

        return nullptr;
    }

    template <auto &CRunningScript__CollectParams>
    uint32_t
    RandomizeMission (class CRunningScript *scr, unsigned char *data,
                      int numParams, int *params)
    {

        uint32_t ret
            = CRunningScript__CollectParams (scr, data, numParams, params);

        if (auto* mission = LookupMap (MissionMap, params[0]))
            {
                OriginalMission = GetMissionInfoForId (params[0]);
                RandomMission = GetMissionInfoForId (*mission);

                CallCommand<SET_CHAR_COORDINATES> (Global{782},
                                                   RandomMission->startPos.x,
                                                   RandomMission->startPos.y,
                                                   RandomMission->startPos.z);

                params[0] = RandomMission->id;

                Rainbomizer::Logger::LogMessage ("Randomized mission: %d",
                                                 params[0]);
            }

        // inform other randomizers we changed the mission
        ThreadUtils::OverrideCurrentMission (params[0]);

        return ret;
    }

    void
    InitialiseMissionsArray ()
    {
        auto f = Rainbomizer::Common::GetRainbomizerDataFile ("Missions.txt");
        f.ReadLines ([this] (std::string_view sv) {
            if (sv.empty () || sv[0] == '#')
                return;

            MissionInfo info;
            sscanf (sv.data (), "%hhd %f %f %f %f %f %f", &info.id,
                    &info.startPos.x, &info.startPos.y, &info.startPos.z,
                    &info.endPos.x, &info.endPos.y, &info.endPos.z);
            Missions.push_back (info);
        });
    }

    void
    InitialiseMissionsMap (int seed)
    {
        auto engine = std::mt19937 (seed);

        auto ids
            = std::views::transform (Missions, [&] (const MissionInfo &info) {
                  return info.id;
            });
        auto shuffledIds = ids | std::ranges::to<std::vector> ();

        std::ranges::shuffle (shuffledIds, engine);
        MissionMap = std::move (std::views::zip (ids, shuffledIds)
                                | std::ranges::to<std::map> ());
    }

    void
    OnMissionPass (CRunningScript* script)
    {
        CallCommand<SET_CHAR_COORDINATES> (Global{782},
                                           OriginalMission->endPos.x,
                                           OriginalMission->endPos.y,
                                           OriginalMission->endPos.z);
        RandomMission = nullptr;
        OriginalMission = nullptr;
    }

    void
    OnMissionFail (CRunningScript *script)
    {
        if (OriginalMission->id == 8)
            {
                CTheScripts::GetGlobal<int> (3) = 1;
            }

        RandomMission = nullptr;
        OriginalMission = nullptr;
    }

    void
    ProcessMissionRandomizerFSM (CRunningScript *script)
    {
        uint16_t currentOpcode
            = CTheScripts::ScriptSpace[script->m_pCurrentIP]
              | (CTheScripts::ScriptSpace[script->m_pCurrentIP + 1] << 8);

        if (currentOpcode == REGISTER_MISSION_PASSED)
            {
                OnMissionPass (script);
            }
        if (currentOpcode == TERMINATE_THIS_SCRIPT)
            {
                OnMissionFail (script);
            }
    }

    template <auto &CRunningScript__ProcessOneCommand>
    int
    ProcessMissionScript (CRunningScript *script)
    {
        if (script->m_bIsMission && RandomMission)
            ProcessMissionRandomizerFSM (script);

        return CRunningScript__ProcessOneCommand (script);
    }

public:
    MissionRandomizer ()
    {
        RB_C_DO_CONFIG ("MissionRandomizer");
        Missions.reserve (64);

        InitialiseMissionsArray ();
        InitialiseMissionsMap (123);

        HOOK_MEMBER (Jal, (0x08abc41c), RandomizeMission,
              uint32_t (class CRunningScript *, unsigned char *data,
                        int numParams, int *params));

        HOOK_MEMBER (Jal, 0x8862524, ProcessMissionScript,
                     int (CRunningScript *));

        GameAddress<0x8ad2b44>::WriteInstructions (li (v0, 0), jr (ra));
    }
};
