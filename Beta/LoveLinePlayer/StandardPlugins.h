/*
    This defines all the stuff needed for the standard plugins
    to LoveLine Player such as the episode page, the playback
    page, etc...
*/


#pragma once


#include <Windows.h>
#include "LLPlugin.h"


void CALLBACK LLPluginEnumerator(LL_PLUGIN_ENUM_PROC pProc, PVOID pPrivate);


struct StandardPlugins
{
    class Plugin_Library*   pLibrary;
    class Plugin_Episode*   pEpisode;
    class Plugin_Links*     pLinks;
    class Plugin_Features*  pFeatures;
    class Plugin_Playback*  pPlayback;
};


// {C64010E5-C920-491f-BA31-0A075AA1A307}
static const GUID GUID_Episode = 
{ 0xc64010e5, 0xc920, 0x491f, { 0xba, 0x31, 0xa, 0x7, 0x5a, 0xa1, 0xa3, 0x7 } };

// {929622CE-A542-42b0-B58B-1001AC48B636}
static const GUID GUID_Links = 
{ 0x929622ce, 0xa542, 0x42b0, { 0xb5, 0x8b, 0x10, 0x1, 0xac, 0x48, 0xb6, 0x36 } };

// {1842025F-80DD-4929-AED3-1CED9DB2894D}
static const GUID GUID_Library = 
{ 0x1842025f, 0x80dd, 0x4929, { 0xae, 0xd3, 0x1c, 0xed, 0x9d, 0xb2, 0x89, 0x4d } };

// {F783CD7A-9925-417e-94F6-2D904C33DCAE}
static const GUID GUID_Features = 
{ 0xf783cd7a, 0x9925, 0x417e, { 0x94, 0xf6, 0x2d, 0x90, 0x4c, 0x33, 0xdc, 0xae } };

// {30E9E517-77D3-4bad-B96C-DE76D1DCD75F}
static const GUID GUID_Playback = 
{ 0x30e9e517, 0x77d3, 0x4bad, { 0xb9, 0x6c, 0xde, 0x76, 0xd1, 0xdc, 0xd7, 0x5f } };

// {5DB68897-12A9-49c0-83FA-34CB1FB8F342}
static const GUID GUID_Log = 
{ 0x5db68897, 0x12a9, 0x49c0, { 0x83, 0xfa, 0x34, 0xcb, 0x1f, 0xb8, 0xf3, 0x42 } };

// {69D1B97B-F90C-4296-96D6-4C426654F96B}
static const GUID GUID_WaveUtils = 
{ 0x69d1b97b, 0xf90c, 0x4296, { 0x96, 0xd6, 0x4c, 0x42, 0x66, 0x54, 0xf9, 0x6b } };
