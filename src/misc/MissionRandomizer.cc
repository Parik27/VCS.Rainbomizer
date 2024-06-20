#include <array>

#include <core/Randomizer.hh>
#include <core/Config.hh>

#include <utils/ContainerUtils.hh>
#include <utils/Utils.hh>

class MissionRandomizer : public Randomizer<MissionRandomizer>
{
    inline static constexpr std::array MissionsList
        = {8,  12, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
           55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
           70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
           85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98};

    template <auto &CRunningScript__CollectParams>
    static uint32_t
    RandomizeMission (class CRunningScript *scr, int *data, int numParams,
                      int *params)
    {
        uint32_t ret
            = CRunningScript__CollectParams (scr, data, numParams, params);

        if (DoesElementExist (MissionsList, params[0]))
            params[0] = GetRandomElement (MissionsList);

        return ret;
    }

public:
    MissionRandomizer ()
    {
        RB_C_DO_CONFIG_NO_OPTIONS ("MissionRandomizer");

        HOOK (Jal, (0x08abc41c), RandomizeMission,
              uint32_t (class CRunningScript *, int *data, int numParams,
                        int *params));
    }

} g_missionRandomizer;
