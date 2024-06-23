#include <hooks/Hooks.hh>
#include "core/Randomizer.hh"
#include "vcs/CVehicle.hh"
#include "Common.hh"

#include <vcs/CStreaming.hh>


class TrafficRandomizer : public Randomizer<TrafficRandomizer>
{
public:
    template <auto &ChooseCarModelToLoad>
    static int
    ChooseModelToLoad ()
    {
        int model = 0;
        for (int i = 0; i < 21; i++)
        {
            model = VehicleCommon::GetRandomUsableVehicle();
            if (CStreaming::HasModelLoaded(model))
                continue;

            return model;
        }

        return -1;
    }

    template <auto &ChooseModel>
    static int
    RandomizeTrafficVehicle ()
    {
        int model = 0;

        for (int i = 0; i < 21; i++)
        {
            model = CStreaming::sm_Instance->m_vehiclesLoaded[RandomInt(0, CStreaming::sm_Instance->m_numVehiclesLoaded)];
            if (!CStreaming::HasModelLoaded(model))
                continue;

            return model;
        }

        return -1;
    }

    TrafficRandomizer ()
    {
        HOOK (Jmp, (0x08b4275c), RandomizeTrafficVehicle, int ());
        HOOK (Jmp, (0x8b42198), ChooseModelToLoad, int ());
    }
} g_TrafficRando;
