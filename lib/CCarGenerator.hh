#pragma once

class CCarGenerator
{
public:
    int m_nModelId;
private:
    int unk[8];
public:
    int m_nTimer;
    int m_nPoolHandle;

    void DoInternalProcessing ();
};
