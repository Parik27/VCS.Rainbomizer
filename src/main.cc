#include <fstream>
#include <pspsdk.h>
#include <pspkernel.h>
#include <pspctrl.h>

#include "core/Logger.hh"
#include "scm/Command.hh"

#include <memory/ThreadManager.hh>

#define MODULE_NAME "VCS.PPSSPP.Rainbomizer"
#define GAME_INTERNAL_NAME "GTA3"

PSP_MODULE_INFO (VCS.Rainbomizer, PSP_MODULE_USER, 1, 0);

#ifdef ENABLE_DEBUG_MENU
PSP_HEAP_SIZE_KB (5000);
#else
PSP_HEAP_SIZE_KB (2000);
#endif

int main(int argc, char** argv)
{
    // if (sceIoDevctl ("kemulator:", 0x00000003, NULL, 0, NULL, 0) != 0)
    //     return 0;

    SceUID modules[10];
    int    count  = 0;
    int    result = 0;

    Rainbomizer::Logger::LogMessage("Memory remaining: %03x", sceKernelMaxFreeMemSize());
    ThreadManager::Get ().UnblockThreads ();

    sceKernelSleepThread (); // pspsdk closes the game if this function returns

    return 0;
}
