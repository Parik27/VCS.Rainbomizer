/* Base class for all randomizers. Make sure the randomizer is inherited from
 * this class */
#include "memory/Memory.hh"

#ifdef ENABLE_DEBUG_MENU
#include <debug/base.hh>
#include <imgui.h>
#endif

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

template <typename T> class RandomizerWithDebugInterface : public Randomizer<T>
{
public:
#ifdef ENABLE_DEBUG_MENU
    class RandomizerDebugInterface final : public DebugInterface
    {
    public:
        virtual const char *
        GetName () const override
        {
            return typeid (T).name ();
        }

        virtual void
        Draw () override
        {
            T::Get ().DrawDebug ();
        }
    } sm_DebugInterface;

    void DrawDebug ()
    {
        ImGui::Text ("No debug information available");
    }
#endif
};
