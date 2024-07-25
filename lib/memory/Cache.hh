#pragma once

#include <string>

class MemorySignatureCache
{
  inline static constexpr int FileSignature = 0xBAB0CEED;

public:
    static void ReadFromFile (const std::string &filename);
    static void WriteToFile (const std::string &filename);
};
