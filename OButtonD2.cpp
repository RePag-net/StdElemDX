/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OButtonD2.cpp
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
#include "OButtonD2.h"

//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COButton* __vectorcall RePag::DirectX::COButtonV(_In_z_ const char* pcFensterName, _In_ unsigned int uiIDElement,
																																 _In_ STDeviceResources* pstDeviceResources)
{
	COButton* vButton = (COButton*)VMBlock(VMDialog(), sizeof(COButton));
	vButton->COButtonV(VMDialog(), pcFensterName, uiIDElement, pstDeviceResources);
	return vButton;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COButton* __vectorcall RePag::DirectX::COButtonV(_In_ VMEMORY vmSpeicher, _In_z_ const char* pcFensterName, _In_ unsigned int uiIDElement,
																																 _In_ STDeviceResources* pstDeviceResources)
{
	COButton* vButton = (COButton*)VMBlock(vmSpeicher, sizeof(COButton));
	vButton->COButtonV(vmSpeicher, pcFensterName, uiIDElement, pstDeviceResources);
	return vButton;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_Button(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	COButton* pButton;
	switch(uiMessage){
		case WM_CREATE			:	((COButton*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
													((COButton*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
													return NULL;
		case WM_SIZE				: pButton = (COButton*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pButton) pButton->WM_Size_Element(hWnd, lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_SETFOCUS		: ((COButton*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_SetFocus();
													return NULL;
		case WM_KILLFOCUS		: ((COButton*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KillFocus();
													return NULL;
		case WM_CHAR				: pButton = (COButton*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(IsWindowEnabled(hWnd)) pButton->WM_Char(wParam);
													return NULL;
		case WM_COMMAND			: PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													break;
		case WM_LBUTTONDOWN	: ((COButton*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDown();
													return NULL;
		case WM_LBUTTONUP		: ((COButton*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonUp(wParam, lParam);
													return NULL;
		case WM_NCDESTROY		: pButton = (COButton*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pButton->htEffect_Timer) DeleteTimerQueueTimer(TimerQueue(), pButton->htEffect_Timer, INVALID_HANDLE_VALUE);
													VMFreiV(pButton);
													return NULL;

	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::COButtonV(_In_ VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName, 
																											_In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources)
{
	COTextLineV(vmMemory, pcClassName, pcWindowName, uiIDElement, pstDeviceResources);

	STFont stFont;
	stFont.fontFamilyName = L"Arial";
	stFont.fontCollection = NULL;
	stFont.fontWeight = DWRITE_FONT_WEIGHT_SEMI_BOLD;
	stFont.fontSytle = DWRITE_FONT_STYLE_NORMAL;
	stFont.fontStretch = DWRITE_FONT_STRETCH_EXPANDED;
	stFont.fontSize = 15.0f;
	stFont.localeName = L"en-en";
  Font(stFont);

	crfBackground = {RGB(210, 100, 20), 1.0f};
  crfBackground_1 = D2D1::ColorF(RGB(210, 100, 20), 1.0f);
	crfBackground_2 = D2D1::ColorF(RGB(20, 210, 20), 1.0f);
	crfBackground_3 = D2D1::ColorF(RGB(200, 200, 200), 1.0f);
  crfText = {RGB(230, 230, 230), 1.0f};
  crfText_1 = D2D1::ColorF(RGB(230, 230, 230), 1.0f);
	ucTextAlignment = TXA_CENTERVERTICAL | TXA_CENTERHORIZONTAL;
  crfText_Focus = D2D1::ColorF(RGB(10, 250, 250), 1.0f);
	bActive = true;

	pfnWM_LButtonDown = nullptr;
	pfnWM_LButtonUp = nullptr;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::COButtonV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																											_In_ STDeviceResources* pstDeviceResources)
{
	COButtonV(vmMemory, pcRePag_Button, pcWindowName, uiIDElement, pstDeviceResources);
}
//---------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COButton::COFreiV(void)
{
	SafeRelease(&ifTextColor_1);
	SafeRelease(&ifTextColor_Focus);
	SafeRelease(&ifBackgroundColor_1);
	SafeRelease(&ifBackgroundColor_2);
	SafeRelease(&ifBackgroundColor_3);

	return ((COTextLine*)this)->COFreiV();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::WM_Create(void)
{
	CharacterMetric();
	ifD2D1Context6->CreateSolidColorBrush(crfText, &ifTextColor);
	ifD2D1Context6->CreateSolidColorBrush(crfText_1, &ifTextColor_1);
	ifD2D1Context6->CreateSolidColorBrush(crfText_Focus, &ifTextColor_Focus);
	ifD2D1Context6->CreateSolidColorBrush(crfBackground_1, &ifBackgroundColor_1);
	ifD2D1Context6->CreateSolidColorBrush(crfBackground_2, &ifBackgroundColor_2);
	ifD2D1Context6->CreateSolidColorBrush(crfBackground_3, &ifBackgroundColor_3);

	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::WM_SetFocus(void)
{
	ThreadSafe_Begin();
	crfText = crfText_Focus;
	if(ifTextColor) ifTextColor->SetColor(crfText);
	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::WM_KillFocus(void)
{
	ThreadSafe_Begin();
	crfText = crfText_1;
	if(ifTextColor) ifTextColor->SetColor(crfText);
	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::WM_Char(WPARAM wParam)
{
	switch(wParam){
		case VK_RETURN	: SendMessage(hWndElement, WM_LBUTTONDOWN, NULL, NULL); Sleep(100); SendMessage(hWndElement, WM_LBUTTONUP, NULL, NULL);
		default					: PostMessage(GetParent(hWndElement), WM_CHAR, wParam, NULL);
											break;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::WM_LButtonDown(void)
{
	ThreadSafe_Begin();
	crfBackground = crfBackground_2;
	SetCapture(hWndElement);
	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	if(hWndElement != GetFocus()) SetFocus(hWndElement);
	if(pfnWM_LButtonDown) pfnWM_LButtonDown(this);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::WM_LButtonUp(WPARAM wParam, LPARAM lParam)
{
	ThreadSafe_Begin();
	crfBackground = crfBackground_1;
	ReleaseCapture();
	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	if(pfnWM_LButtonUp) pfnWM_LButtonUp(this);
	else PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), lParam);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																												 _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfText_1 = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifTextColor_1) ifTextColor_1->SetColor(crfText_1);
	crfText = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifTextColor) ifTextColor->SetColor(crfText);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetTextColor(_In_ D2D1_COLOR_F& crfTextA)
{
	ThreadSafe_Begin();
	crfText = crfText_1 = crfTextA;
	if(ifTextColor) ifTextColor->SetColor(crfText);
	if(ifTextColor_1) ifTextColor_1->SetColor(crfText_1);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetTextColor_Focus(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																															 _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfText_Focus = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifTextColor_Focus) ifTextColor_Focus->SetColor(crfText_Focus);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetTextColor_Focus(_In_ D2D1_COLOR_F& crfTextA)
{
	ThreadSafe_Begin();
	crfText_Focus = crfTextA;
	if(ifTextColor_Focus) ifTextColor_Focus->SetColor(crfText_Focus);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetBackgroundColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, 
																															 _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfBackground_1 = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifBackgroundColor_1) ifBackgroundColor_1->SetColor(crfBackground_1);
	crfBackground = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifBackColor) ifBackColor->SetColor(crfBackground);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetBackgroundColor(_In_ D2D1_COLOR_F& crfBackgroundA)
{
	ThreadSafe_Begin();
	crfBackground = crfBackgroundA;
	if(ifBackColor) ifBackColor->SetColor(crfBackground);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetBackgroundColor_1(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																																 _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfBackground_1 = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifBackgroundColor_1) ifBackgroundColor_1->SetColor(crfBackground_1);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetBackgroundColor_1(_In_ D2D1_COLOR_F& crfBackgroundA)
{
	ThreadSafe_Begin();
	crfBackground_1 = crfBackgroundA;
	if(ifBackgroundColor_1) ifBackgroundColor_1->SetColor(crfBackground_1);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetBackgroundColor_2(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																																 _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfBackground_2 = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifBackgroundColor_2) ifBackgroundColor_2->SetColor(crfBackground_2);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetBackgroundColor_2(_In_ D2D1_COLOR_F& crfBackgroundA)
{
	ThreadSafe_Begin();
	crfBackground_2 = crfBackgroundA;
	if(ifBackgroundColor_2) ifBackgroundColor_2->SetColor(crfBackground_2);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetBackgroundColor_3(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																																 _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfBackground_3 = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifBackgroundColor_3) ifBackgroundColor_3->SetColor(crfBackground_3);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::Label(const char* pcLabel)
{
	ThreadSafe_Begin();
	*vasContent = pcLabel;
	if(hWndElement){
		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetActive(bool bActiveA)
{
	ThreadSafe_Begin();
	bActive = bActiveA;
	if(bActive) crfBackground = crfBackground_1;
	else crfBackground = crfBackground_3;
	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COButton::Active(void)
{
	ThreadSafe_Begin();
	bool bActiveA = bActive;
	ThreadSafe_End();
	return bActiveA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::Lock(void)
{
	ThreadSafe_Begin();
	bActive = false;
	crfBackground = crfBackground_3;
	EnableWindow(hWndElement, false);
	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::Release(void)
{
	ThreadSafe_Begin();
	bActive = true;
	crfBackground = crfBackground_1;
	EnableWindow(hWndElement, true);
	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------