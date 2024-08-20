#include "debug/imgui/Backend.hh"
#include <core/Randomizer.hh>
#include <hooks/Hooks.hh>

#include "base.hh"
#include "ppsspp/KeyCodes.h"
#include "pspiofilemgr.h"
#include <imgui.h>
#include <ppsspp/Keyboard.hh>

class DebugMenuBackend : public Randomizer<DebugMenuBackend>
{
    inline static bool bIsOpen = false;

    static void DrawInterface ()
    {
        ImGui::NewFrame ();

        if (bIsOpen)
            ImGuiPsp::ProcessInput ();

        bool *p_open = &bIsOpen;

        DebugInterfaceManager::DrawAll ();

        ImGui::Render ();

        ImGuiPsp::RenderDrawData (ImGui::GetDrawData ());
    }

    template <auto &Render2dStuff>
    static void
    Render2dStuffHook ()
    {
        Render2dStuff ();

        if (PPSSPPUtils::CheckKeyUp<NKCODE_F9> ())
            bIsOpen = !bIsOpen;

        if (bIsOpen || DebugInterfaceManager::GetIsFloating())
            DrawInterface ();
    }

    template <auto &CPad__Update>
    static void
    PadUpdateHook (class CPad *pad, int num)
    {
        if (!bIsOpen)
            CPad__Update (pad, num);
    }

public:
    DebugMenuBackend ()
    {
        ImGui::CreateContext ();
        ImGui::StyleColorsDark ();
        ImGuiPsp::Initialise ();

        ImGui::GetIO ().IniFilename
            = "ms0:/PSP/PLUGINS/VCS.PPSSPP.Rainbomizer/imgui.ini";

        HOOK (Jal, 0x08938218, Render2dStuffHook, void ());
        HOOK (Jal, 0x0898b794, PadUpdateHook, void (class CPad*, int));
    }
};
