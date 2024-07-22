#pragma once

#include "CMatrix.hh"
#include "memory/Memory.hh"
#include <cstddef>

#include "CVector.hh"
#include "CWeaponInfo.hh"

#define ATOFFSET(offset, type, name) struct { char _##name[offset]; type name; };
#define ATOFFSETARR(offset, type, name, size) struct { char _##name[offset]; type name[size]; };

class CPed
{
public:
    union
    {
        ATOFFSET (0x0, CMatrix, m_matrix);
        ATOFFSET (0x56, short, m_nModelIndex);
        ATOFFSET (0x30, CVector, vecPosition);
        ATOFFSET (0x81c, CPed *, pPointGunAt)
        ATOFFSET (0x450, CPed *, pSeekTarget)
        ATOFFSETARR (0x574, CWeapon, Weapons, 10)
    };

    int
    GiveWeapon (int weaponType, unsigned int ammo)
    {
        return GameFunction<0x891b7dc, int (CPed *, int, unsigned int)>::Call (
            this, weaponType, ammo);
    }
};

static_assert (offsetof (CPed, pPointGunAt) == 0x81c);
static_assert (offsetof (CPed, pSeekTarget) == 0x450);
static_assert (offsetof (CPed, vecPosition) == 0x30);
static_assert (offsetof (CPed, Weapons) == 0x574);
