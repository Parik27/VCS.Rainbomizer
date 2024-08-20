#include <cstdint>

#include <memory/Memory.hh>
#include <hooks/Hooks.hh>

#include "core/Logger.hh"
#include "core/Randomizer.hh"
#include "core/Config.hh"

#include "memory/GameAddress.hh"
#include "ppsspp/Keyboard.hh"
#include "scm/Command.hh"
#include "scm/Opcodes.hh"
#include "vcs/CRunningScript.hh"
#include "vcs/CStreaming.hh"
#include "weapon/Common.hh"
#include "weapon/WeaponPatterns.hh"

 #include <vcs/CVehicle.hh>
#include <vcs/CWeaponInfo.hh>
#include <vcs/CPlayer.hh>
#include <vcs/CPed.hh>
#include <vcs/CDarkel.hh>
#include <core/ThreadUtils.hh>

class WeaponRandomizer : public RandomizerWithDebugInterface<WeaponRandomizer>
{
    inline static int  ForcedWeapon            = -1;
    inline static bool DisableWeaponRandomizer = false;
    inline static int  RampageWeapon           = -1;

    inline static WeaponPatternManager m_Patterns;

    inline static struct PedWeaponEquipManager
    {
        int Mission;
        int OriginalWeapon;
        int RandomWeapon;

        void
        Store (CPed *ped, int original, int random)
        {
            if (ped != FindPlayerPed ())
                return;

            if (!CTheScripts::CurrentScript)
                return;

            Mission = ThreadUtils::GetMissionIdFromThread (
                CTheScripts::CurrentScript);

            OriginalWeapon = original;
            RandomWeapon   = random;

            Rainbomizer::Logger::LogMessage (
                "Storing weapon: %d -> %d (mission = %d)", original, random,
                Mission);
        }

        bool
        Restore (CPed *ped, int &slot)
        {
            if (ped != FindPlayerPed ())
                return false;

            if (!CTheScripts::CurrentScript)
                return false;

            int missionId = ThreadUtils::GetMissionIdFromThread (
                CTheScripts::CurrentScript);

            auto originalInfo = CWeaponInfo::GetWeaponInfo (OriginalWeapon);
            auto randomInfo   = CWeaponInfo::GetWeaponInfo (RandomWeapon);

            Rainbomizer::Logger::LogMessage (
                "Restoring weapon: %d -> %d (slot %d vs %d) %d", RandomWeapon,
                OriginalWeapon, slot, originalInfo->nSlot, missionId);

            if (missionId == Mission)
                {
                    slot = randomInfo->nSlot;
                    return true;
                }

            return false;
        }
    } m_PlayerEquipMgr;

    template <auto &CPed__GiveWeapon>
    static int
    RandomizeWeapon (CPed *ped, int weaponType, int ammo, int p4)
    {
        if (weaponType == 0 || DisableWeaponRandomizer)
            return CPed__GiveWeapon (ped, weaponType, ammo, p4);

        WeaponPattern::Result result{weaponType};

        // We store only the matched patterns so as to not override
        // the equip manager
        bool store = m_Patterns.GetRandomWeapon (ped, weaponType, ammo, result);

        if (store)
            m_PlayerEquipMgr.Store (ped, weaponType, result.Weapon);

        if (ForcedWeapon != -1)
            result.Weapon = ForcedWeapon;

        return CPed__GiveWeapon (ped, result.Weapon,
                                 result.OverrideAmmo > 0 ? result.OverrideAmmo
                                                         : ammo,
                                 p4);
    }

    template <auto &CWeapon__FireWeapon>
    static int
    FixProjectileThrowingThird (CWeapon *weap, class CPed *ped,
                                class CVector *src)
    {
        if (!WeaponsCommon::IsProjectile (eWeapon (weap->Type)))
            return CWeapon__FireWeapon (weap, ped, src);

        auto prevSeekTarget = ped->pSeekTarget;
        if (ped != FindPlayerPed () && !ped->pSeekTarget)
            {
                if (ped->pPointGunAt)
                    {
                        auto &end   = ped->vecPosition;
                        auto &start = ped->pPointGunAt->vecPosition;

                        Rainbomizer::Logger::LogMessage (
                            "Checking pointing: %f %f %f to %f %f %f", start.x,
                            start.y, start.z, end.x, end.y, end.z);

                        // if (!((int (*) (CVector *, CVector *, bool, bool,
                        // bool,
                        //                 bool, bool, bool, bool,
                        //                 bool)) 0x88967ac) (&start, &end,
                        //                 true,
                        //                                    true, false, true,
                        //                                    false, false,
                        //                                    false, false))
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

    template <auto &CPed__SetActiveWeaponSlot>
    static void
    RandomizeSelectedWeapon (CPed *ped, int slot)
    {
        if (ped == FindPlayerPed ())
            {
                Rainbomizer::Logger::LogMessage (
                    "SetActiveWeaponSlot (%p, %d)\nStack: %s", ped, slot,
                    PPSSPPUtils::GetStackTrace ().data ());
            }

        static std::array<uint32_t, 10> usedSlots;

        size_t numSlots = 0;
        for (size_t i = 0; i < 10; i++)
            if (ped->Weapons[i].Type && ped->Weapons[i].Ammo)
                usedSlots[numSlots++] = i;

        if (!numSlots)
            return CPed__SetActiveWeaponSlot (ped, slot);

        if (FindPlayerPed () == ped)
            {
                m_PlayerEquipMgr.Restore (ped, slot);
                return CPed__SetActiveWeaponSlot (ped, slot);
            }

        CPed__SetActiveWeaponSlot (ped, usedSlots[RandomInt (numSlots - 1)]);
    }

    template <auto &CWeapon__FireInstantHitFromCar>
    static void
    FireProjectilesDuringDriveby (CWeapon *weapon, CVehicle *veh, bool left,
                                  bool right)
    {

        if (WeaponsCommon::IsProjectile (eWeapon (weapon->Type)))
            {
                CPed *ped = FindPlayerPed ();

                static CVector pos;
                uint32_t       type = weapon->Type;

                if (weapon->Type == WEAPON_ROCKETLAUNCHER)
                    type = WEAPON_ROCKET;

                pos = FindPlayerPed ()->vecPosition;

                if (left)
                    pos -= ped->m_matrix.right * 3;

                else if (right)
                    pos += ped->m_matrix.right * 10;

                else
                    pos += ped->m_matrix.forward * 3;

                CProjectileInfo::AddProjectile (3.0f, FindPlayerPed (), type,
                                                &pos, false);
            }

        CWeapon__FireInstantHitFromCar (weapon, veh, left, right);
    }

    template <auto &CPickups__Update>
    static void
    SkipWeaponRandomizationForPickups ()
    {
        DisableWeaponRandomizer = true;
        CPickups__Update ();
        DisableWeaponRandomizer = false;
    }

#ifdef MELEE_DEBUG
    inline static float coronaX    = 0;
    inline static float coronaY    = 0;
    inline static float coronaZ    = 0;
    inline static float coronaSize = 0;

    template <auto &CWorld__TestSphereAgainstWorld>
    static int
    DrawMeleeDebugSphere (float p1, float *p2, uint32_t p3, uint32_t p4,
                          uint32_t p5, uint32_t p6, uint32_t p7, uint8_t p8,
                          uint32_t p9, uint8_t p10)
    {
        Rainbomizer::Logger::LogMessage ("DRAWING SPHERE: %f %f %f", p2[0],
                                         p2[1], p2[2]);
        coronaX    = p2[0];
        coronaY    = p2[1];
        coronaZ    = p2[2];
        coronaSize = p1;
        CallCommand<DRAW_CORONA> (p2[0], p2[1], p2[2], p1, 6, 0, 0, 200, 0);
        return CWorld__TestSphereAgainstWorld (p1, p2, p3, p4, p5, p6, p7, p8,
                                               p9, p10);
    }

    template <auto &CRunningScript__Process>
    static void
    DrawCorona (CRunningScript *scr)
    {
        static char textthing[256] = {};

        if (PPSSPPUtils::CheckKeyUp<NKCODE_K> ())
            {
            }

        CallCommand<DRAW_CORONA> (coronaX, coronaY, coronaZ, coronaSize, 4, 2,
                                  255, 0, 0);
        CRunningScript__Process (scr);
    }
#endif

    struct AmmoCheckFix : public ScriptCommandHook<1, AmmoCheckFix, int, int>
    {
        static void
        Impl (int ped, int weaponType)
        {
            if (ped == CTheScripts::GetGlobal<int> (782))
                {
                    Return (100);
                }
            else
                Return (0);
        }
    };

    template<auto &CRunningScript__Process>
    static void FixCurrentScriptPointer (CRunningScript* script)
    {
        CRunningScript__Process (script);
        CTheScripts::CurrentScript.Get () = nullptr;
    }

public:
    void
    DrawDebug ()
    {
        m_Patterns.DrawDebugInfo ();
    }

    WeaponRandomizer ()
    {
        bool EnableCustomDriveBy = false;

        RB_C_DO_CONFIG ("WeaponRandomizer", ForcedWeapon, EnableCustomDriveBy)

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
        if (EnableCustomDriveBy)
            {
                GameAddress<0x08a52270>::Write (li (a0, 5));
                GameAddress<0x08a43d98>::Write (li (a0, 5));
                GameAddress<0x08a52764>::Write (li (a0, 5));
                GameAddress<0x089b3a20>::Nop ();
            }

        HOOK (Jal, (0x08a411b4), FireProjectilesDuringDriveby,
              void (CWeapon *, class CVehicle *, bool, bool));

        HOOK (Jal, 0x08ac53f8, SkipWeaponRandomizationForPickups, void ());

        HOOK (Opcode, GET_AMMO_IN_CHAR_WEAPON, AmmoCheckFix::Hook,
              int (CRunningScript *));

        HOOK (Jal, 0x08869b00, FixCurrentScriptPointer, void (CRunningScript*));

#ifdef MELEE_DEBUG
        HOOK (Jal, 0x8a48038, DrawMeleeDebugSphere,
              int (float, float *, uint32_t, uint32_t, uint32_t, uint32_t,
                   uint32_t, uint8_t, uint32_t, uint8_t));

        HOOK (Jal, 0x08869b00, DrawCorona,
              void (class CRunningScript *));
#endif

        m_Patterns.ReadPatterns("WeaponPatterns.txt");
    }
};
