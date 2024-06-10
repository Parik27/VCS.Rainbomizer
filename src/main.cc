#include <fstream>
#include <pspsdk.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <string.h>

#include "core/Logger.hh"
#include "injector.h"
#include "patterns.h"

#include "Hooks.hh"
#include "core/Randomizer.hh"
#include "ScriptVehicleRandomizer.hh"

#define MODULE_NAME "VCS.PPSSPP.Rainbomizer"
#define GAME_INTERNAL_NAME "GTA3"

PSP_MODULE_INFO (MODULE_NAME, PSP_MODULE_USER, 1, 0);
PSP_HEAP_SIZE_KB (0x3000);

template <auto &CGame__Initialise>
void
InitialiseRandomizers (char *p1)
{
    CGame__Initialise (p1);
}

void
SetupInitialisationHooks ()
{
    ScriptVehicleRandomizer::Initialise ();

    HOOK (Jal, (0x8935e7c), InitialiseRandomizers, void (char *));
}

int main(int argc, char** argv)
{
    if (sceIoDevctl ("kemulator:", 0x00000003, NULL, 0, NULL, 0) != 0)
        return 0;

    SceUID modules[10];
    int    count  = 0;
    int    result = 0;

    Rainbomizer::Logger::LogMessage("Memory remaining: %03x", sceKernelMaxFreeMemSize());


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
                            result = 1;
                        }
                    else if (strcmp (info.name, MODULE_NAME) == 0)
                        {
                            injector.SetModuleBaseAddress (info.text_addr,
                                                           info.text_size);
                        }
                }

            if (result)
                {
                    SetupInitialisationHooks ();
                }
        }

    sceKernelSleepThread (); // pspsdk closes the game if this function returns

    return 0;
}
