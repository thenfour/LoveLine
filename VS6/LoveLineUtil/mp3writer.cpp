

#include "pch.h"
#include "mp3writer.h"


MP3Writer::MP3Writer()
{
    this->pEnc = NULL;
    this->hFile = NULL;
    this->pBuffer = NULL;
    this->pEncoded = 0;
}


MP3Writer::~MP3Writer()
{
    this->Close(NULL);
}


HRESULT MP3Writer::Open(
    WAVEFORMATEX* pFormat, PCWSTR wszFileName, int nBitRate, BOOL bLive, CCLog* pLog)
{
    HRESULT hr;
    XingKey Key;

    this->Close(pLog);

    // Create the MP3 encoder interface.
    if(FAILED(hr = CoCreateInstance(CLSID_MP3Encoder3, NULL, CLSCTX_SERVER, IID_IMP3Encoder3, (PVOID*)&this->pEnc)))
    {
        pLog->AddEntry(L"Could not create MP3 encoder object");
        pLog->AddEntry(hr, L"CoCreateInstance");
        SAFE_RELEASE(this->pEnc);
        return E_FAIL;
    }

    if(FAILED(hr = this->pEnc->InitAuthentication(Key)))
    {
        pLog->AddEntry(L"Could not create MP3 encoder object");
        pLog->AddEntry(hr, L"IMP3Encoder3::InitAuthentication");
        SAFE_RELEASE(this->pEnc);
        return E_FAIL;
    }

    if(FAILED(hr = this->pEnc->SetInputSampleRate(pFormat->nSamplesPerSec)))
    {
        pLog->AddEntry(L"Could not create MP3 encoder object");
        pLog->AddEntry(hr, L"IMP3Encoder3::SetInputSampleRate");
        SAFE_RELEASE(this->pEnc);
        return E_FAIL;
    }

    if(FAILED(hr = this->pEnc->SetInputSampleSize(pFormat->wBitsPerSample)))
    {
        pLog->AddEntry(L"Could not create MP3 encoder object");
        pLog->AddEntry(hr, L"IMP3Encoder3::SetInputSampleSize");
        SAFE_RELEASE(this->pEnc);
        return E_FAIL;
    }

    if(FAILED(hr = this->pEnc->SetInputNumberOfChannels(pFormat->nChannels)))
    {
        pLog->AddEntry(L"Could not create MP3 encoder object");
        pLog->AddEntry(hr, L"IMP3Encoder3::SetInputNumberOfChannels");
        SAFE_RELEASE(this->pEnc);
        return E_FAIL;
    }

    if(FAILED(hr = this->pEnc->SetOutputStereoMode(1)))
    {
        pLog->AddEntry(L"Could not create MP3 encoder object");
        pLog->AddEntry(hr, L"IMP3Encoder3::SetOutputStereoMode");
        SAFE_RELEASE(this->pEnc);
        return E_FAIL;
    }

    if(FAILED(hr = this->pEnc->SetOutputTotalChannelBitrate(nBitRate)))
    {
        pLog->AddEntry(L"Could not create MP3 encoder object");
        pLog->AddEntry(hr, L"IMP3Encoder3::SetOutputTotalChannelBitrate");
        SAFE_RELEASE(this->pEnc);
        return E_FAIL;
    }

    if(FAILED(hr = this->pEnc->SetVariableBitRateEncode(FALSE, FALSE)))
    {
        pLog->AddEntry(L"Could not create MP3 encoder object");
        pLog->AddEntry(hr, L"IMP3Encoder3::SetVariableBitRateEncode");
        SAFE_RELEASE(this->pEnc);
        return E_FAIL;
    }

    if(FAILED(hr = this->pEnc->SetVariableBitRateScale(0)))
    {
        pLog->AddEntry(L"Could not create MP3 encoder object");
        pLog->AddEntry(hr, L"IMP3Encoder3::SetVariableBitRateScale");
        SAFE_RELEASE(this->pEnc);
        return E_FAIL;
    }

    if(FAILED(hr = this->pEnc->SetHighFrequencyMode(3)))
    {
        pLog->AddEntry(L"Could not create MP3 encoder object");
        pLog->AddEntry(hr, L"IMP3Encoder3::SetHighFrequencyMode");
        SAFE_RELEASE(this->pEnc);
        return E_FAIL;
    }

    if(FAILED(hr = this->pEnc->SetCopyrightBit(TRUE)))
    {
        pLog->AddEntry(L"Could not create MP3 encoder object");
        pLog->AddEntry(hr, L"IMP3Encoder3::SetCopyrightBit");
        SAFE_RELEASE(this->pEnc);
        return E_FAIL;
    }

    if(FAILED(hr = this->pEnc->SetOriginalBit(TRUE)))
    {
        pLog->AddEntry(L"Could not create MP3 encoder object");
        pLog->AddEntry(hr, L"IMP3Encoder3::SetOriginalBit");
        SAFE_RELEASE(this->pEnc);
        return E_FAIL;
    }

    if(FAILED(hr = this->pEnc->SetChooseBestOutputSampleRate(FALSE)))
    {
        pLog->AddEntry(L"Could not create MP3 encoder object");
        pLog->AddEntry(hr, L"IMP3Encoder3::SetChooseBestOutputSampleRate");
        SAFE_RELEASE(this->pEnc);
        return E_FAIL;
    }

    if(FAILED(hr = this->pEnc->VerifySettings()))
    {
        pLog->AddEntry(L"Could not create MP3 encoder object");
        pLog->AddEntry(hr, L"IMP3Encoder3::VerifySettings");
        SAFE_RELEASE(this->pEnc);
        return E_FAIL;
    }

    if(FAILED(hr = this->pEnc->StartMemoryEncode(&this->nMinBytes, Key)))
    {
        pLog->AddEntry(L"Could not create MP3 encoder object");
        pLog->AddEntry(hr, L"IMP3Encoder3::StartMemoryEncode");
        SAFE_RELEASE(this->pEnc);
        return E_FAIL;
    }

    /*
        Create our pending audio buffer.
    */
    this->pBuffer = (PBYTE)malloc(this->nMinBytes);
    this->pEncoded = (PBYTE)malloc(this->nMinBytes);
    this->nPendingData = 0;// We don't have any pending audio data.

    // Open our output file.
    this->hFile = CreateFile(wszFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL,
        CREATE_NEW, NULL, NULL);

    if(this->hFile == NULL || this->hFile == INVALID_HANDLE_VALUE)
    {
        pLog->AddEntry(L"Couldn't open the MP3 file for writing");
        pLog->AddEntry(GetLastError(), L"CreateFile");
        this->hFile = NULL;
        SAFE_RELEASE(this->pEnc);
        return E_FAIL;
    }

    return S_OK;
}



HRESULT MP3Writer::Close(CCLog* pLog)
{
    HRESULT hr;
    int be;
    DWORD br;

    if(this->pEnc)
    {
        /*
            If there is pending audio data, write it now.
        */
        if(this->nPendingData != 0)
        {
            // Fill the rest of the buffer with zeros.
            memset(this->pBuffer + this->nPendingData, 0, this->nMinBytes - this->nPendingData);

            // Write the buffer.
            this->WriteBits(this->pBuffer, this->nMinBytes, pLog);
        }

        /*
            Stop the memory encode.
        */
        be = 0;
        if(FAILED(hr = this->pEnc->EndMemoryEncode(this->pEncoded, &be)))
        {
            if(pLog)
            {
                pLog->AddEntry(L"Could not encode MP3 samples");
                pLog->AddEntry(hr, L"IMP3Encoder3::EndMemoryEncode");
            }
        }

        if(be != 0)
        {
            // Write those last bits.
            WriteFile(this->hFile, this->pEncoded, be, &br, NULL);
        }

    }

    if(this->hFile)
    {
        CloseHandle(this->hFile);
        this->hFile = 0;
    }

    SAFE_RELEASE(this->pEnc);

    if(this->pBuffer)
    {
        free(this->pBuffer);
        this->pBuffer = 0;
    }

    if(this->pEncoded)
    {
        free(this->pEncoded);
        this->pEncoded = 0;
    }

    this->nMinBytes = 0;
    this->nPendingData = 0;

    return S_OK;
}


HRESULT MP3Writer::WriteBits(PVOID pData, DWORD dwSize, CCLog* pLog)
{
    int nDataPointer = 0;
    DWORD br;
    HRESULT hr;
    int nBytesRead;
    int nBytesEncoded;

    if(this->pEnc == NULL) return E_FAIL;

    /*
        If there is pending data, let's deal wtih that.
    */
    if(this->nPendingData)
    {
        /*
            We also have to make sure that the audio data will fill the
            buffer completely.
        */
        int nBytesLeftInBuffer = this->nMinBytes - this->nPendingData;

        /*
            If the pending data PLUS the new data is still too small, then
            let's just append the new data and return.
        */
        if(dwSize < (DWORD)nBytesLeftInBuffer)
        {
            memcpy(this->pBuffer + this->nPendingData, pData, dwSize);
            this->nPendingData += dwSize;
            return S_OK;
        }
        else
        {
            /*
                We have enough to fill the buffer completely.
            */
            memcpy(this->pBuffer + this->nPendingData, pData, nBytesLeftInBuffer);
            nBytesRead = this->nMinBytes;

            if(FAILED(hr = this->pEnc->EncodeSamples(this->pBuffer, this->pEncoded,
                &nBytesRead, &nBytesEncoded)))
            {
                pLog->AddEntry(hr, L"IMP3Encoder3::EncodeSamples()");
                return E_FAIL;
            }

            // Write the encoded data.
            WriteFile(this->hFile, this->pEncoded, nBytesEncoded, &br, NULL);

            nDataPointer = this->nMinBytes;
            this->nPendingData = 0;
        }
    }

    /*
        Now write data.
    */
    while((DWORD)nDataPointer < dwSize)
    {
        /*
            If there isn't enough data to encode, just stick it in the pending
            buffer.
        */
        if((dwSize - nDataPointer) < (DWORD)this->nMinBytes)
        {
            // Stick data in the pending buffer.
            memcpy(this->pBuffer, (PBYTE)pData + nDataPointer, dwSize - nDataPointer);
            this->nPendingData = dwSize - nDataPointer;
            return S_OK;
        }

        /*
            Encode nMinSize number of bytes.
        */
        nBytesRead = this->nMinBytes;
        if(FAILED(hr = this->pEnc->EncodeSamples((PBYTE)pData + nDataPointer, this->pEncoded,
            &nBytesRead, &nBytesEncoded)))
        {
            pLog->AddEntry(hr, L"IMP3Encoder3::EncodeSamples()");
            return E_FAIL;
        }
        if(nBytesRead == 0)
        {
            pLog->AddEntry(hr, L"IMP3Encoder3::EncodeSamples() Read 0 bytes... hr=%08x", hr);
            return E_FAIL;
        }

        nDataPointer += nBytesRead;

        // Write the encoded data.
        WriteFile(this->hFile, this->pEncoded, nBytesEncoded, &br, NULL);
    }

    return S_OK;
}


#if 0
HRESULT MP3Writer::Close(CCLog* pLog)
{
    int be;
    int br;
    HRESULT hr;

    if(this->pEnc)
    {
        /*
            If we have pending samples, write them now.
        */
        PBYTE pBuffer = NULL;

        if(this->bPending == TRUE)
        {
            /*
                We'll set up our own buffer, clear the pending buffer
                and write the bits with ZEROs at the end.
            */
            pBuffer = (PBYTE)malloc(this->nMinBytes);

            if(pBuffer)
            {
                CopyMemory(pBuffer, this->pBuffer, this->nBufferSize);
                FillMemory(pBuffer + this->nBufferSize, 0, this->nMinBytes - this->nBufferSize);

                free(this->pBuffer);
                this->pBuffer = NULL;
                this->bPending = FALSE;

                this->WriteBits(pBuffer, this->nMinBytes, pLog);

                free(pBuffer);
                pBuffer = NULL;
            }
        }

        PBYTE pFinal = (PBYTE)malloc(this->nLastSize);

        be = 0;
        if(FAILED(hr = this->pEnc->EndMemoryEncode(pFinal, &be)))
        {
            if(pLog)
            {
                pLog->AddEntry(L"Could not encode MP3 samples");
                pLog->AddEntry(hr, L"IMP3Encoder3::EndMemoryEncode");
            }

            return E_FAIL;
        }

        if(be != 0)
        {
            // Write those last bits.
            WriteFile(this->hFile, this->pBuffer, be, (DWORD*)&br, NULL);
        }

        free(this->pBuffer);
        this->pBuffer = NULL;

        CloseHandle(this->hFile);
        this->hFile = NULL;
    }

    SAFE_RELEASE(pEnc);
    this->bPending = FALSE;

    return S_OK;
}


HRESULT MP3Writer::WriteBits(PVOID pData, DWORD dwSize, CCLog* pLog)
{
    HRESULT hr;
    PBYTE pMP3 = NULL;
    int br;
    int MP3Bytes;
    int bw;

    if(this->pEnc == NULL) return E_FAIL;

    if(this->bPending)
    {
        /*
            Create a totally new buffer that is our previous buffer plus the new one.
        */
        PBYTE pCombined = (PBYTE)malloc(dwSize + this->nBufferSize);

        if(pCombined == NULL)
        {
            pLog->AddEntry(L"Out of memory...");
            return E_FAIL;
        }

        memcpy(pCombined, this->pBuffer, this->nBufferSize);
        memcpy(pCombined + this->nBufferSize, pData, dwSize);

        pData = pCombined;
        dwSize += this->nBufferSize;

        // Free our private buffer.
        free(this->pBuffer);
        this->pBuffer = NULL;
        this->nBufferSize = 0;
    }

    // Create an MP3 buffer that's the same size as the wave buffer.
    pMP3 = (PBYTE)malloc(dwSize);
    if(pMP3 == NULL)
    {
        pLog->AddEntry(L"Out of memory...");
        return E_FAIL;
    }

    this->nLastSize = dwSize;
    int nBytesTotal = 0;

    if(dwSize >= (DWORD)this->nMinBytes)
    {
        do
        {
            if(FAILED(hr = this->pEnc->EncodeSamples((PBYTE)pData + nBytesTotal, pMP3, &br, &MP3Bytes)))
            {
                pLog->AddEntry(hr, L"IMP3Encoder3::EncodeSamples");
                return E_FAIL;
            }

            // Now Write it to disk.
            WriteFile(this->hFile, pMP3, MP3Bytes, (DWORD*)&bw, NULL);
            if(bw != MP3Bytes)
            {
                pLog->AddEntry(GetLastError(), L"WriteFile() failed.");
            }

            nBytesTotal += br;
        }
        while((DWORD)nBytesTotal < dwSize);

        this->bPending = FALSE;
        free(this->pBuffer);
        this->nBufferSize = 0;
        this->pBuffer = NULL;
    }
    else
    {
        // Copy the data into our own private buffer for a pending write.
        this->pBuffer = (PBYTE)malloc(dwSize);
        this->nBufferSize = dwSize;
        this->bPending = TRUE;
    }

    return S_OK;
}


#endif


