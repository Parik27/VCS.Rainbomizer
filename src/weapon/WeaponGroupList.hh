#include "WeaponGroups.hh"
#include "weapon/Common.hh"

constexpr static std::array s_WeaponGroups
{
    WeaponGroup{"all", ~WeaponGroup::CreateBitset (WeaponsCommon::BadWeapons)},

    // Individual weapon groups
    WeaponGroup{"brassknuckle", {WEAPON_BRASSKNUCKLE}},
    WeaponGroup{"butterfly", {WEAPON_BUTTERFLY}},
    WeaponGroup{"golfclub", {WEAPON_GOLFCLUB}},
    WeaponGroup{"nightstick", {WEAPON_NIGHTSTICK}},
    WeaponGroup{"knife", {WEAPON_KNIFE}},
    WeaponGroup{"baseballbat", {WEAPON_BASEBALLBAT}},
    WeaponGroup{"axe", {WEAPON_AXE}},
    WeaponGroup{"gaffhook", {WEAPON_GAFFHOOK}},
    WeaponGroup{"machete", {WEAPON_MACHETE}},
    WeaponGroup{"katana", {WEAPON_KATANA}},
    WeaponGroup{"chainsaw", {WEAPON_CHAINSAW}},
    WeaponGroup{"grenade", {WEAPON_GRENADE}},
    WeaponGroup{"detonatorgrenade", {WEAPON_DETONATOR_GRENADE}},
    WeaponGroup{"landmine", {WEAPON_LANDMINE}},
    WeaponGroup{"teargas", {WEAPON_TEARGAS}},
    WeaponGroup{"molotov", {WEAPON_MOLOTOV}},
    WeaponGroup{"pistol", {WEAPON_PISTOL}},
    WeaponGroup{"python", {WEAPON_PYTHON}},
    WeaponGroup{"shotgun", {WEAPON_SHOTGUN}},
    WeaponGroup{"spas12shotgun", {WEAPON_SPAS12_SHOTGUN}},
    WeaponGroup{"stubbyshotgun", {WEAPON_STUBBY_SHOTGUN}},
    WeaponGroup{"skorpion", {WEAPON_SKORPION}},
    WeaponGroup{"uzi", {WEAPON_UZI}},
    WeaponGroup{"silencedingram", {WEAPON_SILENCED_INGRAM}},
    WeaponGroup{"mp5", {WEAPON_MP5}},
    WeaponGroup{"m4", {WEAPON_M16}},
    WeaponGroup{"ruger", {WEAPON_AK47}},
    WeaponGroup{"sniperrifle", {WEAPON_SNIPERRIFLE}},
    WeaponGroup{"laserscope", {WEAPON_LASERSCOPE}},
    WeaponGroup{"rocketlauncher", {WEAPON_ROCKETLAUNCHER}},
    WeaponGroup{"flamethrower", {WEAPON_FLAMETHROWER}},
    WeaponGroup{"m60", {WEAPON_M249}},
    WeaponGroup{"minigun", {WEAPON_MINIGUN}},
    WeaponGroup{"camera", {WEAPON_CAMERA}},
    WeaponGroup{"binoculars", {WEAPON_BINOCULARS}},

    // Weapon type groups
    WeaponGroup{"melee",
                {WEAPON_BRASSKNUCKLE, WEAPON_BUTTERFLY, WEAPON_GOLFCLUB,
                 WEAPON_NIGHTSTICK, WEAPON_KNIFE, WEAPON_BASEBALLBAT,
                 WEAPON_AXE, WEAPON_GAFFHOOK, WEAPON_MACHETE, WEAPON_KATANA,
                 WEAPON_CHAINSAW}},

    WeaponGroup{"grenades",
                {WEAPON_GRENADE, WEAPON_DETONATOR_GRENADE, WEAPON_LANDMINE,
                 WEAPON_TEARGAS, WEAPON_MOLOTOV}},

    WeaponGroup{"shotguns",
                {WEAPON_SHOTGUN, WEAPON_SPAS12_SHOTGUN, WEAPON_STUBBY_SHOTGUN}},

    WeaponGroup{"smg",
                {WEAPON_SKORPION, WEAPON_UZI, WEAPON_SILENCED_INGRAM, WEAPON_MP5}},

    WeaponGroup{"pistols", {WEAPON_PISTOL, WEAPON_PYTHON}},
    WeaponGroup{"rifles", {WEAPON_M16, WEAPON_AK47, WEAPON_SNIPERRIFLE}},
    WeaponGroup{"special", {WEAPON_CAMERA, WEAPON_BINOCULARS}},
    WeaponGroup{"op", {WEAPON_MINIGUN, WEAPON_M249, WEAPON_MOLOTOV}},

    WeaponGroup{"heavy",
                {WEAPON_ROCKETLAUNCHER, WEAPON_FLAMETHROWER, WEAPON_M249,
                 WEAPON_MINIGUN}},

};
