#include "memory/GameAddress.hh"
#include "ppsspp/KeyCodes.h"
#include "ppsspp/Keyboard.hh"
#include "psploadcore.h"
#include "psputils.h"
#include "vcs/CPlayer.hh"
#include "vcs/CRunningScript.hh"
#include <core/Randomizer.hh>
#include <hooks/Hooks.hh>
#include <thread>

#include <core/Config.hh>
#include <core/Logger.hh>

#include <scm/Command.hh>
#include <scm/Opcodes.hh>

#include <pspsdk.h>
#include <psputility.h>

class AutoSave : public Randomizer<AutoSave>
{
    enum AutosaveState
    {
        AUTOSAVE_WAIT_FOR_MISSION_PASS,
        AUTOSAVE_WAIT_TO_PREVENT_PACB,
        AUTOSAVE_AUTOSAVING
    } State;

    unsigned int AutosaveTimer;

    template <auto &zz_sceUtilitySavedataInitStart>
    int
    OverrideSave (SceUtilitySavedataParam *param)
    {
        if (State == AUTOSAVE_AUTOSAVING
            && param->mode == PSP_UTILITY_SAVEDATA_LISTSAVE)
            {
                State            = AUTOSAVE_WAIT_FOR_MISSION_PASS;
                param->mode      = PSP_UTILITY_SAVEDATA_AUTOSAVE;
                param->focus     = PSP_UTILITY_SAVEDATA_FOCUS_LASTEMPTY;
                param->overwrite = 1;
            }

        return zz_sceUtilitySavedataInitStart (param);
    }

    void
    DoAutosave ()
    {
        auto player = FindPlayerPed ();

        CTheScripts::GetGlobal<int> (4)     = 1;
        CTheScripts::GetGlobal<float> (284) = player->m_matrix.pos.x;
        CTheScripts::GetGlobal<float> (285) = player->m_matrix.pos.y;
        CTheScripts::GetGlobal<float> (286) = player->m_matrix.pos.z;

        GameAddress<0x8bb31b6>::Write (true);
    }

    template <auto &CRunningScript__Process>
    void
    ProcessAutosave (CRunningScript *scr)
    {
        static auto ProgressMade = GameVariable<float, 0x08bb40d0>{};

        float prevProgress = ProgressMade;
        CRunningScript__Process (scr);
        bool progressMade = ProgressMade.Get () != prevProgress;

        switch (State)
            {
            case AUTOSAVE_AUTOSAVING:
            case AUTOSAVE_WAIT_FOR_MISSION_PASS:
                if (progressMade || PPSSPPUtils::CheckKeyUp<NKCODE_F6> ())
                    {
                        State         = AUTOSAVE_WAIT_TO_PREVENT_PACB;
                        AutosaveTimer = sceKernelGetSystemTimeLow () + 1000000;
                        CallCommand<DO_FADE> (1000, 0);
                    }
                break;

            case AUTOSAVE_WAIT_TO_PREVENT_PACB:
                if (sceKernelGetSystemTimeLow () > AutosaveTimer)
                    {
                        DoAutosave ();
                        CallCommand<DO_FADE> (1000, 1);
                        CallCommand<PLAY_MISSION_PASSED_TUNE> (1);
                        State = AUTOSAVE_AUTOSAVING;
                    }
                break;
            }
    }

    template <auto &DoGameSpecificStuffBeforeSave>
    void
    DontDoGameSpecificStuffBeforeAutosave ()
    {
        if (State == AUTOSAVE_AUTOSAVING)
            {
                return;
            }

        DoGameSpecificStuffBeforeSave ();
    }

public:
    AutoSave ()
    {
        RB_C_DO_CONFIG("Autosave");

        HOOK_MEMBER (Jal, 0x08ab5a3c, OverrideSave,
                     int (SceUtilitySavedataParam *));
        HOOK_MEMBER (Jal, 0x08869b00, ProcessAutosave, void (CRunningScript *));
        HOOK_MEMBER (Jal, 0x0882e2a8, DontDoGameSpecificStuffBeforeAutosave,
                     void ());
    }
};
