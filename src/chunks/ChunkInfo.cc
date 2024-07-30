#include "ChunkInfo.hh"

auto
ChunkInfo::GetModelDetails (uint32_t id)
{
    auto *streaming = CStreaming::sm_Instance.Get ();

    auto &modelInfo     = *ModelInfo::GetModelInfo<CBaseModelInfo> (id);
    auto &streamInfo    = streaming->ms_aInfoForModel[id];
    auto &texStreamInfo = streaming->ms_aInfoForModel[modelInfo.m_texlistSlot
                                                      + streaming->m_texOffset];

    return std::tie (modelInfo, streamInfo, texStreamInfo);
}

void
ChunkInfo::StoreInModelId (uint32_t id)
{
    auto [modelInfo, streamInfo, texStreamInfo] = GetModelDetails (id);

    streamInfo.m_cdPosn = cdPosn;
    streamInfo.m_cdSize = cdSize;

    texStreamInfo.m_cdPosn = texCdPosn;
    texStreamInfo.m_cdSize = texCdSize;

    if (replaceCollision)
        modelInfo.m_pColModel = colModel;

    extraData->StoreInModelId (id);
}
void
ChunkInfo::ParseLine (std::string_view line)
{
    auto split_view = std::ranges::split_view (line, ' ');
    auto modelName  = std::string_view (*split_view.begin ());

    hash    = CKeyGen::GetUppercaseKey (modelName);
    modelId = ModelInfo::GetModelIdFromModelHash (hash);

    if (modelId != -1)
        {
            CreateExtraData (modelId);
            FillFromModelId (modelId);
        }
    else
        {
            Rainbomizer::Logger::LogMessage ("%d not found: %s", hash,
                                             modelName.data ());
        }

    ParseFlags (split_view);
}
void
ChunkInfo::FillFromModelId (uint32_t id)
{
    auto [modelInfo, streamInfo, texStreamInfo] = GetModelDetails (id);

    cdPosn = streamInfo.m_cdPosn;
    cdSize = streamInfo.m_cdSize;

    texCdPosn = texStreamInfo.m_cdPosn;
    texCdSize = texStreamInfo.m_cdSize;

    colModel = modelInfo.m_pColModel;

    if (modelInfo.type == 6)
        {
            Rainbomizer::Logger::LogMessage (
                "col model: %d, %d, %p, %d", id, modelInfo.m_hashName, colModel,
                static_cast<CVehicleModelInfo &> (modelInfo).m_vehicleType);
        }

    extraData->FillFromModelId (id);
}

void
ChunkInfo::CreateExtraData (uint32_t modelId)
{
    auto model = ModelInfo::GetModelInfo (modelId);
    if (model->type == 6)
        extraData = std::make_unique<VehicleExtraData> ();
    else
        extraData = std::make_unique<ExtraData> ();
}

void
ChunkInfo::VehicleExtraData::StoreInModelId (uint32_t id)
{
    auto *modelInfo = ModelInfo::GetModelInfo<CVehicleModelInfo> (id);

    if (!modelInfo)
        return;

    auto *vehicleModelInfo = static_cast<CVehicleModelInfo *> (modelInfo);

    for (int i = 0; i < 15; i++)
        vehicleModelInfo->m_positions[i] = positions[i];

    vehicleModelInfo->m_wheelScale     = wheelScale;
    vehicleModelInfo->m_wheelScaleRear = wheelScaleRear;

    vehicleModelInfo->m_pHandlingData->m_fSeatOffsetDistance
        = seatOffsetDistance;
}

void
ChunkInfo::VehicleExtraData::FillFromModelId (uint32_t id)
{
    auto *modelInfo = ModelInfo::GetModelInfo<CVehicleModelInfo> (id);

    if (!modelInfo)
        return;

    auto *model = static_cast<CVehicleModelInfo *> (modelInfo);

    for (int i = 0; i < 15; i++)
        positions[i] = model->m_positions[i];

    wheelScale     = model->m_wheelScale;
    wheelScaleRear = model->m_wheelScaleRear;

    seatOffsetDistance = model->m_pHandlingData->m_fSeatOffsetDistance;
}
