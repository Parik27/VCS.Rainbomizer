#pragma once

#include <cstdio>
#include <string>

namespace Rainbomizer {
class Common
{
public:
    static std::string GetRainbomizerFileName (const std::string &name,
                                               const std::string &subdirs = "",
                                               bool               temp = false);

    static FILE *GetRainbomizerFile (const std::string &name,
                                     const std::string &mode,
                                     const std::string &subdirs      = "",
                                     bool               tempFallback = false);

    static FILE *GetRainbomizerDataFile (const std::string &name,
                                         const std::string &mode = "r");
};
} // namespace Rainbomizer
