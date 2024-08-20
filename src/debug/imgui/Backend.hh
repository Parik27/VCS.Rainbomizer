#pragma once

#include <imgui.h>

namespace ImGuiPsp {
float &GetDrawingScale ();
void   Initialise ();
void   CreateFontsTexture ();
void   ProcessInput ();
void   RenderDrawData (ImDrawData *data);
}; // namespace ImGuiPsp
