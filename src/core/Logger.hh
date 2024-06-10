#pragma once

#include <cstdio>

namespace Rainbomizer {

class Logger
{
    static inline FILE *mFile = nullptr;
    static FILE        *GetLogFile ();

public:
    static void LogMessage (const char *format, ...)
        __attribute__ ((format (printf, 1, 2)));
    ;
};

} // namespace Rainbomizer
