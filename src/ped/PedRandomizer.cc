#include "core/Randomizer.hh"
#include <ranges>
#include <utils/Utils.hh>
#include <utils/Random.hh>
#include <vcs/CStreaming.hh>

class PedRandomizer : Randomizer<PedRandomizer>
{
public:
    template <auto &SetModelIndex>
    static void
    RandomizePeds (class CPed *ped, int model)
    {
        int newModel = GetRandomElement (
            std::views::iota (8, 167) | std::views::filter ([] (int i) {
                return CStreaming::HasModelLoaded (i);
            }));
        CStreaming::RequestModel (newModel, 0x3abccc);
        CStreaming::LoadAllRequestedModels (false);

        SetModelIndex (ped, newModel);
    }

    PedRandomizer ()
    {
        HOOK (Jal, (0x8906354), RandomizePeds, void (class CPed *, int));
    }
};
