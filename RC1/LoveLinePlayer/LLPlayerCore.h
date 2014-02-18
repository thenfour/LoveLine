/*
    The player core is where the work is actually done.
    LLPlayerApp handles the plugins for the player and the plugins
    control the UI.

    The Core handles the playback, database functions, etc...
    The core doesn't handle UI; it's just kind of an API or wrapper
    for database stuff, player stuff, etc...
*/


#pragma once


#include <Windows.h>
#include "..\\LoveLineUtil\\AudioPlayer.h"
#include "..\\LoveLineUtil\\LLDatabase.h"
#include "..\\LoveLineUtil\\CCCustomWnd.h"
#include "LLPlugin.h"
#include "LLPluginInformation.h"
#include "..\\LoveLineUtil\\CCLog.h"
#include "..\\LoveLineUtil\\Splash.h"


class LLPlayerCore
{
public:

    LLPlayerCore();
    ~LLPlayerCore();

    LLDatabase db;
    AudioPlayer Player;
    CCLog Log;

    BOOL IsCurrentEpisodeValid();

    HRESULT Initialize(class LLPlayerApp* pOwner, CCSplash* pSplash);// Opens the database, etc...
    HRESULT SetPlugins(LL_PLUGIN_INFORMATION* pPlugins);// Used to set the plguins.  This will allow the object to send notifications
    HRESULT Uninitialize();

    HRESULT SeekEpisode(GUID idEpisode, DWORD dwMS);
    HRESULT SeekCurrentEpisode(DWORD dwMS);

    HRESULT SendPluginMessage(DWORD dwMsg, PVOID pParam);

    HRESULT GetCurrentEpisode(GUID* p);
    DWORD GetLength();// Episode length

    CCString GetFullMediaPath(CCString sFileName);// Converts a file to a full path using the media path
    CCString GetMediaPath();
    CCString GetDatabasePath();
    CCString GetBinPath();
    CCString GetAppRoot();
    CCString GetDatabaseName();

private:

    CCCustomWnd MediaNotificationWindow;
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    CCString MediaPath;
    CCString DatabasePath;
    CCString BinPath;
    CCString AppRoot;
    CCString DatabaseName;


    // Position in the playback
    GUID  idCurrentEpisode;
    DWORD dwCurrentPos;// in milliseconds.

    DWORD dwLength;// for optimization

    class LLPlayerApp* pOwner;
    LL_PLUGIN_INFORMATION* pPlugins;// Our own SEPARATE copy of the linked list.
};



