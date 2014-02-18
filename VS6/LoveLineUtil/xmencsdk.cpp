/*
Here's an example of how this works:

#include <Windows.h>
#include "xmencsdk.h"
#include "xmencmp3.h"
#include <stdio.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR wszCmd, int)
{
    IMP3Encoder3* pEnc = NULL;
    HRESULT hr;
    XingKey key;

    CoInitialize(0);
    CoCreateInstance(CLSID_MP3Encoder3, NULL, CLSCTX_SERVER, IID_IMP3Encoder3, (PVOID*)&pEnc);

    hr = pEnc->InitAuthentication(key);
    hr = pEnc->Init();
    hr = pEnc->SetInputSampleRate(22050);
    hr = pEnc->SetInputSampleSize(16);
    hr = pEnc->SetInputNumberOfChannels(2);
    hr = pEnc->SetOutputStereoMode(1);
    hr = pEnc->SetOutputTotalChannelBitrate(128);
    hr = pEnc->SetVariableBitRateEncode(0, FALSE);
    hr = pEnc->SetVariableBitRateScale(0);
    hr = pEnc->SetHighFrequencyMode(3);
    hr = pEnc->SetCopyrightBit(1);
    hr = pEnc->SetOriginalBit(1);
    hr = pEnc->SetChooseBestOutputSampleRate(1);
    hr = pEnc->VerifySettings();
    hr = pEnc->ConvertFile((UCHAR*)"C:\\x.wav", (UCHAR*)"C:\\x.mp3", key);

    pEnc->Release();
    CoUninitialize();

    return 0;
}

*/

#include "pch.h"
#include <Windows.h>
#include "xmencsdk.h"
#include <stdio.h>


void GenerateFinalKey(PBYTE pNewKey, PBYTE pXingKey);
void _GenerateFinalKey(DWORD* pUserKey, DWORD* pXingKey, DWORD* pFinalKey);
void TransformKey(DWORD* pUserKey, DWORD* pXingKey, DWORD* pTransKey);
void __GenerateFinalKey(DWORD* pTransKey, DWORD* pFinalKey);
void InitKeyStruct(DWORD* p);
void UnknownFunction5(DWORD* s, DWORD* pTransKey, DWORD n);
void UnknownFunction6(PDWORD pFinalKey, DWORD* s);
void TheBigAssFunction(DWORD* ps, PBYTE pData);


void GenerateFinalKey(PBYTE pNewKey, PBYTE pXingKey)
{
    DWORD dwUserKey[4] = {0x6c998c0f, 0x32c94e93, 0x1828a515, 0x9955dd8e};
    _GenerateFinalKey(dwUserKey, (PDWORD)pXingKey, (PDWORD)pNewKey);
}


void _GenerateFinalKey(PDWORD pUserKey, PDWORD pXingKey, PDWORD pFinalKey)
{
//    char sz[500];
    DWORD pdwTransKey[4];// 16-bytes large. (DWORD p[4])

    // Creates transforms the original key using the user key
    TransformKey(pUserKey, pXingKey, pdwTransKey);

    // Creates the secondary key.
    __GenerateFinalKey(pdwTransKey, pFinalKey);

}


// pSrc1,pSrc2, and pDestination are DWORD arrays 12 bytes long each.
// Basically pDestination is calculated based on pSrc1 and pSrc2.
// TESTED AND VERIFIED.
void TransformKey(DWORD* pUserKey, DWORD* pXingKey, DWORD* pTransKey)
{
    int i = 0;

    for(i=0;i<4;i++)
    {
        pTransKey[i] = pUserKey[i] ^ (~pXingKey[i]);
    }   
}



/*
    Called by CalculateSecondKey() right after calling the other calculation function.
    bp 00451130
*/
void __GenerateFinalKey(DWORD* pTransKey, DWORD* pFinalKey)
{
    DWORD dwKeyStruct[22] = {0};// 88 bytes long - I think this is some sort of struct or class.

    // Initializes var58 with some weird static numbers
/*
0012f738  00000000 0012f704 0012f70c 0012f714
0012f748  004ecaac 77a7d3f0 00000008 00000000
0012f758  00000001 00000000 00ee6301 004510b4
0012f768  31313332 00000000 00313132 0012f7b4
0012f778  004aff86 004f2098 0012f990 00000000
0012f788  00ee6301 0012f7ec
*/

    //bp 0045113A
    //bp 0045113A "ed 12f738 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0"
    InitKeyStruct(dwKeyStruct);
/*
dd 12f738 l16
0012f738  67452301 efcdab89 98badcfe 10325476
0012f748  00000000 00000000 00000000 00000000
0012f758  00000000 00000000 00000000 00000000
0012f768  00000000 00000000 00000000 00000000
0012f778  00000000 00000000 00000000 00000000
0012f788  00000000 00000000
*/

    // bp 00451148
    UnknownFunction5(dwKeyStruct, pTransKey, 16);

    /*
        Going into the next function, dwKeyStruct should look like this:
0012f738  67452301 efcdab89 98badcfe 10325476
0012f748  00000080 00000000 399cb162 dffbdbe8
0012f758  96c3f20d 00dcae19 00000000 00000000
0012f768  00000000 00000000 00000000 00000000
0012f778  00000000 00000000 00000000 00000000
0012f788  00000000 00000000
    */

    // bp 00451155
    UnknownFunction6(pFinalKey, dwKeyStruct);
}



void InitKeyStruct(DWORD* p)
{
    p[0] = 0x67452301;
    p[1] = 0xEFCDAB89;
    p[2] = 0x98BADCFE;
    p[3] = 0x10325476;
    p[4] = 0x00000000;
    p[5] = 0x00000000;
}


/*
    This seems to do the bulk of stuff
    bp 450F8C
    bp 00450F9B ".echo UnknownFunction5"

    (???, {399cb162 dffbdbe8 96c3f20d 00dcae19}, 0x10)
    (???, {00000080 00000000 00000000 00000000}, 0x28)
    (???, {00000080 00000000}, 0x08)

*/
void UnknownFunction5(DWORD* s, DWORD* pTransKey, DWORD n)
{
    DWORD xeax;
    DWORD xebx;

    xeax = (s[4] >> 3) & 0x3F;
    xebx = 0x40 - xeax;

    s[4] += n * 8;
    if(s[4] < n * 8)
    {
        s[5]++;
    }

    s[5] += n >> 0x1D;

    if(xebx <= n)// ebx = 64, n = 16
    {
        CopyMemory(xeax + (PBYTE)s + 0x18, pTransKey, xebx);
        TheBigAssFunction(s, (PBYTE)&s[6]);// ????

        while(xebx + 0x3F < n)
        {
            TheBigAssFunction(s, (PBYTE)s + 0x18);
            xebx += 0x40;
        }

        xeax = 0;
    }
    else
    {
        xebx = 0;
    }

    CopyMemory((PBYTE)&s[6 + (xeax>>2)], ((PBYTE)pTransKey) + xebx, n - xebx);

    return;
}


// bp 451028
void UnknownFunction6(PDWORD pFinalKey, DWORD* s)
{
    BYTE StaticKeyData[] = {
        0x80,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0};

    DWORD pBytes[2] = {0};
    int nSize;
    DWORD eax;

    // pbytes is dd 0012f720
    CopyMemory(pBytes, &s[4], 8);

    eax = (s[4] >> 3) & 0x3F;

    // bp 0045104D
    if(eax < 0x38)
    {
        nSize = 0x38 - eax;
    }
    else
    {
        nSize = 0x78 - eax;
    }

    UnknownFunction5(s, (DWORD*)StaticKeyData, nSize);
    UnknownFunction5(s, pBytes, 8);
    CopyMemory(pFinalKey, s, 16);

    return;
}

/*

    DWORD ps[4]
    BYTE pData[0x40]

    bp 004504E1
    dd poi(ebp+8) l4
    db poi(ebp+c) l40

    End of function:
    bp 00450E82

    ps:
        67452301 efcdab89 98badcfe 10325476

    pData:
        399cb162 dffbdbe8 96c3f20d 00dcae19
        00000080 00000000 00000000 00000000
        00000000 00000000 00000000 00000000
        00000000 00000000 00000080 00000000

    ps Out:
        4dded0ae 22141c4a 0a07081d 734decb2

*/
void TheBigAssFunction(DWORD* ps, PBYTE pData)
{
    DWORD pTemp[0x10] = {0};
    DWORD dw4;

    RtlCopyMemory(pTemp, pData, 0x40);

    DWORD var_44 = pTemp[0];
    DWORD var_40 = pTemp[1];
    DWORD var_3C = pTemp[2];
    DWORD var_38 = pTemp[3];
    DWORD var_34 = pTemp[4];
    DWORD var_30 = pTemp[5];
    DWORD var_2C = pTemp[6];
    DWORD var_28 = pTemp[7];
    DWORD var_24 = pTemp[8];
    DWORD var_20 = pTemp[9];
    DWORD var_1C = pTemp[10];
    DWORD var_18 = pTemp[11];
    DWORD var_14 = pTemp[12];
    DWORD var_10 = pTemp[13];
    DWORD var_C = pTemp[14];
    DWORD var_8 = pTemp[15];

_asm
    {
        push ebx
        push esi
        push edi
        mov eax, ps
        mov ebx, [eax]
        mov esi, [eax+4]
        mov edi, [eax+8]
        mov edx, [eax+0x0C]
        mov dw4, edx

        mov eax, edi
        and eax, esi
        mov edx, esi
        not edx
        and edx, dw4
        or eax, edx
        add eax, var_44
        add eax, 0xD76AA478
        add ebx, eax
        mov ecx, ebx
        shl ecx, 7
        shr ebx, 0x19
        or ecx, ebx
        mov ebx, ecx
        add ebx, esi
        mov eax, esi
        and eax, ebx
        mov edx, ebx
        not edx
        and edx, edi
        or eax, edx
        add eax, var_40
        add eax, 0x0E8C7B756
        add dw4, eax
        mov ecx, dw4
        shl ecx, 0x0C
        mov eax, dw4
        shr eax, 0x14
        or ecx, eax
        mov dw4, ecx
        add dw4, ebx
        mov edx, dw4
        and edx, ebx
        mov ecx, dw4
        not ecx
        and ecx, esi
        or edx, ecx
        add edx, var_3C
        add edx, 0x242070DB
        add edi, edx
        mov eax, edi
        shl eax, 0x11
        shr edi, 0x0F
        or eax, edi
        mov edi, eax
        add edi, dw4
        mov edx, dw4
        and edx, edi
        mov ecx, edi
        not ecx
        and ecx, ebx
        or edx, ecx
        add edx, var_38
        add edx, 0x0C1BDCEEE
        add esi, edx
        mov eax, esi
        shl eax, 0x16
        shr esi, 0x0A
        or eax, esi
        mov esi, eax
        add esi, edi
        mov edx, edi
        and edx, esi
        mov ecx, esi
        not ecx
        and ecx, dw4
        or edx, ecx
        add edx, var_34
        add edx, 0x0F57C0FAF
        add ebx, edx
        mov eax, ebx
        shl eax, 7
        shr ebx, 0x19
        or eax, ebx
        mov ebx, eax
        add ebx, esi
        mov edx, esi
        and edx, ebx
        mov ecx, ebx
        not ecx
        and ecx, edi
        or edx, ecx
        add edx, var_30
        add edx, 0x4787C62A
        add dw4, edx
        mov eax, dw4
        shl eax, 0x0C
        mov edx, dw4
        shr edx, 0x14
        or eax, edx
        mov dw4, eax
        add dw4, ebx
        mov ecx, dw4
        and ecx, ebx
        mov eax, dw4
        not eax
        and eax, esi
        or ecx, eax
        add ecx, var_2C
        add ecx, 0x0A8304613
        add edi, ecx
        mov edx, edi
        shl edx, 0x11
        shr edi, 0x0F
        or edx, edi
        mov edi, edx
        add edi, dw4
        mov ecx, dw4
        and ecx, edi
        mov eax, edi
        not eax
        and eax, ebx
        or ecx, eax
        add ecx, var_28
        add ecx, 0x0FD469501
        add esi, ecx
        mov edx, esi
        shl edx, 0x16
        shr esi, 0x0A
        or edx, esi
        mov esi, edx
        add esi, edi
        mov ecx, edi
        and ecx, esi
        mov eax, esi
        not eax
        and eax, dw4
        or ecx, eax
        add ecx, var_24
        add ecx, 0x698098D8
        add ebx, ecx
        mov edx, ebx
        shl edx, 7
        shr ebx, 0x19
        or edx, ebx
        mov ebx, edx
        add ebx, esi
        mov ecx, esi
        and ecx, ebx
        mov eax, ebx
        not eax
        and eax, edi
        or ecx, eax
        add ecx, var_20
        add ecx, 0x8B44F7AF
        add dw4, ecx
        mov edx, dw4
        shl edx, 0x0C
        mov ecx, dw4
        shr ecx, 0x14
        or edx, ecx
        mov dw4, edx
        add dw4, ebx
        mov eax, dw4
        and eax, ebx
        mov edx, dw4
        not edx
        and edx, esi
        or eax, edx
        add eax, var_1C
        add eax, 0x0FFFF5BB1
        add edi, eax
        mov ecx, edi
        shl ecx, 0x11
        shr edi, 0x0F
        or ecx, edi
        mov edi, ecx
        add edi, dw4
        mov eax, dw4
        and eax, edi
        mov edx, edi
        not edx
        and edx, ebx
        or eax, edx
        add eax, var_18
        add eax, 0x895CD7BE
        add esi, eax
        mov ecx, esi
        shl ecx, 0x16
        shr esi, 0x0A
        or ecx, esi
        mov esi, ecx
        add esi, edi
        mov eax, edi
        and eax, esi
        mov edx, esi
        not edx
        and edx, dw4
        or eax, edx
        add eax, var_14
        add eax, 0x6B901122
        add ebx, eax
        mov ecx, ebx
        shl ecx, 7
        shr ebx, 0x19
        or ecx, ebx
        mov ebx, ecx
        add ebx, esi
        mov eax, esi
        and eax, ebx
        mov edx, ebx
        not edx
        and edx, edi
        or eax, edx
        add eax, var_10
        add eax, 0x0FD987193
        add dw4, eax
        mov ecx, dw4
        shl ecx, 0x0C
        mov eax, dw4
        shr eax, 0x14
        or ecx, eax
        mov dw4, ecx
        add dw4, ebx
        mov edx, dw4
        and edx, ebx
        mov ecx, dw4
        not ecx
        and ecx, esi
        or edx, ecx
        add edx, var_C
        add edx, 0x0A679438E
        add edi, edx
        mov eax, edi
        shl eax, 0x11
        shr edi, 0x0F
        or eax, edi
        mov edi, eax
        add edi, dw4
        mov edx, dw4
        and edx, edi
        mov ecx, edi
        not ecx
        and ecx, ebx
        or edx, ecx
        add edx, var_8
        add edx, 0x49B40821
        add esi, edx
        mov eax, esi
        shl eax, 0x16
        shr esi, 0x0A
        or eax, esi
        mov esi, eax
        add esi, edi
        mov edx, dw4
        and edx, esi
        mov ecx, dw4
        not ecx
        and ecx, edi
        or edx, ecx
        add edx, var_40
        add edx, 0x0F61E2562
        add ebx, edx
        mov eax, ebx
        shl eax, 5
        shr ebx, 0x1B
        or eax, ebx
        mov ebx, eax
        add ebx, esi
        mov edx, edi
        and edx, ebx
        mov ecx, edi
        not ecx
        and ecx, esi
        or edx, ecx
        add edx, var_2C
        add edx, 0x0C040B340
        add dw4, edx
        mov eax, dw4
        shl eax, 9
        mov edx, dw4
        shr edx, 0x17
        or eax, edx
        mov dw4, eax
        add dw4, ebx
        mov ecx, dw4
        and ecx, esi
        mov eax, esi
        not eax
        and eax, ebx
        or ecx, eax
        add ecx, var_18
        add ecx, 0x265E5A51
        add edi, ecx
        mov edx, edi
        shl edx, 0x0E
        shr edi, 0x12
        or edx, edi
        mov edi, edx
        add edi, dw4
        mov ecx, ebx
        and ecx, edi
        mov eax, ebx
        not eax
        and eax, dw4
        or ecx, eax
        add ecx, var_44
        add ecx, 0x0E9B6C7AA
        add esi, ecx
        mov edx, esi
        shl edx, 0x14
        shr esi, 0x0C
        or edx, esi
        mov esi, edx
        add esi, edi
        mov ecx, dw4
        and ecx, esi
        mov eax, dw4
        not eax
        and eax, edi
        or ecx, eax
        add ecx, var_30
        add ecx, 0x0D62F105D
        add ebx, ecx
        mov edx, ebx
        shl edx, 5
        shr ebx, 0x1B
        or edx, ebx
        mov ebx, edx
        add ebx, esi
        mov ecx, edi
        and ecx, ebx
        mov eax, edi
        not eax
        and eax, esi
        or ecx, eax
        add ecx, var_1C
        add ecx, 0x2441453
        add dw4, ecx
        mov edx, dw4
        shl edx, 9
        mov ecx, dw4
        shr ecx, 0x17
        or edx, ecx
        mov dw4, edx
        add dw4, ebx
        mov eax, dw4
        and eax, esi
        mov edx, esi
        not edx
        and edx, ebx
        or eax, edx
        add eax, var_8
        add eax, 0x0D8A1E681
        add edi, eax
        mov ecx, edi
        shl ecx, 0x0E
        shr edi, 0x12
        or ecx, edi
        mov edi, ecx
        add edi, dw4
        mov eax, ebx
        and eax, edi
        mov edx, ebx
        not edx
        and edx, dw4
        or eax, edx
        add eax, var_34
        add eax, 0x0E7D3FBC8
        add esi, eax
        mov ecx, esi
        shl ecx, 0x14
        shr esi, 0x0C
        or ecx, esi
        mov esi, ecx
        add esi, edi
        mov eax, dw4
        and eax, esi
        mov edx, dw4
        not edx
        and edx, edi
        or eax, edx
        add eax, var_20
        add eax, 0x21E1CDE6
        add ebx, eax
        mov ecx, ebx
        shl ecx, 5
        shr ebx, 0x1B
        or ecx, ebx
        mov ebx, ecx
        add ebx, esi
        mov eax, edi
        and eax, ebx
        mov edx, edi
        not edx
        and edx, esi
        or eax, edx
        add eax, var_C
        add eax, 0x0C33707D6
        add dw4, eax
        mov ecx, dw4
        shl ecx, 9
        mov eax, dw4
        shr eax, 0x17
        or ecx, eax
        mov dw4, ecx
        add dw4, ebx
        mov edx, dw4
        and edx, esi
        mov ecx, esi
        not ecx
        and ecx, ebx
        or edx, ecx
        add edx, var_38
        add edx, 0x0F4D50D87
        add edi, edx
        mov eax, edi
        shl eax, 0x0E
        shr edi, 0x12
        or eax, edi
        mov edi, eax
        add edi, dw4
        mov edx, ebx
        and edx, edi
        mov ecx, ebx
        not ecx
        and ecx, dw4
        or edx, ecx
        add edx, var_24
        add edx, 0x455A14ED
        add esi, edx
        mov eax, esi
        shl eax, 0x14
        shr esi, 0x0C
        or eax, esi
        mov esi, eax
        add esi, edi
        mov edx, dw4
        and edx, esi
        mov ecx, dw4
        not ecx
        and ecx, edi
        or edx, ecx
        add edx, var_10
        add edx, 0x0A9E3E905
        add ebx, edx
        mov eax, ebx
        shl eax, 5
        shr ebx, 0x1B
        or eax, ebx
        mov ebx, eax
        add ebx, esi
        mov edx, edi
        and edx, ebx
        mov ecx, edi
        not ecx
        and ecx, esi
        or edx, ecx
        add edx, var_3C
        add edx, 0x0FCEFA3F8
        add dw4, edx
        mov eax, dw4
        shl eax, 9
        mov edx, dw4
        shr edx, 0x17
        or eax, edx
        mov dw4, eax
        add dw4, ebx
        mov ecx, dw4
        and ecx, esi
        mov eax, esi
        not eax
        and eax, ebx
        or ecx, eax
        add ecx, var_28
        add ecx, 0x676F02D9
        add edi, ecx
        mov edx, edi
        shl edx, 0x0E
        shr edi, 0x12
        or edx, edi
        mov edi, edx
        add edi, dw4
        mov ecx, ebx
        and ecx, edi
        mov eax, ebx
        not eax
        and eax, dw4
        or ecx, eax
        add ecx, var_14
        add ecx, 0x8D2A4C8A
        add esi, ecx
        mov edx, esi
        shl edx, 0x14
        shr esi, 0x0C
        or edx, esi
        mov esi, edx
        add esi, edi
        mov ecx, edi
        xor ecx, esi
        xor ecx, dw4
        add ecx, var_30
        add ecx, 0x0FFFA3942
        add ebx, ecx
        mov eax, ebx
        shl eax, 4
        shr ebx, 0x1C
        or eax, ebx
        mov ebx, eax
        add ebx, esi
        mov edx, esi
        xor edx, ebx
        xor edx, edi
        add edx, var_24
        add edx, 0xc8771F681
        add dw4, edx
        mov ecx, dw4
        shl ecx, 0x0B
        mov eax, dw4
        shr eax, 0x15
        or ecx, eax
        mov dw4, ecx
        add dw4, ebx
        mov edx, dw4
        xor edx, ebx
        xor edx, esi
        add edx, var_18
        add edx, 0x6D9D6122
        add edi, edx
        mov ecx, edi
        shl ecx, 0x10
        shr edi, 0x10
        or ecx, edi
        mov edi, ecx
        add edi, dw4
        mov eax, dw4
        xor eax, edi
        xor eax, ebx
        add eax, var_C
        add eax, 0x0FDE5380C
        add esi, eax
        mov edx, esi
        shl edx, 0x17
        shr esi, 9
        or edx, esi
        mov esi, edx
        add esi, edi
        mov ecx, edi
        xor ecx, esi
        xor ecx, dw4
        add ecx, var_40
        add ecx, 0x0A4BEEA44
        add ebx, ecx
        mov eax, ebx
        shl eax, 4
        shr ebx, 0x1C
        or eax, ebx
        mov ebx, eax
        add ebx, esi
        mov edx, esi
        xor edx, ebx
        xor edx, edi
        add edx, var_34
        add edx, 0x4BDECFA9
        add dw4, edx
        mov ecx, dw4
        shl ecx, 0x0B
        mov eax, dw4
        shr eax, 0x15
        or ecx, eax
        mov dw4, ecx
        add dw4, ebx
        mov edx, dw4
        xor edx, ebx
        xor edx, esi
        add edx, var_28
        add edx, 0x0F6BB4B60
        add edi, edx
        mov ecx, edi
        shl ecx, 0x10
        shr edi, 0x10
        or ecx, edi
        mov edi, ecx
        add edi, dw4
        mov eax, dw4
        xor eax, edi
        xor eax, ebx
        add eax, var_1C
        add eax, 0x0BEBFBC70
        add esi, eax
        mov edx, esi
        shl edx, 0x17
        shr esi, 9
        or edx, esi
        mov esi, edx
        add esi, edi
        mov ecx, edi
        xor ecx, esi
        xor ecx, dw4
        add ecx, var_10
        add ecx, 0x289B7EC6
        add ebx, ecx
        mov eax, ebx
        shl eax, 4
        shr ebx, 0x1C
        or eax, ebx
        mov ebx, eax
        add ebx, esi
        mov edx, esi
        xor edx, ebx
        xor edx, edi
        add edx, var_44
        add edx, 0x0EAA127FA
        add dw4, edx
        mov ecx, dw4
        shl ecx, 0x0B
        mov eax, dw4
        shr eax, 0x15
        or ecx, eax
        mov dw4, ecx
        add dw4, ebx
        mov edx, dw4
        xor edx, ebx
        xor edx, esi
        add edx, var_38
        add edx, 0x0D4EF3085
        add edi, edx
        mov ecx, edi
        shl ecx, 0x10
        shr edi, 0x10
        or ecx, edi
        mov edi, ecx
        add edi, dw4
        mov eax, dw4
        xor eax, edi
        xor eax, ebx
        add eax, var_2C
        add eax, 0x4881D05
        add esi, eax
        mov edx, esi
        shl edx, 0x17
        shr esi, 9
        or edx, esi
        mov esi, edx
        add esi, edi
        mov ecx, edi
        xor ecx, esi
        xor ecx, dw4
        add ecx, var_20
        add ecx, 0x0D9D4D039
        add ebx, ecx
        mov eax, ebx
        shl eax, 4
        shr ebx, 0x1C
        or eax, ebx
        mov ebx, eax
        add ebx, esi
        mov edx, esi
        xor edx, ebx
        xor edx, edi
        add edx, var_14
        add edx, 0x0E6DB99E5
        add dw4, edx
        mov ecx, dw4
        shl ecx, 0x0B
        mov eax, dw4
        shr eax, 0x15
        or ecx, eax
        mov dw4, ecx
        add dw4, ebx
        mov edx, dw4
        xor edx, ebx
        xor edx, esi
        add edx, var_8
        add edx, 0x1FA27CF8
        add edi, edx
        mov ecx, edi
        shl ecx, 0x10
        shr edi, 0x10
        or ecx, edi
        mov edi, ecx
        add edi, dw4
        mov eax, dw4
        xor eax, edi
        xor eax, ebx
        add eax, var_3C
        add eax, 0x0C4AC5665
        add esi, eax
        mov edx, esi
        shl edx, 0x17
        shr esi, 9
        or edx, esi
        mov esi, edx
        add esi, edi
        mov ecx, dw4
        not ecx
        or ecx, esi
        xor ecx, edi
        add ecx, var_44
        add ecx, 0x0F4292244
        add ebx, ecx
        mov eax, ebx
        shl eax, 6
        shr ebx, 0x1A
        or eax, ebx
        mov ebx, eax
        add ebx, esi
        mov edx, edi
        not edx
        or edx, ebx
        xor edx, esi
        add edx, var_28
        add edx, 0x432AFF97
        add dw4, edx
        mov ecx, dw4
        shl ecx, 0x0A
        mov eax, dw4
        shr eax, 0x16
        or ecx, eax
        mov dw4, ecx
        add dw4, ebx
        mov edx, esi
        not edx
        or edx, dw4
        xor edx, ebx
        add edx, var_C
        add edx, 0x0AB9423A7
        add edi, edx
        mov ecx, edi
        shl ecx, 0x0F
        shr edi, 0x11
        or ecx, edi
        mov edi, ecx
        add edi, dw4
        mov eax, ebx
        not eax
        or eax, edi
        xor eax, dw4
        add eax, var_30
        add eax, 0x0FC93A039
        add esi, eax
        mov edx, esi
        shl edx, 0x15
        shr esi, 0x0B
        or edx, esi
        mov esi, edx
        add esi, edi
        mov ecx, dw4
        not ecx
        or ecx, esi
        xor ecx, edi
        add ecx, var_14
        add ecx, 0x655B59C3
        add ebx, ecx
        mov eax, ebx
        shl eax, 6
        shr ebx, 0x1A
        or eax, ebx
        mov ebx, eax
        add ebx, esi
        mov edx, edi
        not edx
        or edx, ebx
        xor edx, esi
        add edx, var_38
        add edx, 0x8F0CCC92
        add dw4, edx
        mov ecx, dw4
        shl ecx, 0x0A
        mov eax, dw4
        shr eax, 0x16
        or ecx, eax
        mov dw4, ecx
        add dw4, ebx
        mov edx, esi
        not edx
        or edx, dw4
        xor edx, ebx
        add edx, var_1C
        add edx, 0x0FFEFF47D
        add edi, edx
        mov ecx, edi
        shl ecx, 0x0F
        shr edi, 0x11
        or ecx, edi
        mov edi, ecx
        add edi, dw4
        mov eax, ebx
        not eax
        or eax, edi
        xor eax, dw4
        add eax, var_40
        add eax, 0x85845DD1
        add esi, eax
        mov edx, esi
        shl edx, 0x15
        shr esi, 0x0B
        or edx, esi
        mov esi, edx
        add esi, edi
        mov ecx, dw4
        not ecx
        or ecx, esi
        xor ecx, edi
        add ecx, var_24
        add ecx, 0x6FA87E4F
        add ebx, ecx
        mov eax, ebx
        shl eax, 6
        shr ebx, 0x1A
        or eax, ebx
        mov ebx, eax
        add ebx, esi
        mov edx, edi
        not edx
        or edx, ebx
        xor edx, esi
        add edx, var_8
        add edx, 0x0FE2CE6E0
        add dw4, edx
        mov ecx, dw4
        shl ecx, 0x0A
        mov eax, dw4
        shr eax, 0x16
        or ecx, eax
        mov dw4, ecx
        add dw4, ebx
        mov edx, esi
        not edx
        or edx, dw4
        xor edx, ebx
        add edx, var_2C
        add edx, 0x0A3014314
        add edi, edx
        mov ecx, edi
        shl ecx, 0x0F
        shr edi, 0x11
        or ecx, edi
        mov edi, ecx
        add edi, dw4
        mov eax, ebx
        not eax
        or eax, edi
        xor eax, dw4
        add eax, var_10
        add eax, 0x4E0811A1
        add esi, eax
        mov edx, esi
        shl edx, 0x15
        shr esi, 0x0B
        or edx, esi
        mov esi, edx
        add esi, edi
        mov ecx, dw4
        not ecx
        or ecx, esi
        xor ecx, edi
        add ecx, var_34
        add ecx, 0x0F7537E82
        add ebx, ecx
        mov eax, ebx
        shl eax, 6
        shr ebx, 0x1A
        or eax, ebx
        mov ebx, eax
        add ebx, esi
        mov edx, edi
        not edx
        or edx, ebx
        xor edx, esi
        add edx, var_18
        add edx, 0x0BD3AF235
        add dw4, edx
        mov ecx, dw4
        shl ecx, 0x0A
        mov eax, dw4
        shr eax, 0x16
        or ecx, eax
        mov dw4, ecx
        add dw4, ebx
        mov edx, esi
        not edx
        or edx, dw4
        xor edx, ebx
        add edx, var_3C
        add edx, 0x2AD7D2BB
        add edi, edx
        mov ecx, edi
        shl ecx, 0x0F
        shr edi, 0x11
        or ecx, edi
        mov edi, ecx
        add edi, dw4
        mov eax, ebx
        not eax
        or eax, edi
        xor eax, dw4
        add eax, var_20
        add eax, 0x0EB86D391
        add esi, eax
        mov edx, esi
        shl edx, 0x15
        shr esi, 0x0B
        or edx, esi
        mov esi, edx
        add esi, edi
        mov ecx, ps
        add [ecx], ebx
        mov eax, ps
        add [eax+4], esi
        mov edx, ps
        add [edx+8], edi
        mov ecx, ps
        mov eax, dw4
        add [ecx+0xC], eax

        pop edi
        pop esi
        pop ebx
    }

    FillMemory(&pTemp, 0, 0x40);
}



XingKey::XingKey()
{
    this->bHaveKey = FALSE;
    this->bGeneratedKey = FALSE;
}

XingKey::~XingKey()
{
}

/*
    This will either return the Source key or the Destination key.
    The first time we will return the source key.  That will receive the
    key.

    The next time, we will generate the dest key and return it.

    From there on we will just return the destination key.
*/
XingKey::operator PBYTE()
{
    if(this->bHaveKey == FALSE)
    {
        this->bHaveKey = TRUE;
        return this->pSrcKey;
    }

    if(this->bGeneratedKey == FALSE)
    {
        this->bGeneratedKey = TRUE;
        GenerateFinalKey(this->pDestKey, this->pSrcKey);
    }

    return this->pDestKey;
}


