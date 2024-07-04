#pragma once

#include "memory/GameAddress.hh"
#include "GhidraTypes.hh"

struct CMissionTextOffsetsEntry {
    char szMissionName[8];
    uint offset;
};

struct CMissionTextOffsets {
    struct CMissionTextOffsetsEntry data[200];
    short size;
};

struct CKeyArray
{
    struct CKeyEntry *entries;
    int               numEntries;
    undefined         field2_0x8;
    undefined         field3_0x9;
    undefined         field4_0xa;
    undefined         field5_0xb;
    undefined         field6_0xc;
    undefined         field7_0xd;
    undefined         field8_0xe;
    undefined         field9_0xf;
};

struct CKeyEntry
{
    char16_t *value;
    char      key[8];
};

#pragma pack(push, 1)
class CText
{
public:
    struct CKeyArray keyArray;
    struct CKeyArray mission_keyArray;
    byte encoding;
    bool bHasMissionTextOffsets; /* Created by retype action */
    bool bIsMissionTextLoaded; /* Created by retype action */
    char szMissionTableName[8]; /* Created by retype action */
    struct CMissionTextOffsets MissionTextOffsets;
    undefined field10_0x98f;
    undefined field10_0x990;
    undefined field11_0x991;
    undefined field12_0x992;
    undefined field13_0x993;

    inline static constexpr auto LoadMissionText
        = GameFunction<0x089f6bac, void (CText *, const char *)>{};

    inline static constexpr auto Get = GameFunction<0x089f6b90, CText *()>{};
};
#pragma pack(pop)
