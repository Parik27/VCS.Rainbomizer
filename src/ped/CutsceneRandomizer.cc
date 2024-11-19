#include <cstdio>
#include <vector>
#include <string>

#include <utils/Utils.hh>

#include "core/Randomizer.hh"
#include "core/Common.hh"
#include "core/Config.hh"
#include "core/Logger.hh"

class CutsceneRandomizer : public Randomizer<CutsceneRandomizer>
{
    std::vector<std::vector<std::string>> m_Models;

    // config
    std::string ForcedModel;

    void
    ReadModelsFile ()
    {
        m_Models.push_back (std::vector<std::string> ());

        auto modelsFile = Rainbomizer::Common::GetRainbomizerDataFile (
            "CutsceneModels.txt");

        modelsFile.ReadLines ([this] (std::string_view line) {
            if (line.size() < 2)
                {
                    Rainbomizer::Logger::LogMessage ("New group");
                    m_Models.push_back ({});
                    return;
                }

            Rainbomizer::Logger::LogMessage ("Pushing: %s", line.data());
            m_Models.back ().emplace_back (line.data ());
        });
    }

    const char *
    GetRandomModelForModel (const char *model)
    {
        if (ForcedModel.size ())
            return ForcedModel.c_str ();

        for (auto &modelList : m_Models)
            if (DoesElementExist (modelList, model))
                {
                    auto ret = GetRandomElementMut (modelList).c_str ();
                    return ret;
                }

        return model;
    }

public:
    template <auto &strtok>
    const char *
    RandomizeCutsceneObject (char *dst, char *src)
    {
        const char *origModel = strtok (dst, src);
        return GetRandomModelForModel (origModel);
    }

    CutsceneRandomizer ()
    {
        RB_C_DO_CONFIG ("CutsceneRandomizer", ForcedModel);

        ReadModelsFile ();
        HOOK_MEMBER (Jal, (0x8b3d0a0), RandomizeCutsceneObject,
                     const char *(char *, char *) );
    }
};
