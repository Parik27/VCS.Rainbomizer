#pragma once

#include <algorithm>
#include <cstdint>

struct RslV3
{
    float x, y, z;
};

struct RslLLLink
{
    struct RslLLLink* next;
    struct RslLLLink* prev;
};

struct RslLinkList
{
    struct RslLLLink link;
};

struct RslObject {
    uint8_t  type;
    uint8_t  subType;
    uint8_t  flags;
    uint8_t  privateFlags;
    void* parent;
};

struct RslObjectHasNode {
    struct RslObject   object;
    struct RslLLLink   lNode;
    void      (*sync)();
};

struct RslRaster {
    uint32_t unk1;
    uint8_t* data;
    int16_t  minWidth;
    uint8_t  logWidth;
    uint8_t  logHeight;
    uint8_t  depth;
    uint8_t  mipmaps;
    uint16_t unk2;	// like RW raster format?

    uint32_t
    GetWidth ()
    {
        return 1 << logWidth;
    }

    uint32_t
    GetHeight ()
    {
        return 1 << logHeight;
    }

    uint8_t
    GetMipmaps ()
    {
        return mipmaps & 0b111111;
    }

    uint8_t *
    GetTexel (int32_t n)
    {
        auto     out = data;
        uint32_t w   = GetWidth ();
        uint32_t h   = GetHeight ();

        while (n--)
            {
                w = std::max (w, uint32_t (minWidth));
                out += (w * h * depth) / 8;
                w >>= 1;
                h >>= 1;
            }

        return out;
    }

  uint8_t
  GetPaletteIdx (size_t x, size_t y)
  {
      if (depth > 8)
          return 0;

      uint32_t w = GetWidth ();
      uint32_t h = GetHeight ();

      if (x >= w || y >= h)
          return 0;

      switch (depth)
          {
          case 8: return data[y * w + x];
          case 4: return data[(y * w + x) / 2] >> (4 * (x & 1)) & 0xf;
          }

      return 0;
  }

  uint8_t
  CalculatePaletteSize ()
  {
      if (depth > 8)
          return 0;

      uint8_t maxIdx = 0;
      for (size_t y = 0; y < GetHeight (); y++)
          for (size_t x = 0; x < GetWidth (); x++)
              {
                  maxIdx = std::max (maxIdx, GetPaletteIdx (x, y));
              }

      return maxIdx + 1;
  }

  uint32_t
  GetPixelColour (size_t x, size_t y)
  {
      if (depth <= 8)
          {
              uint8_t   idx     = GetPaletteIdx (x, y);
              uint32_t *palette = GetPalette ();
              return palette ? palette[idx] : 0;
          }

      return 0;
  }

  uint32_t *
  GetPalette ()
  {
      if (depth <= 8)
          return reinterpret_cast<uint32_t *> (GetTexel (GetMipmaps ()));

      return nullptr;
  }
};

struct RslTexList {
    struct RslObject   object;
    struct RslLinkList texturesInDict;
    struct RslLLLink   lInInstance;
};

struct RslTexture {
    struct RslRaster* raster;
    struct RslTexList* dict;
    struct RslLLLink lInDictionary;
    char name[32];
    char mask[32];
};
