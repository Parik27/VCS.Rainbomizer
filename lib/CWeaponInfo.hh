#pragma once

class CWeaponInfo
{
    char unk[0x68];

public:
    int nSlot;
    int unk2;

    static CWeaponInfo *
    GetWeaponInfo (int id)
    {
        return ((CWeaponInfo * (*) (int, int) ) 0x8b20150) (0, id);
    }
};
