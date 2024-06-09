#pragma once

#include <istream>
#include <string>
#include <log.h>

#include <pspsdk.h>
#include <pspkernel.h>

class PSPfilebuf : public std::streambuf
{
private:
    SceUID fd;
    enum
    {
        bufferSize = 1024
    };
    char buffer[bufferSize];

protected:
    // Override underflow to refill the buffer
    virtual int_type
    underflow () override
    {
        if (gptr () == egptr ())
            {
                int bytesRead = sceIoRead (fd, buffer, bufferSize);
                if (bytesRead <= 0)
                    {
                        return traits_type::eof ();
                    }
                setg (buffer, buffer, buffer + bytesRead);
            }
        return traits_type::to_int_type (*gptr ());
    }

    // Override overflow to flush the buffer
    virtual int_type
    overflow (int_type ch) override
    {
        if (ch != traits_type::eof ())
            {
                *pptr () = ch;
                pbump (1);
            }
        if (flushBuffer () == traits_type::eof ())
            {
                return traits_type::eof ();
            }
        return ch;
    }

    // Flush the buffer
    virtual int
    flushBuffer ()
    {
        int bytesToWrite = pptr () - pbase ();
        if (sceIoWrite (fd, buffer, bytesToWrite) != bytesToWrite)
            {
                return traits_type::eof ();
            }
        setp (buffer, buffer + bufferSize);
        return 0;
    }

public:
    PSPfilebuf (const std::string &filename, int flags = PSP_O_RDONLY,
                SceMode mode = 0777)
        : fd (sceIoOpen (filename.c_str (), flags, mode))
    {
        setg (buffer, buffer, buffer); // Initially set buffer pointers to indicate
        //empty buffer
        setp (buffer, buffer + bufferSize);
    }

    bool
    is_bad ()
    {
        return fd < 0;
    }

    ~PSPfilebuf ()
    {
        flushBuffer ();
        sceIoClose (fd);
    }

    virtual pos_type
    seekoff (off_type off, std::ios_base::seekdir way,
             std::ios_base::openmode which
             = std::ios_base::in | std::ios_base::out) override
    {
        if (fd < 0)
            return pos_type(off_type(-1));

        auto whence = SEEK_SET;
        switch (way)
            {
                case std::ios_base::beg:
                whence = PSP_SEEK_SET;
                break;

            case std::ios_base::cur:
                whence = PSP_SEEK_CUR;
                break;

            case std::ios_base::end:
                whence = PSP_SEEK_END;
                break;
            default:
                return pos_type(off_type(-1));
            }

        int newPos = seek(off, whence);
        setg (0, 0, 0);
        return pos_type(newPos);
    }

    virtual pos_type seekpos(pos_type pos,
                             std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) override
    {
        logger.WriteF("\nseekpos\n");
        return seekoff(off_type(pos), std::ios_base::beg, which); // Treat as seek from beginning
    }

private:
    // Seek to a position in the file
    int seek(int offset, int whence = PSP_SEEK_SET)
    {
        logger.WriteF("\nSEEK: %d %d (%d %d %d)\n", offset, whence, PSP_SEEK_SET, PSP_SEEK_CUR, PSP_SEEK_END);
        return sceIoLseek(fd, offset, whence);
    }
};

class PSPifstream : public std::istream
{
private:
    PSPfilebuf filebuf;

public:
    PSPifstream (const std::string &filename, int flags = PSP_O_RDONLY,
                 SceMode mode = 0777)
        : std::istream (&filebuf), filebuf (filename, flags, mode)
    {
        if (filebuf.is_bad ())
            setstate (std::ios_base::badbit);
    }
};

class PSPofstream : public std::ostream
{
private:
    PSPfilebuf filebuf;

public:
    PSPofstream (const std::string &filename,
                 int     flags = PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC,
                 SceMode mode  = 0777)
        : std::ostream (&filebuf), filebuf (filename, flags, mode)
    {
        if (filebuf.is_bad ())
            setstate (std::ios_base::badbit);
    }
};
