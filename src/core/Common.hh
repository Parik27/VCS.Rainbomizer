#pragma once

#include "core/Logger.hh"
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

    // Reads lines from the file and calls the function with each line
    // Line is guaranteed to be null terminated
    template <typename F, int32_t BUFFER_SIZE = 512>
    void
    ReadLines (F &&f)
    {
        if (!file)
            return;

        char buffer[BUFFER_SIZE];
        while (fgets (buffer, sizeof (buffer), file))
            {
                size_t start = 0;
                size_t end   = 0;

                // Remove leading and trailing whitespace
                size_t i = 0;
                while (buffer[i] != '\0')
                    {
                        if (buffer[start] == ' ' || buffer[start] == '\t')
                            start++;

                        if (buffer[i] == '\n' || buffer[i] == '\r')
                            {
                                break;
                            }

                        if (buffer[i] != ' ' && buffer[i] != '\t')
                            end = i;

                        i++;
                    }

                buffer[end + 1] = '\0';
                f (std::string_view(buffer + start, end - start + 1));
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
