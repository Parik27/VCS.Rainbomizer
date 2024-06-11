#pragma once

enum Weapon
{
    Unarmed,
    BrassKnuckle,
    ScrewDriver,
    GolfClub,
    NightStick,
    Knife,
    BaseballBat,
    Hammer,
    Cleaver,
    Machete,
    Katana,
    Chainsaw,
    Grenade,
    DetonateGrenade,
    LandMine,
    SeaMine,
    TearGas,
    Molotov,
    Rocket,
    Colt45,
    Python,
    Shotgun,
    Spas12Shotgun,
    StubbyShotgun,
    Tec9,
    Uzi,
    SilencedIngram,
    Mp5,
    M4,
    Ruger,
    SniperRifle,
    LaserScope,
    RocketLauncher,
    FlameThrower,
    M60,
    Minigun,
    Detonator,
    HeliCannon,
    Camera,
    Binoculars,
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
