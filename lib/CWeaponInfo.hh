#pragma once

enum eWeapon
{
    WEAPON_UNARMED,
    WEAPON_BRASSKNUCKLE,
    WEAPON_SCREWDRIVER,
    WEAPON_GOLFCLUB,
    WEAPON_NIGHTSTICK,
    WEAPON_KNIFE,
    WEAPON_BASEBALLBAT,
    WEAPON_HAMMER,
    WEAPON_CLEAVER,
    WEAPON_MACHETE,
    WEAPON_KATANA,
    WEAPON_CHAINSAW,
    WEAPON_GRENADE,
    WEAPON_DETONATEGRENADE,
    WEAPON_LANDMINE,
    WEAPON_SEAMINE,
    WEAPON_TEARGAS,
    WEAPON_MOLOTOV,
    WEAPON_ROCKET,
    WEAPON_COLT45,
    WEAPON_PYTHON,
    WEAPON_SHOTGUN,
    WEAPON_SPAS12SHOTGUN,
    WEAPON_STUBBYSHOTGUN,
    WEAPON_TEC9,
    WEAPON_UZI,
    WEAPON_SILENCEDINGRAM,
    WEAPON_MP5,
    WEAPON_M4,
    WEAPON_RUGER,
    WEAPON_SNIPERRIFLE,
    WEAPON_LASERSCOPE,
    WEAPON_ROCKETLAUNCHER,
    WEAPON_FLAMETHROWER,
    WEAPON_M60,
    WEAPON_MINIGUN,
    WEAPON_DETONATOR,
    WEAPON_HELICANNON,
    WEAPON_CAMERA,
    WEAPON_BINOCULARS,
};

class CWeaponInfo
{
    char unk[0x68];

public:
    int nSlot;
    int unk2;

    static CWeaponInfo *
    GetWeaponInfo (int id)
    {
        return ((CWeaponInfo * (*) (int, int) ) 0x8b20150) (0, id);
    }
};

class CWeapon
{
    int unk;
public:
    int Type;
};
