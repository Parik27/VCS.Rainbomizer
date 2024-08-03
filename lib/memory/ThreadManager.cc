#include "ThreadManager.hh"
#include "pspthreadman.h"
#include <cstring>

// From kernel/main.c
SceUID
ThreadManager::FindThreadByName (std::string_view name)
{
    SceUID ids[100];
    int    count;

    int ret = sceKernelGetThreadmanIdList (SCE_KERNEL_TMID_Thread, ids, 100,
                                           &count);

    if (ret >= 0)
        {
            for (int i = 0; i < count; i++)
                {
                    SceKernelThreadInfo info;
                    memset (&info, 0, sizeof (info));
                    info.size = sizeof (info);
                    if (sceKernelReferThreadStatus (ids[i], &info) == 0)
                        if (info.name == name)
                            return ids[i];
                }
        }

    return 0;
}

void
ThreadManager::BlockThread (std::string_view name)
{
    while (true)
        {
            SceUID id = FindThreadByName (name);
            if (id)
                {
                    sceKernelSuspendThread (id);
                    m_BlockedThreads.push_back (id);
                    break;
                }
            sceKernelDelayThread (10);
        }
}

void
ThreadManager::UnblockThreads ()
{
    for (auto id : m_BlockedThreads)
            sceKernelResumeThread (id);

    m_BlockedThreads.clear ();
}
