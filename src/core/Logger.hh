#include "log.h"
#include <format>

template <typename... Args>
static void
LogMessage (std::format_string<Args...> fmt, Args &&...args)
{
    logger.WriteF ("%s\n",
                   std::format (fmt, std::forward<Args> (args)...).c_str ());
}
