#include "ContainerUtils.hh"
#include "Hooks.hh"
#include "core/Logger.hh"
#include "core/Randomizer.hh"
#include "CCarGenerator.hh"
#include "Random.hh"
#include "core/Config.hh"
#include "CStreaming.hh"
#include "CVector.hh"
#include "CPickups.hh"
#include <array>

class PickupsRandomizer : public Randomizer<PickupsRandomizer>
{
    static constexpr std::array m_Pickups = {PICKUP_BRIBE, PICKUP_HEALTH, PICKUP_ARMOUR, PICKUP_MICRO_SMG, PICKUP_KNIFE, PICKUP_AK47, PICKUP_PISTOL,
    PICKUP_AXE, PICKUP_BRASS_KNUCKLES, PICKUP_BUTTERFLY_KNIFE, PICKUP_BASEBALL_BAT, PICKUP_SHOTGUN, PICKUP_GRENADE, PICKUP_GAFF_HOOK, PICKUP_MACHETTE,
    PICKUP_MAC_10, PICKUP_NIGHTSTICK, PICKUP_BINOCULARS, PICKUP_MOLOTOV, PICKUP_SCORPION, PICKUP_REMOTE_EXPLOSIVE, PICKUP_ASSAULT_RIFLE, PICKUP_KATANA, 
    PICKUP_FLAMETHROWER, PICKUP_PYTHON, PICKUP_M249, PICKUP_MINIGUN, PICKUP_CAMERA, PICKUP_LANDMINE, PICKUP_SEAMINE, PICKUP_GOLF_CLUB, PICKUP_SNIPER, 
    PICKUP_COMBAT_SNIPER, PICKUP_RLAUNCHER, PICKUP_COMBAT_SHOTGUN,  PICKUP_STUBBY_SHOTGUN, PICKUP_CHAINSAW, PICKUP_SMG, PICKUP_INFORMATION, 
    PICKUP_ADRENALINE, PICKUP_MOVIE, PICKUP_BUY_VEHICLE, PICKUP_BRIEFCASE, PICKUP_KEYCARD, PICKUP_GD_DILDO, PICKUP_NDC_DRUGPACKET, 
    PICKUP_GD_BRIEFCASE_RED, PICKUP_GD_BRIEFCASE_BLUE, PICKUP_GD_BRIEFCASE_BROWN, PICKUP_PINATA_MAN, PICKUP_MEGADAMAGE, PICKUP_REGENHEALTH,
    PICKUP_INVISIBLE, PICKUP_RACEBAD, PICKUP_RACEGOOD};

public:
    template <auto &CPickups__GenerateNewOne>
    static int
    RandomizePickups (CVector* pos, int modelId, char arg3, int arg4, int arg5, bool arg6, char arg7)
    {
        for (auto &model : m_Pickups)
            if (model == modelId)
            {
                modelId = GetRandomElement(m_Pickups);
                modelId = PICKUP_REGENHEALTH;
                break;
            }

        return CPickups__GenerateNewOne(pos, modelId, arg3, arg4, arg5, arg6, arg7);
    }

    template<auto &CPickups__PickupTheDamnPickup>
    static bool
    PickupPicked (class CPickup *pickup, class CPed* player, bool inVeh, int playerInFocus, int p5, int p6)
    {
        if (pickup->m_modelIndex == PICKUP_REGENHEALTH || pickup->m_modelIndex == PICKUP_MEGADAMAGE || pickup->m_modelIndex == PICKUP_INVISIBLE)
            {
                static struct PowerupParam
                {
                    uint8_t unk[3] = {};
                    uint8_t pupType = 0;
                    uint8_t pupTime = 0;
                    uint8_t unk2 = 0;
                    uint8_t unk3[2 + 8];
                } param;

                static_assert(offsetof(PowerupParam, pupType) == 3);
                static_assert(sizeof(PowerupParam) == 16);

                *(bool*) 0x08bb0578 = true;
                param.pupTime = 255;

                if (pickup->m_modelIndex == PICKUP_REGENHEALTH)
                    param.pupType = 2;
                if (pickup->m_modelIndex == PICKUP_MEGADAMAGE)
                    param.pupType = 1;
                if (pickup->m_modelIndex == PICKUP_INVISIBLE)
                    param.pupType = 4;

                ((void (*) (PowerupParam *, uint32_t, uint32_t)) (
                    0x8a0b5a8)) (&param, 0, 0);

                *(bool*) 0x08bb0578 = false;
            }

        return CPickups__PickupTheDamnPickup (pickup, player, inVeh, playerInFocus, p5, p6);
    }

    PickupsRandomizer()
    {
        HOOK (Jmp, (0x88f5a3c), RandomizePickups, int (CVector*, int, char, int, int, bool, char));
        HOOK (Jal, (0x088f47bc), PickupPicked, bool (class CPickup*, class CPed*, bool, int, int, int));

        // Powerups
        injector.MakeNOP (0x08a0b608);
        injector.MakeNOP (0x08a0b60c);
        injector.MakeNOP (0x089bd2d4); // draw powerup timers
        injector.MakeNOP (0x08a0b62c); // ped state check
        //injector.MakeNOP (0x08947af8);
        injector.MakeNOP (0x0894ad94);
    }
} g_pickupsRando;
