#pragma once

#include "memory/GameAddress.hh"
#include "vcs/CVector.hh"
#include <GhidraTypes.hh>

#include <cstdint>

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
    WEAPON_NUM_WEAPONS
};

class CWeaponInfo
{
public:
    uint      m_flags;
    uint      m_fireType;
    float     m_fRange;
    uint      m_nFiringRate;
    uint      m_nReload;
    uint      m_nAmountOfAmmunition;
    uint      m_nDamage;
    float     m_fSpeed;
    float     m_fRadius;
    float     m_fLifeSpan;
    float     m_fSpread;
    undefined field11_0x2c;
    undefined field12_0x2d;
    undefined field13_0x2e;
    undefined field14_0x2f;
    CVector   m_vecFireOffset;
    dword     m_AnimToPlay;
    float     m_fAnimLoopStart;
    float     m_fAnimLoopEnd;
    float     m_fAnimFrameFire;
    float     m_fAnim2LoopStart;
    float     m_fAnim2LoopEnd;
    float     m_fAnim2FrameFire;
    float     m_fAnimBreakOut;
    uint      nWeaponModel1;
    int       nWeaponModel2; /* Created by retype action */
    uint      nSlot;
    undefined field27_0x6c;
    undefined field28_0x6d;
    undefined field29_0x6e;
    undefined field30_0x6f;

    static CWeaponInfo *
    GetWeaponInfo (int id)
    {
        return GameFunction<0x8b20150, CWeaponInfo *(int)>::Call (id);
    }
};

class CWeapon {
public:
    uint8_t field0_0x0;
    uint8_t field1_0x1;
    uint8_t field2_0x2;
    uint8_t field3_0x3;
    uint32_t Type; /* Created by retype action */
    int field5_0x8;
    uint8_t field6_0xc;
    uint8_t field7_0xd;
    uint8_t field8_0xe;
    uint8_t field9_0xf;
    uint32_t Ammo; /* Created by retype action */
    uint32_t nTimer;
    uint8_t field12_0x18;
    uint8_t field13_0x19;
    uint8_t field14_0x1a;
    uint8_t field15_0x1b;
};

static_assert (sizeof (CWeapon) == 0x1c);

class CProjectileInfo
{
public:
    static int
    AddProjectile (float speed, class CPed *shooter, int type,
                   class CVector *pos, bool quadDamage)
    {
        return GameFunction<0x88809f8, int (float, class CPed *, int, CVector *,
                                            bool)>::Call (speed, shooter, type,
                                                          pos, quadDamage);
    }
};
