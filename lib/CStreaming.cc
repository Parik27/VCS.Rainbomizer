#include "CStreaming.hh"

void
CStreaming::RequestModel (int id, int flags)
{
    ((void (*) (int, int)) (0x8ad3660)) (id, flags);
}

void
CStreaming::LoadAllRequestedModels (bool p1)
{
    ((void (*) (bool)) 0x8ad39f0) (p1);
}
