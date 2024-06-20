#pragma once

#include <array>

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

    static int
    GetRandomUsableVehicle ()
    {
        return RandomIntExcept<BadVehicles> (VEHICLE_6ATV, VEHICLE_AIRTRAIN);
    }
};
