#include "ContainerUtils.hh"
#include "Hooks.hh"
#include "core/Logger.hh"
#include "core/Randomizer.hh"
#include "CCarGenerator.hh"
#include "Random.hh"
#include "core/Config.hh"
#include "CStreaming.hh"
#include "CVector.hh"

std::vector<int> m_Pickups = { 474, 466, 468, 354, 355, 348, 346, 337, 331, 332, 336, 349, 342, 338, 339, 355, 351, 
334, 368, 344, 353, 363, 352, 340, 360, 347, 361, 333, 358, 350, 351, 341, 356, 465, 467, 481, 505, 7490 };

class PickupsRandomizer : public Randomizer<PickupsRandomizer>
{
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