#include "chunks/ModelFun.hh"
#include "core/Common.hh"
#include "core/Logger.hh"
#include "memory/GameAddress.hh"
#include "utils/ContainerUtils.hh"
#include "vcs/RslTexture.hh"
#include <core/Randomizer.hh>
#include <hooks/Hooks.hh>
#include <vcs/CStreaming.hh>
#include <utils/Random.hh>
#include <vcs/CModelInfo.hh>
#include <core/Config.hh>
#include <vcs/Rsl.hh>

#include "ChunkInfo.hh"

class ChunkRandomizer : public Randomizer<ChunkRandomizer>
{
    bool                                m_ChunksInitialised = false;
    std::vector<std::vector<ChunkInfo>> m_Chunks;

    void
    DumpModelHashes ()
    {
        for (size_t i = 0; i < CStreaming::sm_Instance->m_texOffset; i++)
            {
                auto modelInfo = ModelInfo::ms_modelInfoPtrs[i];

                if (!modelInfo)
                    continue;

                Rainbomizer::Logger::LogMessage ("Model %d: %x, Type: %d", i,
                                                 modelInfo->m_hashName,
                                                 modelInfo->type);
            }
    }

    void
    ReadChunkInfo ()
    {
        if (std::exchange (m_ChunksInitialised, true))
            return;

        auto f = Rainbomizer::Common::GetRainbomizerDataFile ("Chunks.txt");

        DumpModelHashes ();

        m_Chunks.emplace_back ();
        f.ReadLines ([this] (std::string_view line) {
            if (line.length () < 2)
                {
                    Rainbomizer::Logger::LogMessage("");
                    m_Chunks.emplace_back ();
                    return;
                }

            auto &chunkInfo = m_Chunks.back ().emplace_back ();
            chunkInfo.ParseLine (line);

            Rainbomizer::Logger::LogMessage (
                "%s: %d", line.data (),
                ModelInfo::GetModelInfo (chunkInfo.modelId)->type);
        });
    }

    ChunkInfo *
    GetRandomChunkForModelId (CStreaming *p1, int modelId)
    {
        auto [modelInfo, streamInfo, texStreamInfo]
            = ChunkInfo::GetModelDetails (modelId);

        if (modelId >= p1->m_texOffset
            || p1->ms_aInfoForModel[modelId].m_status != 0
            || p1->ms_aInfoForModel[modelInfo.m_texlistSlot + p1->m_texOffset]
                       .m_status
                   != 0)
            return nullptr;

        for (auto &chunkGroup : m_Chunks)
            {
                auto chunkGroupIds
                    = std::views::transform (chunkGroup,
                                             [] (ChunkInfo &chunkInfo) {
                                                 return chunkInfo.modelId;
                                             });

                if (DoesElementExist (chunkGroupIds, modelId))
                    return &GetRandomElementMut (chunkGroup);
            }

        return nullptr;
    }

    template <auto &CStreaming__GetStreamingInfo>
    CStreamingInfo *
    RandomizeChunk (CStreaming *p1, int modelId)
    {
        ReadChunkInfo ();
        auto randomChunk = GetRandomChunkForModelId (p1, modelId);

        if (randomChunk)
            {
                Rainbomizer::Logger::LogMessage ("randomizing %d to %d",
                                                 modelId, randomChunk->modelId);
                randomChunk->StoreInModelId (modelId);
            }

        return CStreaming__GetStreamingInfo (p1, modelId);
    }

    // The game may force a component that exists for the original model
    // but not a randomized chunk model.
    template <auto &CVehicleModelInfo__ChooseComponent>
    static int
    FixComponentCrash (CVehicleModelInfo *model)
    {
        int component = CVehicleModelInfo__ChooseComponent (model);
        if (component != -1 && model->m_numComponents <= component)
            return -1;
        return component;
    }

public:
    ChunkRandomizer ()
    {
        RB_C_DO_CONFIG("ChunkRandomizer");

        ModelFun::InstallNodesArrayFix ();

        HOOK_MEMBER (Jal, 0x08ad44c0, RandomizeChunk,
                     CStreamingInfo * (CStreaming *, int) );

        HOOK (Jal, 0x08aa1974, FixComponentCrash, int (CVehicleModelInfo*));
        HOOK (Jal, 0x08aa1a00, FixComponentCrash, int (CVehicleModelInfo*));

        GameAddress<0x08ad8274>::WriteInstructions (jr (ra));
    }
};
