#include "core/Randomizer.hh"
#include "log.h"
#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <vector>
#include <string>
#include <Utils.hh>

#include "core/Common.hh"
#include "core/Config.hh"
#include "core/Logger.hh"

#include <limits>

class CutsceneRandomizer
{
    BEGIN_RANDOMIZER (CutsceneRandomizer)

    std::vector<std::vector<std::string>> m_Models;

    // config
    std::string ForcedModel;

    void
    ReadModelsFile ()
    {
        m_Models.push_back (std::vector<std::string> ());
        PSPifstream modelsFile
            = Rainbomizer::Common::OpenFileForReading ("CutsceneModels.txt");

        if (!modelsFile)
            return;

        for (std::string line; std::getline (modelsFile, line);)
            {
                if (line.size () < 2) {
                    m_Models.push_back ({});
                    continue;
                }

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
                LogMessage ("Randomizing {} to {}", model, ret);
                return ret;
            }

        LogMessage ("Couldn't find model to randomize: {}", model);
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
        REGISTER_HOOK_MEMBER_ADDR (0x8b3d0a0, RandomizeCutsceneObject,
                                   const char *, char *, char *);
    }

    END_RANDOMIZER (CutsceneRandomizer)
};
