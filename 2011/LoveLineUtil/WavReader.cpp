

#include "pch.h"
#include "WavReader.h"
#include "TraceErrorSys.h"
#include "CCString.h"


DWORD GetFourCC(PCSTR sz)
{
    return ((DWORD)(BYTE)(sz[0]) | ((DWORD)(BYTE)(sz[1]) << 8) | \
        ((DWORD)(BYTE)(sz[2]) << 16) | ((DWORD)(BYTE)(sz[3]) << 24 )); 
}


/*
	Completed 010309 Carl Corcoran
    Skips chunks until the one is found.
*/
HRESULT CC_FindChunk(DWORD id, HANDLE hFile, CCLog* pLog)
{
    DWORD dwTest;
    DWORD dwSize;
    DWORD br;

    while(1)
    {
        ReadFile(hFile, &dwTest, 4, &br, NULL);

        if(br != 4)
        {
            pLog->AddEntry(L"Could not read from WAV file.");
            pLog->AddEntry(GetLastError(), L"ReadFile returned");
            return E_FAIL;
        }

        if(dwTest == id)
        {
            break;
        }

        ReadFile(hFile, &dwSize, 4, &br, NULL);

        if(br != 4)
        {
            pLog->AddEntry(L"Could not read from WAV file.");
            pLog->AddEntry(GetLastError(), L"ReadFile returned");
            return E_FAIL;
        }

        SetFilePointer(hFile, dwSize, 0, FILE_CURRENT);
    }

    return S_OK;
}


/*
      010310 Carl Corcoran
*/
HRESULT WAVReader::ReturnToData()
{
    SetFilePointer(this->hFile, this->dwDataPos, 0, FILE_BEGIN);

    return S_OK;
}


/*
      010309 Carl Corcoran
*/
DWORD WAVReader::GetDataSize()
{
    return this->dwSize;
}


/*
      010309 Carl Corcoran
*/
WAVReader::WAVReader()
{
    this->hFile = NULL;
    this->bCircular = TRUE;
    this->dwDataPos = 0;
    this->dwSize = 0;
    ZeroMemory(&this->Format, sizeof(this->Format));
}


/*
      010309 Carl Corcoran
*/
WAVReader::~WAVReader()
{
    this->Close();
}


/*
      010309 Carl Corcoran
*/
HRESULT WAVReader::Open(CCString FileName, BOOL bCircular, CCLog* pLog)
{
    this->bCircular = bCircular;
    DWORD idRiff = 0;
    DWORD idWave = 0;
    DWORD br = 0;
    DWORD dwSize;

    this->Close();

    this->hFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
        NULL, NULL);
    if(this->hFile == INVALID_HANDLE_VALUE || this->hFile == 0)
    {
        pLog->AddEntry(L"Could not open \"%s\" for reading.", (PCWSTR)FileName);
        pLog->AddEntry(GetLastError(), L"CreateFile returned");
        return E_FAIL;
    }

    //RIFF
    ReadFile(this->hFile, &idRiff, 4, &br, NULL);

    if(br != 4)
    {
        pLog->AddEntry(L"Could not read from WAV file.");
        pLog->AddEntry(GetLastError(), L"ReadFile returned");
        return E_FAIL;
    }

    if(idRiff != mmioFOURCC('R', 'I', 'F', 'F'))
    {
        pLog->AddEntry(L"The WAV file is invalid or corrupt.");
        pLog->AddEntry(L"File does not contain a valid RIFF chunk.");
        return E_FAIL;
    }

    //Size of riff chunk
    ReadFile(this->hFile, &idRiff, 4, &br, NULL);

    //WAVE
    ReadFile(this->hFile, &idWave, 4, &br, NULL);

    if(br != 4)
    {
        pLog->AddEntry(L"Could not read from WAV file.");
        pLog->AddEntry(GetLastError(), L"ReadFile returned");
        return E_FAIL;
    }

    if(idWave != mmioFOURCC('W', 'A', 'V', 'E'))
    {
        pLog->AddEntry(L"The WAV file is invalid or corrupt.");
        pLog->AddEntry(L"File does not contain a valid WAVE chunk.");
        return E_FAIL;
    }

    //Search for the fmt chunk.
    if(CC_FindChunk(GetFourCC("fmt "), this->hFile, pLog) != S_OK)
    {
        return E_FAIL;
    }

    ReadFile(this->hFile, &dwSize, 4, &br, NULL);

    if(br != 4)
    {
        pLog->AddEntry(L"Could not read from WAV file.");
        pLog->AddEntry(GetLastError(), L"ReadFile returned");
        return E_FAIL;
    }

    DWORD dwWFX = sizeof(WAVEFORMATEX);
    DWORD dwWF = sizeof(WAVEFORMAT);
    DWORD dwBR = 0;

    //Let's read in the format.
    if((dwSize < sizeof(WAVEFORMAT)) || (dwSize > sizeof(WAVEFORMATEX)))
    {
        pLog->AddEntry(L"The WAV file is invalid or corrupt.");
        pLog->AddEntry(L"The 'fmt ' chunk is the wrong size.");
        return E_FAIL;
    }

    ReadFile(this->hFile, &this->Format, dwSize, &br, NULL);

    //We now should have the format.
    //Find the data chunk.
    if(CC_FindChunk(GetFourCC("data"), this->hFile, pLog) != S_OK)
    {
        return E_FAIL;
    }

    //Get the size of the data chunk.
    ReadFile(this->hFile, &this->dwSize, sizeof(DWORD), &br, NULL);

    if(br != sizeof(DWORD))
    {
        pLog->AddEntry(L"Could not read from WAV file.");
        pLog->AddEntry(GetLastError(), L"ReadFile returned");
        return E_FAIL;
    }

    //We're all set.
    this->dwDataPos = SetFilePointer(this->hFile, 0, 0, FILE_CURRENT);

    return S_OK;

}


/*
      010309 Carl Corcoran
*/
HRESULT WAVReader::Close()
{
    if(this->hFile == NULL) return S_OK;

    CloseHandle(this->hFile);
    this->hFile = NULL;

    ZeroMemory(&this->Format, sizeof(this->Format));

    return S_OK;
}


/*
      010309 Carl Corcoran
*/
HRESULT WAVReader::GetFormat(WAVEFORMATEX * pFormat)
{
    CopyMemory(pFormat, &this->Format, sizeof(this->Format));
    return S_OK;
}


/*
      010309 Carl Corcoran
      ERROR_HANDLE_EOF
*/
HRESULT WAVReader::Read(PVOID pBuf, DWORD dwSize, DWORD* pBr, CCLog* pLog)
{
    DWORD br = 0;

    ReadFile(this->hFile, pBuf, dwSize, &br, NULL);

    if(br != dwSize)
    {
        if(this->bCircular == TRUE)
        {
            /*
                If the wav is to be played circular, then
                if we ran out of wav, start over again at the beginning.
            */
            DWORD BytesLeft = dwSize - br;

            SetFilePointer(this->hFile, this->dwDataPos, 0, FILE_BEGIN);

            /*
                Now read in from the beginning.  In the event
                that the file is smaller than the bytes requested,
                we make this function recursive.
            */
            DWORD br2 = 0;
            ReadFile(this->hFile, ((UCHAR*)pBuf) + br, BytesLeft, &br2, NULL);
            *pBr = br2 + br;

            return S_OK;
        }
        else
        {
            *pBr = br;

            //pLog->AddEntry(L"Could not read any more from the WAV file.");
            //pLog->AddEntry(GetLastError(), L"ReadFile returned");

            return ERROR_HANDLE_EOF;
        }
    }

    *pBr = br;
    return S_OK;
}




