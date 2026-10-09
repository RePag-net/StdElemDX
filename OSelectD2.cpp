/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OTextD2.cpp
For more information see https://github.com/RePag-net/StdElemDX

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/
#include "HStdElemDX.h"
#include "OSelectD2.h"
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSelect::COSelectV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName,
														_In_ unsigned int uiIDElementA, _In_ STDeviceResources* pstDeviceResourcesA)
{
	COTextV(vmMemory, pcClassName, pcWindowName, uiIDElementA, pstDeviceResourcesA);

	ptfCaret = {0};
	ucCaretStrength = 1;
	cSelect = 0;
	bDoNotCopy = false;
	ulCharacterPos = 0;
	ucCharacterSpecification = ZV_ALLE;
	crfSelectText = D2D1::ColorF(RGB(0, 0, 0), 1.0f);
	crfSelectBack = D2D1::ColorF(RGB(215, 215, 0), 1.0f);
	crfCaret = D2D1::ColorF(RGB(255, 255, 255), 1.0f);
	htCaret = nullptr;
	heCaret = CreateEvent(nullptr, true, true, nullptr);
	ifSelectBackColor = nullptr;
	ifCaretColor = nullptr;

	hMenu = CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING, IDM_CUT, "Cut		Crtl+X");
	AppendMenu(hMenu, MF_STRING, IDM_COPY, "Copy		Crtl+C");
	AppendMenu(hMenu, MF_STRING, IDM_PASTE, "Paste		Crtl+V");
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COSelect::CharacterCheck(_In_ WPARAM wParam)
{
	for(BYTE ucBit = 0; ucBit < 4; ucBit++){
		switch(ucBit){
			case 0: if(ucCharacterSpecification & (1 << ucBit)){
				if(wParam >= 0x41 && wParam <= 0x5a || wParam >= 0x61 && wParam <= 0x7a || wParam == 0x20) return true;
			} break;
			case 1: if(ucCharacterSpecification & (1 << ucBit)){
				if(wParam >= 0x30 && wParam <= 0x39) return true;
			} break;
			case 2: if(ucCharacterSpecification & (1 << ucBit)){
				if(wParam >= 0x80 && wParam <= 0xff) return true;
			} break;
			case 3: if(ucCharacterSpecification & (1 << ucBit)){
				if(wParam >= 0x20 && wParam <= 0x2f || wParam >= 0x3a && wParam <= 0x40 ||
					wParam >= 0x5b && wParam <= 0x60 || wParam >= 0x7b && wParam <= 0x7f) return true;
			} break;
		}
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COSelect::GetTextPoint(_In_ char* pcText, _In_ unsigned long ulTextLength, _Out_ D2D_SIZE_F& szfTextPoint)
{
	IDWriteTextLayout* ifTextLayout; DWRITE_TEXT_METRICS stTextMetrics;
	size_t szBytes_Text; WCHAR wc255Content[255];
	if(mbstowcs_s(&szBytes_Text, wc255Content, 255, pcText, ulTextLength)) return false;
	pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wc255Content, (UINT)szBytes_Text, ifText, fTextLine_maxwidth, (float)lHeight, &ifTextLayout);
	ifTextLayout->GetMetrics(&stTextMetrics);
	SafeRelease(&ifTextLayout);

	szfTextPoint.width = stTextMetrics.width;
	szfTextPoint.height = stTextMetrics.height;
	return true;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
inline long __vectorcall RePag::DirectX::COSelect::FloatToLong(_In_ float fNumber)
{
	long lZahl = (long)fNumber;
	fNumber -= (float)lZahl;
	if(fNumber >= 0.5f) lZahl++;
	return lZahl;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSelect::CaretStrength(_In_ BYTE ucCaretStrengthA)
{
	ThreadSafe_Begin();
	ucCaretStrength = ucCaretStrengthA;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSelect::SetSelectTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																																 _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfSelectText = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSelect::SetSelectTextColor(_In_ D2D1_COLOR_F& crfSelectTextA)
{
	ThreadSafe_Begin();
	crfSelectText = crfSelectTextA;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSelect::SetSelectBackgroundColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																																			 _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfSelectBack = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	if(ifSelectBackColor) ifSelectBackColor->SetColor(crfSelectBack);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSelect::SetSelectBackgroundColor(_In_ D2D1_COLOR_F& crfSelectBackA)
{
	ThreadSafe_Begin();
	crfSelectBack = crfSelectBackA;
	if(ifSelectBackColor) ifSelectBackColor->SetColor(crfSelectBack);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSelect::SetCaretColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																														_In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfCaret = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	if(ifCaretColor) ifCaretColor->SetColor(crfCaret);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSelect::SetCaretColor(_In_ D2D1_COLOR_F& crfCaretA)
{
	ThreadSafe_Begin();
	crfCaret = crfCaretA;
	if(ifCaretColor) ifCaretColor->SetColor(crfCaret);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSelect::DoNotCopy(_In_ bool bDoNotCopyA)
{
	ThreadSafe_Begin();
	bDoNotCopy = bDoNotCopyA;
	ThreadSafe_End();
}