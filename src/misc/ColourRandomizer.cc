#include "core/Logger.hh"
#include "memory/GameAddress.hh"
#include "utils/ContainerUtils.hh"
#include "utils/Random.hh"
#include <cstdint>
#include <cstring>
#include <ctime>
#include <string>

#include <pspsysmem.h>

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
};

class ColourRandomizer : public Randomizer<ColourRandomizer>
{

  inline static uint32_t* addr = nullptr;

    template <auto &TextureDefaultFind>
    static RslTexture *
    TextureFun (const char *name)
    {
        auto tex = TextureDefaultFind (name);

        /* Randomizes the colours of the textures */
        static constexpr std::array randomizedTextures
            = {"hudnumbers", "bar_inside1", "bar_inside2", "ndc_redballoon"};

        static constexpr std::array randomizedColours
            = {0x206e20, 0x0000ff, 0xfaff09, 0x2c8cc1, 0x955ce4};

        if (DoesElementExist (randomizedTextures, std::string_view (name)))
            {
                for (uint32_t &col : tex->raster->GetPalette ())
                    {
                        if (DoesElementExist (randomizedColours,
                                              col & 0xFFFFFF))
                            col = (col & 0xFF000000) | RandomSize (0xFFFFFF);
                    }
            }

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
    HOOK (Jal, 0x8a65fe8, TextureFun, RslTexture*(const char*));
    HOOK (Jal, 0x08a53c74, RandomizeVehCols, void (void*, ColourTable*));
    //injector.MakeJMPwNOP (GameAddress<0x08a358a0>::Get(), uintptr_t(RandomizeColours));
  }
};
