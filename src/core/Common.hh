#pragma once

#include <string>
#include "Stream.hh"

namespace Rainbomizer
{
  namespace Common {
    inline std::string GetRainbomizerFileName (const std::string &fileName) {
      return "ms0:/PSP/PLUGINS/VCS.PPSSPP.Rainbomizer/" + fileName;
    }

    template<typename ... Args>
    inline PSPifstream OpenFileForReading (const std::string& fileName, Args... args)
    {
      return PSPifstream (GetRainbomizerFileName(fileName), args...);
    }

    template<typename ... Args>
    inline PSPofstream OpenFileForWriting (const std::string& fileName, Args... args)
    {
      return PSPofstream (GetRainbomizerFileName(fileName), args...);
    }
  }
};
