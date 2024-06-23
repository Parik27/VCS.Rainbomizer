#pragma once

#include "memory/GameAddress.hh"
#include <cstdint>

class CBaseModelInfo
{
public:
    char     unknown[8];
    uint32_t m_hashName;
    void    *modelChunk;
    uint8_t  type;
    bool     m_bOwnsColModel;
    void    *m_pColModel;
    int16_t  m_first2dEffect;
    int16_t  m_objectId;
    int16_t  m_refCount;
    int16_t  m_texlistSlot;
    int16_t  unkId;
    void    *vftable;
};

class CSimpleModelInfo : public CBaseModelInfo
{
};

class CClumpModelInfo : public CSimpleModelInfo
{

};

class CVehicleModelInfo : public CClumpModelInfo
{
public:
    static constexpr auto GetMaximumNumberOfPassengersFromNumberOfDoors
        = GameFunction<0x8aa2670, int (int)>{};
};

class ModelInfo
{
public:
    static constexpr auto ms_modelInfoPtrs
        = GameVariable<CBaseModelInfo **, 0x08bb2158>{};
};
