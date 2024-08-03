#pragma once

#include "pspkerneltypes.h"
#include <vector>
#include <thread>

class ThreadManager
{
    std::vector<uint32_t> m_BlockedThreads;

    SceUID FindThreadByName (std::string_view name);

public:
    void BlockThread (std::string_view name);
    void UnblockThreads ();

    static ThreadManager &
    Get ()
    {
        static ThreadManager instance;
        return instance;
    }
};
