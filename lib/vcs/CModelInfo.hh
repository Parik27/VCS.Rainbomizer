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


struct CBox {
    struct CVector min;
    struct CVector max;
};

struct RslV3d {
    float x;
    float y;
    float z;
};

struct CSphere
{
    RslV3d center;
    float  radius;
};

struct CColSphere
{
    struct CSphere _;
    undefined1     surface; /* Created by retype action */
    byte           pieceType;
    undefined      field3_0x12;
    undefined      field4_0x13;
    undefined      field5_0x14;
    undefined      field6_0x15;
    undefined      field7_0x16;
    undefined      field8_0x17;
    undefined      field9_0x18;
    undefined      field10_0x19;
    undefined      field11_0x1a;
    undefined      field12_0x1b;
    undefined      field13_0x1c;
    undefined      field14_0x1d;
    undefined      field15_0x1e;
    undefined      field16_0x1f;
};

class CColModel
{
public:
    CSphere     boundingSphere;
    CBox        boundingBox;
    short       numBoxes;
    short       numTrianglePlanes;
    undefined1  numSpheres;  /* Created by retype action */
    undefined1  numLines;    /* Created by retype action */
    undefined1  numVertices; /* Created by retype action */
    undefined1  level;       /* Created by retype action */
    CColSphere *spheres;     /* Created by retype action */
    dword       lines;
    dword       boxes;
    dword       vertices;
    dword       triangles;
    dword       trianglePlanes;

    const CVector
    GetBounds () const
    {
        return boundingBox.max - boundingBox.min;
    }
};

class CBaseModelInfo
{
public:
    uint32_t   unknown1;
    uint32_t   unknown2;
    uint32_t   m_hashName;
    void      *modelChunk;
    eModelType type;
    bool       m_bOwnsColModel;
    CColModel *m_pColModel;
    int16_t    m_first2dEffect;
    int16_t    m_objectId;
    int16_t    m_refCount;
    int16_t    m_texlistSlot;
    int16_t    unkId;
    void      *vftable;
};

class CSimpleModelInfo : public CBaseModelInfo
{
public:
    RslElement* m_objects;
    float m_lodDistances[3];
    uint8_t m_numObjects;
    uint16_t m_flags;
    CSimpleModelInfo* m_relatedObject;
};

class CWeaponModelInfo : public CSimpleModelInfo
{
public:
    uint32_t m_animId;

    uint32_t
    GetWeaponId ()
    {
        return reinterpret_cast<uint32_t> (m_relatedObject);
    }
};

class CElementModelInfo : public CBaseModelInfo
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

    char           field1_0x30[8];
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
    uint32_t       unk3;
    uint8_t        m_numDoors;
    uint8_t        unk4;
    uint8_t        unk5;
    uint8_t        m_numComponents;
    uint32_t       unk6;
    uint32_t       unk7;

    void
    AddPrimaryMaterial (RslMaterial *material)
    {
        for (size_t i = 0; i < 30; i++)
            {
                if (m_materials1[i] == material)
                    return;

                if (!m_materials1[i])
                    {
                        m_materials1[i] = material;
                        break;
                    }
            }
    }
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
