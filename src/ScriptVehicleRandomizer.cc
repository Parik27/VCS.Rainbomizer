#include "ScriptVehicleRandomizer.hh"
#include "core/Logger.hh"
#include "core/Randomizer.hh"
#include "Hooks.hh"
#include <CStreaming.hh>

template <auto &CollectParams>
static int
RandomizeVehicle (class CRunningScript *scr, int *p2, int p3, int *params)
{
    CStreaming::RequestModel (244, 0x3abccc);
    CStreaming::LoadAllRequestedModels (false);

    int ret   = CollectParams (scr, p2, p3, params);
    params[0] = 270;

    return ret;
}

void
ScriptVehicleRandomizer::Initialise ()
{
    HOOK (Jal, ("15 3c ? ? b6 26 25 28 c0 03 04 00 06 34 ? ? ? ?", 14),
          RandomizeVehicle, int (class CRunningScript *, int *, int, int *));
}
