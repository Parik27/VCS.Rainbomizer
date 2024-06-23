#pragma once

#include <memory/GameAddress.hh>

struct CStreamingRequest {
    struct CStreamingRequest *m_pPrev;
    struct CStreamingRequest *m_pNext;
    uint32_t nModelId;
    int *pBuffer;
    int *field4_0x10;
};

struct CStreamingInfo {
    struct CStreamingInfo *m_next;
    struct CStreamingInfo *m_prev;
    uint8_t m_status;
    uint8_t m_flags;
    short m_nextModelOnCd;
    uint32_t m_cdPosn;
    uint32_t m_cdSize;
};

class CStreaming
{
public:
    bool                      m_disableStrewaming;
    uint8_t                   field1_0x1;
    uint8_t                   field2_0x2;
    uint8_t                   field3_0x3;
    int                       m_memoryUsed;
    int                       m_texOffset;
    int                       m_colOffset;
    int                       m_anmOffset;
    int                       m_numStreamInfo;
    int                       m_numVehiclesLoaded;
    uint32_t                  m_desiredNumVehiclesLoaded;
    int                       m_vehiclesLoaded[20];
    int                       m_lastVehicleDeleted;
    uint32_t                  field13_0x74;
    uint32_t                  m_currentPedGrp;
    bool                      m_aIsPedFromPedGroupLoaded[20];
    int                       field16_0x90;
    uint32_t                  field17_0x94;
    struct CStreamingInfo    *ms_aInfoForModel;
    struct CStreamingInfo    *pStreamingInfoEnd;
    struct CStreamingInfo    *pStreamingInfoBegin;
    struct CStreamingRequest *ms_startRequestedList;
    struct CStreamingRequest *ms_endRequestedList;
    uint8_t                   field23_0xac;
    uint8_t                   field24_0xad;
    uint8_t                   field25_0xae;
    uint8_t                   field26_0xaf;
    uint8_t                   field27_0xb0;
    uint8_t                   field28_0xb1;
    uint8_t                   field29_0xb2;
    uint8_t                   field30_0xb3;
    uint8_t                   field31_0xb4;
    uint8_t                   field32_0xb5;
    uint8_t                   field33_0xb6;
    uint8_t                   field34_0xb7;
    uint8_t                   field35_0xb8;
    uint8_t                   bLoadingAllModels; /* Created by retype action */

    static void RequestModel (int id, int flags = 0x3abccc);
    static void LoadAllRequestedModels (bool p1);
    static bool HadModelLoaded(int id);

    inline constexpr static auto sm_Instance
        = GameVariable<CStreaming *, 0x8bb1ea8>{};
};
