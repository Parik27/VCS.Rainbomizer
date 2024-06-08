/* Base class for all randomizers. Make sure the randomizer is inherited from
 * this class */
template <typename T> class Randomizer
{
public:
    static T &
    Get ()
    {
        static T instance;
        return instance;
    }
};

#define CREATE_RANDOMIZER_WRAPPER(className)                                   \
  extern class className *g_##className;					\
  void Initialise##className ();

// Put this in your randomizer class to make sure it can be initialised
#define DEFINE_RANDOMIZER_WRAPPER(className)                                   \
    class className *g_##className;                                            \
    CREATE_RANDOMIZER_WRAPPER (className)                                      \
    void Initialise##className ()                                              \
    {                                                                          \
        if (!g_##className)                                                    \
            g_##className = new className ();                                  \
    }

#define CREATE_GET_FUNCTION(className)                                         \
    static className &Get () { return *g_##className; }

// Put this in main.cc
#define INITIALISE_RANDOMIZER(className)                                       \
    CREATE_RANDOMIZER_WRAPPER (className) Initialise##className ();

#define BEGIN_RANDOMIZER(className)                                            \
    CREATE_RANDOMIZER_WRAPPER (className);                                     \
    class className                                                            \
    {                                                                          \
    public:                                                                    \
        CREATE_GET_FUNCTION (className);                                       \
                                                                               \
    private:

#define END_RANDOMIZER(className)                                              \
    }                                                                          \
    ;                                                                          \
    DEFINE_RANDOMIZER_WRAPPER (className);
