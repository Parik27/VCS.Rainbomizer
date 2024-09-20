#include "core/Common.hh"
#include "core/Logger.hh"
#include "core/ThreadUtils.hh"
#include "memory/GameAddress.hh"
#include "ppsspp/Keyboard.hh"
#include "scm/Command.hh"
#include "scm/Globals.hh"
#include "scm/Opcodes.hh"
#include "vcs/CRunningScript.hh"
#include "vcs/CTimer.hh"
#include "vcs/CVector.hh"
#include "vcs/CVehicle.hh"
#include "vcs/CWeaponInfo.hh"
#include "vcs/eMissions.hh"

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

#include <core/ConfigDebugHelpers.hh>

class MissionRandomizer : public RandomizerWithDebugInterface<MissionRandomizer>
{
    inline static std::array sm_MissionValueList = {
        std::make_pair ("None", -1),
        std::make_pair ("Soldier", 8),
        std::make_pair ("In The Air Tonight", 12),
        std::make_pair ("Cleaning House", 42),
        std::make_pair ("Conduct Unbecoming", 43),
        std::make_pair ("Cholo Victory", 44),
        std::make_pair ("Boomshine Blowout", 45),
        std::make_pair ("Truck Stop", 46),
        std::make_pair ("Marked Men", 47),
        std::make_pair ("Shakedown", 48),
        std::make_pair ("Fear the Repo", 49),
        std::make_pair ("Waking Up the Neighbors", 50),
        std::make_pair ("O, Brothel, Where Art Thou?", 51),
        std::make_pair ("Got Protection?", 52),
        std::make_pair ("When Funday Comes", 53),
        std::make_pair ("Takin' Out the White-Trash", 54),
        std::make_pair ("D.I.V.O.R.C.E.", 55),
        std::make_pair ("To Victor, the Spoils", 56),
        std::make_pair ("Hose the Hoes", 57),
        std::make_pair ("Robbing the Cradle", 58),
        std::make_pair ("Jive Drive", 59),
        std::make_pair ("The Audition", 60),
        std::make_pair ("Caught as an Act", 61),
        std::make_pair ("Snitch Hitch", 62),
        std::make_pair ("From Zero to Hero", 63),
        std::make_pair ("Nice Package", 64),
        std::make_pair ("Balls", 65),
        std::make_pair ("Papi Don't Screech", 66),
        std::make_pair ("Havana Good Time", 67),
        std::make_pair ("Money for Nothing", 68),
        std::make_pair ("Leap and Bound", 69),
        std::make_pair ("The Bum Deal", 70),
        std::make_pair ("The Mugshot Longshot", 71),
        std::make_pair ("Hostile Takeover", 72),
        std::make_pair ("Unfriendly Competition", 73),
        std::make_pair ("High Wire", 74),
        std::make_pair ("Burning Bridges", 75),
        std::make_pair ("Accidents Will Happen", 76),
        std::make_pair ("The Colonel's Coke", 77),
        std::make_pair ("Kill Phil", 78),
        std::make_pair ("Say Cheese", 79),
        std::make_pair ("Kill Phil: Part 2", 80),
        std::make_pair ("So Long Schlong", 81),
        std::make_pair ("Brawn of the Dead", 82),
        std::make_pair ("Blitzkrieg", 83),
        std::make_pair ("Turn on, Tune in, Bug out", 84),
        std::make_pair ("Taking the Fall", 85),
        std::make_pair ("White Lies", 86),
        std::make_pair ("Where it Hurts Most", 87),
        std::make_pair ("Blitzkrieg Strikes Again", 88),
        std::make_pair ("Lost and Found", 89),
        std::make_pair ("Light My Pyre", 90),
        std::make_pair ("Home's on the Range", 91),
        std::make_pair ("Purple Haze", 92),
        std::make_pair ("Farewell To Arms", 93),
        std::make_pair ("Steal the Deal", 94),
        std::make_pair ("The Exchange", 95),
        std::make_pair ("Domo Arigato Domestoboto", 96),
        std::make_pair ("Over the Top", 97),
        std::make_pair ("Last Stand", 98),
    };

    using MissionId = EnumConfigOption<int, sm_MissionValueList>;

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
    MissionId ForcedMission = -1;

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
                            Global{GLOBAL_PLAYER_CHAR}, RandomMission->startPos.x,
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
        // Fix the factory for havana good time
        if (RandomMission->id == MISSION_HAVANA_GOOD_TIME)
            CallCommand<UNKNOWN_ENABLE_BUILDING_SWAP_FOR_MODEL> (660173992, 0);

/*
        // Player needs 8 rockets for the mission and rockets may not have been
        // unlocked yet.
        if (RandomMission->id == MISSION_TURN_ON_TUNE_IN_BUG_OUT)
            CallCommand<GIVE_WEAPON_TO_CHAR> (Global{782},
                                              int (WEAPON_ROCKETLAUNCHER), 8);
*/

        // Open bridges and hurricane Gordy gone
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
        ResetPlayerInterior ();
        CallCommand<SET_CHAR_COORDINATES> (Global{GLOBAL_PLAYER_CHAR},
                                           OriginalMission->endPos.x,
                                           OriginalMission->endPos.y,
                                           OriginalMission->endPos.z);
        OnMissionEnd (script);
        RandomMission   = nullptr;
        OriginalMission = nullptr;
    }

    void
    OnMissionFail (CRunningScript *script)
    {
        // Allow prologue to restart properly
        if (OriginalMission->id == MISSION_SOLDIER)
            {
                CTheScripts::GetGlobal<int> (GLOBAL_SAVING) = 1;
            }

        OnMissionEnd (script);
        RandomMission   = nullptr;
        OriginalMission = nullptr;
    }

    void
    ProcessCodeSectionSkip (CRunningScript *script, size_t target_mission, int at,
                            int to)
    {
        // Check if running the correct script
        if (ThreadUtils::GetMissionIdFromThread(script) == target_mission) {
            // Mission script is located in script space after the main script
            // Their relative (local) offset is given in negative.
            at = at < 0 ? CTheScripts::MainScriptSize - at : at - 8;
            to = to < 0 ? CTheScripts::MainScriptSize - to : to - 8;
    
            if (script->m_pCurrentIP == at)
                script->m_pCurrentIP = to;
        }
    }

    void
    SkipMissionCode (size_t conditional_mission, CRunningScript *script,
                            size_t target_mission, size_t at, size_t to)
    {
        if (RandomMission && RandomMission->id == conditional_mission)
            ProcessCodeSectionSkip(script, target_mission, at, to);
    }

    void
    ProcessMissionRandomizerFSM (CRunningScript *script)
    {
        uint16_t currentOpcode
            = CTheScripts::ScriptSpace[script->m_pCurrentIP]
              | (CTheScripts::ScriptSpace[script->m_pCurrentIP + 1] << 8);

        // O, Brothel, Where Art Thou? -> AMMUNAT | skip resetting Free Shotgun
        // Status to 1
        SkipMissionCode (MISSION_O_BROTHEL_WHERE_ART_THOU, script, -1, 161048,
                         161054);
        // Turn on, Tune in, Bug out -> AMMUNAT | skip act3 condition for
        // unlocking Rocket Launcher
        SkipMissionCode (MISSION_TURN_ON_TUNE_IN_BUG_OUT, script, -1, 154718,
                         154730);

        if (CTheScripts::GetGlobal<int> (GLOBAL_ACTS_COMPLETED) == 0)
            {
                // Blitzkrieg | skip insufficient empire sites scenario
                SkipMissionCode (MISSION_BLITZKRIEG, script, MISSION_BLITZKRIEG,
                                 -10210, -10217);
                // Blitzkrieg Strikes Again | skip insufficient empire sites
                // scenario
                SkipMissionCode (MISSION_BLITZKRIEG_STRIKES_AGAIN, script,
                                 MISSION_BLITZKRIEG_STRIKES_AGAIN, -9826,
                                 -9833);
            }

        if (script->m_bIsMission && RandomMission)
            {
                if (script->m_pCurrentIP == CTheScripts::MainScriptSize)
                    OnMissionStart (script);
                if (currentOpcode == REGISTER_MISSION_PASSED)
                    OnMissionPass (script);
                if (currentOpcode == TERMINATE_THIS_SCRIPT)
                    OnMissionFail (script);
            }
    }

    bool
    DeleteLanceGarage (CRunningScript *script)
    {
        using namespace std::string_view_literals;
        if (script->m_szName == "ga_laga"sv)
            {
                if (CallCommand<DOES_OBJECT_EXIST> (
                        script->GetLocalVariable (2)))
                    CallCommand<DELETE_OBJECT> (script->GetLocalVariable (2));
                return true;
            }

        return false;
    }

    template <auto &CRunningScript__ProcessOneCommand>
    int
    ProcessScript (CRunningScript *script)
    {
        if (script->m_bIsMission && RandomMission
            && std::exchange (DelayMissionScript, false))
            {
                // wait 1 frame before starting the mission
                // to allow init scripts to run properly

                // Just returning 1 here doesn't work because
                // the game runs mission script twice in the
                // same frame.
                script->m_nWakeTime = CTimer::TimeInMilliseconds + 1;
                Rainbomizer::Logger::LogCritical (
                    "Delaying prologue mission script");
                return 1;
            }

        ProcessMissionRandomizerFSM (script);

        if (DeleteLanceGarage (script))
            return 1;

        return CRunningScript__ProcessOneCommand (script);
    }

    void
    ResetPlayerInterior ()
    {
        // Need to set player to -1 so game doesn't try to fade and set player
        // pos
        int origPlayer = CTheScripts::GetGlobal<int> (GLOBAL_PLAYER_CHAR);

        CTheScripts::GetGlobal<int> (GLOBAL_PLAYER_CHAR) = -1;
        CommandCaller::CallGameFunction (0x20751 - 0x8, 0, 0);
        CTheScripts::GetGlobal<int> (GLOBAL_PLAYER_CHAR) = origPlayer;
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

        HOOK_MEMBER (Jal, 0x8862524, ProcessScript,
                     int (CRunningScript *));

        GameAddress<0x8ad2b44>::WriteInstructions (li (v0, 0), jr (ra));
    }
};
