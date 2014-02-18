

#include "WMAWriter.h"
#include "global.h"
#include "TraceErrorSys.h"
#include <wmsysprf.h>// For the standard profiles.
#include "genprofile.h"


WMAWriter::WMAWriter()
{
    this->bLive = FALSE;
    this->pWriter = NULL;
    this->dwAudioTime = 0;
    this->nBytesPerSecond = 0;
    this->pAdv = NULL;
}


WMAWriter::~WMAWriter()
{
    CCLog l;
    this->Close(&l);
}


/*
    pFormat is the format that will be passed in.
    wszFileName is the .WMA file to save it as.
*/
HRESULT WMAWriter::Open(WAVEFORMATEX* pFormat, IN PCWSTR wszFileName, int nbps, BOOL bLive, CCLog* pLog)
{
    HRESULT hr;
    IWMInputMediaProps* pInputMediaProps = NULL;
    IWMStreamConfig* pStream = NULL;
    WM_MEDIA_TYPE   mt;

    this->Close(pLog);
    this->bLive = bLive;

    this->nBytesPerSecond = pFormat->nAvgBytesPerSec;

    //////////////////////////////////////////////////////////////////////// CREATE OUR WRITER
    /*
        Create the writer
    */
    if(FAILED(hr = WMCreateWriter(NULL, &this->pWriter)))
    {
        pLog->AddEntry(
            L"Error creating Windows Media writer object. "
            L"Be sure that Windows Media Format codecs are installed correctly.");
        pLog->AddEntry(hr, L"WMCreateWriter");
        return E_FAIL;
    }

    ////////////////////////////////////////////////////////////////////// SET PROFILE
    if(FAILED(this->SetOutputFormat(pFormat, nbps, pLog)))
    {
        SAFE_RELEASE(this->pWriter);
        return E_FAIL;
    }

    ////////////////////////////////////////////////////////////////////// SET OUTPUT FILENAME
    if(FAILED(hr = this->pWriter->SetOutputFilename(wszFileName)))
    {
        pLog->AddEntry(
            L"Could not set the output filename to \"%s\".  The filename "
            L"may be invalid, may point to a directory that doesn't exist, "
            L"or the file may already exist.", wszFileName);
        pLog->AddEntry(hr, L"IWMWriter::SetOutputFilename");
        SAFE_RELEASE(this->pWriter);
        return E_FAIL;
    }

    ////////////////////////////////////////////////////////////////////// SET INPUT FORMAT
    // There will only be one input for this profile.
    // We grab it so we can modify it.
    if(FAILED(hr = this->pWriter->GetInputProps(0, &pInputMediaProps)))
    {
        pLog->AddEntry(L"Error getting information about the writer's input format.");
        pLog->AddEntry(hr, L"IWMWriter::GetInputProps");
        SAFE_RELEASE(this->pWriter);
        return E_FAIL;
    }

    // Now set the input format to what was specified in the pFormat argument
    mt.majortype			= WMMEDIATYPE_Audio;
    mt.subtype				= WMMEDIASUBTYPE_PCM;
    mt.bFixedSizeSamples	= TRUE;
    mt.bTemporalCompression = FALSE;
    mt.lSampleSize			= pFormat->nBlockAlign;
    mt.formattype			= WMFORMAT_WaveFormatEx;
    mt.pUnk					= NULL;
    mt.cbFormat				= sizeof(*pFormat);
    mt.pbFormat				= (BYTE*)pFormat;

    // Update it in the inputmediaprops object
    if(FAILED(hr = pInputMediaProps->SetMediaType(&mt)))
    {
        pLog->AddEntry(L"Error setting information about the writer's input format.");
        pLog->AddEntry(hr, L"IWMInputMediaProps::SetMediaType");
        SAFE_RELEASE(pInputMediaProps);
        SAFE_RELEASE(this->pWriter);
        return E_FAIL;
    }

    // And tell our writer to use the updated version and not the old version.
    if(FAILED(hr = this->pWriter->SetInputProps(0, pInputMediaProps)))
    {
        pLog->AddEntry(L"Error setting information about the writer's input format.");
        pLog->AddEntry(hr, L"IWMWriter::SetInputProps");
        SAFE_RELEASE(pInputMediaProps);
        SAFE_RELEASE(this->pWriter);
        return E_FAIL;
    }

    SAFE_RELEASE(pInputMediaProps);

    this->dwAudioTime = 0;

    /*
        Do other stuff.
    */
    if(FAILED(hr = this->pWriter->QueryInterface(IID_IWMWriterAdvanced, (PVOID*)&this->pAdv)))
    {
        pLog->AddEntry(L"Error creating Windows Media objects.");
        pLog->AddEntry(hr, L"QI IID_IWMWriterAdvanced");
        SAFE_RELEASE(this->pWriter);
        return E_FAIL;
    }
/*
    if(FAILED(hr = this->pAdv->SetLiveSource(bLive)))
    {
        TraceErrorSys(&ErrBuf, hr);
        pErrText->FormatW(
            L"Error creating Windows Media objects.\r\n"
            L"The error returned by IWMWriterAdvanced::SetLiveSource was %s", (PCWSTR)ErrBuf);

        SAFE_RELEASE(this->pAdv);
        SAFE_RELEASE(this->pWriter);
        return E_FAIL;
    }
*/
    //////////////////////////////////////////////////////////////////////////// BEGIN WRITING
    if(FAILED(hr = this->pWriter->BeginWriting()))
    {
        pLog->AddEntry(L"Error creating the output file.");
        pLog->AddEntry(hr, L"IWMWriter::BeginWriting");
        SAFE_RELEASE(this->pAdv);
        SAFE_RELEASE(this->pWriter);
        return E_FAIL;
    }

    return S_OK;
}


/*
    This will set the output format.  It's only called by Open().
*/
HRESULT WMAWriter::SetOutputFormat(WAVEFORMATEX* pFormat, int nBitRate, CCLog* pLog)
{
    IWMProfile* pProfile = NULL;
    HRESULT hr;

    /*
        WMProfile_V70_6VoiceAudio
        WMProfile_V70_288FMRadioMono
        WMProfile_V70_288FMRadioStereo
        WMProfile_V70_56DialUpStereo
        WMProfile_V70_64AudioISDN
        WMProfile_V70_96Audio
        WMProfile_V70_128Audio
    */
    if(FAILED(hr = this->pWriter->SetProfileByID(WMProfile_V70_288FMRadioMono)))
    {
        pLog->AddEntry(L"Error setting the profile for the Windows Media file. "
            L"This problem can most likely be fixed by reinstalling the Windows "
            L"Media dependencies.");
        pLog->AddEntry(hr, L"IWMWriter::SetProfileByID");
        return E_FAIL;
    }
    SAFE_RELEASE(pProfile);

    return S_OK;
}


HRESULT WMAWriter::Close(CCLog* pLog)
{
    HRESULT hr;

    if(this->pWriter)
    {
        // Flush the current queue
        if(FAILED(hr = this->pWriter->Flush()))
        {
            pLog->AddEntry(L"Error writing to the media file.");
            pLog->AddEntry(hr, L"IWMWriter::Flush");
            return E_FAIL;
        }

        this->pWriter->EndWriting();
        SAFE_RELEASE(this->pAdv);
        SAFE_RELEASE(this->pWriter);
    }

    return S_OK;
}


HRESULT WMAWriter::WriteBits(PVOID pData, DWORD dwSize, CCLog* pLog)
{
    HRESULT hr;
    PVOID pWriteBuffer;
    INSSBuffer* pBuffer = NULL;
    DWORD dwBufferLength = 0;

    if(this->pWriter == NULL)
    {
        pLog->AddEntry(
            L"Could not write data because the objects are uninitialized.");
        return E_FAIL;
    }

    QWORD qwWriterTime;

    if(FAILED(hr = this->pWriter->AllocateSample(dwSize, &pBuffer)))
    {
        pLog->AddEntry(L"Error getting information about an audio buffer.");
        pLog->AddEntry(hr, L"INSSBuffer::GetBuffer");
        return E_FAIL;
    }

    if(FAILED(hr = pBuffer->GetBufferAndLength((PBYTE*)&pWriteBuffer, &dwBufferLength)))
    {
        pLog->AddEntry(L"Error getting information about an audio buffer.");
        pLog->AddEntry(hr, L"INSSBuffer::GetBufferAndLength");
        SAFE_RELEASE(pBuffer);
        return E_FAIL;
    }

    // Now copy our bits into the buffer
    memcpy(pWriteBuffer, pData, dwSize);

    if(FAILED(pBuffer->SetLength(dwSize)))
    {
        pLog->AddEntry(L"Error setting information about an audio buffer.");
        pLog->AddEntry(hr, L"INSSBuffer::SetLength");
        SAFE_RELEASE(pBuffer);
        return E_FAIL;
    }

    // Tell the writer to start writing the samples.
    QWORD qwPerfTime = 10000 * (QWORD)this->dwAudioTime;
    this->pAdv->GetWriterTime(&qwWriterTime);
    DWORD dwWriterMS = (DWORD)(qwWriterTime / 10000);

    // Wait until the writer time catches up.
    DWORD dwWriterGoalMS = (DWORD)(qwPerfTime / 10000);
    if(dwWriterGoalMS > 2000) dwWriterGoalMS -= 2000;

    while(dwWriterMS < dwWriterGoalMS)
    {
        Sleep(0);
        this->pAdv->GetWriterTime(&qwWriterTime);
        dwWriterMS = (DWORD)(qwWriterTime / 10000);
    }

    // Advance our own pointer
    this->dwAudioTime += MulDiv(dwSize, 1000, this->nBytesPerSecond);

    /*
        If it's not live, go ahead and wait for it to finish.
    */
    if(FAILED(hr = this->pWriter->WriteSample(0, qwPerfTime, 0, pBuffer)))
    {
        pLog->AddEntry(
            L"Error writing audio samples to file. "
            L"Common causes are your computer's CPU may "
            L"be too busy to encode or the disk may be full.");
        pLog->AddEntry(hr, L"INSSBuffer::WriteSample");
        SAFE_RELEASE(pBuffer);
        return E_FAIL;
    }

    SAFE_RELEASE(pBuffer);
    return S_OK;
}




