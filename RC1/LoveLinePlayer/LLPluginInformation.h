

#pragma once


#include <Windows.h>
#include "..\\LoveLineUtil\\PluginContainer.h"


class LL_PLUGIN_INFORMATION
{
public:
    LL_PLUGIN_INFORMATION()
    {
        pUser = 0; pNotify = 0; pNext = 0; pPrevious = 0; pOwner = 0;
        bEnabled = FALSE; pUserAttach = 0;
    }

    CCString                Name;// The name that goes on the tab.

    BOOL                   bEnabled;// Whether it's visible or not.
    class LLPlayerApp*     pOwner;
    GUID                   id;

    // specified to the enum function, this gets put in the
    // LL_ATTACH_STRUCT struct and nowhere after that unless it's removed
    // and re-added.
    PVOID                  pUserAttach;

    // This is the one that's specified after the attach message
    PVOID                  pUser;
    LL_PLUGIN_NOTIFICATION pNotify;// The notification function

    LL_PLUGIN_INFORMATION* pNext;
    LL_PLUGIN_INFORMATION* pPrevious;

    PluginContainer        Container;
};

