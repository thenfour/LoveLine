

#pragma once


#include <Windows.h>
#include "..\\LoveLineUtil\\WaveCapture.h"
#include "..\\LoveLineUtil\\WAVWriter.h"
#include "..\\LoveLineUtil\\WAVReader.h"
#include "..\\LoveLineUtil\\WMAWriter.h"
#include "..\\LoveLineUtil\\MP3Writer.h"
#include "..\\LoveLineUtil\\CCString.h"
#include "..\\LoveLineUtil\\CCLog.h"


class LLApp;


/*
    This is used when initializing the dialog.
*/
class LLMainInitVars
{
public:

    LLMainInitVars();
    ~LLMainInitVars();

    LLApp* pThis;

    CCString FileName;
    BOOL     bRegisterInDB;
    BOOL     bStartRecording;
    BOOL     bDeleteFile;
    BOOL     bExitAutomatically;
    int      nSeconds;
    int      nBitDepth;
    int      nSampleRate;
    int      nChannels;
    int      nBitRate;

    BOOL     bIsRecording;
    BOOL     bIsEncoding;
    HWND    hDlg;// The recording dialog (for use with progress stuff).
    HFONT   hVerdana;
    HFONT   hCourier;
    HFONT   hTitle;// For "RECORDING" and "ENCODING" boxes.

    CCLog Log;

    CCString IntermediateFile;

    HANDLE hEncodingThread;
    HANDLE hEncodingCancel;
};


class LLApp
{
public:
    LLApp();
    ~LLApp();

    int Entry(HINSTANCE hInstance, PCWSTR wszCmdLine);

private:

    static HRESULT CALLBACK CaptureDataProc(PBYTE pData, DWORD dwSize, PVOID pUser);
    HRESULT StartRecording(LLMainInitVars* pVars);
    HRESULT StopRecording(LLMainInitVars* pVars);

    static DWORD WINAPI EncodeThreadProc(PVOID pUser);
    HRESULT StartEncoding(LLMainInitVars* pVars);
    HRESULT StopEncoding(LLMainInitVars* pVars);
    HRESULT AddToDatabase(LLMainInitVars* pVars);

    // Store this when we start recording so we get the correct day.
    SYSTEMTIME st;

    BOOL bIsTracking;
    DLGPROC pOldProc;
    static INT_PTR CALLBACK PropSheetSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static INT_PTR CALLBACK WelcomeProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK FileProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK OptionsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK BitRateProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK ReadyProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK RecordProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK EncodeProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK CompletedProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    HINSTANCE hInstance;
    WaveCapture Capturer;
    WavWriter  Writer;
    WAVReader  Reader;
//    WMAWriter  WMAWriter;
    MP3Writer  WMAWriter;

    // Stuff used for progress
    DWORD dwStartTick;
};

