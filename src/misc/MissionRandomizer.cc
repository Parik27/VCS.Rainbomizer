#include "core/Logger.hh"
#include "memory/GameAddress.hh"
#include "scm/Command.hh"
#include "vcs/CMatrix.hh"
#include "vcs/CRunningScript.hh"
#include "vcs/CPlayer.hh"
#include "vcs/CPed.hh"
#include <array>

#include <core/Randomizer.hh>
#include <core/Config.hh>

#include <utils/ContainerUtils.hh>
#include <utils/Utils.hh>
#include <vector>

class MissionRandomizer : public Randomizer<MissionRandomizer>
{
    inline static constexpr std::array MissionsList
        = {8,  12, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
           55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
           70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
           85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98};

    template <auto &CRunningScript__CollectParams>
    static uint32_t
    RandomizeMission (class CRunningScript *scr, unsigned char *data,
                      int numParams, int *params)
    {


        uint32_t ret
            = CRunningScript__CollectParams (scr, data, numParams, params);

        if (DoesElementExist (MissionsList, params[0]))
            {
              //CallCommand<0x44> (Global{782}, -18.37f, 925.89f, 10.94f);
              params[0] = 98; //GetRandomElement (MissionsList);

                Rainbomizer::Logger::LogMessage ("Randomized mission: %d",
                                                 params[0]);
            }

        return ret;
    }

    template <uint32_t KEY>
    static bool
    CheckKeyDown ()
    {
        bool currState;
        sceIoDevctl ("kemulator:", 0x34, (void *) KEY, 0, &currState, 1);

        return currState;
    }

    template <uint32_t KEY>
    static bool
    CheckKeyUp ()
    {
        static bool prevState = false;
        bool        currState;
        sceIoDevctl ("kemulator:", 0x34, (void *) KEY, 0, &currState, 1);

        if (std::exchange (prevState, currState) && !currState)
            return true;

        return false;
    }

  template<auto& CRunningScript__Process>
  static void DoThing (CRunningScript* scr)
  {
    static float offsetX = 0;
    static float offsetY = 0;
    static float offsetZ = 0;

    static float rotX = 0;
    static float rotY = 0;
    static float rotZ = 0;

    static bool enabled = false;

    if (CheckKeyUp<134>()) {
      enabled = !enabled;
      auto player = FindPlayerPed ();
      offsetX = player->m_matrix.pos.x;
      offsetY = player->m_matrix.pos.y;
      offsetZ = player->m_matrix.pos.z;
    }

    if (CheckKeyDown<152>())
      rotX += 0.01;

    if (CheckKeyDown<146>())
      rotX -= 0.01;

    if (CheckKeyDown<150>())
      rotZ += 0.01;

    if (CheckKeyDown<148>())
      rotZ -= 0.01;

    if (CheckKeyDown<151>())
      rotY += 0.01;

    if (CheckKeyDown<145>())
      rotY -= 0.01;

    if (CheckKeyDown<92>())
      offsetZ += 0.01;

    if (CheckKeyDown<93>())
      offsetZ -= 0.01;

    if (CheckKeyDown<123>())
      offsetX -= 0.01;

    if (CheckKeyDown<124>())
      offsetX += 0.01;

    if (enabled)
        {
            CallCommand<0x00B9> (offsetX, offsetY, offsetZ, rotX, rotY, rotZ);
            CallCommand<0x00BA>(offsetX, offsetY, offsetZ, 2);
        }

    CRunningScript__Process (scr);
  }

public:
    MissionRandomizer ()
    {
      RB_C_DO_CONFIG ("MissionRandomizer");

      HOOK (Jal, (0x08abc41c), RandomizeMission,
            uint32_t (class CRunningScript *, unsigned char *data,
                      int numParams, int *params));

      HOOK (Jal, (0x08869b00), DoThing, void (CRunningScript* scr));

      GameAddress<0x8ad2b44>::WriteInstructions (li (v0, 0), jr (ra));
    }

} g_missionRandomizer;
