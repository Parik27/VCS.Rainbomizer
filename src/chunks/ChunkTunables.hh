#pragma once

#include "vcs/CVector.hh"
#include <cstdint>

class ChunkRandomizerTunables
{
    inline static uint16_t disableChunkRandomizerForModel = -1;
    inline static uint16_t modelForBoundsCheck            = -1;
    inline static CVector  boundsCheck;

    friend class ChunkRandomizer;

public:
    static void
    DisableChunkRandomizerForModel (uint16_t modelId)
    {
        disableChunkRandomizerForModel = modelId;
    }

    static void
    SetBoundsCheckForModel (uint16_t modelId, const CVector &bounds)
    {
        modelForBoundsCheck = modelId;
        boundsCheck         = bounds;
    }

    static void
    ClearBoundsCheck ()
    {
        modelForBoundsCheck = -1;
    }
};
