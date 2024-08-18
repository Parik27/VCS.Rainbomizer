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
#include "vcs/CStreaming.hh"
#include "vcs/CVector.hh"
#include <array>

#include <core/Randomizer.hh>
#include <core/Config.hh>
#include <core/Events.hh>

#include <cstdio>
#include <random>
#include <ranges>
#include <utils/ContainerUtils.hh>
#include <utils/Utils.hh>
#include <vector>
#include <map>

class MissionRandomizer : public RandomizerWithDebugInterface<MissionRandomizer>
{
    struct MissionInfo
    {
        int8_t  id;
        char    gxtName[8];
        CVector startPos;
        CVector endPos;
    };

    std::vector<MissionInfo> Missions;
    std::map<int8_t, int8_t> MissionMap;

    MissionInfo *OriginalMission;
    MissionInfo *RandomMission;

    bool DelayMissionScript = false;
    int Seed;
    int ForcedMission = -1;

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

        if (auto *mission = LookupMap (MissionMap, params[0]))
            {
                if (ForcedMission != -1)
                    *mission = ForcedMission;

                if (*mission != params[0])
                    {
                        OriginalMission = GetMissionInfoForId (params[0]);
                        RandomMission   = GetMissionInfoForId (*mission);

                        CallCommand<SET_CHAR_COORDINATES> (
                            Global{782}, RandomMission->startPos.x,
                            RandomMission->startPos.y,
                            RandomMission->startPos.z);

                        params[0] = RandomMission->id;

                        // For prologue mission we need to delay the script
                        // so that the game doesn't crash as it allows time for
                        // essential mission scripts to start.
                        if (OriginalMission->id == 8)
                            DelayMissionScript = true;

                        Rainbomizer::Logger::LogCritical (
                            "Randomized mission: %d", params[0]);
                    }
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
            sscanf (sv.data (), "%hhd %8s %f %f %f %f %f %f", &info.id,
                    info.gxtName, &info.startPos.x, &info.startPos.y,
                    &info.startPos.z, &info.endPos.x, &info.endPos.y,
                    &info.endPos.z);
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
    OnMissionStart (CRunningScript *script)
    {
        CallCommand<CREATE_OBJECT_NO_OFFSET> (7338, -935.f, -124.0f, 6.5f, Local{0});

        // Open bridges and hurricane gordy gone
        for (uint16_t i = 1562; i <= 1566; i++)
            CallCommand<DELETE_OBJECT> (Global{i});

        CallCommand<SET_PLAYER_PUSHED_TOWARDS_MAINLAND> (0);
    }

    void
    OnMissionEnd (CRunningScript *script)
    {
    }

    void
    OnMissionPass (CRunningScript *script)
    {
        // Restore original mission gxt name for save files
        // We need to check if the name matches because the
        // mission name might be in a different variable by
        // some weird script and we don't want to modify
        // random script variable in that case.
        char *gxtName = (char *) (CTheScripts::ScriptSpace
                                  + script->GetLocalVariable (0));

        if (strncmp (gxtName, RandomMission->gxtName, 8) == 0)
            memcpy (gxtName, OriginalMission->gxtName, 8);

        // Teleport player to the original mission end position
        CallCommand<SET_CHAR_COORDINATES> (Global{782},
                                           OriginalMission->endPos.x,
                                           OriginalMission->endPos.y,
                                           OriginalMission->endPos.z);

        OnMissionEnd (script);
        RandomMission = nullptr;
        OriginalMission = nullptr;
    }

    void
    OnMissionFail (CRunningScript *script)
    {
        // Allow prologue to restart properly
        if (OriginalMission->id == 8)
            {
                CTheScripts::GetGlobal<int> (3) = 1;
            }

        OnMissionEnd (script);
        RandomMission = nullptr;
        OriginalMission = nullptr;
    }

    void
    ProcessCodeSectionSkip (CRunningScript *script, size_t mission, size_t at,
                            size_t to)
    {
        if (RandomMission->id != mission)
            return;

        if (script->m_pCurrentIP == CTheScripts::MainScriptSize + at)
            script->m_pCurrentIP = CTheScripts::MainScriptSize + to;
    }

    void
    ProcessMissionRandomizerFSM (CRunningScript *script)
    {
        uint16_t currentOpcode
            = CTheScripts::ScriptSpace[script->m_pCurrentIP]
              | (CTheScripts::ScriptSpace[script->m_pCurrentIP + 1] << 8);

        ProcessCodeSectionSkip (script, 83, 10210, 10217);
        ProcessCodeSectionSkip (script, 88, 9826, 9833);

        if (script->m_pCurrentIP == CTheScripts::MainScriptSize)
            {
                OnMissionStart (script);
            }
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
        if (script->m_bIsMission && RandomMission) {
            if (std::exchange(DelayMissionScript, false)) {
                Rainbomizer::Logger::LogCritical (
                    "Delaying prologue mission script");
                return 1;
            }
            ProcessMissionRandomizerFSM (script);
        }

        return CRunningScript__ProcessOneCommand (script);
    }

public:

    void
    DrawDebug ()
    {
#ifdef ENABLE_DEBUG_MENU
        if (OriginalMission == nullptr || RandomMission == nullptr)
            {
                ImGui::TextColored (ImVec4 (1, 0, 0, 1),
                                    "No mission randomized yet");
                return;
            }

        ImGui::LabelText ("Original Mission", "%d %s", OriginalMission->id,
                          OriginalMission->gxtName);
        ImGui::LabelText ("Random Mission", "%d %s", RandomMission->id,
                          RandomMission->gxtName);
#endif
    }

    MissionRandomizer ()
    {
        RB_C_DO_CONFIG ("MissionRandomizer", ForcedMission);
        Missions.reserve (64);

        InitialiseMissionsArray ();

        RandomizationSeedEvent::Add (
            [] (int seed) { Get ().InitialiseMissionsMap (seed); });

        HOOK_MEMBER (Jal, (0x08abc41c), RandomizeMission,
              uint32_t (class CRunningScript *, unsigned char *data,
                        int numParams, int *params));

        HOOK_MEMBER (Jal, 0x8862524, ProcessMissionScript,
                     int (CRunningScript *));

        GameAddress<0x8ad2b44>::WriteInstructions (li (v0, 0), jr (ra));
    }
};
