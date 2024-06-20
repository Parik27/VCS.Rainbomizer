#pragma once

#include "memory/Memory.hh"

inline class CPed *
FindPlayerPed ()
{
    return GameFunction<0x896075c, CPed *()>::Call ();
}
