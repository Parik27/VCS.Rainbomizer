#pragma once

#include <cstdint>
#include <string_view>

struct Pattern
{
    uintptr_t        address;
    std::string_view pattern_str;
    int32_t          offset = 0;
    enum
    {
        NONE,
        BRANCH,
        GPOFFSET
    } resolver
        = NONE;
};
