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
#include "OTextD2.h"
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COText::COTextV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName,
													_In_ unsigned int uiIDElementA, _In_ STDeviceResources* pstDeviceResourcesA)
{
	COGraphicV(vmMemory, pcClassName, pcWindowName, uiIDElementA, pstDeviceResourcesA);

	vasContent = COStringAV(vmMemory);
	crfText = D2D1::ColorF(RGB(0, 0, 0), 1.0f);
	ucTextAlignment = TXA_LEFT | TXA_CENTERVERTICAL;

	pstDeviceResources->ifdwriteFactory7->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
																												 13.0f, L"de-DE", &ifText);
}
//---------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COText::COFreiV(void)
{
	SafeRelease(&ifText);
	VMFreiV(vasContent);
	return ((COElement*)this)->COFreiV();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COText::CharacterMetric(void)
{
	IDWriteTextLayout* ifTextLayout; DWRITE_TEXT_METRICS stTextMetrics;
	pstDeviceResources->ifdwriteFactory7->CreateTextLayout(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
																												 63, ifText, 5000.0f, (float)lHeight, &ifTextLayout);
	ifTextLayout->GetMetrics(&stTextMetrics);
	szfCharacter.height = stTextMetrics.height;
	szfCharacter.width = stTextMetrics.width / 62.0f;
	fTextLine_maxwidth = stTextMetrics.width / 62.0f * 255.0f;
	SafeRelease(&ifTextLayout);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COText::SetFont(STFont& stFont)
{
	ThreadSafe_Begin();
	SafeRelease(&ifText);
	pstDeviceResources->ifdwriteFactory7->CreateTextFormat(stFont.fontFamilyName, stFont.fontCollection, stFont.fontWeight, stFont.fontSytle,
																												 stFont.fontStretch, stFont.fontSize, stFont.localeName, &ifText);
	CharacterMetric();
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COText::SetTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfText = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	if(ifTextColor) ifTextColor->SetColor(crfText);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COText::SetTextColor(_In_ D2D1_COLOR_F& stTextA)
{
	ThreadSafe_Begin();
	crfText = stTextA;
	if(ifTextColor) ifTextColor->SetColor(crfText);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COText::TextAlignment(_In_ IDWriteTextLayout* ifTextLayout, _Out_ float& fTextWidth, _Out_ D2D1_POINT_2F& ptfText)
{
	DWRITE_TEXT_METRICS stTextMetrics;
	ifTextLayout->GetMetrics(&stTextMetrics);
	ptfText.x = ptfText.y = 0; fTextWidth = stTextMetrics.width;
	if(ucTextAlignment & TXA_RIGHT) ptfText.x = (float)lWidth - stTextMetrics.width;
	if(ucTextAlignment & TXA_CENTERHORIZONTAL) ptfText.x = ((float)lWidth - stTextMetrics.width) / 2.0f;
	if(ucTextAlignment & TXA_BOTTOM) ptfText.y = (float)lHeight - stTextMetrics.height;
	if(ucTextAlignment & TXA_CENTERVERTICAL) ptfText.y = ((float)lHeight - stTextMetrics.height) / 2.0f;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COText::TextAlignment(_In_ IDWriteTextLayout* ifTextLayout, _Out_ float& fTextWidth, _Out_ D2D1_RECT_F& rcfText)
{
	DWRITE_TEXT_METRICS stTextMetrics;
	ifTextLayout->GetMetrics(&stTextMetrics);
	rcfText.top = rcfText.left = 0; fTextWidth = stTextMetrics.width;
	if(ucTextAlignment & TXA_RIGHT) rcfText.left = (float)lWidth - stTextMetrics.width;
	if(ucTextAlignment & TXA_CENTERHORIZONTAL) rcfText.left = ((float)lWidth - stTextMetrics.width) / 2.0f;
	if(ucTextAlignment & TXA_BOTTOM) rcfText.top = (float)lHeight - stTextMetrics.height;
	if(ucTextAlignment & TXA_CENTERVERTICAL) rcfText.top = ((float)lHeight - stTextMetrics.height) / 2.0f;
	rcfText.right = rcfText.left + stTextMetrics.width; rcfText.bottom = rcfText.top + stTextMetrics.height;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COText::TextAlignment(_In_ unsigned char ucTextAlignmentA)
{
	ThreadSafe_Begin();
	ucTextAlignment = ucTextAlignmentA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------