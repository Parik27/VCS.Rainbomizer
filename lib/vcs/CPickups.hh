#pragma once

#include "CVector.hh"

#include <cstddef>
#include <cstdint>

enum ePickup
{
    // WEAPONS
    PICKUP_PHONE = 330,
    PICKUP_BRASS_KNUCKLES = 331,
    PICKUP_BUTTERFLY_KNIFE = 332,
    PICKUP_GOLF_CLUB = 333,
    PICKUP_NIGHTSTICK = 334,
    PICKUP_KNIFE = 335,
    PICKUP_BASEBALL_BAT = 336,
    PICKUP_AXE = 337,
    PICKUP_GAFF_HOOK = 338,
    PICKUP_MACHETTE = 339,
    PICKUP_KATANA = 340,
    PICKUP_CHAINSAW = 341,
    PICKUP_GRENADE = 342,
    PICKUP_TEARGAS = 343,
    PICKUP_MOLOTOV = 344,
    PICKUP_MISSILE = 345,
    PICKUP_PISTOL = 346,
    PICKUP_PYTHON = 347,
    PICKUP_AK47 = 348,
    PICKUP_SHOTGUN = 349,
    PICKUP_COMBAT_SHOTGUN = 350,
    PICKUP_STUBBY_SHOTGUN = 351,
    PICKUP_ASSAULT_RIFLE = 352,
    PICKUP_SCORPION = 353,
    PICKUP_MICRO_SMG = 354,
    PICKUP_MAC_10 = 355,
    PICKUP_SMG = 356,
    PICKUP_SNIPER = 357,
    PICKUP_COMBAT_SNIPER = 358,
    PICKUP_RLAUNCHER = 359,
    PICKUP_FLAMETHROWER = 360,
    PICKUP_M249 = 361,
    PICKUP_MINIGUN = 362,
    PICKUP_REMOTE_EXPLOSIVE = 363,
    PICKUP_CAMERA = 364,

    PICKUP_BINOCULARS = 267,
    PICKUP_LANDMINE = 368,
    PICKUP_SEAMINE = 269,
    // GENERIC PICKUPS
    PICKUP_BRIEFCASE = 435,

    PICKUP_INFORMATION = 465,
    PICKUP_HEALTH = 466,
    PICKUP_ADRENALINE = 467,
    PICKUP_ARMOUR = 468,

    PICKUP_BRIBE = 474,
    PICKUP_MOVIE = 481,
    PICKUP_KILLFRENZY = 482,
    PICKUP_CLOTHES = 505,

    PICKUP_SAVE = 507,
    PICKUP_MUSIC = 510,
    PICKUP_KEYCARD = 608,
    // NEW VCS PICKUPS
    PICKUP_GD_DILDO = 7311,
    PICKUP_PINATA_MAN = 7313, // CObject
    PICKUP_NDC_DRUGPACKET = 7344,
    PICKUP_GD_BRIEFCASE_RED = 7371,
    PICKUP_GD_BRIEFCASE_BLUE = 7372,
    PICKUP_GD_BRIEFCASE_BROWN = 7373,
    // VCS MULTIPLAYER PICKUPS
    PICKUP_MEGADAMAGE = 7384,
    PICKUP_REGENHEALTH = 7385,
    PICKUP_INVISIBLE = 7386,
    PICKUP_RACEBAD = 7387,
    PICKUP_RACEGOOD = 7388,
    // NEW VCS PICKUPS
    PICKUP_RAMPAGECOMPLETED = 7401,
    PICKUP_BUY_VEHICLE = 7490,
};

class CPickup
{
public:
    CVector                    m_vecPos;
    float                      field1_0x10;
    uint8_t                    field2_0x14;
    uint8_t                    field3_0x15;
    uint8_t                    field4_0x16;
    uint8_t                    field5_0x17;
    uint8_t                    field6_0x18;
    uint8_t                    field7_0x19;
    uint8_t                    field8_0x1a;
    uint8_t                    field9_0x1b;
    uint8_t                    field10_0x1c;
    uint8_t                    field11_0x1d;
    uint8_t                    field12_0x1e;
    uint8_t                    field13_0x1f;
    struct CPickup            *m_pObject;
    float                      m_pExtraObject;
    struct CMultiplayerPickup *pMultiplayerPickup;
    uint32_t                   m_nQuantity;
    uint8_t                   *m_nTimer;
    uint16_t                   m_modelIndex;
    uint16_t                   m_nIndex;
    uint8_t                    m_eType;
    bool                       m_bRemoved;
    uint8_t                    m_nFlags;
    uint8_t                    field24_0x3b;
    char                       field25_0x3c;
    uint8_t                    field26_0x3d;
    uint8_t                    field27_0x3e;
    uint8_t                    field28_0x3f;
};

static_assert(offsetof(CPickup, m_modelIndex) == 0x34);
