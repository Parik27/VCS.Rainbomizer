#pragma once

#include "memory/Memory.hh"

class CDarkel
{
public:
    static bool
    FrenzyGoingOn ()
    {
        return GameFunction<0x8a3675c, bool ()>::Call ();
    }
};