#include "ContainerUtils.hh"
#include "Hooks.hh"
#include "core/Logger.hh"
#include "core/Randomizer.hh"
#include "CCarGenerator.hh"
#include "Random.hh"
#include "core/Config.hh"
#include "CStreaming.hh"
#include "CVector.hh"

class PickupsRandomizer : public Randomizer<PickupsRandomizer>
{
public:
    template <auto &CPickups__GenerateNewOne>
    static int
    RandomizePickups (CVector* pos, int modelId, char arg3, int arg4, int arg5, bool arg6, char arg7)
    {
        int newModel = 342; // Grenades
        return CPickups__GenerateNewOne(pos, newModel, arg3, arg4, arg5, arg6, arg7);
    }

    PickupsRandomizer()
    {
        for (int addr : {0x88f2b94, 0x88f7ab8, 0x8938a78, 0x8946a2c, 0x8946a2c, 0x8947db8, 
        0x8947e0c, 0x8947e54, 0x89e1dd8, 0x89e1dd8, 0x89e214c, 0x89e2404, 0x8a0a764})
            HOOK (Jal, (addr), RandomizePickups, int (CVector*, int, char, int, int, bool, char));
    }
} g_pickupsRando;