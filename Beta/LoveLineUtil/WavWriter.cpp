

#include "wavwriter.h"
#include "TraceErrorSys.h"


WavWriter::WavWriter()
{
    this->hFile = NULL;
}


WavWriter::~WavWriter()
{
    CCLog l;
    this->Close(&l);
}


// Opens and initializes the file.  The next thing you can do is call WriteBits().
HRESULT WavWriter::Open(WAVEFORMATEX* pFormat, PCWSTR wszFileName, CCLog* pLog)
{
    CCString s;
    BYTE buf[1024];

    // We don't take extra information.
    //pFormat->cbSize = 0;

    this->Close(pLog);

    this->dwDATASizeOffset = 0;
    this->dwRIFFSizeOffset = 4;

    this->hFile = CreateFile(wszFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL,
        CREATE_NEW, 0, 0);
    if(this->hFile == NULL || this->hFile == INVALID_HANDLE_VALUE)
    {
        pLog->AddEntry(L"Error creating file \"%s\"", wszFileName);
        pLog->AddEntry(GetLastError(), L"CreateFile");
        return E_FAIL;
    }

    // Now write the RIFF header.
    buf[0] = 'R';
    buf[1] = 'I';
    buf[2] = 'F';
    buf[3] = 'F';
    buf[4] = 0;
    buf[5] = 0;
    buf[6] = 0;
    buf[7] = 0;

    if(FAILED(this->_WriteFile(buf, 8, pLog)))
    {
        pLog->AddEntry(L"Error writing the 'RIFF' header.");
        pLog->AddEntry(GetLastError(), L"WriteFile");

        CloseHandle(this->hFile);
        this->hFile = NULL;
        return E_FAIL;
    }
    this->dwDATASizeOffset += 8;

    // Write the WAVEfmt header
    buf[0] = 'W';
    buf[1] = 'A';
    buf[2] = 'V';
    buf[3] = 'E';
    buf[4] = 'f';
    buf[5] = 'm';
    buf[6] = 't';
    buf[7] = ' ';
    *(DWORD*)(&buf[8]) = sizeof(WAVEFORMATEX);

    if(FAILED(this->_WriteFile(buf, 12, pLog)))
    {
        pLog->AddEntry(L"Error writing the 'WAVE' header.");
        pLog->AddEntry(GetLastError(), L"WriteFile");
        CloseHandle(this->hFile);
        this->hFile = NULL;
        return E_FAIL;
    }
    this->dwDATASizeOffset += 12;

    WAVEFORMATEX wfx;
    memcpy(&wfx, pFormat, sizeof(WAVEFORMATEX));
    wfx.cbSize = 0;

    if(FAILED(this->_WriteFile(&wfx, sizeof(WAVEFORMATEX), pLog)))
    {
        pLog->AddEntry(L"Error writing the .WAV format header.");
        pLog->AddEntry(GetLastError(), L"WriteFile");
        CloseHandle(this->hFile);
        this->hFile = NULL;
        return E_FAIL;
    }
    this->dwDATASizeOffset += sizeof(WAVEFORMATEX);

    buf[0] = 'd';
    buf[1] = 'a';
    buf[2] = 't';
    buf[3] = 'a';
    buf[4] = 0;
    buf[5] = 0;
    buf[6] = 0;
    buf[7] = 0;

    if(FAILED(this->_WriteFile(buf, 8, pLog)))
    {
        pLog->AddEntry(L"Error writing the .WAV data header.");
        pLog->AddEntry(GetLastError(), L"WriteFile");
        CloseHandle(this->hFile);
        this->hFile = NULL;
        return E_FAIL;
    }
    this->dwDATASizeOffset += 4;

    this->dwWavSize = 0;

    return S_OK;
}


HRESULT WavWriter::_WriteFile(PVOID pData, DWORD dwSize, CCLog* pLog)
{
    DWORD br;

    if(WriteFile(this->hFile, pData, dwSize, &br, NULL) == 0)
    {
        pLog->AddEntry(L"Error writing to WAV file.");
        pLog->AddEntry(GetLastError(), L"WriteFile");
        return E_FAIL;
    }

    return S_OK;
}


HRESULT WavWriter::Close(CCLog* pLog)
{
    DWORD dwSize;

    if(this->hFile == NULL) return S_OK;

    // Update the RIFF chunk size now that we're finished recording audio data
    SetFilePointer(this->hFile, this->dwRIFFSizeOffset, 0, FILE_BEGIN);
    dwSize = (this->dwDATASizeOffset + 4) + this->dwWavSize - (this->dwRIFFSizeOffset + 4);
    this->_WriteFile(&dwSize, 4, pLog);

    // Update the data chunk size too...
    SetFilePointer(this->hFile, this->dwDATASizeOffset, 0, FILE_BEGIN);
    this->_WriteFile(&this->dwWavSize, 4, pLog);

    CloseHandle(this->hFile);
    this->hFile = NULL;

    return S_OK;
}


HRESULT WavWriter::WriteBits(PVOID pData, DWORD dwSize, CCLog* pLog)
{
    if(FAILED(this->_WriteFile(pData, dwSize, pLog)))
    {
        return E_FAIL;
    }

    this->dwWavSize += dwSize;

    return S_OK;
}


