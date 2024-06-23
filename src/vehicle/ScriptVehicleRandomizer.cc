#include <vcs/CRunningScript.hh>
#include <vcs/CStreaming.hh>
#include <vcs/CVehicle.hh>
#include <vcs/CModelInfo.hh>

#include <core/Logger.hh>
#include <core/Randomizer.hh>

#include <hooks/Hooks.hh>

#include "Common.hh"

class ScriptVehicleRandomizer : public Randomizer<ScriptVehicleRandomizer>
{

    template <auto &CollectParams>
    static int
    RandomizeVehicle (CRunningScript *scr, int *p2, int p3, int *params)
    {
        int newVehicle = VehicleCommon::GetRandomUsableVehicle ();

        CStreaming::RequestModel (newVehicle, 0x3abccc);
        CStreaming::LoadAllRequestedModels (false);

        int ret = CollectParams (scr, p2, p3, params);

        for (auto i : CStreaming::sm_Instance->m_vehiclesLoaded)
            Rainbomizer::Logger::LogMessage("%d", i);

        Rainbomizer::Logger::LogMessage (
            "hash: %x, seats: %d",
            ModelInfo::ms_modelInfoPtrs[newVehicle]->m_hashName,
            CVehicleModelInfo::GetMaximumNumberOfPassengersFromNumberOfDoors (
                newVehicle));

        Rainbomizer::Logger::LogMessage (
            "Vehicle spawn: [%s]:%x (%f %f %f): %d", scr->m_szName,
            scr->m_pCurrentIP, std::bit_cast<float> (params[1]),
            std::bit_cast<float> (params[2]), std::bit_cast<float> (params[3]),
            params[0]);

        params[0] = newVehicle;

        return ret;
    }

 public:
    ScriptVehicleRandomizer ()
    {
        HOOK (Jal, (0x08aec324), RandomizeVehicle,
              int (class CRunningScript *, int *, int, int *));
    }
} g_scriptVehicleRandomizer;
