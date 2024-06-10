#include "core/Randomizer.hh"
#include <cstdio>
#include <vector>
#include <string>
#include <Utils.hh>

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

        FILE *modelsFile = Rainbomizer::Common::GetRainbomizerDataFile (
            "CutsceneModels.txt");

        if (!modelsFile)
            return;

        char line[256] = {};
        while (fgets (line, 256, modelsFile))
            {
                if (strlen(line) < 2)
                    {
                        Rainbomizer::Logger::LogMessage("New group");
                        m_Models.push_back ({});
                        continue;
                    }

                line[strcspn (line, "\n")] = 0;
                Rainbomizer::Logger::LogMessage("Pushing: %s", line);
                m_Models.back ().push_back (line);
            }
    }

    const char *
    GetRandomModelForModel (const char *model)
    {
        if (ForcedModel.size ())
            return ForcedModel.c_str ();

        for (auto &modelList : m_Models)
            if (DoesElementExist(modelList, model)) {
                auto ret = GetRandomElement(modelList).c_str();
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
} g_cutsRando;
