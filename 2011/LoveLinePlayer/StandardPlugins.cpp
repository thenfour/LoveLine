

#include "stdafx.h"
#include "StandardPlugins.h"
#include "P_Library.h"
#include "P_Episode.h"
#include "P_Playback.h"
#include "P_Features.h"
#include "P_Log.h"
#include "P_WavUtils.h"
#include "P_Commercial.h"


StandardPlugins Plugins;


Plugin_Library     P_Library;
Plugin_Commercial  P_Commercial;
Plugin_WavUtils    P_WavUtils;
Plugin_Log         P_Log;
Plugin_Episode     P_Episode;
Plugin_Features    P_Features;
Plugin_Playback    P_Playback;


void CALLBACK LLPluginEnumerator(LL_PLUGIN_ENUM_PROC pProc, PVOID pPrivate)
{
    Plugins.pLibrary = &P_Library;
    Plugins.pEpisode = &P_Episode;
    Plugins.pFeatures = &P_Features;
    Plugins.pPlayback = &P_Playback;
    Plugins.pCommercial = &P_Commercial;
    Plugins.pWavUtils = &P_WavUtils;
    Plugins.pLog = &P_Log;

    pProc(pPrivate, L"Library",  &GUID_Library,  Plugin_Library::NotificationProc,  &Plugins);
    pProc(pPrivate, L"Playback", &GUID_Playback, Plugin_Playback::NotificationProc, &Plugins);
    pProc(pPrivate, L"Episode",  &GUID_Episode,  Plugin_Episode::NotificationProc,  &Plugins);
    pProc(pPrivate, L"Features", &GUID_Features, Plugin_Features::NotificationProc, &Plugins);
    pProc(pPrivate, L"Commercial Editor", &GUID_Commercial, Plugin_Commercial::NotificationProc, &Plugins);
    pProc(pPrivate, L"Wave Utils", &GUID_WavUtils, Plugin_WavUtils::NotificationProc, &Plugins);
    pProc(pPrivate, L"Error Log", &GUID_Log, Plugin_Log::NotificationProc, &Plugins);
}



