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

#include <utils/ContainerUtils.hh>
#include <utils/Random.hh>

class PickupsRandomizer : public Randomizer<PickupsRandomizer>
{
    static constexpr std::array m_Pickups = {PICKUP_BRIBE,
                                             PICKUP_HEALTH,
                                             PICKUP_ARMOUR,
                                             PICKUP_MICRO_SMG,
                                             PICKUP_KNIFE,
                                             PICKUP_AK47,
                                             PICKUP_PISTOL,
                                             PICKUP_AXE,
                                             PICKUP_BRASS_KNUCKLES,
                                             PICKUP_BUTTERFLY_KNIFE,
                                             PICKUP_BASEBALL_BAT,
                                             PICKUP_SHOTGUN,
                                             PICKUP_GRENADE,
                                             PICKUP_GAFF_HOOK,
                                             PICKUP_MACHETTE,
                                             PICKUP_MAC_10,
                                             PICKUP_NIGHTSTICK,
                                             PICKUP_BINOCULARS,
                                             PICKUP_MOLOTOV,
                                             PICKUP_SCORPION,
                                             PICKUP_REMOTE_EXPLOSIVE,
                                             PICKUP_ASSAULT_RIFLE,
                                             PICKUP_KATANA,
                                             PICKUP_FLAMETHROWER,
                                             PICKUP_PYTHON,
                                             PICKUP_M249,
                                             PICKUP_MINIGUN,
                                             PICKUP_CAMERA,
                                             PICKUP_LANDMINE,
                                             PICKUP_SEAMINE,
                                             PICKUP_GOLF_CLUB,
                                             PICKUP_SNIPER,
                                             PICKUP_COMBAT_SNIPER,
                                             PICKUP_RLAUNCHER,
                                             PICKUP_COMBAT_SHOTGUN,
                                             PICKUP_STUBBY_SHOTGUN,
                                             PICKUP_CHAINSAW,
                                             PICKUP_SMG,
                                             PICKUP_INFORMATION,
                                             PICKUP_ADRENALINE,
                                             PICKUP_MOVIE,
                                             PICKUP_BUY_VEHICLE,
                                             PICKUP_KEYCARD,
                                             PICKUP_GD_DILDO,
                                             PICKUP_NDC_DRUGPACKET,
                                             PICKUP_GD_BRIEFCASE_RED,
                                             PICKUP_GD_BRIEFCASE_BLUE,
                                             PICKUP_GD_BRIEFCASE_BROWN,
                                             PICKUP_PINATA_MAN,
                                             PICKUP_MEGADAMAGE,
                                             PICKUP_REGENHEALTH,
                                             PICKUP_INVISIBLE,
                                             PICKUP_RACEBAD,
                                             PICKUP_RACEGOOD};

    inline static int ForcedPickup = -1;
    inline static bool EnablePowerups = true;

public:
    template <auto &CPickups__GenerateNewOne>
    static int
    RandomizePickups (CVector *pos, int modelId, char arg3, int arg4, int arg5,
                      bool arg6, char arg7)
    {
        for (auto &model : m_Pickups)
            if (model == modelId)
                {
                    modelId = GetRandomElement (m_Pickups);
                    break;
                }

        int ret = CPickups__GenerateNewOne (pos, modelId, arg3, arg4, arg5,
                                            arg6, arg7);
        return ret;
    }

    template <auto &CPickups__PickupTheDamnPickup>
    static bool
    PickupPicked (class CPickup *pickup, class CPed *player, bool inVeh,
                  int playerInFocus, int p5, int p6)
    {
        if (pickup->m_modelIndex == PICKUP_REGENHEALTH
            || pickup->m_modelIndex == PICKUP_MEGADAMAGE
            || pickup->m_modelIndex == PICKUP_INVISIBLE)
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
                param.pupTime = 255;

                if (pickup->m_modelIndex == PICKUP_REGENHEALTH)
                    param.pupType = 2;
                if (pickup->m_modelIndex == PICKUP_MEGADAMAGE)
                    param.pupType = 1;
                if (pickup->m_modelIndex == PICKUP_INVISIBLE)
                    param.pupType = 4;

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
        if (modelId == PICKUP_RACEGOOD)
            {
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

        HOOK (Jmp, (0x088f5a3c), RandomizePickups,
              int (CVector *, int, char, int, int, bool, char));
        HOOK (Jal, (0x088f47bc), PickupPicked,
              bool (class CPickup *, class CPed *, bool, int, int, int));
        HOOK (Jmp, (0x088f74ec), FixCollectedPickups, 
              bool (int, int));

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
