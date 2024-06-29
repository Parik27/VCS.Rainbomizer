#pragma once

#include <chrono>
#include <cstdio>
#include <source_location>
#include <utility>

namespace Rainbomizer {

class Logger
{
    static inline FILE *mFile = nullptr;
    static FILE        *GetLogFile ();

public:
    static void LogMessage (const char *format, ...)
        __attribute__ ((format (printf, 1, 2)));
    ;

    static void
    LogLocation (const std::source_location &location
                 = std::source_location::current ())
    {
#ifdef EXTREME_LOGGING
        LogMessage ("%s:%d %s", location.file_name (), location.line (),
                    location.function_name ());
        fclose(mFile);
        mFile = nullptr;
#endif
    }

    template<typename ... T>
    static void
    LogCritical (T &&...args)
    {
        LogMessage (std::forward<T> (args)...);
        fclose(mFile);
        mFile = nullptr;
    }

    class FunctionBenchmark
    {
        std::source_location location;
        std::chrono::time_point<std::chrono::high_resolution_clock> start;

    public:
        FunctionBenchmark (const std::source_location &location
                           = std::source_location::current ())
            : location (location), start (std::chrono::high_resolution_clock::now ())
        {
        }

        ~FunctionBenchmark ()
        {
            auto end = std::chrono::high_resolution_clock::now ();
            auto duration
                = std::chrono::duration_cast<std::chrono::microseconds> (
                    end - start);

            LogMessage ("%s:%d %s took %ld microseconds", location.file_name (),
                        location.line (), location.function_name (),
                        duration.count ());
        }
    };
};

} // namespace Rainbomizer
