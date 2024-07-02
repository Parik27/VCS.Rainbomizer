#include "core/Logger.hh"
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

  public:
  ColourRandomizer ()
  {
    HOOK (Jal, 0x8a65fe8, TextureFun, RslTexture*(const char*));
  }
};
