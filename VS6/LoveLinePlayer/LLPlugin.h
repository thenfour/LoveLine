/*
  Abstract:

    Contains the standard definitions for LoveLine Plugins.
    All plugins for LL should include this header file.

  Notes:

  History:

    11/04/2001  carlco      Created

*/


#pragma once


#include <windows.h>


/*
    LL_ATTACH
    
    This is sent to the plugin when it is supposed to create itself
    and report back its window handle and other information.  Because
    this is kinda like a IClassFactory::CreateInstance() call, we don't
    yet know much about the plugin.

    The LPARAM is a pointer to a LL_ATTACH_STRUCT structure
    where the plugin will return the hWnd, and the LPARAM to use for the 
    rest of the messages.

    The pUserData member of LL_ATTACH_STRUCT will be what the enumeration
    function specifies.

    It is *NOT* SAFE for the plugin to send commands to the core or anything
    during this message.  It should be assumed that nothing has started up
    yet and that the plugin should only get itself ready for action here - 
    not interfacing with other stuff.

    (LL_ATTACH_STRUCT*)lParam;
*/
#define LL_ATTACH             0x00000001


/*
    LL_DETACH

    This is sent when the plugin is to be destroyed.  This happens
    right before the application window is destroyed.
*/
#define LL_DETACH             0x00000002


/*
    LL_INITIALIZE
    This is sent second to the LL_ATTACH message.  Here is where
    the plugin can interact with the database, player, etc...
    The plugin can assume that everything is initialized at this
    point.
*/
#define LL_INITIALIZE         0x0000000B


/*
    LL_SETFOCUS
    Sent after the plugin's container is made visible.
*/
//#define LL_SETFOCUS           0x0000000C


/*
    LL_KILLFOCUS
    Sent when the user has selected a different plugin.  This is
    sent AFTER the plugin is made invisible.
*/
#define LL_KILLFOCUS          0x00000009


/*
    LL_SETFOCUS
    Sent when the plugin has been selected by the user.  This is
    sent right BEFORE it is made visible.
*/
#define LL_SETFOCUS           0x0000000A


/*
    This is sent when the application is requesting that the plguin
    refresh its display.
*/
#define LL_REFRESH            0x00000003


/*
    This is sent when the current episode has completed playing.
    (LL_SEEKEPISODE_STRUCT*) lParam;// the id of the stopped episode.

    The current episode MAY be different by the time the plugin receives this
    message.
*/
#define LL_COMPLETED          0x0000000D
#define LL_COMPLETE LL_COMPLETED

/*
    This is sent when playback was stopped and it has started.
*/
#define LL_PLAY               0x00000007


/*
    This is sent after playback stops.
*/
#define LL_STOP               0x00000008


/*
    This is sent when the application is seeking to a new episode.
    This is always followed by a LL_SEEKCURRENT message to indicate
    where in the episode it will seek to.

    (LL_SEEKEPISODE*)lParam;
*/
#define LL_SEEKEPISODE        0x00000004


/*
    This is sent when the application is seeking within the current
    episode.  This only happens when the USER specifically seeks
    to a new position, or any time the current episode changes.

    (LL_SEEKCURRENT*)lParam;
*/
#define LL_SEEKINGCURRENT     0x0000000E


/*
    This is sent when the application is seeking within the current
    episode.  This only happens when the USER specifically seeks
    to a new position, or any time the current episode changes.

    (LL_SEEKCURRENT*)lParam;
*/
#define LL_SEEKCURRENT        0x00000005


/*
    This is sent when the LoveLine database has changed and the
    display may need to be updated.  The parameter will give information
    on what in the database changed.

    (LL_DBUPDATE*)lParam;
*/
#define LL_DBUPDATE           0x00000006


// Next: 0x0000000F


struct LL_ATTACH_STRUCT
{
    IN     class LLPlayerCore* pCore;// This is passed in so the plugin can control playback.
    OUT    HWND                hWnd;// Specify the handle to the plugin's window.
    IN OUT PVOID               pUserData;// This is the user data that will be used from now on.
};


struct LL_SEEKEPISODE_STRUCT
{
    GUID guidEpisode;
};


struct LL_SEEKCURRENT_STRUCT
{
    DWORD dwMS;
};


struct LL_DBUPDATE_STRUCT
{
    DWORD dwOperation;//DBUPDATE_NEW, _CHANGED, or _REMOVE
    DWORD dwType;//DBUPDATE_COMMERCIAL, _FEATURE, _EPISODE, _LINK
};

#define DBUPDATE_NEW       0x00000001
#define DBUPDATE_CHANGED   0x00000002
#define DBUPDATE_REMOVE    0x00000004

#define DBUPDATE_ADD DBUPDATE_NEW
#define DBUPDATE_CHANGE DBUPDATE_CHANGED
#define DBUPDATE_REMOVED DBUPDATE_REMOVE

#define DBUPDATE_SEGMENT    0x00000010
#define DBUPDATE_COMMERCIAL 0x00000001
#define DBUPDATE_FEATURE    0x00000002
#define DBUPDATE_EPISODE    0x00000004
#define DBUPDATE_LINK       0x00000008

//
// This is the typedef for the notification function that tlist receives
// From the plugins.
//
// Example:
// void CALLBACK LLNotification(PVOID pUser, DWORD dwMsg, PVOID pParams);
//
typedef void (CALLBACK* LL_PLUGIN_NOTIFICATION)
    (PVOID pUser, DWORD dwMsg, PVOID pParams);


//
// This typedef is for the callback that the plugin's enum function will
// call for each plugin in the DLL.
//
// Example:
// void CALLBACK LLPluginEnumProc(
//     PVOID pPrivate, PCWSTR wszTitle, const GUID* pGuid,
//     LL_PLUGIN_NOTIFICATION pNotification, PVOID pUser);
//
typedef void (CALLBACK* LL_PLUGIN_ENUM_PROC)
    (PVOID pPrivate, PCWSTR wszTitle, const GUID* pGuid, 
    LL_PLUGIN_NOTIFICATION pNotification, PVOID pUser);

//
// The plugin will export "LLPluginEnumerator" which is of this type.
//
// Example:
// void CALLBACK LLPluginEnumerator(LL_PLUGIN_ENUM_PROC pProc, PVOID pPrivate)
//
typedef void (CALLBACK* LL_PLUGIN_ENUMERATOR)
    (LL_PLUGIN_ENUM_PROC pProc, PVOID pPrivate);


