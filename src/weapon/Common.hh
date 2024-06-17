#pragma once

#include "CWeaponInfo.hh"
#include "ContainerUtils.hh"
#include "Random.hh"
#include <array>
#include <Utils.hh>

class WeaponsCommon
{
    static constexpr std::array BadWeapons
        = {WEAPON_UNARMED, WEAPON_HELICANNON, WEAPON_ROCKET, WEAPON_SEAMINE};

    static constexpr std::array Projectiles
        = {WEAPON_ROCKET,  WEAPON_ROCKETLAUNCHER, WEAPON_MOLOTOV,
           WEAPON_TEARGAS, WEAPON_SEAMINE,        WEAPON_LANDMINE,
           WEAPON_GRENADE};

public:
    static constexpr bool
    IsBadWeapon (eWeapon weapon)
    {
        return DoesElementExist (BadWeapons, weapon);
    }

    static int
    GetRandomUsableWeapon ()
    {
        return RandomIntExcept<BadWeapons> (WEAPON_UNARMED, WEAPON_BINOCULARS);
    }

    static constexpr bool
    IsProjectile (eWeapon weapon)
    {
        return DoesElementExist (Projectiles, weapon);
    }
};
