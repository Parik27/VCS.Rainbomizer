#include <pspsdk.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <string.h>

#include "log.h"
#include "injector.h"
#include "patterns.h"
#include "inireader.h"
#include "gvm.h"
#include "mips.h"

#include "Hooks.hh"
#include "core/Randomizer.hh"
#include "ScriptVehicleRandomizer.hh"

#define MODULE_NAME "VCS.PPSSPP.Rainbomizer"
#define GAME_INTERNAL_NAME "GTA3"
#define LOG_PATH                                                               \
    "ms0:/PSP/PLUGINS/VCS.PPSSPP.Rainbomizer/VCS.PPSSPP.Rainbomizer.log"
#define INI_PATH                                                               \
    "ms0:/PSP/PLUGINS/VCS.PPSSPP.Rainbomizer/VCS.PPSSPP.Rainbomizer.ini"

PSP_MODULE_INFO (MODULE_NAME, PSP_MODULE_USER, 1, 0);

template <auto &CGame__Initialise>
void
InitialiseRandomizers (char *p1)
{
    INITIALISE_RANDOMIZER (CutsceneRandomizer);
    CGame__Initialise (p1);
}

void
SetupInitialisationHooks ()
{

    ScriptVehicleRandomizer::Initialise ();

    REGISTER_HOOK_ADDR (0x8935e7c, InitialiseRandomizers, void, char*);
}

extern "C" int
module_start (SceSize args, void *argp)
{
    if (sceIoDevctl ("kemulator:", 0x00000003, NULL, 0, NULL, 0) != 0)
        return 0;

    SceUID modules[10];
    int    count  = 0;
    int    result = 0;
    if (sceKernelGetModuleIdList (modules, sizeof (modules), &count) >= 0)
        {
            SceKernelModuleInfo info;
            for (int i = 0; i < count; ++i)
                {
                    info.size = sizeof (SceKernelModuleInfo);
                    if (sceKernelQueryModuleInfo (modules[i], &info) < 0)
                        {
                            continue;
                        }

                    if (strcmp (info.name, GAME_INTERNAL_NAME) == 0)
                        {
                            injector.SetGameBaseAddress (info.text_addr,
                                                         info.text_size);
                            pattern.SetGameBaseAddress (info.text_addr,
                                                        info.text_size);
                            // inireader.SetIniPath (INI_PATH);
                            logger.SetPath (LOG_PATH);
                            result = 1;
                        }
                    else if (strcmp (info.name, MODULE_NAME) == 0)
                        {
                            injector.SetModuleBaseAddress (info.text_addr,
                                                           info.text_size);
                        }
                }

            if (result) {
                SetupInitialisationHooks ();
            }
        }
    return 0;
}
