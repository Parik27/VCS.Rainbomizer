#include "Events.hh"
#include <hooks/Hooks.hh>
#include "core/Logger.hh"
#include "utils/Random.hh"
#include <pspsdk.h>

bool triggerSave = false;

template <auto &CCamera__ProcessFade>
void
FadeEvent::ProcessFadeEvent (class CCamera *cam)
{
  CCamera__ProcessFade (cam);
}

FadeEvent::FadeEvent ()
{
    HOOK_MEMBER (Jmp, (0x8a1cc64), ProcessFadeEvent, void (class CCamera *));
}

template <auto &CRunningScript__Process>
void
RandomizationSeedEvent::ProcessRandomizationSeedEvent (CRunningScript *script)
{
    static constexpr GameVariable<int, 0x08badd74> CarsCollected{};

    if (script->m_pCurrentIP == 0)
        {
            if (CarsCollected.Get () == 0)
                CarsCollected.Get () = RandomSize (0xFFFFFFFF);

            Rainbomizer::Logger::LogMessage ("Game started. Seed: %08x",
                                             CarsCollected.Get ());
            this->operator() (CarsCollected);
        }

    CRunningScript__Process (script);
}

RandomizationSeedEvent::RandomizationSeedEvent ()
{
    HOOK_MEMBER (Jal, 0x08869b00, ProcessRandomizationSeedEvent,
                 void (CRunningScript *));
}

// enum
// {
//     EMULATOR_DEVCTL__TOGGLE_FASTFORWARD = 0x30,
//     EMULATOR_DEVCTL__GET_ASPECT_RATIO,
//     EMULATOR_DEVCTL__GET_SCALE,
//     EMULATOR_DEVCTL__GET_AXIS,
//     EMULATOR_DEVCTL__GET_VKEY,
// };

// void
// KeyEvent::Process ()
// {
//     constexpr static const int NKCODE_A = 29;
//     constexpr static const int NKCODE_Z = 54;

//     for (size_t i = 44; i <= 44; i++)
//         {
//             bool prev = m_KeyState[i];
//             sceIoDevctl ("kemulator:", EMULATOR_DEVCTL__GET_VKEY, (void *) i, 0,
//                          &m_KeyState[i], sizeof (m_KeyState[i]));

//             if (m_KeyState[i] && prev != m_KeyState[i]) {
//               *(bool*) (0x8bb31b6) = true;
//               //triggerSave = true;
//               //FadeEvent::Get().haha = true;
//               Rainbomizer::Logger::LogMessage("Here");
//             }
//         }
// }
