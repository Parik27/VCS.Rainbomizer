#include "Common.hh"
#include "Logger.hh"

#include <cstring>
#include <filesystem>

namespace Rainbomizer {

/*******************************************************/
std::string
Common::GetRainbomizerFileName (const std::string &name,
                                const std::string &subdirs, bool temp)
{
    std::string baseDir = "ms0:/PSP/PLUGINS/VCS.PPSSPP.Rainbomizer/" + subdirs;

    std::error_code ec;
    std::filesystem::create_directories (baseDir, ec);

    return baseDir + name;
}

/*******************************************************/
FileWrapper
Common::GetRainbomizerFile (const std::string &name, const std::string &mode,
                            const std::string &subdirs, bool tempFallback)
{
    FILE *file = fopen (GetRainbomizerFileName (name, subdirs).c_str (),
                        mode.c_str ());

    return std::move(FileWrapper{file});
}

/*******************************************************/
FileWrapper
Common::GetRainbomizerDataFile (const std::string &name,
                                const std::string &mode)
{
    FILE *f = GetRainbomizerFile (name, mode, "data/").Release ();

    if (!f)
        Logger::LogMessage ("Failed to read Rainbomizer data file: data/%s: %s",
                            name.c_str (), std::strerror (errno));

    return std::move(FileWrapper{f});
}
} // namespace Rainbomizer
