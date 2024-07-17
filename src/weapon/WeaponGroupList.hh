#include "WeaponGroups.hh"

constexpr static std::array s_WeaponGroups{
    WeaponGroup{"all",
                ~WeaponGroup::CreateBitset ({WEAPON_UNARMED, WEAPON_HELICANNON,
                                             WEAPON_ROCKET, WEAPON_SEAMINE})},

    // Individual weapon groups
    WeaponGroup{"brassknuckle", {WEAPON_BRASSKNUCKLE}},
    WeaponGroup{"screwdriver", {WEAPON_SCREWDRIVER}},
    WeaponGroup{"golfclub", {WEAPON_GOLFCLUB}},
    WeaponGroup{"nightstick", {WEAPON_NIGHTSTICK}},
    WeaponGroup{"knife", {WEAPON_KNIFE}},
    WeaponGroup{"baseballbat", {WEAPON_BASEBALLBAT}},
    WeaponGroup{"hammer", {WEAPON_HAMMER}},
    WeaponGroup{"cleaver", {WEAPON_CLEAVER}},
    WeaponGroup{"machete", {WEAPON_MACHETE}},
    WeaponGroup{"katana", {WEAPON_KATANA}},
    WeaponGroup{"chainsaw", {WEAPON_CHAINSAW}},
    WeaponGroup{"grenade", {WEAPON_GRENADE}},
    WeaponGroup{"detonategrenade", {WEAPON_DETONATEGRENADE}},
    WeaponGroup{"landmine", {WEAPON_LANDMINE}},
    WeaponGroup{"teargas", {WEAPON_TEARGAS}},
    WeaponGroup{"molotov", {WEAPON_MOLOTOV}},
    WeaponGroup{"colt45", {WEAPON_COLT45}},
    WeaponGroup{"python", {WEAPON_PYTHON}},
    WeaponGroup{"shotgun", {WEAPON_SHOTGUN}},
    WeaponGroup{"spas12shotgun", {WEAPON_SPAS12SHOTGUN}},
    WeaponGroup{"stubbyshotgun", {WEAPON_STUBBYSHOTGUN}},
    WeaponGroup{"tec9", {WEAPON_TEC9}},
    WeaponGroup{"uzi", {WEAPON_UZI}},
    WeaponGroup{"silencedingram", {WEAPON_SILENCEDINGRAM}},
    WeaponGroup{"mp5", {WEAPON_MP5}},
    WeaponGroup{"m4", {WEAPON_M4}},
    WeaponGroup{"ruger", {WEAPON_RUGER}},
    WeaponGroup{"sniperrifle", {WEAPON_SNIPERRIFLE}},
    WeaponGroup{"laserscope", {WEAPON_LASERSCOPE}},
    WeaponGroup{"rocketlauncher", {WEAPON_ROCKETLAUNCHER}},
    WeaponGroup{"flamethrower", {WEAPON_FLAMETHROWER}},
    WeaponGroup{"m60", {WEAPON_M60}},
    WeaponGroup{"minigun", {WEAPON_MINIGUN}},
    WeaponGroup{"detonator", {WEAPON_DETONATOR}},
    WeaponGroup{"camera", {WEAPON_CAMERA}},
    WeaponGroup{"binoculars", {WEAPON_BINOCULARS}},

    // Weapon type groups
    WeaponGroup{"melee",
                {WEAPON_BRASSKNUCKLE, WEAPON_SCREWDRIVER, WEAPON_GOLFCLUB,
                 WEAPON_NIGHTSTICK, WEAPON_KNIFE, WEAPON_BASEBALLBAT,
                 WEAPON_HAMMER, WEAPON_CLEAVER, WEAPON_MACHETE, WEAPON_KATANA,
                 WEAPON_CHAINSAW}},

    WeaponGroup{"grenades",
                {WEAPON_GRENADE, WEAPON_DETONATEGRENADE, WEAPON_LANDMINE,
                 WEAPON_TEARGAS, WEAPON_MOLOTOV}},

    WeaponGroup{"shotguns",
                {WEAPON_SHOTGUN, WEAPON_SPAS12SHOTGUN, WEAPON_STUBBYSHOTGUN}},

    WeaponGroup{"smg",
                {WEAPON_TEC9, WEAPON_UZI, WEAPON_SILENCEDINGRAM, WEAPON_MP5}},

    WeaponGroup{"pistols", {WEAPON_COLT45, WEAPON_PYTHON}},
    WeaponGroup{"rifles", {WEAPON_M4, WEAPON_RUGER, WEAPON_SNIPERRIFLE}},
    WeaponGroup{"special",
                {WEAPON_DETONATOR, WEAPON_CAMERA, WEAPON_BINOCULARS}},
    WeaponGroup{"op", {WEAPON_MINIGUN, WEAPON_M60, WEAPON_MOLOTOV}},

    WeaponGroup{"heavy",
                {WEAPON_ROCKETLAUNCHER, WEAPON_FLAMETHROWER, WEAPON_M60,
                 WEAPON_MINIGUN}},

};
