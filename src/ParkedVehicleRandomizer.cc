#include "ContainerUtils.hh"
#include "Hooks.hh"
#include "core/Logger.hh"
#include "core/Randomizer.hh"
#include "CCarGenerator.hh"
#include "Random.hh"
#include "core/Config.hh"
#include "CStreaming.hh"

#include <map>

//#define DONT_USE_MAP // decreases randomness, but decreases memory usage.

class ParkedVehicleRandomizer : public Randomizer<ParkedVehicleRandomizer>
{
    int ForcedVehicle = -1;
#ifndef DONT_USE_MAP
    std::map<CCarGenerator*, int> m_LoadingCarGens;
#endif

public:
    template <auto &DoInternalProcessing>
    void
    RandomizeParkedVehicle (CCarGenerator *gen)
    {
        static int currentVeh = 170;
        int origModel = gen->m_nModelId;

        if (currentVeh == 242)
            currentVeh++;

        gen->m_nModelId
            = ForcedVehicle == -1 ? RandomInt (170, 280) : ForcedVehicle;

#ifndef DONT_USE_MAP
        if (int *id = LookupMap (m_LoadingCarGens, gen))
            {
                gen->m_nModelId = *id;
                m_LoadingCarGens.erase (gen);
            }
#endif

        DoInternalProcessing (gen);

        Rainbomizer::Logger::LogMessage("Spawning: %d", gen->m_nModelId);

#ifndef DONT_USE_MAP
        // Model is currently loading, let it load.
        if (gen->m_nPoolHandle == -1)
            m_LoadingCarGens[gen] = gen->m_nModelId;
#endif

        gen->m_nModelId = origModel;
    }

    ParkedVehicleRandomizer ()
    {
        RB_C_DO_CONFIG ("ParkedVehicleRandomizer", ForcedVehicle);
        HOOK_MEMBER (Jal, (0x8aed784), RandomizeParkedVehicle, void(CCarGenerator *));
    }
} g_parkedRando;
