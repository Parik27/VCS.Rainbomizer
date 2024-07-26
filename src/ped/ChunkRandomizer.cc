#include "core/Common.hh"
#include "core/Logger.hh"
#include "memory/GameAddress.hh"
#include "utils/ContainerUtils.hh"
#include "vcs/CKeyGen.hh"
#include "vcs/CVector.hh"
#include <core/Randomizer.hh>
#include <hooks/Hooks.hh>
#include <vcs/CStreaming.hh>
#include <utils/Random.hh>
#include <map>
#include <vcs/CModelInfo.hh>
#include <core/Config.hh>

class ChunkRandomizer : public Randomizer<ChunkRandomizer>
{
    struct ChunkInfo
    {
        uint32_t hash = -1;
        uint32_t modelId = -1;

        uint32_t cdPosn = -1;
        uint32_t cdSize = -1;
        uint32_t texCdPosn = -1;
        uint32_t texCdSize = -1;

        void *colModel         = nullptr;
        bool  replaceCollision = true;

        auto
        GetModelDetails (uint32_t id)
        {
            auto *streaming = CStreaming::sm_Instance.Get ();

            auto &modelInfo  = *ModelInfo::GetModelInfo<CBaseModelInfo> (id);
            auto &streamInfo = streaming->ms_aInfoForModel[id];
            auto &texStreamInfo
                = streaming->ms_aInfoForModel[modelInfo.m_texlistSlot
                                              + streaming->m_texOffset];

            return std::tie (modelInfo, streamInfo, texStreamInfo);
        }

        void
        FillFromModelId (uint32_t id)
        {
            auto [modelInfo, streamInfo, texStreamInfo] = GetModelDetails (id);

            cdPosn = streamInfo.m_cdPosn;
            cdSize = streamInfo.m_cdSize;

            texCdPosn = texStreamInfo.m_cdPosn;
            texCdSize = texStreamInfo.m_cdSize;

            colModel = modelInfo.m_pColModel;
        }

        void
        ParseFlag (std::string_view flag)
        {
            if (flag == "original_collision")
                replaceCollision = false;
        }

        void
        ParseFlags (auto split_view)
        {
            auto flag = std::next (split_view.begin ());

            while (flag != split_view.end ())
                ParseFlag (std::string_view (*flag++));
        }

        void
        ParseLine (std::string_view line)
        {
            auto split_view = std::ranges::split_view (line, ' ');
            auto modelName  = std::string_view (*split_view.begin ());

            hash    = CKeyGen::GetUppercaseKey (modelName);
            modelId = ModelInfo::GetModelIdFromModelHash (hash);

            if (modelId != -1)
                {
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
        StoreInModelId (uint32_t id)
        {
            auto [modelInfo, streamInfo, texStreamInfo] = GetModelDetails (id);

            streamInfo.m_cdPosn = cdPosn;
            streamInfo.m_cdSize = cdSize;

            texStreamInfo.m_cdPosn = texCdPosn;
            texStreamInfo.m_cdSize = texCdSize;

            if (replaceCollision)
                modelInfo.m_pColModel = colModel;
        }
    };

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

                Rainbomizer::Logger::LogMessage ("Model %d: %x", i,
                                                 modelInfo->m_hashName);
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
                    m_Chunks.emplace_back ();
                    return;
                }

            auto &chunkInfo = m_Chunks.back ().emplace_back ();
            chunkInfo.ParseLine (line);
        });
    }

    ChunkInfo *
    GetRandomChunkForModelId (CStreaming *p1, int modelId)
    {
        if (modelId >= p1->m_texOffset || p1->ms_aInfoForModel[modelId].m_status != 0)
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
        auto randomChunk = GetRandomChunkForModelId(p1, modelId);

        if (randomChunk)
            {
                Rainbomizer::Logger::LogMessage ("randomizing %d to %d",
                                                 modelId, randomChunk->modelId);
                randomChunk->StoreInModelId (modelId);
            }

        return CStreaming__GetStreamingInfo (p1, modelId);
    }

public:
    ChunkRandomizer ()
    {
        RB_C_DO_CONFIG("ChunkRandomizer");

        HOOK_MEMBER (Jal, 0x08ad44c0, RandomizeChunk,
                     CStreamingInfo * (CStreaming *, int) );
        GameAddress<0x08ad8274>::WriteInstructions (jr (ra));
    }
};
