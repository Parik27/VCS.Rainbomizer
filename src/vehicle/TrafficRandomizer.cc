#include <hooks/Hooks.hh>
#include "core/Logger.hh"
#include "core/Randomizer.hh"
#include "utils/Random.hh"
#include "Common.hh"
#include "vcs/CModelInfo.hh"

#include <ranges>
#include <vcs/CStreaming.hh>

class TrafficRandomizer : public Randomizer<TrafficRandomizer>
{
public:
    template <auto &CCarCtrl__ChooseCarModelToLoad>
    static int
    ChooseModelToLoad ()
    {
        return VehicleCommon::GetRandomUsableVehicle();
    }

    template <auto &CCarCtrl__ChooseModel>
    static int
    RandomizeTrafficVehicle (class CZoneInfo* zone, int* pClass)
    {
        int model = CCarCtrl__ChooseModel (zone, pClass);

        model = GetRandomElement (
            VehicleCommon::LoadedUsableVehicles ()
            | std::views::filter (
                VehicleCommon::IsVehicleOfType<VEHICLE_TYPE_AUTOMOBILE,
                                               VEHICLE_TYPE_BIKE>));

        return model;
    }

    TrafficRandomizer ()
    {
        HOOK (Jmp, (0x08b4275c), RandomizeTrafficVehicle, int (class CZoneInfo*, int*));
        HOOK (Jmp, (0x08b42198), ChooseModelToLoad, int ());
    }
} g_TrafficRandomizer;
