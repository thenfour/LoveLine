

#pragma once


#include <Windows.h>
#include "..\\LoveLineUtil\\CCCustomWnd.h"
#include "LLPlugin.h"
#include "StandardPlugins.h"
#include "LLPlayerCore.h"
#include "..\\LoveLineUtil\\CCListBox.h"
#include "..\\LoveLineUtil\\CCEditBox.h"
#include "..\\LoveLineUtil\\CCButton.h"
#include "..\\LoveLineUtil\\CCStatic.h"
#include "..\\LoveLineUtil\\AudioReader.h"
#include "..\\LoveLineUtil\\WavWriter.h"
#include <vector>
//#include <string>



class Plugin_WavUtils
{
public:

    Plugin_WavUtils();
    ~Plugin_WavUtils();

    static LPARAM CALLBACK WndProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void CALLBACK NotificationProc(PVOID pUser, DWORD dwMsg, PVOID pParams);

    HRESULT Create();
    HRESULT Destroy();
    HRESULT Refresh();

private:
    static HRESULT CALLBACK EnumFeaturesProc(LLLink* pl, LLFeature* pf, PVOID pUser);
    static HRESULT CALLBACK EnumEpisodesProc(LLEpisode* pEpisode, PVOID pUser);
    static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

    // for the "legend" dialog
    static INT_PTR CALLBACK _LegendProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void OnBrowse();
    void OnStart();
    void OnSelect();

    CCListBox m_cListLinks;
    CCListBox m_cEpisodeList;
    CCButton m_cGroupEpisodes;
    CCButton m_cGroupEpisodeDetails;
    CCImageList m_cImages;
    CCButton m_cCommercials;
    CCButton m_cMP3;

    CCStatic m_csFilename;
    CCEditBox m_cFilename;

    CCStatic m_csSegments;
    CCEditBox m_cSegments;

    CCStatic m_csBitrate;
    CCEditBox m_cBitrate;

    CCStatic m_csOutput;
    CCEditBox m_cOutput;

    CCButton m_cBrowse;
    CCButton m_cStart;
    CCButton m_cLegend;
    CCButton m_cCancel;

    CCEditBox m_cProgress;
    CCButton m_cClear;
    CCEditBox m_cLog;

    int m_iImageEpisode;
    int m_iImageLink;

    LLPlayerCore* m_pCore;
    StandardPlugins* m_pPlugins;

    CCCustomWnd m_cMainWindow;

    // Stuff for when we're actually ripping:
    DWORD m_dwTick;// tick-count so we can time this sucker.
    AudioReader m_Source;
    LLEpisode m_Episode;
    WavWriter m_Output;
    bool m_bExtractCommercials;
    CCString m_sOutputDir;
    CCString m_sFilenaming;
    CCString m_sCurrentWavFile;
    int m_nBitrate;
    bool m_bCompress;
    int m_nSegment;// current # of file
    DWORD m_dwSegmentLength;
    std::vector<LLCommercial> m_Commercials;
    DWORD m_dwRead;// how much have we already read in?
    DWORD m_dwWritten;// how much have we written in the output file?
    WAVEFORMATEX m_wfx;
    struct FilenameInfo
    {
        CCString e;// extension
        CCString f;// filename w/ extension
        CCString F;// filename w/o extension
        CCString m;// month 2-digit
        CCString M;// month name
        CCString d;// day 2-digit
        CCString D;// day name
        CCString y;// year 2-digit
        CCString Y;// year 4-digit
        CCString g;// guests
    } m_fi;

    void _SetupFilenameInfo();

    // After the above members have been set:
    bool _Rip();
    bool _GetCommercialInfo();
    static HRESULT CALLBACK EnumCommercials(LLCommercial* pc, PVOID pUser);

    void _CompressOutput();

    bool _OpenNewFile();
    bool _CreateFilename(CCString& s);

    void _GetNextBufferSegment(DWORD& dw);
    void _GetNextOutputSegment(DWORD& dw);
    void _GetNextCommercialSegment(DWORD& dw);
    bool _PtBetweenInCommercial(DWORD dwStart, DWORD dwEnd);

    DWORD _BytesToMS(DWORD bytes);

    static void _cdecl _StartThread(void*);
    void _Start2();

    // thread stuff
    HANDLE m_hThread;// while the thread is running, this handle is valid.
    HANDLE m_hCancelEvent;
};




