/* Base class for all randomizers. Make sure the randomizer is inherited from
 * this class */
template <typename T> class Randomizer
{
    inline static T* instance;

public:
    Randomizer () { instance = static_cast<T *> (this); }

    static T &
    Get ()
    {
        return *instance;
    }
};
