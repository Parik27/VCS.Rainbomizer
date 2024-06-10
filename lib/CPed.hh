#pragma once

class CPed
{
public:
    int
    GiveWeapon (int weaponType, unsigned int ammo)
    {
        return ((int (*) (CPed *, int, unsigned int)) (
            0x891b7dc)) (this, weaponType, ammo);
    }
};
