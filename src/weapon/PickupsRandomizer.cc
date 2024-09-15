#include <hooks/Hooks.hh>
#include <memory/Memory.hh>

#include <core/Logger.hh>
#include <core/Randomizer.hh>
#include <core/Config.hh>

#include "memory/GameAddress.hh"
#include "mips.h"

#include <vcs/CCarGenerator.hh>
#include <vcs/CStreaming.hh>
#include <vcs/CVector.hh>
#include <vcs/CPickups.hh>
#include <vcs/CPlayer.hh>

#include <array>

#include "WeaponPatterns.hh"
#include "scm/Command.hh"
#include "scm/Opcodes.hh"
#include "vcs/CModelInfo.hh"
#include "vehicle/Common.hh"

#include <utils/ContainerUtils.hh>
#include <utils/Random.hh>

class PickupsRandomizer : public Randomizer<PickupsRandomizer>
{
    static constexpr std::array m_Pickups = {PICKUP_BRIBE,
                                             PICKUP_HEALTH,
                                             PICKUP_BODYARMOUR,
                                             PICKUP_UZI,
                                             PICKUP_KNIFECUR,
                                             PICKUP_AK47,
                                             PICKUP_BERETTA,
                                             PICKUP_HANDAXE,
                                             PICKUP_BRASSKNUCKLE,
                                             PICKUP_BUTT_KNIFE,
                                             PICKUP_BAT,
                                             PICKUP_CHROMEGUN,
                                             PICKUP_GRENADE,
                                             PICKUP_GAFF,
                                             PICKUP_MACHETE,
                                             PICKUP_INGRAMSL,
                                             PICKUP_NITESTICK,
                                             PICKUP_BINOCULARS,
                                             PICKUP_MOLOTOV,
                                             PICKUP_SKORPION,
                                             PICKUP_BOMB,
                                             PICKUP_M16,
                                             PICKUP_KATANA,
                                             PICKUP_FLAME,
                                             PICKUP_PYTHON,
                                             PICKUP_M249,
                                             PICKUP_MINIGUN,
                                             PICKUP_CAMERA,
                                             PICKUP_LANDMINE,
                                             PICKUP_SEAMINE,
                                             PICKUP_GOLFCLUB,
                                             PICKUP_SNIPER,
                                             PICKUP_LASER,
                                             PICKUP_ROCKETLA,
                                             PICKUP_SHOTGSPA,
                                             PICKUP_BUDDYSHOT,
                                             PICKUP_CHNSAW,
                                             PICKUP_MPLNG,
                                             PICKUP_INFO,
                                             PICKUP_ADRENALINE,
                                             PICKUP_BUY_VEHICLE,
                                             PICKUP_GD_BRIEFCASE_RED,
                                             PICKUP_PINATA_MAN,
                                             PICKUP_MEGADAMAGE,
                                             PICKUP_REGENERATOR,
                                             PICKUP_INVISIBLE,
                                             PICKUP_BAD_CAR,
                                             PICKUP_GOOD_CAR};

    inline static int  ForcedPickup   = -1;
    inline static bool EnablePowerups = true;

    WeaponPatternManager m_Patterns;

    bool
    RandomizeWeaponPickup (int modelId, WeaponPattern::Result &result)
    {
        auto modelInfo = ModelInfo::GetModelInfo (modelId);

        if (modelInfo->type != MODEL_TYPE_WEAPON)
            return false;

        auto     weaponModel = static_cast<CWeaponModelInfo *> (modelInfo);
        uint32_t weaponId    = weaponModel->GetWeaponId ();

        return m_Patterns.GetRandomWeapon (nullptr, weaponId, 0, result);
    }

public:
    template <auto &CPickups__GenerateNewOne>
    int
    RandomizePickups (CVector *pos, int modelId, char type, int quantity, int rate,
                      bool arg6, char arg7)
    {
        WeaponPattern::Result result;

        // Attempt to randomize a weapon pickup
        if (RandomizeWeaponPickup (modelId, result))
            {
                CWeaponInfo *info = CWeaponInfo::GetWeaponInfo (result.Weapon);
                modelId           = info->nWeaponModel1;

                if (result.OverrideAmmo > 0)
                    quantity = result.OverrideAmmo;
            }
        else
            {
                // else do generic randomization
                for (auto &model : m_Pickups)
                    if (model == modelId)
                        {
                            modelId = GetRandomElement (m_Pickups);
                            if (ForcedPickup != -1)
                              modelId = ForcedPickup;

                            break;
                        }
            }

        int ret = CPickups__GenerateNewOne (pos, modelId, type, quantity, rate,
                                            arg6, arg7);
        return ret;
    }

    template <auto &CPickups__PickupTheDamnPickup>
    static bool
    PickupPicked (class CPickup *pickup, class CPed *player, bool inVeh,
                  int playerInFocus, int p5, int p6)
    {
        if (pickup->m_modelIndex == PICKUP_REGENERATOR
            || pickup->m_modelIndex == PICKUP_MEGADAMAGE)
            {
                static struct PowerupParam
                {
                    uint8_t unk[3]  = {};
                    uint8_t pupType = 0;
                    uint8_t pupTime = 0;
                    uint8_t unk2    = 0;
                    uint8_t unk3[2 + 8];
                } param;

                static_assert (offsetof (PowerupParam, pupType) == 3);
                static_assert (sizeof (PowerupParam) == 16);

                GameAddress<0x8bb0578>::Write (true);
                param.pupTime = 30;

                if (pickup->m_modelIndex == PICKUP_REGENERATOR)
                    param.pupType = 2;
                if (pickup->m_modelIndex == PICKUP_MEGADAMAGE)
                    param.pupType = 1;

                ((void (*) (PowerupParam *, uint32_t, uint32_t)) (
                    GAMEADDR (0x8a0b5a8))) (&param, 0, 0);

                GameAddress<0x8bb0578>::Write (false);
            }

        return CPickups__PickupTheDamnPickup (pickup, player, inVeh,
                                              playerInFocus, p5, p6);
    }

    template <auto &CPickups__GivePlayerGoodiesWithPickUpMI>
    static bool
    FixCollectedPickups (int modelId, int p2)
    {
        switch (modelId)
        {
            case PICKUP_GD_BRIEFCASE_RED:
                if (FindPlayerPed())
                    CallCommand<ADD_SCORE>(Global{782}, RandomInt(500, 3000));
                break;

            case PICKUP_BUY_VEHICLE:
            {
                if (!FindPlayerPed ())
                    break;

                auto pos = FindPlayerPed ()->m_matrix.pos;
                int  veh = VehicleCommon::GetRandomUsableVehicle ();
                CallCommand<GET_CLOSEST_CAR_NODE> (pos.x, pos.y, pos.z,
                                                    Local{0}, Local{1},
                                                    Local{2});
                CStreaming::RequestModel (veh, 0);
                CStreaming::LoadAllRequestedModels (false);

                if (!CStreaming::HasModelLoaded (veh))
                    break;

                CallCommand<CREATE_CAR> (veh, Local{0}, Local{1}, Local{2},
                                        Local{3});
                CallCommand<MARK_CAR_AS_NO_LONGER_NEEDED> (Local{3});
                break;
            }

            case PICKUP_INVISIBLE:
                if (FindPlayerPed())
                    CallCommand<SET_EVERYONE_IGNORE_PLAYER> (Global{782}, 1);
                break;

            case PICKUP_GOOD_CAR:
                CVehicle *vehicle = FindPlayerVehicle ();
                if (vehicle)
                    vehicle->m_fHealth += 1000;

                // Return here to prevent crashing
                return false;

        }
        return CPickups__GivePlayerGoodiesWithPickUpMI (modelId, p2);
    }

    PickupsRandomizer ()
    {
        RB_C_DO_CONFIG ("PickupRandomizer", ForcedPickup, EnablePowerups);

        HOOK_MEMBER (Jmp, (0x088f5a3c), RandomizePickups,
                     int (CVector *, int, char, int, int, bool, char));

        HOOK (Jal, (0x088f47bc), PickupPicked,
              bool (class CPickup *, class CPed *, bool, int, int, int));
        HOOK (Jmp, (0x088f74ec), FixCollectedPickups, bool (int, int));

        m_Patterns.ReadPatterns ("WeaponPickupPatterns.txt");

        // Powerups
        if (EnablePowerups)
            {
                // Quad damage, health boost and invisibility
                GameAddress<0x08a0b608>::Nop ();
                GameAddress<0x08a0b60c>::Nop ();
                GameAddress<0x089bd2d4>::Nop (); // draw powerup timers
                GameAddress<0x08a0b62c>::Nop (); // ped state check
                GameAddress<0x0894ad94>::Nop ();

                // Racebad fix
                GameAddress<0x08947b38>::Nop ();
                GameAddress<0x08947b90>::LuiOri (a0, 5.0f);
            }
    }
};
