#pragma once

#include "CVector.hh"

#include <cstddef>
#include <cstdint>

enum ePickup
{
    PICKUP_NONE = -1,

    // WEAPONS
    PICKUP_CELLPHONE = 330,
    PICKUP_BRASSKNUCKLE = 331,
    PICKUP_BUTT_KNIFE = 332,
    PICKUP_GOLFCLUB = 333,
    PICKUP_NITESTICK = 334,
    PICKUP_KNIFECUR = 335,
    PICKUP_BAT = 336,
    PICKUP_HANDAXE = 337,
    PICKUP_GAFF = 338,
    PICKUP_MACHETE = 339,
    PICKUP_KATANA = 340,
    PICKUP_CHNSAW = 341,
    PICKUP_GRENADE = 342,
    PICKUP_TEARGAS = 343,
    PICKUP_MOLOTOV = 344,
    PICKUP_MISSILE = 345,
    PICKUP_BERETTA = 346,
    PICKUP_PYTHON = 347,
    PICKUP_AK47 = 348,
    PICKUP_CHROMEGUN = 349,
    PICKUP_SHOTGSPA = 350,
    PICKUP_BUDDYSHOT = 351,
    PICKUP_M16 = 352,
    PICKUP_SKORPION = 353,
    PICKUP_UZI = 354,
    PICKUP_INGRAMSL = 355,
    PICKUP_MPLNG = 356,
    PICKUP_SNIPER = 357,
    PICKUP_LASER = 358,
    PICKUP_ROCKETLA = 359,
    PICKUP_FLAME = 360,
    PICKUP_M249 = 361,
    PICKUP_MINIGUN = 362,
    PICKUP_BOMB = 363,
    PICKUP_CAMERA = 364,
    // leftover from middle finger animation, same model as PICKUP_INFO
    PICKUP_FINGERS = 365,
    PICKUP_MINIGUN2 = 366, // extra model for PICKUP_MINIGUN
    PICKUP_BINOCULARS = 367,
    PICKUP_LANDMINE = 368,
    PICKUP_SEAMINE = 369,

    PICKUP_FIRST_WEAPON = PICKUP_CELLPHONE,
    PICKUP_LAST_WEAPON = PICKUP_SEAMINE,
    PICKUP_NUM_WEAPONS = PICKUP_LAST_WEAPON - PICKUP_FIRST_WEAPON + 1,
    // WEAPONS END

    // GENERIC PICKUPS
    PICKUP_BRIEFCASE = 435,

    PICKUP_INFO = 465,
    PICKUP_HEALTH = 466,
    PICKUP_ADRENALINE = 467,
    PICKUP_BODYARMOUR = 468,

    PICKUP_BRIBE = 474,
    PICKUP_BONUS = 475,

    PICKUP_CAMERAPICKUP = 481,
    PICKUP_KILLFRENZY = 482,

//  PICKUP_PROPERTY_LOCKED = 502,
    PICKUP_PROPERTY_FSALE  = 503,
    PICKUP_BIGDOLLAR       = 504,
    PICKUP_CLOTHESSP       = 505,
//  PICKUP_PACKAGE1        = 506,
    PICKUP_PICKUPSAVE      = 507,

//  PICKUP_PICKUPMUSIC = 510,

//  PICKUP_DRUG_WHITE  = 602,
//  PICKUP_DRUG_ORANGE = 603,
//  PICKUP_DRUG_YELLOW = 604,
//  PICKUP_DRUG_GREEN  = 605,
//  PICKUP_DRUG_BLUE   = 606,
//  PICKUP_DRUG_RED    = 607,
    PICKUP_KEYCARD     = 608,
    // GENERIC END

    // ISLAND03 PICKUPS
    PICKUP_GD_DILDO = 7311,

    PICKUP_PINATA_MAN = 7313, // originally a mission prop

    PICKUP_NDC_DRUGPACKET = 7344,

    PICKUP_GD_BRIEFCASE_RED   = 7371,
    PICKUP_GD_BRIEFCASE_BLUE  = 7372,
    PICKUP_GD_BRIEFCASE_BROWN = 7373,

    PICKUP_MEGADAMAGE  = 7384,
    PICKUP_REGENERATOR = 7385,
    PICKUP_INVISIBLE   = 7386,
    PICKUP_BAD_CAR     = 7387,
    PICKUP_GOOD_CAR    = 7388,

    PICKUP_RAMPAGECOMPLETED = 7401,

    PICKUP_BUY_VEHICLE = 7490
    // ISLAND03 END
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
