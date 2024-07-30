#pragma once

#include "memory/GameAddress.hh"
#include "vcs/CKeyGen.hh"
#include "vcs/CVector.hh"
#include "vcs/RslTexture.hh"
#include <vcs/CPed.hh>
#include <vcs/CStreaming.hh>
#include <cstdint>

enum eModelType : uint8_t
{
    MODEL_TYPE_NA,
    MODEL_TYPE_SIMPLE,
    MODEL_TYPE_MLO,
    MODEL_TYPE_TIME,
    MODEL_TYPE_WEAPON,
    MODEL_TYPE_ELEMENT_GROUP,
    MODEL_TYPE_VEHICLE,
    MODEL_TYPE_PED,
};

class CBaseModelInfo
{
public:
    char       unknown[8];
    uint32_t   m_hashName;
    void      *modelChunk;
    eModelType type;
    bool       m_bOwnsColModel;
    void      *m_pColModel;
    int16_t    m_first2dEffect;
    int16_t    m_objectId;
    int16_t    m_refCount;
    int16_t    m_texlistSlot;
    int16_t    unkId;
    void      *vftable;
};

class CSimpleModelInfo : public CBaseModelInfo
{
};

class CElementModelInfo : public CSimpleModelInfo
{
    RslElementGroup *m_elementGroup;
    union
    {
        uint32_t m_animFileIndex;
        char    *m_animFileName;
    };
};

enum eVehicleType : uint32_t
{
    VEHICLE_TYPE_AUTOMOBILE = 0,
    VEHICLE_TYPE_BOAT       = 1,
    VEHICLE_TYPE_JETSKI     = 2,
    VEHICLE_TYPE_TRAIN      = 3,
    VEHICLE_TYPE_HELI       = 4,
    VEHICLE_TYPE_PLANE      = 5,
    VEHICLE_TYPE_BIKE       = 6,
    VEHICLE_TYPE_BMX        = 8,
    VEHICLE_TYPE_QUAD       = 9
};

class CHandlingData
{
public:
    ATOFFSET (0x8C, float, m_fSeatOffsetDistance);
};

class CVehicleModelInfo : public CElementModelInfo
{
public:
    static constexpr auto GetMaximumNumberOfPassengersFromNumberOfDoors
        = GameFunction<0x8aa2670, int (int)>{};

    char         field1_0x30[8];
    CHandlingData *m_pHandlingData;
    void          *field3_0x3c;
    void          *field4_0x40;
    void          *field5_0x44;
    void          *field6_0x48;
    void          *field7_0x4c;
    float          field8_0x50;
    eVehicleType   m_vehicleType;
    float          m_wheelScale;
    float          m_wheelScaleRear;
    CVector        m_positions[15];
    uint8_t        unk[8];
    char           m_sName[8];
    uint8_t        unk2[36];
    uint32_t       lastPrimaryColour;
    uint32_t       lastSecondaryColour;
    RslMaterial   *m_materials1[30];
    RslMaterial   *m_materials2[25];
    RslElement   **m_apComponents;
    uint32_t       m_nAnimIndex;
};

class ModelInfo
{
public:
    static constexpr auto ms_modelInfoPtrs
        = GameVariable<CBaseModelInfo **, 0x08bb2158>{};

    template <typename T = CBaseModelInfo>
    static T *
    GetModelInfo (uint32_t Id)
    {
        return static_cast<T *> (ms_modelInfoPtrs[Id]);
    }

    static size_t
    GetModelIdFromModelHash (uint32_t hash)
    {
        for (size_t i = 0; i < CStreaming::sm_Instance->m_texOffset; i++)
            {
                auto modelInfo = ModelInfo::ms_modelInfoPtrs[i];
                if (modelInfo && modelInfo->m_hashName == hash)
                    return i;
            }

        return -1;
    }

    template <typename T = CBaseModelInfo>
    static T *
    GetModelByHash (uint32_t hash)
    {
        auto id = GetModelIdFromModelHash (hash);
        if (id != -1)
            return GetModelInfo<T> (id);

        return nullptr;
    }

    template <typename T = CBaseModelInfo>
    static T *
    GetModelByName (std::string_view str)
    {
        return GetModelByHash<T> (CKeyGen::GetUppercaseKey (str));
    }
};
