#include <ranges>
#include <vcs/CRunningScript.hh>
#include <vcs/CStreaming.hh>
#include <vcs/CVehicle.hh>
#include <vcs/CModelInfo.hh>

#include <core/Logger.hh>
#include <core/Randomizer.hh>

#include <hooks/Hooks.hh>

#include "Common.hh"
#include "core/ThreadUtils.hh"
#include "memory/GameAddress.hh"
#include "ppsspp/KeyCodes.h"
#include "ppsspp/Keyboard.hh"
#include "vehicle/VehiclePatterns.hh"

class ScriptVehicleRandomizer : public Randomizer<ScriptVehicleRandomizer>
{
    VehiclePatternManager m_Patterns;

    template <auto &CollectParams>
    int
    RandomizeVehicle (CRunningScript *scr, int *p2, int p3, int *params)
    {
        int ret = CollectParams (scr, p2, p3, params);

        PatternResult result{params[0]};
        m_Patterns.GetRandomVehicle (eVehicle (params[0]), scr, result);

        int originalVehicle = params[0];
        int newVehicle      = result.vehId;

        Rainbomizer::Logger::LogMessage("Num Vehicles Loaded vehicles: %d", CStreaming::sm_Instance->m_numVehiclesLoaded);

        if (!VehicleCommon::AttemptToLoadVehicle(newVehicle))
            {
                Rainbomizer::Logger::LogMessage("Failed to load model: %d", newVehicle);
                newVehicle = originalVehicle;
            }

        Rainbomizer::Logger::LogMessage (
            "Num Vehicles Loaded vehicles: %d",
            CStreaming::sm_Instance->m_numVehiclesLoaded);

        if (!CStreaming::HasModelLoaded (newVehicle))
            newVehicle = originalVehicle;

        params[0] = newVehicle;
        if (result.coords)
            {
                auto coords = reinterpret_cast<float *> (params);
                coords[1] += result.coords->x;
                coords[2] += result.coords->y;
                coords[3] += result.coords->z;
            }

        Rainbomizer::Logger::LogMessage (
            "Vehicle spawn: [%s]:%x (mission = %d) (%f %f %f): %d -> %d",
            scr->m_szName, scr->m_pCurrentIP,
            ThreadUtils::GetMissionIdFromThread (scr),
            std::bit_cast<float> (params[1]), std::bit_cast<float> (params[2]),
            std::bit_cast<float> (params[3]), originalVehicle, params[0]);

        return ret;
    }

    template <auto &CRunningScript__Process>
    static void
    ReloadPatternsCheck (CRunningScript *scr)
    {
        if (PPSSPPUtils::CheckKeyDown<NKCODE_F4> ())
            {
                Get ().m_Patterns.ReadPatterns ("VehiclePatterns.txt");
            }

        CRunningScript__Process (scr);
    }

public:
    ScriptVehicleRandomizer ()
    {
        m_Patterns.ReadPatterns ("VehiclePatterns.txt");
        HOOK_MEMBER (Jal, (0x08aec324), RandomizeVehicle,
                     int (class CRunningScript *, int *, int, int *));

        // Remove vehicle checks in several missions (hopefully no side-effects BlessRNG)
        GameAddress<0x8ae4efc>::WriteInstructions (li (a0, 1));

        ThreadUtils::Initialise ();
    }
};
