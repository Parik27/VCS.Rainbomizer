#pragma once

#include <cstddef>
#include <CVector.hh>

#define ATOFFSET(offset, type, name) struct { char _##name[offset]; type name; };

class CPed
{
public:
    union
    {
        ATOFFSET (0x30, CVector, vecPosition);
        ATOFFSET (0x81c, CPed *, pPointGunAt)
        ATOFFSET (0x450, CPed *, pSeekTarget)
    };

    int
    GiveWeapon (int weaponType, unsigned int ammo)
    {
        return ((int (*) (CPed *, int, unsigned int)) (
            0x891b7dc)) (this, weaponType, ammo);
    }
};

static_assert (offsetof (CPed, pPointGunAt) == 0x81c);
static_assert (offsetof (CPed, pSeekTarget) == 0x450);
static_assert (offsetof (CPed, vecPosition) == 0x30);
