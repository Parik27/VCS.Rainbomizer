#pragma once

#include <cstdint>

struct Pattern
{
    uintptr_t   address;
    const char *pattern_str;
    int32_t     offset = 0;
    enum
    {
        NONE,
        BRANCH,
        GP_DATA
    } resolver
        = NONE;
};
