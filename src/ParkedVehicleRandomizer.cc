#include "Hooks.hh"
#include "core/Logger.hh"
#include "core/Randomizer.hh"
#include "CCarGenerator.hh"
#include "Random.hh"

class ParkedVehicleRandomizer : public Randomizer<ParkedVehicleRandomizer>
{
public:
    template <auto &DoInternalProcessing>
    static void
    RandomizeParkedVehicle (CCarGenerator *gen)
    {
        int origModel = gen->m_nModelId;

        gen->m_nModelId = RandomInt (170, 280);
        Rainbomizer::Logger::LogMessage("Spawned: %d", gen->m_nModelId);
        DoInternalProcessing (gen);
        gen->m_nModelId = origModel;
    }

    ParkedVehicleRandomizer ()
    {
        HOOK (Jal, (0x8aed784), RandomizeParkedVehicle, void(CCarGenerator *));
    }
} g_parkedRando;
