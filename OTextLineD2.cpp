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
RePag::DirectX::COTextLine* __vectorcall RePag::DirectX::COTextLineV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																														 _In_ STDeviceResources* pstDeviceResourcesA)
{
	COTextLine* vTextZeile = (COTextLine*)VMBlock(VMDialog(), sizeof(COTextLine));
	vTextZeile->COTextLineV(VMDialog(), pcWindowName, uiIDElement, pstDeviceResourcesA);
	return vTextZeile;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COTextLine* __vectorcall RePag::DirectX::COTextLineV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																														 _In_ STDeviceResources* pstDeviceResourcesA)
{
	COTextLine* vTextZeile = (COTextLine*)VMBlock(vmMemory, sizeof(COTextLine));
	vTextZeile->COTextLineV(vmMemory, pcWindowName, uiIDElement, pstDeviceResourcesA);
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
void __vectorcall RePag::DirectX::COTextLine::COTextLineV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName,
																											_In_ unsigned int uiIDElementA,	_In_ STDeviceResources* pstDeviceResourcesA)
{
	COGraphicV(vmMemory, pcClassName, pcWindowName, uiIDElementA, pstDeviceResourcesA);

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

	vasContent = COStringAV(vmMemory);

	//stTextColor.r = stTextColor.g = stTextColor.b = 0.0f; stTextColor.a = 1.0f;
	crfText = D2D1::ColorF(RGB(0, 0, 0), 1.0f);
	ucTextAlignment = TXA_LINKS | TXA_MITTEVERTICAL;

	pstDeviceResources->ifdwriteFactory7->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
																												 13.0f, L"de-DE", &ifText);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::COTextLineV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																											_In_ STDeviceResources* pstDeviceResourcesA)
{
	COTextLineV(vmMemory, pcRePag_TextLine, pcWindowName, uiIDElementA, pstDeviceResourcesA);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COTextLine::COFreiV(void)
{
	SafeRelease(&ifText); SafeRelease(&ifTextColor);
	VMFreiV(vasContent);
	//if(hFont) DeleteObject(hFont);
	return ((COElement*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::OnRender(void)
{
	IDWriteTextLayout* ifTextLayout; float fTextWidth; D2D1_RECT_F rcfText;	size_t szBytes_Text; WCHAR wcInhalt[255];

	WaitForSingleObjectEx(heRender, INFINITE, false);
	mbstowcs_s(&szBytes_Text, wcInhalt, vasContent->c_Str(), vasContent->Length());
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
	IDWriteTextLayout* ifTextLayout; DWRITE_TEXT_METRICS stTextMetrics;
	pstDeviceResources->ifdwriteFactory7->CreateTextLayout(L"888lllBBBIIIiiiaaaAAATTTttt", 28, ifText, (float)lWidth, (float)lHeight, &ifTextLayout);
	ifTextLayout->GetMetrics(&stTextMetrics);
	szfSign.height = stTextMetrics.height;
	szfSign.width = stTextMetrics.width / 27.0f;
	fTextLine_maxwidth = stTextMetrics.width / 27.0f * 255.0f;
	SafeRelease(&ifTextLayout);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::TextAlignment(_In_ IDWriteTextLayout* ifTextLayout, _Out_ float& fTextWidth, _Out_ D2D1_POINT_2F& ptfText)
{
	DWRITE_TEXT_METRICS stTextMetrics;
	ifTextLayout->GetMetrics(&stTextMetrics);
	ptfText.x = ptfText.y = 0; fTextWidth = stTextMetrics.width;
	if(ucTextAlignment & TXA_RECHTS) ptfText.x = (float)lWidth - stTextMetrics.width;
	if(ucTextAlignment & TXA_MITTEHORIZONTAL) ptfText.x = ((float)lWidth - stTextMetrics.width) / 2.0f;
	if(ucTextAlignment & TXA_UNTEN) ptfText.y = (float)lHeight - stTextMetrics.height;
	if(ucTextAlignment & TXA_MITTEVERTICAL) ptfText.y = ((float)lHeight - stTextMetrics.height) / 2.0f;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::TextAlignment(_In_ IDWriteTextLayout* ifTextLayout, _Out_ float& fTextWidth, _Out_ D2D1_RECT_F& rcfText)
{
	DWRITE_TEXT_METRICS stTextMetrics;
	ifTextLayout->GetMetrics(&stTextMetrics);
	rcfText.top = rcfText.left = 0; fTextWidth = stTextMetrics.width;
	if(ucTextAlignment & TXA_RECHTS) rcfText.left = (float)lWidth - stTextMetrics.width;
	if(ucTextAlignment & TXA_MITTEHORIZONTAL) rcfText.left = ((float)lWidth - stTextMetrics.width) / 2.0f;
	if(ucTextAlignment & TXA_UNTEN) rcfText.top = (float)lHeight - stTextMetrics.height;
	if(ucTextAlignment & TXA_MITTEVERTICAL) rcfText.top = ((float)lHeight - stTextMetrics.height) / 2.0f;
	rcfText.right = rcfText.left + stTextMetrics.width; rcfText.bottom = rcfText.top + stTextMetrics.height;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::Text(_In_z_ char* pcText)
{
	ThreadSafe_Begin();
	*vasContent = pcText; 
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
COStringA* __vectorcall RePag::DirectX::COTextLine::Content(_Out_ COStringA* vasContentA)
{
	ThreadSafe_Begin();
	*vasContentA = *vasContent;
	ThreadSafe_End();
	return vasContentA;
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
	ucTextAlignment = ucTextAlignment;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------