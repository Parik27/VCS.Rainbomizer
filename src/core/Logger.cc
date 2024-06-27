#include "Logger.hh"
#include "Common.hh"
#include <cstdlib>
#include <ctime>
#include <utils/Utils.hh>

#ifndef NDEBUG
#define RAINBOMIZER_BUILD "Debug Build: " __DATE__ " " __TIME__
#else
#define RAINBOMIZER_BUILD "Release v1.0" __DATE__ " " __TIME__
#define RAINBOMIZER_BUILD_SHORT "Release v1.0"
#endif

namespace Rainbomizer {

/*******************************************************/
std::string
GetTimeNow ()
{
    time_t currentTime;
    char   str[256];

    sceKernelLibcTime (&currentTime);
    time (&currentTime);

    auto tm = std::localtime (&currentTime);
    sprintf (str, "%04d-%02d-%02d", 1900 + tm->tm_year, tm->tm_mon + 1,
             tm->tm_mday);

    return str;
}

/*******************************************************/
FILE *
Logger::GetLogFile ()
{
    if (!mFile)
        {
            mFile = Common::GetRainbomizerFile (GetTimeNow () + ".txt", "a+",
                                                "logs/", true)
                        .Release ();
            if (!mFile)
                {
                    mFile = stdout;
                }

            fprintf (mFile, "===========================================\n");
            fprintf (mFile, "%d\n", (int) sceKernelLibcTime (NULL));
            fprintf (mFile, "Rainbomizer VCS (PPSSPP) Build: %s \n", RAINBOMIZER_BUILD);
            fprintf (mFile, "===========================================\n");
        }

    return mFile;
}

/*******************************************************/
void
Logger::LogMessage (const char *format, ...)
{
    FILE *file = GetLogFile ();
    fprintf (file, "[%d | MEM = %u]: ", int (sceKernelLibcTime (NULL)), pspSdkTotalFreeUserMemSize ());

    va_list args;
    va_start (args, format);
    unsigned int size = vfprintf (file, format, args) + 1;
    va_end (args);

    fputc ('\n', file);
    fflush (file);
}
} // namespace Rainbomizer
