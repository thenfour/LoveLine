

#pragma once


#include <Windows.h>
#include "..\\LoveLineUtil\\WaveCapture.h"
#include "..\\LoveLineUtil\\WAVWriter.h"
#include "..\\LoveLineUtil\\WAVReader.h"
//#include "..\\LoveLineUtil\\WMAWriter.h"
#include "..\\LoveLineUtil\\MP3Writer.h"
#include "..\\LoveLineUtil\\CCString.h"
#include "..\\LoveLineUtil\\CCLog.h"
#include "..\\LoveLineUtil\\LevelMeter.h"
#include "..\\LoveLineUtil\\LevelView.h"
#include <string>
#include <map>

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

    CCString Device;
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


class LLRecorderDevice
{
public:
    LLRecorderDevice(){}
    LLRecorderDevice(const LLRecorderDevice& x){ assign(x); }
    ~LLRecorderDevice(){}

    void assign(const LLRecorderDevice& x)
    {
        m_sDescription = x.m_sDescription;
        m_sGuid = x.m_sGuid;
        m_sModule = x.m_sModule;
    }

    CCString m_sDescription;// <-- key.

    CCString m_sGuid;
    CCString m_sModule;
};


class LLApp : public CLevelMeterEvents
{
    // Methods of CLevelMeterEvents
public:
    bool LM_SetExtents(long lmin, long lmax);
    bool LM_SetValue(long pos);

public:
    LLApp();
    ~LLApp();

    int Entry(HINSTANCE hInstance, PCWSTR wszCmdLine);

private:

    static HRESULT CALLBACK CaptureDataProc(PBYTE pData, DWORD dwSize, PVOID pUser);
    HRESULT StartRecording(LLMainInitVars* pVars);
    HRESULT StopRecording(LLMainInitVars* pVars);

    static UINT _stdcall EncodeThreadProc(PVOID pUser);
    HRESULT StartEncoding(LLMainInitVars* pVars);
    HRESULT StopEncoding(LLMainInitVars* pVars);
    HRESULT AddToDatabase(LLMainInitVars* pVars);

    // Store this when we start recording so we get the correct day.
    SYSTEMTIME st;

    BOOL bIsTracking;
    DLGPROC pOldProc;
    static INT_PTR CALLBACK PropSheetSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static INT_PTR CALLBACK WelcomeProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK DeviceProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
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
    MP3Writer  WMAWriter;

    CLevelMeter m_lm;
    CLevelView m_lv;

    // Stuff used for progress
    DWORD dwStartTick;

    // Stuff for device enumeration
    static BOOL CALLBACK DSEnumCallback(LPGUID lpGuid, LPCWSTR lpcstrDescription, LPCWSTR lpcstrModule, LPVOID lpContext);
    typedef std::map<std::wstring, LLRecorderDevice> DeviceList_T;
    DeviceList_T m_Devices;
    CCString m_sDeviceDesc;// the selected device description, for later use
};

