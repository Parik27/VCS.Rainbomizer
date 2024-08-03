#include "ModelFun.hh"
#include "vcs/CModelInfo.hh"
#include "vcs/Rsl.hh"
#include "vcs/RslTexture.hh"
#include <hooks/Hooks.hh>

template <auto &CElementGroupModelInfo__CreateInstance>
RslElementGroup *
ModelFun::ModelLoadEvent::ProcessModelLoad (CElementModelInfo *element)
{
    this->operator() (element, nullptr);
    RslElementGroup *group = CElementGroupModelInfo__CreateInstance (element);
    this->operator() (element, group);

    return group;
}

ModelFun::ModelLoadEvent::ModelLoadEvent ()
{
    HOOK_MEMBER (Jal, 0x08aa1940, ProcessModelLoad,
                 RslElementGroup * (CElementModelInfo *) );
}

void
ModelFun::NodesArrayFix (CElementModelInfo *info, RslElementGroup *group)
{
    if (info->type != MODEL_TYPE_VEHICLE || !group)
        return;

    auto *model = static_cast<CVehicleModelInfo *> (info);

    int numNodes = 29;
    switch (model->m_vehicleType)
        {
        case VEHICLE_TYPE_AUTOMOBILE:
        case VEHICLE_TYPE_QUAD:
        case VEHICLE_TYPE_HELI:
        case VEHICLE_TYPE_PLANE: numNodes = 30; break;

        case VEHICLE_TYPE_BOAT:
        case VEHICLE_TYPE_JETSKI: numNodes = 12; break;
        case VEHICLE_TYPE_TRAIN: numNodes = 0; break;
        case VEHICLE_TYPE_BIKE:
        case VEHICLE_TYPE_BMX: numNodes = 11; break;
        }

    std::vector<bool> hasNodes (numNodes, false);
    group->data->ForAllChildrenRecursive ([&hasNodes] (RslNode *node, ...) {
        if (node->nodeTreeId > 0)
            {
                if (node->nodeTreeId < hasNodes.size ())
                    hasNodes[node->nodeTreeId] = true;
                else
                    node->nodeTreeId = 0;
            }
    });

    for (size_t i = 1; i < numNodes; i++)
        {
            if (hasNodes[i])
                continue;

            RslNode *node    = RslNodeCreate ();
            node->nodeTreeId = i;
            group->data->AddChildAtEnd (node);
        }
}

void
ModelFun::InstallNodesArrayFix ()
{
    ModelLoadEvent::Add (NodesArrayFix);
}
