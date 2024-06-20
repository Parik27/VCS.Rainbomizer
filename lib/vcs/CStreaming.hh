#pragma once

#include <memory/GameAddress.hh>

struct CStreaming
{
    static void RequestModel (int id, int flags = 0x3abccc);
    static void LoadAllRequestedModels (bool p1);
};
