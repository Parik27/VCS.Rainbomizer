#include "vcs/CRunningScript.hh"
#include <vector>

template <typename Class, typename... Args> class Event
{
    std::vector<void (*) (Args...)> m_Listeners;

public:
    void
    operator() (Args... args)
    {
        for (auto i : m_Listeners)
            i (args...);
    }

    static Class &
    Get ()
    {
        static Class m_Event;
        return m_Event;
    }

    static void
    Add (void (*ptr) (Args...))
    {
        Get ().m_Listeners.push_back (ptr);
    }
};

class FadeEvent : public Event<FadeEvent>
{
    template <auto &CCamera__ProcessFade>
    void ProcessFadeEvent (class CCamera *);

public:
    FadeEvent ();
};

class RandomizationSeedEvent : public Event<RandomizationSeedEvent, int>
{
    template <auto &CRunningScript__Process>
    void ProcessRandomizationSeedEvent (CRunningScript* scr);

public:
    RandomizationSeedEvent ();
};

class KeyEvent : public Event<KeyEvent>
{
    bool m_KeyState[200] = {};

public:
    void Process ();
};
