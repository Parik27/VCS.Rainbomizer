#include <hooks/Hooks.hh>
#include "core/Randomizer.hh"

#include <vcs/CStreaming.hh>


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
        HOOK (Jmp, (0x08b4275c), RandomizeTrafficVehicle, int ());
    }
} g_TrafficRando;
