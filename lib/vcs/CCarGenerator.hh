#pragma once

#include <cstdint>
class CCarGenerator
{
public:
    int m_nModelId;
    float x;
    float y;
    float z;
    float angle;
    int color1;
    int color2;
    uint8_t forceSpawn;
    uint8_t alarm;
    uint8_t doorLock;
    short delay1;
    short delay2;
    int m_nTimer;
    int m_nPoolHandle;
    short unk;
    char unk2;

    void DoInternalProcessing ();
};
