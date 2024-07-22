#include "core/Logger.hh"
#include "memory/GameAddress.hh"
#include "utils/ContainerUtils.hh"
#include "utils/Random.hh"
#include "vcs/CStreaming.hh"
#include "vcs/CModelInfo.hh"
#include "vcs/CVehicle.hh"
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <map>
#include <string>

#include <pspsysmem.h>
#include <core/Config.hh>

#include <hooks/Hooks.hh>
#include <core/Common.hh>
#include <core/Randomizer.hh>

#include <vcs/RslTexture.hh>

struct CRGBA
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  void __attribute__((noinline))
  MakeGrey ()
  {
      double r = this->r / 255.;
      double g = this->g / 255.;
      double b = this->b / 255.;

      double c_lin  = 0.2126 * r + 0.7152 * g + 0.0722 * b;
      double c_srgb = c_lin <= 0.0031308
                          ? 12.92 * c_lin
                          : 1.055 * std::pow (c_lin, 1.0 / 2.4) - 0.055;

      this->r = this->g = this->b = static_cast<uint8_t> (c_srgb * 255);
  }
};

class ColourRandomizer : public Randomizer<ColourRandomizer>
{
    enum ColourizerFlags
    {
        REPLACE_COLOUR = 1, // replace the colour instead of modulating it

        NO_SINGLE_COLOUR = 1 << 1, // don't use a single colour for replacing
                                   // all colours in the list

        REPLACE_ALL_COLORS = 1 << 2, // replace all colours in the palette
    };

    template <size_t N> struct TextureColourizer
    {
        const char             *textureName;
        std::array<uint32_t, N> colours;
        ColourizerFlags         flags;
    };

    template <size_t N, typename... Args>
    TextureColourizer (const char *textureName, const uint32_t (&colours)[N],
                       Args... args) -> TextureColourizer<N>;

    // clang-format off
    inline static constexpr std::tuple colourizers{
        TextureColourizer{"hudnumbers", {0x206e20}},
        TextureColourizer{"ndc_redballoon", {0xff0000}},
        TextureColourizer{"bar_inside1", {0x09fffa}},
        TextureColourizer{"bar_inside2", {0xe45c95}},
        TextureColourizer{"starflame",
                          {0x145ED3,0x11479B,0x0B2B62,0x061A39,0x070A0F,}},

        TextureColourizer{"vcs_bmxboybit",
            {0x747577,0x434448,0x27262B,0x161416,0x060608,0xCDCDCD,0xB5377F,
             0x6D0439,0x3D0019,0x050406,}},

        TextureColourizer{"cabbie8bit",
            {0xC49C0A,0x8E6501,0x734103,}},

        TextureColourizer{"patstar",
            {0xCAC9B7,}},

        TextureColourizer{"pimpbit",
            {0xFAFAFA,0xFAFAFA,0xA7A7A9,0x858585,0x6C6C6E,0x555556,0x3E3E3F,}},

        TextureColourizer{"armytruk8bit128",
            {0x595652,0x595652,0x383832,0x0B0B0A,0x251F15,0x989493,0x1A1A16,
             0x1F211E,0x040404,0x76736F,0x151612,0x2A2B25,}},

        TextureColourizer{"hunterbody8bit256a",
            {0x13160E,0x272D1D,0x272D1D,0x0A0C08,0x1B2014,0x323927,0x000000,
             0x202618,0x454C37,0x060705,0x0F110B,0x181B12,0x020302,}},

        TextureColourizer{"marquis86body",
            {0xFDFDFD,0x7D827F,0xB9BEB9,0x959898,0xD1D7D1,0x484A4C,0x303241,
             0x666566,0xEAEEEA,}},

        TextureColourizer{"rhino868bit128",
            {0x9B5B38,0x10110A,0x181A18,0x000000,0x323124,0x0B0B0A,0x14150E,
             0x25261A,0x050604,0x1E1A0F,0x514935,0x0C0E08,0x1A1F14,0x080906,
             0x020302,}},

        TextureColourizer{"rhinocam868bit128",
            {0x5B5231,0x15130B,0x232412,0x050803,0x181C0E,0x000000,0x2C2E1A,
             0x0B0F07,0x14180C,0x010200,0x3E3C23,0x1B2211,0x0E1208,0x090C05,
             0x272916,0x10140A,}},

        TextureColourizer{"rio86body8bit128a",
            {0xF9F9F9,0x858585,0x3C3C3C,0xC2C2C2,0x1A1A1A,0x626262,0xE0E0E0,
             0xA5A5A5,0x6E6E6E,0x000000,0xAFAFAF,0xEDEDED,0xD3D3D3,0x555555,
             0x2F2F2F,0x9B9B9B,}},

        TextureColourizer{"rio86body8bit128b",
            {0xF7F7F7,0x707070,0x313131,0xA5A5A5,0x101010,0x525252,0xC2C2C2,
             0x242424,0xE1E1E1,0x414141,0x878787,0x000000,0x616161,0x1A1A1A,
             0xEEEEEE,0x060606,}},

        TextureColourizer{"sparrowbody128a",
                          {0xC4C4C4, 0x3E3E3E, 0x767676, 0x171717, 0x5D5D5D,
                           0x949494, 0x2F2F2F, 0x040404, 0x4E4E4E, 0x6E6E6E,
                           0x232323, 0x0E0E0E, 0x000000, 0x888888, 0xB3B3B3,
                           0x666666}},
    };

    // clang-format on
    inline static uint32_t *addr = nullptr;

    template <size_t I, typename T>
    static void
    ProcessColourizer (std::string_view name, RslTexture *tex, T palette)
    {
        static constexpr auto &colourizer = std::get<I> (colourizers);

        if (name != colourizer.textureName)
            return;

        auto &colours   = colourizer.colours;
        CRGBA newColour = std::bit_cast<CRGBA> (RandomSize (0xFFFFFFFF));

        for (uint32_t &col : palette)
            {
                const uint32_t rgb     = std::byteswap (col) >> 8;
                CRGBA          colRGBA = std::bit_cast<CRGBA> (col);

                if (!(colourizer.flags & ColourizerFlags::REPLACE_ALL_COLORS)
                    && !DoesElementExist (colourizer.colours, rgb))
                    continue;

                // If we are replacing colour, set colour to white
                if constexpr (colourizer.flags
                              & ColourizerFlags::REPLACE_COLOUR)
                    {
                        colRGBA.r = 255;
                        colRGBA.g = 255;
                        colRGBA.b = 255;
                    }

                // otherwise we convert the colour to greyscale and then
                // multiply
                else
                    colRGBA.MakeGrey ();

                colRGBA.r = ((colRGBA.r * newColour.r) / 255);
                colRGBA.g = ((colRGBA.g * newColour.g) / 255);
                colRGBA.b = ((colRGBA.b * newColour.b) / 255);

                col = std::bit_cast<uint32_t> (colRGBA);

                // Same colour vs different colour
                if constexpr (colourizer.flags
                              & ColourizerFlags::NO_SINGLE_COLOUR)
                    newColour = std::bit_cast<CRGBA> (RandomSize (0xFFFFFFFF));
            }
    }

    template <auto &TextureDefaultFind>
    static RslTexture *
    TextureFun (const char *name)
    {
        auto tex = TextureDefaultFind (name);

        if (!tex || !tex->raster)
            return tex;

        auto palette = tex->raster->GetPalette ();

        [palette, tex, name]<std::size_t... I> (std::index_sequence<I...>) {
            (..., (ProcessColourizer<I> (name, tex, palette)));
        }(std::make_index_sequence<
            std::tuple_size_v<decltype (colourizers)>>{});

        return tex;
    }

  struct ColourTable
  {
    struct RGBA {
      uint8_t r;
      uint8_t g;
      uint8_t b;
    } pedCols[128];
    RGBA vehCols[128];
  };

  template <auto &ColourTable__Load>
  static void
  RandomizeVehCols (void *p1, ColourTable *p2)
  {
      ColourTable__Load (p1, p2);
      for (auto &i : p2->pedCols)
          {
              i.r = RandomSize (0xFF);
              i.g = RandomSize (0xFF);
              i.b = RandomSize (0xFF);
          }

      for (auto &i : p2->vehCols)
          {
              i.r = RandomSize (0xFF);
              i.g = RandomSize (0xFF);
              i.b = RandomSize (0xFF);
          }
  }

  public:
  ColourRandomizer ()
  {
    bool EnableTextureFun = false;
    bool RandomizeCarAndPedCols = true;

    RB_C_DO_CONFIG ("ColourRandomizer", EnableTextureFun,
                    RandomizeCarAndPedCols);

    if (EnableTextureFun)
      HOOK (Jal, 0x8a65fe8, TextureFun, RslTexture*(const char*));

    if (RandomizeCarAndPedCols)
      HOOK (Jal, 0x08a53c74, RandomizeVehCols, void (void*, ColourTable*));
  }
};
