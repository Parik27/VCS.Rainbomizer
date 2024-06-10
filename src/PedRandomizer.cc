#include "core/Randomizer.hh"
#include <cstdio>
#include <vector>
#include <string>
#include <Utils.hh>
#include "Random.hh"
#include "CStreaming.hh"
#include "core/Common.hh"
#include "core/Config.hh"
#include "core/Logger.hh"

class PedRandomizer
{
public:
    template <auto &SetModelIndex>
    static void
    RandomizePeds (class CPed* ped, int model)
    {
        int newModel = 154;
        CStreaming::RequestModel (newModel, 0x3abccc);
        CStreaming::LoadAllRequestedModels (false);

        SetModelIndex(ped, newModel);
    }
    
    PedRandomizer()
    {
        HOOK (Jal, (0x8906354), RandomizePeds, void (class CPed*, int));
    }
} g_pedRando;