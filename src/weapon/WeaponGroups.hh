#pragma once

#include <bitset>
#include <vcs/CWeaponInfo.hh>

struct WeaponGroup
{
    template<typename T>
    static consteval std::bitset<WEAPON_NUM_WEAPONS>
    CreateBitset (const T &weapons)
    {
        std::bitset<WEAPON_NUM_WEAPONS> result;
        for (auto w : weapons)
            result.set (w);
        return result;
    }

    const char                     *Name;
    std::bitset<WEAPON_NUM_WEAPONS> Weapons;

    consteval WeaponGroup (const char *name, std::initializer_list<int> weapons)
        : Name (name), Weapons (CreateBitset (weapons))
    {
    }

    consteval WeaponGroup (const char                     *name,
                           std::bitset<WEAPON_NUM_WEAPONS> weapons)
        : Name (name), Weapons (weapons)
    {
    }

    bool
    CheckWeapon (int weapon) const
    {
        return Weapons.test (weapon);
    }

  template<typename Functor>
    auto
    ForEachWeapon (Functor f) const
    {
        for (int i = 0; i < WEAPON_NUM_WEAPONS; i++)
            if (Weapons.test (i))
                f (i);
    }
};
