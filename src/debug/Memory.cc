#include "base.hh"
#include "imgui.h"
#include "pspsdk.h"

#include <array>

class MemoryDebugInterface : public DebugInterface
{
    void
    Draw () override
    {
        static int MemoryLeft = pspSdkTotalFreeUserMemSize ();
        ImGui::Text ("Memory left: %d", MemoryLeft);
	ImGui::Text ("Memory left: %f KB", MemoryLeft / 1024.0f);
	ImGui::Text ("Memory left: %f MB", MemoryLeft / 1024.0f / 1024.0f);
        if (ImGui::Button ("Refresh"))
            {
                MemoryLeft = pspSdkTotalFreeUserMemSize ();
            }
    }

public:
    const char *
    GetName () const override
    {
        return "Memory";
    }
} g_MemoryDebugInterface;
