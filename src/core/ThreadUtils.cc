#include "ThreadUtils.hh"
#include <utility>
#include <hooks/Hooks.hh>

template <auto &CRunningScript__CollectParams>
int
ThreadUtils::StoreCurrentMission (class CRunningScript *scr, int *p2, int argc,
                                  int *argv)
{
    int ret          = CRunningScript__CollectParams (scr, p2, argc, argv);
    currentMissionId = argv[0];
    return ret;
}

void
ThreadUtils::Initialise ()
{
    if (std::exchange (initialised, true))
        return;

    HOOK (Jal, 0x08abc41c, StoreCurrentMission,
          uint32_t (class CRunningScript *, int *, int, int *));
}
