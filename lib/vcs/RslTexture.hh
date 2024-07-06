#pragma once

#include <algorithm>
#include <cstdint>
#include <span>

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

struct RslRaster
{
    void    *commandBuffer;
    uint8_t *data;
    int16_t  stride;
    uint8_t  logWidth;
    uint8_t  logHeight;
    uint8_t  depth;

    uint8_t mipmaps : 6;
    bool    uClamp : 1;
    bool    vClamp : 1;

    uint8_t unk;

    enum ColorMode
    {
        GU_PSM_5551 = 1,
        GU_PSM_8888 = 5,
    } colorMode : 5;

    enum PaletteMode
    {
        NONE,
        GU_PSM_T4,
        GU_PSM_T8,

    } paletteMode : 2;

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

    uint8_t *
    GetTexel (int32_t n)
    {
        auto     out = data;
        uint32_t w   = stride;
        uint32_t lH  = logHeight;

        while (n--)
            {
                out += w * (1 << lH);
                lH--;

                if (0x10 < w)
                    w /= 2;
            }

        return out;
    }

    uint8_t
    GetPaletteIdx (size_t x, size_t y)
    {
        uint32_t w = GetWidth ();
        uint32_t h = GetHeight ();

        if (x >= w || y >= h)
            return 0;

        switch (paletteMode)
            {
            case NONE: return 0;
            case GU_PSM_T4: return data[y * w + x];
            case GU_PSM_T8: return data[(y * w + x) / 2] >> (4 * (x & 1)) & 0xf;
            }

        return 0;
    }

    uint16_t
    CalculatePaletteSize ()
    {
        switch (paletteMode)
            {
            case GU_PSM_T8: return 0x100;
            case GU_PSM_T4: return 0x10;
            default: return 0;
            }
    }

    uint32_t
    GetPixelColour (size_t x, size_t y)
    {
        if (depth <= 8)
            {
                uint8_t   idx     = GetPaletteIdx (x, y);
                uint32_t *palette = GetPalettePtr ();
                return palette ? palette[idx] : 0;
            }

        return 0;
    }

    uint32_t *
    GetPalettePtr ()
    {
        if (depth <= 8)
            return reinterpret_cast<uint32_t *> (GetTexel (mipmaps));

        return nullptr;
    }

  auto
  GetPalette ()
  {
    return std::span (GetPalettePtr (), CalculatePaletteSize ());
  }
};

static_assert (sizeof (RslRaster) == 16, "RslRaster size mismatch");

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
