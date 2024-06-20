#include "CStreaming.hh"
#include "memory/Memory.hh"

void
CStreaming::RequestModel (int id, int flags)
{
    return GameFunction<0x8ad3660, void (int, int)>::Call (id, flags);
}

void
CStreaming::LoadAllRequestedModels (bool p1)
{
    return GameFunction<0x8ad39f0, void (bool)>::Call (p1);
}
