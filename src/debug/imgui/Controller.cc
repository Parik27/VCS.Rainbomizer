#include "debug/imgui/Backend.hh"
#include "ppsspp/KeyCodes.h"
#include "pspthreadman.h"
#include <algorithm>
#include <array>
#include <tuple>
#include <utility>
#include <imgui.h>
#include <ppsspp/Keyboard.hh>

namespace ImGuiPsp {
static std::array keyTranslation = {
    std::make_pair (ImGuiKey_Tab, NKCODE_TAB),
    std::make_pair (ImGuiKey_LeftArrow, NKCODE_DPAD_LEFT),
    std::make_pair (ImGuiKey_RightArrow, NKCODE_DPAD_RIGHT),
    std::make_pair (ImGuiKey_UpArrow, NKCODE_DPAD_UP),
    std::make_pair (ImGuiKey_DownArrow, NKCODE_DPAD_DOWN),
    std::make_pair (ImGuiKey_PageUp, NKCODE_PAGE_UP),
    std::make_pair (ImGuiKey_PageDown, NKCODE_PAGE_DOWN),
    std::make_pair (ImGuiKey_Home, NKCODE_MOVE_HOME),
    std::make_pair (ImGuiKey_End, NKCODE_MOVE_END),
    std::make_pair (ImGuiKey_Insert, NKCODE_INSERT),
    std::make_pair (ImGuiKey_Delete, NKCODE_FORWARD_DEL),
    std::make_pair (ImGuiKey_Backspace, NKCODE_DEL),
    std::make_pair (ImGuiKey_Space, NKCODE_SPACE),
    std::make_pair (ImGuiKey_Enter, NKCODE_ENTER),
    std::make_pair (ImGuiKey_Escape, NKCODE_BACK),
    std::make_pair (ImGuiKey_LeftShift, NKCODE_SHIFT_LEFT),
    std::make_pair (ImGuiKey_RightShift, NKCODE_SHIFT_RIGHT),
    std::make_pair (ImGuiKey_LeftCtrl, NKCODE_CTRL_LEFT),
    std::make_pair (ImGuiKey_RightCtrl, NKCODE_CTRL_RIGHT),
};

static std::array inputTranslation = {
    std::make_tuple ('a', 'A', NKCODE_A, 0),
    std::make_tuple ('b', 'B', NKCODE_B, 0),
    std::make_tuple ('c', 'C', NKCODE_C, 0),
    std::make_tuple ('d', 'D', NKCODE_D, 0),
    std::make_tuple ('e', 'E', NKCODE_E, 0),
    std::make_tuple ('f', 'F', NKCODE_F, 0),
    std::make_tuple ('g', 'G', NKCODE_G, 0),
    std::make_tuple ('h', 'H', NKCODE_H, 0),
    std::make_tuple ('i', 'I', NKCODE_I, 0),
    std::make_tuple ('j', 'J', NKCODE_J, 0),
    std::make_tuple ('k', 'K', NKCODE_K, 0),
    std::make_tuple ('l', 'L', NKCODE_L, 0),
    std::make_tuple ('m', 'M', NKCODE_M, 0),
    std::make_tuple ('n', 'N', NKCODE_N, 0),
    std::make_tuple ('o', 'O', NKCODE_O, 0),
    std::make_tuple ('p', 'P', NKCODE_P, 0),
    std::make_tuple ('q', 'Q', NKCODE_Q, 0),
    std::make_tuple ('r', 'R', NKCODE_R, 0),
    std::make_tuple ('s', 'S', NKCODE_S, 0),
    std::make_tuple ('t', 'T', NKCODE_T, 0),
    std::make_tuple ('u', 'U', NKCODE_U, 0),
    std::make_tuple ('v', 'V', NKCODE_V, 0),
    std::make_tuple ('w', 'W', NKCODE_W, 0),
    std::make_tuple ('x', 'X', NKCODE_X, 0),
    std::make_tuple ('y', 'Y', NKCODE_Y, 0),
    std::make_tuple ('z', 'Z', NKCODE_Z, 0),
    std::make_tuple ('0', ')', NKCODE_0, 0),
    std::make_tuple ('1', '!', NKCODE_1, 0),
    std::make_tuple ('2', '@', NKCODE_2, 0),
    std::make_tuple ('3', '#', NKCODE_3, 0),
    std::make_tuple ('4', '$', NKCODE_4, 0),
    std::make_tuple ('5', '%', NKCODE_5, 0),
    std::make_tuple ('6', '^', NKCODE_6, 0),
    std::make_tuple ('7', '&', NKCODE_7, 0),
    std::make_tuple ('8', '*', NKCODE_8, 0),
    std::make_tuple ('9', '(', NKCODE_9, 0),
    std::make_tuple (' ', ')', NKCODE_SPACE, 0),
    std::make_tuple (',', '<', NKCODE_COMMA, 0),
    std::make_tuple ('.', '>', NKCODE_PERIOD, 0),
    std::make_tuple ('/', '?', NKCODE_SLASH, 0),
    std::make_tuple (';', ':', NKCODE_SEMICOLON, 0),
    std::make_tuple ('\'', '"', NKCODE_APOSTROPHE, 0),
    std::make_tuple ('[', '}', NKCODE_LEFT_BRACKET, 0),
    std::make_tuple (']', '{', NKCODE_RIGHT_BRACKET, 0),
    std::make_tuple ('\\', '|', NKCODE_BACKSLASH, 0),
    std::make_tuple ('`', '~', NKCODE_GRAVE, 0),
    std::make_tuple ('-', '_', NKCODE_MINUS, 0),
    std::make_tuple ('=', '+', NKCODE_EQUALS, 0),
};

void
ProcessInput ()
{
    ImGuiIO &io = ImGui::GetIO ();

    // mouse
    {
        auto [x, y] = PPSSPPUtils::GetMousePosition ();
        x *= io.DisplaySize.x;
        y *= io.DisplaySize.y;

        x = std::clamp (x, 0.0f, io.DisplaySize.x);
        y = std::clamp (y, 0.0f, io.DisplaySize.y);

        io.AddMousePosEvent (x, y);

        for (int i = NKCODE_EXT_MOUSEBUTTON_1; i <= NKCODE_EXT_MOUSEBUTTON_5;
             i++)
            {
                io.AddMouseButtonEvent (i - NKCODE_EXT_MOUSEBUTTON_1,
                                        PPSSPPUtils::CheckKeyDown (
                                            InputKeyCode (i)));
            }

        if (PPSSPPUtils::CheckKeyDown (NKCODE_EXT_MOUSEWHEEL_UP))
            io.AddMouseWheelEvent (0.0f, 1.0f);
        if (PPSSPPUtils::CheckKeyDown (NKCODE_EXT_MOUSEWHEEL_DOWN))
            io.AddMouseWheelEvent (0.0f, -1.0f);

    }

    // keyboard
    {
        for (auto [imguiKey, nkKey] : keyTranslation)
            {
                io.AddKeyEvent (imguiKey, PPSSPPUtils::CheckKeyDown (nkKey));
            }

        io.AddKeyEvent (ImGuiMod_Shift,
                        PPSSPPUtils::CheckKeyDown (NKCODE_SHIFT_LEFT)
                            || PPSSPPUtils::CheckKeyDown (NKCODE_SHIFT_RIGHT));

        io.AddKeyEvent (ImGuiMod_Ctrl,
                        PPSSPPUtils::CheckKeyDown (NKCODE_CTRL_LEFT)
                            || PPSSPPUtils::CheckKeyDown (NKCODE_CTRL_RIGHT));

        static char lastKey = 0;
        bool        shift   = PPSSPPUtils::CheckKeyDown (NKCODE_SHIFT_LEFT)
                     || PPSSPPUtils::CheckKeyDown (NKCODE_SHIFT_RIGHT);

        for (auto &[imguiKey, upcaseKey, nkKey, delay] : inputTranslation)
            {
                if (PPSSPPUtils::CheckKeyDown (nkKey))
                    {
                        if (delay < sceKernelGetSystemTimeLow ())
                            {
                                int delayAmount = delay == 0 ? 500000 : 1000;
                                delay           = sceKernelGetSystemTimeLow ()
                                        + delayAmount;
                                if (shift)
                                    io.AddInputCharacter (upcaseKey);
                                else
                                    io.AddInputCharacter (imguiKey);
                                lastKey = nkKey;
                            }
                    }
                else
                    {
                        delay = 0;
                    }
            }
    }
}

}; // namespace ImGuiPsp
