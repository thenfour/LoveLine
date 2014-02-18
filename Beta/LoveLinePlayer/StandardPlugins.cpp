

#include "StandardPlugins.h"
#include "P_Library.h"
#include "P_Episode.h"
#include "P_Playback.h"
#include "P_Links.h"
#include "P_Features.h"


StandardPlugins Plugins;


Plugin_Library  P_Library;
Plugin_Episode  P_Episode;
Plugin_Links    P_Links;
Plugin_Features P_Features;
Plugin_Playback P_Playback;


void CALLBACK LLPluginEnumerator(LL_PLUGIN_ENUM_PROC pProc, PVOID pPrivate)
{
    Plugins.pLibrary = &P_Library;
    Plugins.pEpisode = &P_Episode;
    Plugins.pLinks = &P_Links;
    Plugins.pFeatures = &P_Features;
    Plugins.pPlayback = &P_Playback;

    pProc(pPrivate, L"Library",  &GUID_Library,  Plugin_Library::NotificationProc,  &Plugins);
    pProc(pPrivate, L"Playback", &GUID_Playback, Plugin_Playback::NotificationProc, &Plugins);
    pProc(pPrivate, L"Episode",  &GUID_Episode,  Plugin_Episode::NotificationProc,  &Plugins);
    pProc(pPrivate, L"Features", &GUID_Features, Plugin_Features::NotificationProc, &Plugins);
    //pProc(pPrivate, L"Links",    &GUID_Links,    Plugin_Links::NotificationProc,    &Plugins);
}



