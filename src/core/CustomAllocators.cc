#include <cstdint>

void (*operator_delete) (void *);
void *(*operator_new) (int);

void *
operator new (std::size_t n)
{

    operator_delete = reinterpret_cast<void (*) (void *)> (0x8abe678);
    operator_new    = reinterpret_cast<void *(*) (int)> (0x8abe60c);

    return operator_new (n);
}

void
operator delete (void *p) noexcept
{
    operator_delete = reinterpret_cast<void (*) (void *)> (0x8abe678);
    operator_new    = reinterpret_cast<void *(*) (int)> (0x8abe60c);

    return operator_delete (p);
}

extern "C" void
__dso_handle ()
{
}
