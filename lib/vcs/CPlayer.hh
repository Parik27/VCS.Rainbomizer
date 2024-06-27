#pragma once

#include "memory/GameAddress.hh"
#include "memory/Memory.hh"
#include "vcs/CVehicle.hh"

inline class CPed *
FindPlayerPed ()
{
    return GameFunction<0x896075c, CPed *()>::Call ();
}

inline static GameFunction<0x08960600, CVehicle* ()> FindPlayerVehicle{};
