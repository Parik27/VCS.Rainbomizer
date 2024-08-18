#include "base.hh"
#include "imgui.h"
#include "vcs/CPlayer.hh"
#include "vcs/CRunningScript.hh"
#include <vendor/imgui_memory_editor.h>

#include <array>

class HexEditorDebugInterface : public DebugInterface
{
    void
    Draw () override
    {
        CPed *ped = FindPlayerPed ();
	if (!ped)
	  return;

	static MemoryEditor mem_edit;
	//mem_edit.DrawContents (ped, 0xd10, (uintptr_t)ped);
        mem_edit.DrawContents (CTheScripts::ScriptSpace.Get (),
                               CTheScripts::MainScriptSize,
                               (uintptr_t) CTheScripts::ScriptSpace.Get ());
    }

public:
    const char *
    GetName () const override
    {
        return "Hex Editor";
    }
} g_HexEditorDebugInterface;
