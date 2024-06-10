#include "core/Logger.hh"
#include "core/Randomizer.hh"
#include "core/Config.hh"

#include "Hooks.hh"

#include "CWeaponInfo.hh"
#include "CPlayer.hh"
#include "CPed.hh"

class WeaponRandomizer : public Randomizer<WeaponRandomizer>
{
    template <auto &CPed__GiveWeapon>
    static int
    RandomizeWeapon (CPed *ped, int weaponType, int ammo, int p4)
    {
        if (weaponType == 0)
            return CPed__GiveWeapon (ped, weaponType, ammo, p4);

        int targetSlot = CWeaponInfo::GetWeaponInfo(weaponType)->nSlot;

        int newWeapon = 32;
        CWeaponInfo* newWeaponInfo = CWeaponInfo::GetWeaponInfo(newWeapon);
        int newWeaponOriginalSlot = newWeaponInfo->nSlot;

        if (ped != FindPlayerPed())
            newWeaponInfo->nSlot = targetSlot;

        int ret = CPed__GiveWeapon (ped, newWeapon, ammo, p4);

        newWeaponInfo->nSlot = newWeaponOriginalSlot;
        return ret;
    }

public:
    WeaponRandomizer ()
    {
        RB_C_DO_CONFIG_NO_OPTIONS ("WeaponRandomizer")

        HOOK (Jmp, (0x0891b7dc), RandomizeWeapon,
              int (class CPed *, int, int, int));
    }
} g_WeaponRandomizer;
