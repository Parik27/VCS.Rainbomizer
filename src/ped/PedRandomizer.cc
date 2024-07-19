#include "core/Logger.hh"
#include "core/Config.hh"

#include "core/Randomizer.hh"
#include "memory/GameAddress.hh"
#include "vcs/CVehicle.hh"
#include "vcs/RslTexture.hh"
#include <ranges>
#include <utils/Utils.hh>
#include <utils/Random.hh>
#include <vcs/CStreaming.hh>
#include <vcs/CPed.hh>
#include <vcs/CDarkel.hh>
#include <vcs/CModelInfo.hh>
#include <vcs/CPickups.hh>

class PedRandomizer : Randomizer<PedRandomizer>
{
    inline static int ForcedPed = -1;

public:
    template <auto &SetModelIndex>
    static void
    RandomizePeds (CPed *ped, int model)
    {
        int newModel = model;

        if (!CStreaming::HasModelLoaded (model))
            {
                CStreaming::RequestModel (model);
                CStreaming::LoadAllRequestedModels (false);
            }

        SetModelIndex (ped, newModel);
    }

    PedRandomizer ()
    {
        RB_C_DO_CONFIG("PedRandomizer", ForcedPed);

        HOOK (Jal, (0x8906354), RandomizePeds, void (CPed *, int));
    }
};
