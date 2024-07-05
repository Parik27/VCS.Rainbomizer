#include <cstdint>

#include <memory/Memory.hh>
#include <hooks/Hooks.hh>

#include "core/Logger.hh"
#include "core/Randomizer.hh"
#include "core/Config.hh"

#include "memory/GameAddress.hh"
#include "weapon/Common.hh"

 #include <vcs/CVehicle.hh>
#include <vcs/CWeaponInfo.hh>
#include <vcs/CPlayer.hh>
#include <vcs/CPed.hh>
#include <vcs/CDarkel.hh>


class WeaponRandomizer : public Randomizer<WeaponRandomizer>
{
    inline static int ForcedWeapon = -1;
    inline static bool DisableWeaponRandomizer = false;
    inline static int rampage_weapon = -1;

    template <auto &CPed__GiveWeapon>
    static int
    RandomizeWeapon (CPed *ped, int weaponType, int ammo, int p4)
    {
        if (weaponType == 0 || DisableWeaponRandomizer)
            return CPed__GiveWeapon (ped, weaponType, ammo, p4);

        int newWeapon = WeaponsCommon::GetRandomUsableWeapon ();

        if (ped == FindPlayerPed () && CDarkel::FrenzyGoingOn ()
            && rampage_weapon != -1)
        {
            newWeapon = rampage_weapon;
            rampage_weapon = -1;
        }

        if (ForcedWeapon != -1)
            newWeapon = ForcedWeapon;

        return CPed__GiveWeapon (ped, newWeapon, ammo, p4);
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

                        // if (!((int (*) (CVector *, CVector *, bool, bool, bool,
                        //                 bool, bool, bool, bool,
                        //                 bool)) 0x88967ac) (&start, &end, true,
                        //                                    true, false, true,
                        //                                    false, false, false,
                        //                                    false))
                        //     return 0;
                    }
                ped->pSeekTarget = ped->pPointGunAt;
            }

        Rainbomizer::Logger::LogMessage ("SEEK TARGET: %p -> %p",
                                         prevSeekTarget, ped->pSeekTarget);

        int ret          = CWeapon__FireWeapon (weap, ped, src);
        ped->pSeekTarget = prevSeekTarget;

        return ret;
    }

 template<auto& CPed__SetActiveWeaponSlot>
 static void RandomizeSelectedWeapon (CPed* ped, int slot)
    {
        static std::array<uint32_t, 10> usedSlots;

        size_t numSlots = 0;
        for (size_t i = 0; i < 10; i++)
            if (ped->Weapons[i].Type && ped->Weapons[i].Ammo)
                usedSlots[numSlots++] = i;

        if (!numSlots || (FindPlayerPed () == ped))
            return CPed__SetActiveWeaponSlot (ped, slot);

        CPed__SetActiveWeaponSlot (ped, usedSlots[RandomInt (numSlots - 1)]);
    }

    template <auto &CWeapon__FireInstantHitFromCar>
    static void
    FireProjectilesDuringDriveby (CWeapon *weapon, CVehicle *veh,
                                  bool left, bool right)
    {

        if (WeaponsCommon::IsProjectile(eWeapon(weapon->Type)))
            {
                CPed* ped = FindPlayerPed();

                static CVector pos;
                uint32_t type = weapon->Type;

                if (weapon->Type == WEAPON_ROCKETLAUNCHER)
                    type = WEAPON_ROCKET;

                pos = FindPlayerPed ()->vecPosition;

                if (left)
                    pos -= ped->m_matrix.right * 3;

                else if (right)
                    pos += ped->m_matrix.right * 10;

                else
                    pos += ped->m_matrix.forward * 3;

                CProjectileInfo::AddProjectile (3.0f, FindPlayerPed (),
                                                type, &pos, false);
            }

        CWeapon__FireInstantHitFromCar (weapon, veh, left, right);
    }

 template<auto& CPickups__Update>
 static void
 SkipWeaponRandomizationForPickups ()
    {
        DisableWeaponRandomizer = true;
        CPickups__Update ();
        DisableWeaponRandomizer = false;
    }

template<auto &CDarkel__StartFrenzy>
static void 
SetRandomPlayerWeaponForRampage(int weapon, int time, short kill, int modelId0, short* text,
		int modelId2, int modelId3, int modelId4, bool standardSound, bool needHeadshot)
{
    rampage_weapon = WeaponsCommon::GetRandomUsableWeapon ();
    CDarkel__StartFrenzy(rampage_weapon, time, kill, modelId0, text,
        modelId2, modelId3, modelId4, standardSound, needHeadshot);
 }
 
public:
    WeaponRandomizer ()
    {
        RB_C_DO_CONFIG ("WeaponRandomizer", ForcedWeapon)

        HOOK (Jmp, (0x0891b7dc), RandomizeWeapon,
              int (class CPed *, int, int, bool));

        HOOK (Jmp, (0x08908080), RandomizeSelectedWeapon,
              void (class CPed *, int));

        // Projectile throwing fixes
        // ===========================================
        GameAddress<0x08911e6c>::Nop ();
        GameAddress<0x08949b50>::Nop ();
        GameAddress<0x0894b934>::Nop ();
        GameAddress<0x089511a4>::Nop ();

        HOOK (Jmp, (0x08a456cc), FixProjectileThrowingThird,
              int (CWeapon * weaon, CPed * shooter, CVector * src));

        // patch to increase their throwing power
        GameAddress<0x8a45d50>::LuiOri (a0, 0.1);

        // Custom drive-by
        // ==============================================
        GameAddress<0x08a52270>::Write (li (a0, 5));
        GameAddress<0x08a43d98>::Write (li (a0, 5));
        GameAddress<0x08a52764>::Write (li (a0, 5));
        GameAddress<0x089b3a20>::Nop ();

        HOOK (Jal, (0x08a411b4), FireProjectilesDuringDriveby,
              void (CWeapon *, class CVehicle *, bool, bool));

        HOOK (Jal, 0x08ac53f8, SkipWeaponRandomizationForPickups, void ());

        HOOK (Jal, (0x8aa0640), SetRandomPlayerWeaponForRampage, 
            void (int, int, short, int, short*, int, int, int, bool, bool));
    }
};
