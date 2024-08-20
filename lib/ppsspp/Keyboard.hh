#pragma once

#include "KeyCodes.h"
#include "pspiofilemgr.h"
#include <cstring>
#include <utility>
#include <string>
#include <string_view>

namespace PPSSPPUtils {
static bool
CheckKeyDown (InputKeyCode Key)
{
    bool state = false;
    sceIoDevctl ("kemulator:", EMULATOR_DEVCTL__GET_VKEY, (void *) Key, 0,
                 &state, sizeof (state));
    return state;
}

template <InputKeyCode KEY>
static bool
CheckKeyDown ()
{
    return CheckKeyDown (KEY);
}

template <InputKeyCode KEY>
static bool
CheckKeyUp ()
{
    static bool prevState = false;
    bool        currState = CheckKeyDown<KEY> ();

    return std::exchange (prevState, currState) && !currState;
}

static std::pair<float, float>
GetMouseDelta ()
{
    float MouseX = 0;
    float MouseY = 0;

    sceIoDevctl ("kemulator:", EMULATOR_DEVCTL__GET_AXIS,
                 (void *) JOYSTICK_AXIS_MOUSE_REL_X, 0, &MouseX,
                 sizeof (MouseX));
    sceIoDevctl ("kemulator:", EMULATOR_DEVCTL__GET_AXIS,
                 (void *) JOYSTICK_AXIS_MOUSE_REL_Y, 0, &MouseY,
                 sizeof (MouseY));

    return std::make_pair (MouseX * 30, MouseY * 30);
}

static std::pair<float, float>
GetMousePosition ()
{
    float MouseX = 0;
    float MouseY = 0;

    sceIoDevctl ("kemulator:", EMULATOR_DEVCTL__GET_MOUSE_X, 0, 0, &MouseX,
                 sizeof (MouseX));
    sceIoDevctl ("kemulator:", EMULATOR_DEVCTL__GET_MOUSE_Y, 0, 0, &MouseY,
                 sizeof (MouseY));

    return std::make_pair (MouseX, MouseY);
}

inline std::string_view
GetStackTrace ()
{
    static char buffer[4096];
    memset (buffer, 0, sizeof (buffer));
    sceIoDevctl ("kemulator:", EMULATOR_DEVCTL__GET_STACK_TRACE, 0, 0, buffer,
                 sizeof (buffer));
    return buffer;
}

}; // namespace PPSSPPUtils
