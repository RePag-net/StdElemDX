/******************************************************************************
MIT License

Copyright(c) 2025 René Pagel

Filename: OTextLineD2.cpp
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
#include "OTextLineD2.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COTextLine* __vectorcall RePag::DirectX::COTextLineV(_In_z_ const char* pcFensterName, _In_ unsigned int uiIDElement,
																														 _In_ STDeviceResources* pstDeviceResourcesA)
{
	COTextLine* vTextZeile = (COTextLine*)VMBlock(VMDialog(), sizeof(COTextLine));
	vTextZeile->COTextLineV(VMDialog(), pcFensterName, uiIDElement, pstDeviceResourcesA);
	return vTextZeile;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COTextLine* __vectorcall RePag::DirectX::COTextLineV(_In_ const VMEMORY vmSpeicher, _In_z_ const char* pcFensterName, _In_ unsigned int uiIDElement,
																														 _In_ STDeviceResources* pstDeviceResourcesA)
{
	COTextLine* vTextZeile = (COTextLine*)VMBlock(vmSpeicher, sizeof(COTextLine));
	vTextZeile->COTextLineV(vmSpeicher, pcFensterName, uiIDElement, pstDeviceResourcesA);
	return vTextZeile;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_TextLine(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	COTextLine* pTextZeile;
	switch(uiMessage){
		case WM_CREATE		: ((COTextLine*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
												((COTextLine*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
												return NULL;
		case WM_SIZE			:	pTextZeile = (COTextLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
												if(pTextZeile) pTextZeile->WM_Size_Element(hWnd, lParam);
												else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
												return NULL;
		case WM_NCDESTROY :	pTextZeile = (COTextLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
												if(pTextZeile->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pTextZeile->htEffekt_Timer, INVALID_HANDLE_VALUE);
												VMFreiV(pTextZeile);
												return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::COTextLineV(_In_ const VMEMORY vmSpeicher, _In_z_ const char* pcKlassenName, _In_z_ const char* pcFensterName,
																											_In_ unsigned int uiIDElementA,	_In_ STDeviceResources* pstDeviceResourcesA)
{
	COGraphicV(vmSpeicher, pcKlassenName, pcFensterName, uiIDElementA, pstDeviceResourcesA);

	//hFont = nullptr;
	//lfSchrift.lfHeight = -13;
	//lfSchrift.lfWidth = 0;
	//lfSchrift.lfFaceName[0] = '\0';
	//lfSchrift.lfEscapement = GM_COMPATIBLE;
	//lfSchrift.lfOrientation = GM_COMPATIBLE;
	//lfSchrift.lfItalic = FALSE;
	//lfSchrift.lfUnderline = FALSE;
	//lfSchrift.lfStrikeOut = FALSE;
	//lfSchrift.lfWeight = FW_DONTCARE;
	//lfSchrift.lfCharSet = DEFAULT_CHARSET;
	//lfSchrift.lfOutPrecision = OUT_DEVICE_PRECIS;
	//lfSchrift.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	//lfSchrift.lfQuality = DEFAULT_QUALITY;
	//lfSchrift.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

	vasInhalt = COStringAV(vmSpeicher);

	//stTextColor.r = stTextColor.g = stTextColor.b = 0.0f; stTextColor.a = 1.0f;
	crfText = D2D1::ColorF(RGB(0, 0, 0), 1.0f);
	ucSchriftausrichtung = TXA_LINKS | TXA_MITTEVERTICAL;

	pstDeviceResources->ifdwriteFactory7->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
																												 13.0f, L"de-DE", &ifText);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::COTextLineV(_In_ const VMEMORY vmSpeicher, _In_z_ const char* pcFensterName, _In_ unsigned int uiIDElementA,
																											_In_ STDeviceResources* pstDeviceResourcesA)
{
	COTextLineV(vmSpeicher, pcRePag_TextLine, pcFensterName, uiIDElementA, pstDeviceResourcesA);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COTextLine::COFreiV(void)
{
	SafeRelease(&ifText); SafeRelease(&ifTextColor);
	VMFreiV(vasInhalt);
	//if(hFont) DeleteObject(hFont);
	return ((COElement*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::OnRender(void)
{
	IDWriteTextLayout* ifTextLayout; float fTextWidth; D2D1_RECT_F rcfText;	size_t szBytes_Text; WCHAR wcInhalt[255];

	WaitForSingleObjectEx(heRender, INFINITE, false);
	mbstowcs_s(&szBytes_Text, wcInhalt, vasInhalt->c_Str(), vasInhalt->Length());
	pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wcInhalt, szBytes_Text, ifText, (float)lWidth, (float)lHeight, &ifTextLayout);
	TextAlignment(ifTextLayout, fTextWidth, rcfText);
	SafeRelease(&ifTextLayout);

	ifD2D1Context6->BeginDraw();
	ifD2D1Context6->Clear(crfBackground);
	ifD2D1Context6->DrawText(wcInhalt, szBytes_Text, ifText, rcfText, ifTextColor);
	ifD2D1Context6->EndDraw();
	SetEvent(heRender);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::OnPaint(void)
{
	ThreadSafe_Begin();
	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::WM_Create(void)
{
	CharacterMetric();
	ifD2D1Context6->CreateSolidColorBrush(crfText, &ifTextColor);

	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::CharacterMetric(void)
{
	//TEXTMETRIC stTextmetric;
	//HDC hdc = GetDC(hWndElement);
	//if(hFont) DeleteObject(hFont);
	//hFont = CreateFontIndirect(&lfSchrift);	
	//SelectObject(hdc, hFont);
	//GetTextMetrics(hdc, &stTextmetric);
	//lZeichen_mittel = stTextmetric.tmAveCharWidth;
	////lZeichen_hohe = stTextmetric.tmHeight + stTextmetric.tmExternalLeading;
	//ReleaseDC(hWndElement, hdc);

	IDWriteTextLayout* ifTextLayout; DWRITE_TEXT_METRICS stTextMetrics;
	//size_t szBytes_Text; WCHAR wcInhalt[255];
	//if(vasInhalt->Length()) mbstowcs_s(&szBytes_Text, wcInhalt, vasInhalt->c_Str(), vasInhalt->Length());
	//else mbstowcs_s(&szBytes_Text, wcInhalt, "Text", 5);
	//pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wcInhalt, szBytes_Text, ifText, (float)lWidth, (float)lHeight, &ifTextLayout);
	//ifTextLayout->GetMetrics(&stTextMetrics);
	//lZeichen_hohe = (long)stTextMetrics.height;

	pstDeviceResources->ifdwriteFactory7->CreateTextLayout(L"888lllBBBIIIiiiaaaAAATTTttt", 28, ifText, (float)lWidth, (float)lHeight, &ifTextLayout);
	ifTextLayout->GetMetrics(&stTextMetrics);
	lZeichen_hohe = (long)stTextMetrics.height;
	stTextMetrics.height -= (float)lZeichen_hohe;
	if(stTextMetrics.height >= 0.5f) lZeichen_hohe += 1;
	lZeichen_mittel = (long)(stTextMetrics.width / 27.0f);
	SafeRelease(&ifTextLayout);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
//void __vectorcall RePag::DirectX::COTextLine::Schriftausrichtung(HDC hdc, SIZE& stZeichengrosse, POINT& ptText)
//{
//	TEXTMETRIC stTextmetric;
//	GetTextMetrics(hdc, &stTextmetric);
//	GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
//	ptText.x = ptText.y = 0;
//	if(ucSchriftausrichtung & TXA_RECHTS) ptText.x = lWidth - stZeichengrosse.cx;
//	if(ucSchriftausrichtung & TXA_MITTEHORIZONTAL) ptText.x = (lWidth - stZeichengrosse.cx) / 2;
//	if(ucSchriftausrichtung & TXA_UNTEN) ptText.y = lHeight - stTextmetric.tmHeight + stTextmetric.tmExternalLeading;
//	if(ucSchriftausrichtung & TXA_MITTEVERTICAL) ptText.y = (lHeight - stTextmetric.tmHeight + stTextmetric.tmExternalLeading) / 2;
//}
////-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::TextAlignment(_In_ IDWriteTextLayout* ifTextLayout, _Out_ float& fTextWidth, _Out_ D2D1_POINT_2F& ptfText)
{
	DWRITE_TEXT_METRICS stTextMetrics;
	ifTextLayout->GetMetrics(&stTextMetrics);
	ptfText.x = ptfText.y = 0; fTextWidth = stTextMetrics.width;
	if(ucSchriftausrichtung & TXA_RECHTS) ptfText.x = (float)lWidth - stTextMetrics.width;
	if(ucSchriftausrichtung & TXA_MITTEHORIZONTAL) ptfText.x = ((float)lWidth - stTextMetrics.width) / 2.0f;
	if(ucSchriftausrichtung & TXA_UNTEN) ptfText.y = (float)lHeight - stTextMetrics.height;
	if(ucSchriftausrichtung & TXA_MITTEVERTICAL) ptfText.y = ((float)lHeight - stTextMetrics.height) / 2.0f;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::TextAlignment(_In_ IDWriteTextLayout* ifTextLayout, _Out_ float& fTextWidth, _Out_ D2D1_RECT_F& rcfText)
{
	DWRITE_TEXT_METRICS stTextMetrics;
	ifTextLayout->GetMetrics(&stTextMetrics);
	rcfText.top = rcfText.left = 0; fTextWidth = stTextMetrics.width;
	if(ucSchriftausrichtung & TXA_RECHTS) rcfText.left = (float)lWidth - stTextMetrics.width;
	if(ucSchriftausrichtung & TXA_MITTEHORIZONTAL) rcfText.left = ((float)lWidth - stTextMetrics.width) / 2.0f;
	if(ucSchriftausrichtung & TXA_UNTEN) rcfText.top = (float)lHeight - stTextMetrics.height;
	if(ucSchriftausrichtung & TXA_MITTEVERTICAL) rcfText.top = ((float)lHeight - stTextMetrics.height) / 2.0f;
	rcfText.right = rcfText.left + stTextMetrics.width; rcfText.bottom = rcfText.top + stTextMetrics.height;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::Text(_In_z_ char* pcText)
{
	ThreadSafe_Begin();
	*vasInhalt = pcText; 
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::Font(STFont& stFont)
{
	ThreadSafe_Begin();
	SafeRelease(&ifText);
	pstDeviceResources->ifdwriteFactory7->CreateTextFormat(stFont.fontFamilyName, stFont.fontCollection, stFont.fontWeight, stFont.fontSytle,
																												 stFont.fontStretch, stFont.fontSize, stFont.localeName, &ifText);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::DirectX::COTextLine::Content(_Out_ COStringA* vasInhaltA)
{
	ThreadSafe_Begin();
	*vasInhaltA = *vasInhalt;
	ThreadSafe_End();
	return vasInhaltA;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::SetTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfText = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifTextColor) ifTextColor->SetColor(crfText);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::SetTextColor(_In_ D2D1_COLOR_F& stTextA)
{
	ThreadSafe_Begin();
	crfText = stTextA;
	if(ifTextColor) ifTextColor->SetColor(crfText);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::TextAlignment(_In_ unsigned char ucTextAlignment)
{
	ThreadSafe_Begin();
	ucSchriftausrichtung = ucTextAlignment;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------