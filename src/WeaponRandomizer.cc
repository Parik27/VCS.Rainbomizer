#include "core/Logger.hh"
#include "core/Randomizer.hh"
#include "core/Config.hh"

#include "Hooks.hh"
#include "weapon/Common.hh"

#include "CWeaponInfo.hh"
#include "CPlayer.hh"
#include "CPed.hh"
#include <bit>
#include <cstdint>

class WeaponRandomizer : public Randomizer<WeaponRandomizer>
{
    inline static int ForcedWeapon = -1;

    template <auto &CPed__GiveWeapon>
    static int
    RandomizeWeapon (CPed *ped, int weaponType, int ammo, int p4)
    {
        if (weaponType == 0)
            return CPed__GiveWeapon (ped, weaponType, ammo, p4);

        int targetSlot = CWeaponInfo::GetWeaponInfo(weaponType)->nSlot;

        int newWeapon = WeaponsCommon::GetRandomUsableWeapon ();
        if (ForcedWeapon != -1)
            newWeapon = ForcedWeapon;

        CWeaponInfo *newWeaponInfo = CWeaponInfo::GetWeaponInfo (newWeapon);
        int          newWeaponOriginalSlot = newWeaponInfo->nSlot;

        if (ped != FindPlayerPed ())
            newWeaponInfo->nSlot = targetSlot;

        int ret = CPed__GiveWeapon (ped, newWeapon, ammo, p4);

        newWeaponInfo->nSlot = newWeaponOriginalSlot;

        return ret;
    }

    template <auto &CWeapon__FireWeapon>
    static int
    FixProjectileThrowingThird (CWeapon *weap, class CPed *ped,
                                 class CVector *src)
    {
        if (!WeaponsCommon::IsProjectile(eWeapon(weap->Type)))
            return CWeapon__FireWeapon (weap, ped, src);

        auto prevSeekTarget = ped->pSeekTarget;
        if (ped != FindPlayerPed () && !ped->pSeekTarget)
            {
                if (ped->pPointGunAt)
                    {
                        auto& end   = ped->vecPosition;
                        auto& start = ped->pPointGunAt->vecPosition;

                        Rainbomizer::Logger::LogMessage (
                            "Checking pointing: %f %f %f to %f %f %f", start.x,
                            start.y, start.z, end.x, end.y, end.z);

                        if (!((int (*) (CVector *, CVector *, bool, bool, bool,
                                        bool, bool, bool, bool,
                                        bool)) 0x88967ac) (&start, &end, true,
                                                           true, false, true,
                                                           false, false, false,
                                                           false))
                            return 0;
                    }
                ped->pSeekTarget = ped->pPointGunAt;
            }

        Rainbomizer::Logger::LogMessage ("SEEK TARGET: %p -> %p",
                                         prevSeekTarget, ped->pSeekTarget);

        int ret          = CWeapon__FireWeapon (weap, ped, src);
        ped->pSeekTarget = prevSeekTarget;

        return ret;
    }

public:
    WeaponRandomizer ()
    {
        RB_C_DO_CONFIG ("WeaponRandomizer", ForcedWeapon)

        HOOK (Jmp, (0x0891b7dc), RandomizeWeapon,
              int (class CPed *, int, int, int));

        HOOK (Jmp, (0x08a456cc), FixProjectileThrowingThird,
              int (CWeapon* weaon, CPed* shooter, CVector* src));
        static constexpr const float thing = 0.1;
        static constexpr const uint32_t thing_ieee = std::bit_cast<uint32_t>(thing);
        static constexpr const uint32_t lower_bytes = (thing_ieee >> 16) & 0xFFFF;
        static constexpr const uint32_t higher_bytes = thing_ieee & 0xFFFF;

        injector.WriteMemory32 (0x8a45d50, lui(a0, lower_bytes));
        injector.WriteMemory32 (0x8a45d54, ori(a0, a0, higher_bytes));
    }
} g_WeaponRandomizer;
