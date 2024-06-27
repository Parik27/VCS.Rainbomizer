#include <cstdint>
#include <ctime>
#include <string>

#include <pspsysmem.h>

#include <hooks/Hooks.hh>
#include <core/Common.hh>

struct CRGBA
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

class ColourRandomizer
{
  template<auto &CRGBA__CRGBA>
  static CRGBA* RandomizeColour (CRGBA* rgba, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
  {

    uint32_t memLeft = sceKernelMaxFreeMemSize();

      // if (memLeft < 0x10000)
      // 	{
      // 	  rgba->r = 255;
      // 	  rgba->g = 0;
      // 	  rgba->b = 0;
      // 	  rgba->a = a;
      // 	}
      // else if (memLeft < 0x20000) {
      // 	rgba->r = 255;
      // 	rgba->g = 255;
      // 	rgba->b = 0;
      // 	rgba->a = a;
      // }
      // else {
      // 	rgba->r = 0;
      // 	rgba->g = 255;
      // 	rgba->b = 0;
      // 	rgba->a = a;
      // }

    rgba->r = r;
    rgba->b = b;
    rgba->g = g;
    rgba->a = a;

    if (CRGBA__CRGBA == 0x0)
      {
	rgba->r = 0;
	rgba->g = 0;
	rgba->b = 255;
	rgba->a = a;
      }
    else if (uintptr_t(CRGBA__CRGBA) != 0x08a358a0)
      {
	rgba->r = 0;
	rgba->g = 255;
	rgba->b = 255;
	rgba->a = a;
      }

    else if (*(int*) 0x08a358a0 == 0 || *(int*) 0x08a358a0 == 0)
      {
	rgba->r = 255;
	rgba->g = 0;
	rgba->b = 0;
	rgba->a = a;
      }
    else
      {
	rgba->r = 0;
	rgba->g = 255;
	rgba->b = 0;
	rgba->a = a;
      }


      return rgba;
  }

  /*******************************************************/
static std::string
GetTimeNow ()
{
    static time_t currentTime;
    static char   str[256];

    time (&currentTime);

    auto tm = std::localtime (&currentTime);
    sprintf (str, "%04d-%02d-%02d", 1900 + tm->tm_year, tm->tm_mon + 1,
             tm->tm_mday);

    return str;
}


  template<auto& CText__Get>
  static char*
  RandomizeText (void* p1, char* p2)
  {
    static char buffer[512] = {};
    static char buffer2[1024] = {};
    static FILE* f = fopen(Rainbomizer::Common::GetRainbomizerFileName("1970-01-01.txt", "logs/").c_str(), "a+");
    fprintf(f, "Hello Fry");
    sprintf(buffer, "%p", f);
    for (size_t i = 0; i < 1024; i++) {
      if (i % 2 == 1)
	buffer2[i] = 0;
      else
	buffer2[i] = buffer[i/2];
    }
    return buffer2;
  }
  
  public:
  ColourRandomizer ()
  {
      // HOOK (Jmp, (0x08a358a0), RandomizeColour,
      //       CRGBA * (CRGBA * rgba, uint8_t r, uint8_t g, uint8_t b, uint8_t a))

    //HOOK (Jmp, (0x089f6724), RandomizeText, char* (void*, char*));
  }
};
