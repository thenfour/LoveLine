

#pragma once


#include <windows.h>
#include "global.h"
#include "wmsdk.h"


class CCEXPORT CAudioReaderCallback : public IWMReaderCallback, public IWMReaderCallbackAdvanced
{
public:
    CAudioReaderCallback();
    ~CAudioReaderCallback();

    // IUnknown
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();

    // IWMStatusCallback
    HRESULT STDMETHODCALLTYPE OnStatus(WMT_STATUS Status, HRESULT hr, WMT_ATTR_DATATYPE dwType, BYTE* pValue, void* pvContext);

    // IWMReaderCallback
    HRESULT STDMETHODCALLTYPE OnSample(DWORD, QWORD qwTime, QWORD qwLen, DWORD dwFlags, INSSBuffer* pSample, PVOID pUser);

    // IWMReaderCallbackAdvanced
    HRESULT STDMETHODCALLTYPE AllocateForOutput(DWORD, DWORD cbBuffer, INSSBuffer** ppBuffer, PVOID pUser);
    HRESULT STDMETHODCALLTYPE AllocateForStream(WORD wStreamNum, DWORD cbBuffer, INSSBuffer** ppBuffer, PVOID pUser);
    HRESULT STDMETHODCALLTYPE OnOutputPropsChanged(DWORD dwOutputNum, WM_MEDIA_TYPE* pMediaType, PVOID pUser);
    HRESULT STDMETHODCALLTYPE OnStreamSample(WORD, QWORD qwTime, QWORD qwLen, DWORD dwFlags, INSSBuffer* pSample, PVOID pUser);
    HRESULT STDMETHODCALLTYPE OnStreamSelection(WORD wStreamCount, WORD* pStreamNumbers, WMT_STREAM_SELECTION* pSelections, PVOID pUser);
    HRESULT STDMETHODCALLTYPE OnTime(QWORD qwCurrentTime,PVOID pUser);

private:

    class AudioReader* pOwner;
};



