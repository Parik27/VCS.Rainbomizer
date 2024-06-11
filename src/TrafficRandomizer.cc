
#include "Hooks.hh"
#include "core/Logger.hh"
#include "core/Randomizer.hh"
#include "log.h"
#include <CStreaming.hh>
#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <set>
#include <string>

class TrafficRandomizer : public Randomizer<TrafficRandomizer>
{
public:
    template <auto &ChooseModel>
    static int
    RandomizeTrafficVehicle ()
    {
        int model = 270;

        CStreaming::RequestModel (model, 0x3abccc);
        CStreaming::LoadAllRequestedModels (false);

        return model;
    }

    TrafficRandomizer ()
    {
        for (int addr : {0x8b47b94, 0x8b47478, 0x8b4729c, 0x08acbccc})
            HOOK (Jal, (addr), RandomizeTrafficVehicle, int ());
    }
} g_TrafficRando;
