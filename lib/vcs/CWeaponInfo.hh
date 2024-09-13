#pragma once

#include "memory/GameAddress.hh"
#include "vcs/CVector.hh"
#include <GhidraTypes.hh>

#include <cstdint>

enum eWeapon
{
    WEAPON_NONE              = 255, // -1
    WEAPON_UNARMED           = 0,
    WEAPON_BRASSKNUCKLE      = 1,
    WEAPON_BUTTERFLY         = 2,
    WEAPON_GOLFCLUB          = 3,
    WEAPON_NIGHTSTICK        = 4,
    WEAPON_KNIFE             = 5,
    WEAPON_BASEBALLBAT       = 6,
    WEAPON_AXE               = 7,
    WEAPON_GAFFHOOK          = 8,
    WEAPON_MACHETE           = 9,
    WEAPON_KATANA            = 10,
    WEAPON_CHAINSAW          = 11,
    WEAPON_GRENADE           = 12,
    WEAPON_DETONATOR_GRENADE = 13,
    WEAPON_LANDMINE          = 14,
    WEAPON_SEAMINE           = 15,
    WEAPON_TEARGAS           = 16,
    WEAPON_MOLOTOV           = 17,
    WEAPON_ROCKET            = 18,
    WEAPON_PISTOL            = 19,
    WEAPON_PYTHON            = 20,
    WEAPON_SHOTGUN           = 21,
    WEAPON_SPAS12_SHOTGUN    = 22,
    WEAPON_STUBBY_SHOTGUN    = 23,
    WEAPON_SKORPION          = 24,
    WEAPON_UZI               = 25,
    WEAPON_SILENCED_INGRAM   = 26,
    WEAPON_MP5               = 27,
    WEAPON_M16               = 28,
    WEAPON_AK47              = 29,
    WEAPON_SNIPERRIFLE       = 30,
    WEAPON_LASERSCOPE        = 31,
    WEAPON_ROCKETLAUNCHER    = 32,
    WEAPON_FLAMETHROWER      = 33,
    WEAPON_M249              = 34,
    WEAPON_MINIGUN           = 35,
    WEAPON_DETONATOR         = 36,
    WEAPON_HELICANNON        = 37,
    WEAPON_CAMERA            = 38,
    WEAPON_BINOCULARS        = 39,

    WEAPON_FIRST_WEAPON = WEAPON_UNARMED,
    WEAPON_LAST_WEAPON  = WEAPON_BINOCULARS,
    WEAPON_NUM_WEAPONS  = WEAPON_LAST_WEAPON - WEAPON_FIRST_WEAPON + 1,

    // Does this really continue from 41 instead of 40?
    WEAPON_HEALTH       = 41,
    WEAPON_ARMOUR       = 42,
    WEAPON_RAMMEDBYCAR  = 43,
    WEAPON_RUNOVERBYCAR = 44,
    // Did Leeds swap the order of UZI_DRIVEBY and EXPLOSION?
    WEAPON_UZI_DRIVEBY  = 45,
    WEAPON_EXPLOSION    = 46,
    WEAPON_DROWNING     = 47,
    WEAPON_FALL         = 48,
    WEAPON_UNIDENTIFIED = 49,
    WEAPON_ANYMELEE     = 50,
    WEAPON_ANYWEAPON    = 51
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
