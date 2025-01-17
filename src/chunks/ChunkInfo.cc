#include "ChunkInfo.hh"
#include "core/Logger.hh"
#include "vcs/CModelInfo.hh"

void
ChunkInfo::StoreInModelId (uint32_t id)
{
    auto [modelInfo, streamInfo] = GetModelDetails (id);

    Rainbomizer::Logger::LogMessage("Randomizing chunk %d to %d", this->modelId, id);

    streamInfo.m_cdPosn = cdPosn;
    streamInfo.m_cdSize = cdSize;

    modelInfo.m_texlistSlot = texSlot;
    modelInfo.unknown1 = modelId;

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
            Rainbomizer::Logger::LogMessage ("%d not found: %.*s %d", hash,
                                             modelName.size(),
                                             modelName.data (),
                                             modelName.size());
        }

    ParseFlags (split_view);
}
void
ChunkInfo::FillFromModelId (uint32_t id)
{
    auto [modelInfo, streamInfo] = GetModelDetails (id);

    cdPosn  = streamInfo.m_cdPosn;
    cdSize  = streamInfo.m_cdSize;
    texSlot = modelInfo.m_texlistSlot;

    colModel = modelInfo.m_pColModel;

    if (modelInfo.type == MODEL_TYPE_VEHICLE)
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
    switch (model->type)
        {
        case MODEL_TYPE_VEHICLE:
            extraData = std::make_unique<VehicleExtraData> ();
            break;
        case MODEL_TYPE_SIMPLE:
        case MODEL_TYPE_WEAPON:
        case MODEL_TYPE_TIME:
            extraData = std::make_unique<SimpleExtraData> ();
            break;
        default: extraData = std::make_unique<ExtraData> (); break;
        }

    extraData->SetParent (this);
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
        = std::max (seatOffsetDistance, 1.0f);
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

void
ChunkInfo::SimpleExtraData::FillFromModelId (uint32_t id)
{
    auto *modelInfo = ModelInfo::GetModelInfo<CSimpleModelInfo> (id);
    numAtomics      = modelInfo->m_numObjects;
}

void
ChunkInfo::SimpleExtraData::StoreInModelId (uint32_t id)
{
    auto *modelInfo = ModelInfo::GetModelInfo<CSimpleModelInfo> (id);
    modelInfo->m_numObjects = numAtomics;
}
