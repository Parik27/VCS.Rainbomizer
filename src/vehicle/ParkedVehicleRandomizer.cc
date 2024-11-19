#include <hooks/Hooks.hh>

#include <core/Logger.hh>
#include <core/Randomizer.hh>
#include <core/Config.hh>

#include <vcs/CVehicle.hh>
#include <vcs/CCarGenerator.hh>
#include <vcs/CStreaming.hh>

#include "Common.hh"

#include <map>

#include <utils/ContainerUtils.hh>
#include <utils/Random.hh>

//#define DONT_USE_MAP // decreases randomness, but decreases memory usage.

class ParkedVehicleRandomizer : public Randomizer<ParkedVehicleRandomizer>
{
    int ForcedVehicle = -1;
    bool UseSeed = false;
#ifndef DONT_USE_MAP
    std::map<CCarGenerator*, int> m_LoadingCarGens;
#endif

public:
    template <auto &DoInternalProcessing>
    void
    RandomizeParkedVehicle (CCarGenerator *gen)
    {
        int origModel = gen->m_nModelId;

        RainbomizerRandomizationEngine::RandomizerBlock block{
            UseSeed, gen->m_nModelId, gen->x, gen->y, gen->z
        };

        if (CStreaming::sm_Instance->m_numVehiclesLoaded < 15)
            gen->m_nModelId = ForcedVehicle == -1
                                  ? VehicleCommon::GetRandomUsableVehicle ()
                                  : ForcedVehicle;
        else
            gen->m_nModelId = VehicleCommon::GetRandomUsableLoadedVehicle ();

#ifndef DONT_USE_MAP
        if (int *id = LookupMap (m_LoadingCarGens, gen))
            {
                gen->m_nModelId = *id;
                m_LoadingCarGens.erase (gen);
            }
#endif

        DoInternalProcessing (gen);

#ifndef DONT_USE_MAP
        // Model is currently loading, let it load.
        if (gen->m_nPoolHandle == -1)
            m_LoadingCarGens[gen] = gen->m_nModelId;
#endif

        gen->m_nModelId = origModel;
    }

    ParkedVehicleRandomizer ()
    {
        RB_C_DO_CONFIG ("ParkedVehicleRandomizer", ForcedVehicle, UseSeed);
        HOOK_MEMBER (Jal, (0x8aed784), RandomizeParkedVehicle, void(CCarGenerator *));
    }
};
