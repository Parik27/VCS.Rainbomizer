#pragma once

#include <cstdio>
#include <cstring>
#include <string>
#include <cstdint>

namespace Rainbomizer {

class FileWrapper
{
    FILE *file;

public:
    explicit FileWrapper (FILE *file) : file (file) {}
    ~FileWrapper ()
    {
        if (file)
            {
                fclose (file);
                file = 0;
            }
    }

    operator FILE * () { return file; }
    FILE *
    operator->()
    {
        return file;
    }

    explicit operator bool () { return file; }

    FileWrapper (FileWrapper &&other) : file (other.file)
    {
        other.file = nullptr;
    }
    FileWrapper &
    operator= (FileWrapper &&other)
    {
        file       = other.file;
        other.file = nullptr;
        return *this;
    }
    FileWrapper (const FileWrapper &)            = delete;
    FileWrapper &operator= (const FileWrapper &) = delete;

    FILE *
    Release ()
    {
        FILE *f = file;
        file    = nullptr;
        return f;
    }

    template <typename F, int32_t BUFFER_SIZE = 512>
    void
    ReadLines (F &&f)
    {
        if (!file)
            return;

        char buffer[BUFFER_SIZE];
        while (fgets (buffer, sizeof (buffer), file))
            {
                buffer[strcspn (buffer, "\r")] = '\0';
                buffer[strcspn (buffer, "\n")] = '\0';
                f (buffer);
            }
    }
};

class Common
{
public:
    static std::string GetRainbomizerFileName
        [[nodiscard]] (const std::string &name, const std::string &subdirs = "",
                       bool temp = false);

    static FileWrapper GetRainbomizerFile
        [[nodiscard]] (const std::string &name, const std::string &mode,
                       const std::string &subdirs      = "",
                       bool               tempFallback = false);

    static FileWrapper GetRainbomizerDataFile
        [[nodiscard]] (const std::string &name, const std::string &mode = "r");
};
} // namespace Rainbomizer
