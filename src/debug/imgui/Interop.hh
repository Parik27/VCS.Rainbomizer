#pragma once

/* A file to call gu commands from pspsdk and have them work in game!
   Normally they wouldn't work because of separate draw lists, but this
   hack seems to work. What it does is, it sets the internal gu_list
   current pointer to the game's current command pointer. After executing
   the gu commands, it adjusts the game's current command pointer accordingly
   effectively injecting the gu commands in the game's draw list.
 */

#include "memory/GameAddress.hh"
#include <cstring>

typedef struct
{
    unsigned int *start;
    unsigned int *current;
    int           parent_context;
} GuDisplayList;

extern GuDisplayList *gu_list;

template <typename Functor>
inline void
GuCommandWrapper (Functor &&functor)
{
    static GuDisplayList                            sm_gu_list;
    static GameVariable<unsigned int *, 0x08bc7730> g_pCurrentDisplayList{};

    if (!gu_list)
        {
            gu_list = &sm_gu_list;
            memset (gu_list, 0, sizeof (GuDisplayList));
        }

    gu_list->current = g_pCurrentDisplayList;
    functor ();
    g_pCurrentDisplayList.Get () = gu_list->current;
}
