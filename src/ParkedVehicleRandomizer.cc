#include "Hooks.hh"
#include "core/Randomizer.hh"
#include "log.h"
#include "CCarGenerator.hh"
#include "Random.hh"
#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <set>
#include <string>

class ParkedVehicleRandomizer : public Randomizer<ParkedVehicleRandomizer>
{
public:
    template <auto &DoInternalProcessing>
    static void
    RandomizeParkedVehicle (CCarGenerator *gen)
    {
        int origModel = gen->m_nModelId;

        gen->m_nModelId = RandomInt(170, 280);
        DoInternalProcessing (gen);
        gen->m_nModelId = origModel;
    }

    ParkedVehicleRandomizer ()
    {
        REGISTER_HOOK_ADDR (0x8aed784, RandomizeParkedVehicle, void, CCarGenerator*);
    }
} g_parkedRando;
