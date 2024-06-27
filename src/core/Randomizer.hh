/* Base class for all randomizers. Make sure the randomizer is inherited from
 * this class */
#include "memory/Memory.hh"
template <typename T> class Randomizer
{
    inline static T* instance;

public:
    Randomizer ()
    {
        instance = static_cast<T *> (this);

        // Initialise memory manager
        MemoryManager::Get ().Init ();
    }

    static T &
    Get ()
    {
        return *instance;
    }
};
