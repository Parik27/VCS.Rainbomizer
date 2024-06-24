#pragma once

#include "KeyCodes.h"
#include "pspiofilemgr.h"
#include <utility>

namespace PPSSPPUtils {
template <InputKeyCode KEY>
static bool
CheckKeyDown ()
{
    bool state = false;
    sceIoDevctl ("kemulator:", EMULATOR_DEVCTL__GET_VKEY, (void *) KEY, 0,
                 &state, sizeof (state));
    return state;
}

template <InputKeyCode KEY>
static bool
CheckKeyUp ()
{
    static bool prevState = false;
    bool        currState = CheckKeyDown<KEY> ();

    return std::exchange (prevState, currState) && !currState;
}

}; // namespace PPSSPPUtils
