#pragma once

#include <cstdint>
#include <vcs/CModelInfo.hh>

struct ChunkInfo
{
    class ExtraData
    {
    public:
        virtual ~ExtraData () = default;
        virtual void StoreInModelId (uint32_t id){};
        virtual void FillFromModelId (uint32_t id){};
    };

    class VehicleExtraData : public ExtraData
    {
    public:
        CVector positions[15];
        float   wheelScale;
        float   wheelScaleRear;
        float   seatOffsetDistance;

        virtual void StoreInModelId (uint32_t id) override;
        virtual void FillFromModelId (uint32_t id) override;
    };

    uint32_t hash    = -1;
    uint32_t modelId = -1;

    uint32_t cdPosn    = -1;
    uint32_t cdSize    = -1;
    uint32_t texCdPosn = -1;
    uint32_t texCdSize = -1;

    void *colModel         = nullptr;
    bool  replaceCollision = true;

    std::unique_ptr<ExtraData> extraData;

    void CreateExtraData (uint32_t modelId);
    auto GetModelDetails (uint32_t id);

    void FillFromModelId (uint32_t id);
    void StoreInModelId (uint32_t id);

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

    void ParseLine (std::string_view line);

};
