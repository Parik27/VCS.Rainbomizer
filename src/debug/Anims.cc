#include "core/Logger.hh"
#include "scm/Opcodes.hh"
#include <core/Randomizer.hh>
#include <GhidraTypes.hh>
#include <core/Events.hh>
#include <memory/GameAddress.hh>
#include <hooks/Hooks.hh>
#include <utils/Random.hh>
#include <vcs/CStreaming.hh>
#include <scm/Command.hh>

struct AnimDescriptor {
    dword id;
    char name[45];
    dword defaultFlags;
};

struct CAnimManager {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined *vftable;
    struct CAnimManagerData *Data;
};

struct AnimAssocDefinition {
    char groupname[24];
    char blockName[20];
    int animBase;
    dword numAnims;
};

struct CAnimBlendAssocGroup {
    void *m_pAnimBlock;
    struct CAnimBlendAssociation *m_aAssociationArray;
    dword m_numAssociations;
    dword m_idOffset;
    dword m_grpId;
};

struct CAnimManagerData {
    struct AnimAssocDefinition m_aAnimAssocDefinitions[200];
    struct AnimDescriptor associations[990];
    dword numAssocGroups;
    dword numAssocs;
    dword numUnk;
    dword m_aAnimations;
    dword m_aAnimBlocks;
    dword m_numAnimations;
    dword m_numAnimBlocks;
    struct CAnimBlendAssocGroup *m_aAnimAssocGroups;
};

class CAnimBlendTree
{
public:
    	class CAnimBlendSequence* blendSequences;
	char name[24];
	int16 numSequences;
	bool loadSpecial;     // still not sure about those two...copied from vc_anim
	bool compressed;      // where I already didn't know where they came from
	float totalLength;
	int unknown;
};

struct CAnimBlendAssociation {
    undefined4 field0_0x0;
    undefined4 field1_0x4;
    undefined4 field2_0x8;
    undefined4 field3_0xc;
    float blendAmount;
    undefined field5_0x14;
    undefined field6_0x15;
    undefined field7_0x16;
    undefined field8_0x17;
    int numNodes;
    int field10_0x1c;
    CAnimBlendTree* hierarchy;
    float blendDelta;
    undefined4 field13_0x28;
    undefined4 field14_0x2c;
    undefined2 field15_0x30;
    undefined2 field16_0x32;
    undefined4 field17_0x34;
    undefined field18_0x38;
    undefined field19_0x39;
    undefined field20_0x3a;
    undefined field21_0x3b;
    undefined field22_0x3c;
    undefined field23_0x3d;
    undefined field24_0x3e;
    undefined field25_0x3f;
    undefined field26_0x40;
    undefined field27_0x41;
    undefined field28_0x42;
    undefined field29_0x43;
};

class AnimDebugger : public Randomizer<AnimDebugger>
{
    void
    PrintAnimAssocDefinitions ()
    {
        static GameVariable<CAnimManager *, 0x08bb380c> g_AnimManager;
	auto data = g_AnimManager->Data;

        for (size_t i = 10; i < data->m_numAnimBlocks - 70; i++)
            {
                CStreaming::RequestModel(CStreaming::sm_Instance->m_anmOffset + i);
                Rainbomizer::Logger::LogMessage ("Loading block: %d", i);

                CStreaming::LoadAllRequestedModels(false);
            }


        for (size_t i = 0; i < data->numAssocGroups; i++)
            {
                auto &group = data->m_aAnimAssocGroups[i];
                Rainbomizer::Logger::LogMessage (
                    "%s %s %d %d", data->m_aAnimAssocDefinitions[i].groupname,
                    data->m_aAnimAssocDefinitions[i].blockName,
                    group.m_numAssociations, group.m_idOffset);

                for (size_t i = 0; i < group.m_numAssociations; i++)
                    {
                        auto &assoc = group.m_aAssociationArray[i];
                        Rainbomizer::Logger::LogMessage ("\t\t%s %d", assoc.hierarchy->name, data->associations[group.m_idOffset + i].id);
                    }
            }
        
        // for (auto assoc : data->associations)
	//   {
	//     Rainbomizer::Logger::LogMessage ("AnimAssoc: %s %d", assoc.name, assoc.id);
	//   }

	// for (auto group : data->m_aAnimAssocDefinitions)
	//   {
	//     Rainbomizer::Logger::LogMessage ("AnimGroup: %s %s %d %d", group.groupname, group.blockName, group.animBase, group.numAnims);
	//   }
    }

  template<auto &CAimBlendAssocGroup__GetAnimation>
  static CAnimBlendAssociation* AnimCrashFix (CAnimBlendAssocGroup* group, int id)
  {
      static GameVariable<CAnimManager *, 0x08bb380c> g_AnimManager;
      int offset = id - g_AnimManager->Data->associations[group->m_idOffset].id;

      std::vector<CAnimBlendAssociation*> associations;

      CallCommand<REQUEST_ANIMATION>("LAN_C1");

      using namespace std::literals;

      bool enabled = true;

      if (id == 4)
          id = 252;

      else if (id == 5)
          id = 253;

      else if (id == 6)
          id = 250;

      else
          enabled = false;

      for (size_t i = 0; i < g_AnimManager->Data->numAssocGroups && enabled; i++)
          {

              auto &group = g_AnimManager->Data->m_aAnimAssocGroups[i];

              // if (g_AnimManager->Data->m_aAnimAssocDefinitions[group.m_grpId]
              //         .groupname
              //     != "LAN_C1"sv)
              //     continue;

              auto offset = id - g_AnimManager->Data->associations[group.m_idOffset].id;

              if (offset < group.m_numAssociations && offset >= 0)
                  {
                      // Rainbomizer::Logger::LogMessage ("Assoc name: %s", group.m_aAssociationArray[offset].hierarchy->name);
                      associations.push_back(&group.m_aAssociationArray[offset]);
                  }
          }

        if (associations.size() > 0)
            {
                return GetRandomElement (associations);
            }

      if (offset < group->m_numAssociations && offset >= 0)
        {
            return &group->m_aAssociationArray[offset];
        }

      return &group->m_aAssociationArray[0];
  }

public:
    AnimDebugger ()
    {
      // HOOK (Jmp, 0x088e2798, AnimCrashFix, void(CAnimBlendAssocGroup*, int));
        RandomizationSeedEvent::Add (
            [] (int) { Get ().PrintAnimAssocDefinitions (); });
    }
};
