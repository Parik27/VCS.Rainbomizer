/* Base class for all randomizers. Make sure the randomizer is inherited from
 * this class */
#include "memory/Memory.hh"
template <typename T> class Randomizer
{
    inline static T  sm_Instance;

public:
    Randomizer ()
    {
        sm_Instance;

        // Initialise memory manager
        MemoryManager::Get ().Init ();
    }

    static T &
    Get ()
    {
        return sm_Instance;
    }
};
