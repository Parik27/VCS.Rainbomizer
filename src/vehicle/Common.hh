#pragma once

#include "vcs/CModelInfo.hh"
#include "vcs/CStreaming.hh"
#include <array>

#include <bitset>
#include <vcs/CVehicle.hh>

#include <utils/ContainerUtils.hh>
#include <utils/Random.hh>
#include <utils/Utils.hh>

class VehicleCommon
{
    static constexpr std::array BadVehicles
        = {VEHICLE_TOPFUN, VEHICLE_AIRTRAIN};

public:
    static constexpr bool
    IsBadVehicle (eVehicle veh)
    {
        return DoesElementExist (BadVehicles, veh);
    }

    static auto
    AllUsableVehicles ()
    {
        return std::views::iota (int (VEHICLE_6ATV), VEHICLE_AIRTRAIN)
               | std::views::filter (
                   [] (int i) { return !IsBadVehicle (eVehicle (i)); });
    }

    static auto
    LoadedUsableVehicles ()
    {
        return std::span (CStreaming::sm_Instance->m_vehiclesLoaded)
               | std::views::filter ([] (int i) {
                     return i != -1 && !IsBadVehicle (eVehicle (i));
                 });
    }

    static int
    GetRandomUsableVehicle ()
    {
        return RandomIntExcept<BadVehicles> (VEHICLE_6ATV, VEHICLE_AIRTRAIN);
    }

    static int
    GetRandomUsableLoadedVehicle ()
    {
        return GetRandomElement (LoadedUsableVehicles ());
    }

    static bool
    AttemptToLoadVehicle (int model, int retries = 5)
    {
        size_t tries = 0;
        while (!CStreaming::HasModelLoaded (model))
            {
                if (tries++ >= retries)
                    {
                        return false;
                    }

                CStreaming::RequestModel (model, 0);
                CStreaming::LoadAllRequestedModels (false);
            }

        return true;
    }

    template <eVehicleType... TYPES>
    static bool
    IsVehicleOfType (int veh)
    {
        auto type = ModelInfo::GetModelInfo<CVehicleModelInfo> (veh);
        return (... || (type->m_vehicleType == TYPES));
    }
};
