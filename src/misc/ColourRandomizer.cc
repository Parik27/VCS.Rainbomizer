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
        if (std::string_view (name) == "bar_inside1"
            || std::string_view (name) == "hudnumbers"
            || std::string_view (name) == "bar_inside2")
            {
              Rainbomizer::Logger::LogMessage("%s", name);
                for (int i = 0; i < tex->raster->CalculatePaletteSize (); i++)
                    {

                        uint32_t &col = tex->raster->GetPalette ()[i];
                        Rainbomizer::Logger::LogMessage("%08x", col);

                        if ((col & 0x00FFFFFF) == 0x206e20
                            || (col & 0x00FFFFFF) == 0xfaff09
                            || (col & 0x00FFFFFF) == 0x2c8cc1
                            || (col & 0x00FFFFFF) == 0x955ce4)
                            col = (col & 0xFF000000) | RandomSize (0xFFFFFF);
                    }
            }

        return tex;
    }

    static CRGBA *
    RandomizeColours (CRGBA *th, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        th->r = r;
        th->g = g;
        th->b = b;
        th->a = a;

        if (std::abs(r - g) + std::abs(r - b) > 10)
            {
                th->r = 255; // RandomSize (0xFF);
                th->g = 0;   // RandomSize (0xFF);
                th->b = 0;   // RandomSize (0xFF);
                //th->a = 0;
            }

        return th;
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
