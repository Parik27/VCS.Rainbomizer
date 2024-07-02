#include <hooks/Hooks.hh>

#include "core/Logger.hh"
#include "core/Randomizer.hh"
#include "core/Config.hh"

#include "utils/Random.hh"
#include "Common.hh"
#include "vcs/CModelInfo.hh"
#include "vcs/CVehicle.hh"

#include <ranges>
#include <vcs/CStreaming.hh>

class TrafficRandomizer : public Randomizer<TrafficRandomizer>
{
    inline static int ForcedVehicle = -1;

public:
    template <auto &CCarCtrl__ChooseCarModelToLoad>
    static int
    ChooseModelToLoad ()
    {
        return VehicleCommon::GetRandomUsableVehicle ();
    }

    template <auto &CCarCtrl__ChooseModel>
    static int
    RandomizeTrafficVehicle (class CZoneInfo *zone, int *pClass)
    {
        int model = CCarCtrl__ChooseModel (zone, pClass);

        model = GetRandomElement (VehicleCommon::LoadedUsableVehicles ());

        if (ForcedVehicle != -1)
            {
                if (VehicleCommon::AttemptToLoadVehicle (ForcedVehicle))
                    {
                        return ForcedVehicle;
                    }
            }

        return model;
    }

    template <auto &CCarCtrl__ChoosePoliceCarModel>
    static int
    RandomizePoliceTraffic ()
    {
        int model = CCarCtrl__ChoosePoliceCarModel ();

        model = GetRandomElement (VehicleCommon::LoadedUsableVehicles ());

        if (ForcedVehicle != -1)
            {
                if (VehicleCommon::AttemptToLoadVehicle (ForcedVehicle))
                    {
                        return ForcedVehicle;
                    }
            }

        return model;
    }

    template <auto &CCarAI__AddPoliceCarOccupants>
    static void 
    FixEmptyPoliceCars(CVehicle* vehicle)
    {       
        auto origModel = vehicle->m_nModelIndex;
        vehicle->m_nModelIndex = VEHICLE_POLICEM;

        CCarAI__AddPoliceCarOccupants(vehicle);
        vehicle->m_nModelIndex = origModel;
    }

    template <auto &CAutomobile__CAutomobile>
    static CVehicle *
    AddMisingVehicleConstructors (CVehicle *vehicle, int modelId,
                                  uint8_t createdBy, uint32_t a4)
    {
        int type = ModelInfo::GetModelInfo<CVehicleModelInfo> (modelId)
                       ->m_vehicleType;

        static constexpr auto CHeli__CHeli
            = GameFunction<0x89edce0, CVehicle *(CVehicle *, int, uint8_t)>{};

        static constexpr auto CPlane__CPlane
            = GameFunction<0x089d9a48, CVehicle *(CVehicle *, int, uint8_t)>{};

        switch (type)
            {
            case VEHICLE_TYPE_AUTOMOBILE:
                return CAutomobile__CAutomobile (vehicle, modelId, createdBy,
                                                 a4);
            case VEHICLE_TYPE_PLANE:
                return CPlane__CPlane (vehicle, modelId, createdBy);
            case VEHICLE_TYPE_HELI:
                return CHeli__CHeli (vehicle, modelId, createdBy);
            };

        return vehicle;
    }

    TrafficRandomizer ()
    {
        RB_C_DO_CONFIG ("TrafficRandomizer", ForcedVehicle);

        HOOK (Jmp, (0x08b4275c), RandomizeTrafficVehicle,
              int (class CZoneInfo *, int *));
        HOOK (Jmp, (0x8b45350), RandomizePoliceTraffic, int());
        HOOK (Jmp, (0x08b42198), ChooseModelToLoad, int ());
        HOOK (Jal, (0x08b48060), AddMisingVehicleConstructors,
              CVehicle * (CVehicle *, int, uint8_t, uint32_t));
        HOOK (Jal, (0x8b49444), FixEmptyPoliceCars, 
            void (CVehicle*));

            
    }
};
