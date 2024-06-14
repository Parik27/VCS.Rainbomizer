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
    PICKUP_MAC_10, PICKUP_NIGHTSTICK, PICKUP_BINOCULARS, PICKUP_MOLOTOV, PICKUP_SCORPION, PICKUP_REMOTE_EXPLOSIVE, PICKUP_ASSAULT_RIFLE,
    PICKUP_KATANA, PICKUP_FLAMETHROWER, PICKUP_PYTHON, PICKUP_M249, PICKUP_GOLF_CLUB, PICKUP_COMBAT_SNIPER, PICKUP_COMBAT_SHOTGUN,
    PICKUP_STUBBY_SHOTGUN, PICKUP_CHAINSAW, PICKUP_SMG, PICKUP_INFORMATION, PICKUP_ADRENALINE, PICKUP_MOVIE, PICKUP_BUY_VEHICLE};

public:
    template <auto &CPickups__GenerateNewOne>
    static int
    RandomizePickups (CVector* pos, int modelId, char arg3, int arg4, int arg5, bool arg6, char arg7)
    {
         for (auto &model : m_Pickups)
            if (model == modelId)
            {
                modelId = GetRandomElement(m_Pickups);
                break;
            }
    
        return CPickups__GenerateNewOne(pos, modelId, arg3, arg4, arg5, arg6, arg7);
    }

    PickupsRandomizer()
    {
        HOOK (Jmp, (0x88f5a3c), RandomizePickups, int (CVector*, int, char, int, int, bool, char));
    }
} g_pickupsRando;