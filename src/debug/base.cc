#include "base.hh"

DebugInterface::DebugInterface ()
{
    DebugInterfaceManager::RegisterInterface (*this);
}
DebugInterfaceManager::DebugInterfaceManager ()
{

}

void
DebugInterfaceManager::RunThreadHook ()
{
    for (auto i : GetDebugInterfaces ())
        {
            i->Process ();
        }
}

DebugInterfaceManager g_DebugInterfaceManager;
