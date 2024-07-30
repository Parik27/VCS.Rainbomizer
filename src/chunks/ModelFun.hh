#pragma once

#include <core/Events.hh>
#include "vcs/CModelInfo.hh"
#include "vcs/RslTexture.hh"
#include <vcs/Rsl.hh>

/* Class for editing chunk models in runtime */
class ModelFun
{
    static void NodesArrayFix (CElementModelInfo *info, RslElementGroup *group);

public:
    // Event fired whenever an instance for a model is created.
    // It's fired twice. Once before the instance is created, and once after
    // RslElementGroup will be nullptr for the call before it's created
    struct ModelLoadEvent
        : public Event<ModelLoadEvent, CElementModelInfo *, RslElementGroup *>
    {

        template <auto &CElementGroupModelInfo__CreateInstance>
        RslElementGroup *ProcessModelLoad (CElementModelInfo *element);

        ModelLoadEvent ();
    };

    static void InstallNodesArrayFix ();
};
