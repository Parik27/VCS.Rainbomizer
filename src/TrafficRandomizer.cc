
#include "Hooks.hh"
#include "core/Randomizer.hh"
#include "log.h"
#include <CStreaming.hh>
#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <set>
#include <string>

class TrafficRandomizer
{
    BEGIN_RANDOMIZER (TrafficRandomizer)

public:
template <auto &ChooseModel>
static int
RandomizeTrafficVehicle ()
{
    int model = 244;
    CStreaming::RequestModel (model, 0x3abccc);
    CStreaming::LoadAllRequestedModels (false);

    return model;
}

    TrafficRandomizer()
    {
        REGISTER_HOOK_ADDR(0x8b47b94, RandomizeTrafficVehicle, int);
    }

    END_RANDOMIZER (TrafficRandomizer)
};