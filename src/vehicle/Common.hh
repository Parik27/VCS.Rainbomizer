#pragma once

#include <array>
#include "CVehicle.hh"
#include "ContainerUtils.hh"
#include "Random.hh"
#include <Utils.hh>

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

    static int
    GetRandomUsableVehicle ()
    {
        return RandomIntExcept<BadVehicles> (VEHICLE_6ATV, VEHICLE_AIRTRAIN);
    }
};
