#include "ScriptVehicleRandomizer.hh"
#include "core/Randomizer.hh"
#include "Hooks.hh"
#include "log.h"
#include <math.h>
#include <CStreaming.hh>

CREATE_RANDOMIZER_WRAPPER (CutsceneRandomizer);

template <auto &CollectParams>
static int
RandomizeVehicle (class CRunningScript *scr, int *p2, int p3, int *params)
{
    INITIALISE_RANDOMIZER (CutsceneRandomizer);

    CStreaming::RequestModel (280, 0x3abccc);
    CStreaming::LoadAllRequestedModels (false);

    int ret = CollectParams (scr, p2, p3, params);
    params[0] = 280;

    return ret;
}

void
ScriptVehicleRandomizer::Initialise ()
{
    REGISTER_HOOK ("15 3c ? ? b6 26 25 28 c0 03 04 00 06 34 ? ? ? ?", 14,
                   RandomizeVehicle, int, class CRunningScript*, int*, int, int*);
}
