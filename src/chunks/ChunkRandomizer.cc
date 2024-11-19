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

#define ENABLE_CHUNK_SUB_GROUPS

class ChunkRandomizer : public Randomizer<ChunkRandomizer>
{
    bool m_ChunksInitialised       = false;
    bool m_InitialChunksRandomized = false;

    struct ChunkGroup
    {
#ifdef ENABLE_CHUNK_SUB_GROUPS
        uint32_t                  nameHash;
        std::vector<ChunkGroup *> subGroups;
        bool                      metaGroup = false;
#endif

        bool                   onlyOnLoad;
        std::vector<ChunkInfo> chunks;

        bool
        CheckModel (uint32_t modelId)
        {
#ifdef ENABLE_CHUNK_SUB_GROUPS
            for (auto &subGroups : subGroups)
                if (subGroups->CheckModel (modelId))
                    return true;
#endif

            return std::ranges::any_of (chunks,
                                        [modelId] (ChunkInfo &chunkInfo) {
                                            return chunkInfo.modelId == modelId;
                                        });
        }

        size_t
        GetSize ()
        {
            size_t size = 0;

#ifdef ENABLE_CHUNK_SUB_GROUPS
            for (auto &subGroups : subGroups)
                size += subGroups->GetSize ();
#endif
            return size + chunks.size ();
        }

        ChunkInfo *
        GetModelByIndex (size_t index)
        {
#ifdef ENABLE_CHUNK_SUB_GROUPS
            for (auto &subGroups : subGroups)
                {
                    if (index < subGroups->GetSize ())
                        return subGroups->GetModelByIndex (index);
                    index -= subGroups->GetSize ();
                }
#endif
            return &chunks[index];
        }

        ChunkInfo *
        GetRandomChunk ()
        {
            size_t randomChunk = RandomSize (GetSize () - 1);
            return GetModelByIndex (randomChunk);
        }

        void
        ReadFlag (std::string_view line)
        {
            Rainbomizer::Logger::LogMessage ("FLAG: %.*s", line.size(), line.data ());

            if (line == "OnLoad")
                onlyOnLoad = true;

#ifdef ENABLE_CHUNK_SUB_GROUPS
            if (line.starts_with ("Include"))
                {
                    auto     groupName = line.substr (8);
                    uint32_t nameHash
                        = CKeyGen::GetUppercaseKey (groupName.data ());

                    auto group
                        = std::find_if (Get ().m_Chunks.begin (),
                                        Get ().m_Chunks.end (),
                                        [nameHash] (auto &group) {
                                            return group.nameHash == nameHash;
                                        });

                    if (group != Get ().m_Chunks.end ())
                        subGroups.push_back (&*group);
                }

            if (line.starts_with ("Name"))
                {
                    auto groupName = line.substr (5);
                    nameHash = CKeyGen::GetUppercaseKey (groupName.data ());
                }

            if (line == "Meta")
                metaGroup = true;
#endif
        }
    };

    std::vector<ChunkGroup> m_Chunks;

    void
    DumpModelHashes ()
    {
        for (size_t i = 0; i < CStreaming::sm_Instance->m_texOffset; i++)
            {
                auto modelInfo  = ModelInfo::ms_modelInfoPtrs[i];
                auto streamInfo = CStreaming::sm_Instance->ms_aInfoForModel[i];

                if (!modelInfo || !streamInfo.m_cdSize)
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
                    m_Chunks.emplace_back ();
                    return;
                }

            // Read flags
            if (line.starts_with ('['))
                {
                    m_Chunks.back ().ReadFlag (
                        line.substr (1, line.length () - 2));
                    return;
                }

            auto &chunkInfo = m_Chunks.back ().chunks.emplace_back ();
            chunkInfo.ParseLine (line);
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
                if (chunkGroup.onlyOnLoad || chunkGroup.metaGroup)
                    continue;

                if (chunkGroup.CheckModel (modelId))
                    return chunkGroup.GetRandomChunk ();
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

    void
    RandomizeGroup (ChunkGroup *group)
    {
        size_t              groupSize = group->GetSize ();
        std::vector<size_t> randomIndices (groupSize);
        std::iota (randomIndices.begin (), randomIndices.end (), 0);
        std::shuffle (randomIndices.begin (), randomIndices.end (),
                      RandEngine ());

        for (size_t i = 0; i < groupSize; i++)
            {
                auto &chunk = *group->GetModelByIndex (i);
                chunk.StoreInModelId (
                    group->GetModelByIndex (randomIndices[i])->modelId);
            }
    }

    void
    RandomizeGroupsOnLoad ()
    {
        ReadChunkInfo ();
        for (auto &group : m_Chunks)
            {
                if (group.onlyOnLoad)
                    RandomizeGroup (&group);
            }
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
        RB_C_DO_CONFIG ("ChunkRandomizer");

        ModelFun::InstallNodesArrayFix ();

        HOOK_MEMBER (Jal, 0x08ad44c0, RandomizeChunk,
                     CStreamingInfo * (CStreaming *, int) );

        HOOK (Jal, 0x08aa1974, FixComponentCrash, int (CVehicleModelInfo *));
        HOOK (Jal, 0x08aa1a00, FixComponentCrash, int (CVehicleModelInfo *));

        GameAddress<0x08ad8274>::WriteInstructions (jr (ra));

        RandomizationSeedEvent::Add ([] (int seed) {
            Get ().m_InitialChunksRandomized = false;
            Get ().RandomizeGroupsOnLoad ();
        });
    }
};
