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
#include "TStdElemDX.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COButton* __vectorcall RePag::DirectX::COButtonV(_In_z_ const char* pcWindowsName, _In_ unsigned int uiIDElement,
																																 _In_ STDeviceResources* pstDeviceResources)
{
	COButton* vButton = (COButton*)VMBlock(VMDialog(), sizeof(COButton));
	vButton->COButtonV(VMDialog(), pcWindowsName, uiIDElement, pstDeviceResources);
	return vButton;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COButton* __vectorcall RePag::DirectX::COButtonV(_In_ VMEMORY vmSpeicher, _In_z_ const char* pcWindowsName, _In_ unsigned int uiIDElement,
																																 _In_ STDeviceResources* pstDeviceResources)
{
	COButton* vButton = (COButton*)VMBlock(vmSpeicher, sizeof(COButton));
	vButton->COButtonV(vmSpeicher, pcWindowsName, uiIDElement, pstDeviceResources);
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
													if(pButton) pButton->WM_Size(lParam);
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
													VMFreiV(pButton);
													return NULL;

	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void CALLBACK RePag::DirectX::Timer_Button(_In_ void* pvParam, _In_ bool bTimerOrWaitFired)
{
	_Button->ThreadSafe_Begin();
	SetEvent(_Button->he2Render[BUTTON_TRIGGER]);
	_Button->ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::COButtonV(_In_ VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName, 
																											_In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources)
{
	COTextV(vmMemory, pcClassName, pcWindowName, uiIDElement, pstDeviceResources);

	STFont stFont;
	stFont.fontFamilyName = L"Arial";
	stFont.fontCollection = NULL;
	stFont.fontWeight = DWRITE_FONT_WEIGHT_SEMI_BOLD;
	stFont.fontSytle = DWRITE_FONT_STYLE_NORMAL;
	stFont.fontStretch = DWRITE_FONT_STRETCH_EXPANDED;
	stFont.fontSize = 15.0f;
	stFont.localeName = L"en-en";
  SetFont(stFont);

	crfButtonColor_1_0 = D2D1::ColorF(RGB(110, 110, 110), 1.0f);
	crfButtonColor_1_1 = D2D1::ColorF(RGB(35, 35, 35), 1.0f);
	crfButtonColor_2_0 = D2D1::ColorF(RGB(100, 140, 100), 1.0f);
	crfButtonColor_2_1 = D2D1::ColorF(RGB(0, 255, 0), 1.0f);
	crfButtonColor_3 = D2D1::ColorF(RGB(200, 200, 200), 1.0f);
	crfGradientStopsColor_0 = crfButtonColor_1_0;
	crfGradientStopsColor_1 = crfButtonColor_1_1;
  crfText_Focus = D2D1::ColorF(RGB(10, 250, 250), 1.0f);
	ucTextAlignment = TXA_CENTERVERTICAL | TXA_CENTERHORIZONTAL;
	bEnabled = true;

	fEdgeRadius_X = 15.0f;
	fEdgeRadius_Y = 15.0f;

	pfnWM_LButtonDown = nullptr;
	pfnWM_LButtonUp = nullptr;

	bLinearGradient_LButton = bLinearGradient = false;
	ifLinearGradientBrush = nullptr;
	ptfStartPoint = D2D1::Point2F(0.0f, 0.0f);
	ptfEndPoint = D2D1::Point2F(0.0f, 0.0f);

	bBlink = false;
	bThreadEnd = false;
	he2Render[BUTTON_TRIGGER] = CreateEvent(nullptr, false, false, nullptr);
	he2Render[BUTTON_BYPASS] = CreateEvent(nullptr, true, false, nullptr);
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
	bThreadEnd = true; SetEvent(he2Render[BUTTON_TRIGGER]); SetEvent(he2Render[BUTTON_BYPASS]);
	DeleteTimerQueueTimer(TimerQueue(), htTimer_Button, INVALID_HANDLE_VALUE);
	CloseHandle(he2Render[BUTTON_TRIGGER]); CloseHandle(he2Render[BUTTON_BYPASS]);
	CloseHandle(hRenderThread);
	SafeRelease(&ifTextColor); SafeRelease(&ifButtonColor); SafeRelease(&ifButtom);
	return ((COText*)this)->COFreiV();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::OnRender(void)
{
	IDWriteTextLayout* ifTextLayout; float fTextWidth; 	size_t szBytes_Text; WCHAR wc255Inhalt[255];
	D2D1_RECT_F rcfText = D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);
	D2D1_RECT_F rcfButton = {0.0f, 0.0f, (float)lWidth, (float)lHeight};
	D2D1_ROUNDED_RECT rrcfButton = {rcfButton, fEdgeRadius_X, fEdgeRadius_Y};

	if(mbstowcs_s(&szBytes_Text, wc255Inhalt, 255, vasContent->c_Str(), vasContent->Length())) return;
	if(pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wc255Inhalt, (UINT32)szBytes_Text, ifText, (float)lWidth, (float)lHeight, &ifTextLayout)) return;
	TextAlignment(ifTextLayout, fTextWidth, rcfText);
	SafeRelease(&ifTextLayout);

	ifD2D1Context6->BeginDraw();
	ifD2D1Context6->Clear(crfBackground);

	SafeRelease(&ifButtom);
	pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfButton, &ifButtom);
	ifD2D1Context6->DrawGeometry(ifButtom, ifButtonColor, 0);
	if(!bLinearGradient || !bEnabled) ifD2D1Context6->FillGeometry(ifButtom, ifButtonColor);
	else{
		D2D1_GRADIENT_STOP gradientStops[2];
		gradientStops[0].color = crfGradientStopsColor_0; gradientStops[0].position = 0.0f;
		gradientStops[1].color = crfGradientStopsColor_1; gradientStops[1].position = 1.0f;

		ID2D1GradientStopCollection* ifGradientStops = nullptr;
		ifD2D1Context6->CreateGradientStopCollection(gradientStops, 2, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &ifGradientStops);

		ifD2D1Context6->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(ptfStartPoint, ptfEndPoint),
			D2D1::BrushProperties(1.0f, D2D1::IdentityMatrix()), ifGradientStops, &ifLinearGradientBrush);


		ifD2D1Context6->FillGeometry(ifButtom, ifLinearGradientBrush);
		SafeRelease(&ifGradientStops); SafeRelease(&ifLinearGradientBrush);
	}

	ifD2D1Context6->DrawText(wc255Inhalt, (UINT32)szBytes_Text, ifText, rcfText, ifTextColor);

	ifD2D1Context6->EndDraw();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::OnPaint(void)
{
	ThreadSafe_Begin();
	OnRender();
	rclDirty.left = 0; rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::WM_Create(void)
{
	CharacterMetric();
	ifD2D1Context6->CreateSolidColorBrush(crfText, &ifTextColor);
	ifD2D1Context6->CreateSolidColorBrush(crfButtonColor_1_0, &ifButtonColor);

	if(bEnabled) EnableWindow(hWndElement, true);
	else{
		EnableWindow(hWndElement, false);
		ifButtonColor->SetColor(crfButtonColor_3);
		ResetEvent(he2Render[BUTTON_BYPASS]);
	} 

	if(crfBackground.a == 0.0f){
		D2D1_COLOR_F crfBackground_Main;
		((CODialog*)pMainElement)->GetBackgroundColor(crfBackground_Main);
		crfBackground = crfBackground_Main;
	}

	if(ptfStartPoint.x == 0.0f && ptfStartPoint.y == 0.0f && ptfEndPoint.x == 0.0f && ptfEndPoint.y == 0.0f){
		ptfStartPoint = {(float)lWidth / 2.0f, 0.0f}; ptfEndPoint = {(float)lWidth / 2.0f, (float)lHeight};
	};

	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);

	hRenderThread = CreateThread(NULL, 0, thRender_Button, this, NULL, NULL);
	CreateTimerQueueTimer(&htTimer_Button, TimerQueue(), (WAITORTIMERCALLBACK)Timer_Button, this, 0, 500, 0);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::WM_Size(_In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	if(lHeight != HIWORD(lParam) || lWidth != LOWORD(lParam)){
		lHeight = HIWORD(lParam); lWidth = LOWORD(lParam);
		CreateWindowSizeDependentResources();
		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::WM_SetFocus(void)
{
	ThreadSafe_Begin();
	ifTextColor->SetColor(crfText_Focus);
	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::WM_KillFocus(void)
{
	ThreadSafe_Begin();
	ifTextColor->SetColor(crfText);
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
	SetCapture(hWndElement);
	if(IsWindowEnabled(hWndElement)){
		ResetEvent(he2Render[BUTTON_BYPASS]);
		ifButtonColor->SetColor(crfButtonColor_2_0);
		bLinearGradient_LButton = bLinearGradient;
		bLinearGradient = false;
		if(hWndElement != GetFocus()) SetFocus(hWndElement);
		else{
			OnRender();
			ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		}
		if(pfnWM_LButtonDown) pfnWM_LButtonDown(this);
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::WM_LButtonUp(WPARAM wParam, LPARAM lParam)
{
	ThreadSafe_Begin();
	if(IsWindowEnabled(hWndElement)){
		ifButtonColor->SetColor(crfButtonColor_1_0);
		bLinearGradient = bLinearGradient_LButton;
		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		if(bBlink) SetEvent(he2Render[BUTTON_BYPASS]);
		if(pfnWM_LButtonUp) pfnWM_LButtonUp(this);
		else PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), lParam);
	}
	ReleaseCapture();
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																												 _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfText = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	if(ifTextColor) ifTextColor->SetColor(crfText);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetTextColor(_In_ D2D1_COLOR_F& crfTextA)
{
	ThreadSafe_Begin();
	crfText = crfTextA;
	if(ifTextColor) ifTextColor->SetColor(crfText);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetTextColor_Focus(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																															 _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfText_Focus = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetTextColor_Focus(_In_ D2D1_COLOR_F& crfTextA)
{
	ThreadSafe_Begin();
	crfText_Focus = crfTextA;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetButtonColor_1_0(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																																 _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfGradientStopsColor_0 = crfButtonColor_1_0 = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetButtonColor_1_0(_In_ D2D1_COLOR_F& crfButtonColor)
{
	ThreadSafe_Begin();
	crfGradientStopsColor_0 = crfButtonColor_1_0 = crfButtonColor;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetButtonColor_1_1(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfGradientStopsColor_1 = crfButtonColor_1_1 = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetButtonColor_1_1(_In_ D2D1_COLOR_F& crfButtonColor)
{
	ThreadSafe_Begin();
	crfGradientStopsColor_1 = crfButtonColor_1_1 = crfButtonColor;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetButtonColor_2_0(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																																 _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfGradientStopsColor_0 = crfButtonColor_2_0 = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetButtonColor_2_0(_In_ D2D1_COLOR_F& crfButtonColor)
{
	ThreadSafe_Begin();
	crfGradientStopsColor_0 = crfButtonColor_2_0 = crfButtonColor;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetButtonColor_2_1(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																																 _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfGradientStopsColor_1 = crfButtonColor_2_1 = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetButtonColor_2_1(_In_ D2D1_COLOR_F& crfButtonColor)
{
	ThreadSafe_Begin();
	crfGradientStopsColor_1 = crfButtonColor_2_1 = crfButtonColor;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetButtonColor_3(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																																 _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfButtonColor_3 = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::SetButtonColor_3(_In_ D2D1_COLOR_F& crfButtonColor)
{
	ThreadSafe_Begin();
	crfButtonColor_3 = crfButtonColor;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::LinearGradient(_In_ bool bLinearGradientA)
{
	ThreadSafe_Begin();
	bLinearGradient = bLinearGradientA;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::StartPoint(D2D1_POINT_2F ptfStartPointA)
{
	ThreadSafe_Begin();
	ptfStartPoint = ptfStartPointA;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::EndPoint(D2D1_POINT_2F ptfEndPointA)
{
	ThreadSafe_Begin();
	ptfEndPoint = ptfEndPointA;
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
void __vectorcall RePag::DirectX::COButton::Enable(_In_ bool bEnable)
{
	ThreadSafe_Begin();
	bEnabled = bEnable;
	if(hWndElement){
		EnableWindow(hWndElement, bEnable);
		if(!bEnabled){
			ResetEvent(he2Render[BUTTON_BYPASS]);
			ifButtonColor->SetColor(crfButtonColor_3);
		}
		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::Edge(_In_ float fRadius_X, _In_ float fRadius_Y)
{
	ThreadSafe_Begin();
	fEdgeRadius_X = fRadius_X;
	fEdgeRadius_Y = fRadius_Y;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::BlinkPeriod(_In_ unsigned long ulPeriod_msec)
{
	ThreadSafe_Begin();
	if(htTimer_Button) ChangeTimerQueueTimer(TimerQueue(), htTimer_Button, 0, ulPeriod_msec);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::Blink_Stop(void)
{
	ThreadSafe_Begin();
	if(hWndElement){
		bBlink = false;
		ResetEvent(he2Render[BUTTON_BYPASS]);
		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	else ResetEvent(he2Render[BUTTON_BYPASS]);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COButton::Blink_Go(void)
{
	ThreadSafe_Begin();
	bBlink = true;
	SetEvent(he2Render[BUTTON_BYPASS]);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------