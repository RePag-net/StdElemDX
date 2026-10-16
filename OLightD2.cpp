/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OLightD2.cpp
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
#include "OLightD2.h"

#define _Light ((RePag::DirectX::COLight*)pvParam)
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COLight* __vectorcall RePag::DirectX::COLightV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																															 _In_ STDeviceResources* pstDeviceResources)
{
	COLight* vLight = (COLight*)VMBlock(VMDialog(), sizeof(COLight));
	vLight->COLightV(VMDialog(), pcWindowName, uiIDElement, pstDeviceResources);
	return vLight;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COLight* __vectorcall RePag::DirectX::COLightV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																															 _In_ STDeviceResources* pstDeviceResources)
{
	COLight* vLight = (COLight*)VMBlock(vmMemory, sizeof(COLight));
	vLight->COLightV(vmMemory, pcWindowName, uiIDElement, pstDeviceResources);
	return vLight;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_Light(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	COLight* pLight;
	switch(uiMessage){
		case WM_CREATE		: ((COLight*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
												((COLight*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
												return NULL;
		case WM_SIZE			: pLight = (COLight*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
												if(pLight) pLight->WM_Size(lParam);
												else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
												return NULL;
		case WM_NCDESTROY	: pLight = (COLight*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
												if(pLight->htEffect_Timer){	SetEvent(pLight->heBlink);
													DeleteTimerQueueTimer(TimerQueue(), pLight->htEffect_Timer, INVALID_HANDLE_VALUE); }
												VMFreiV(pLight);
												return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void CALLBACK RePag::DirectX::Timer_Light(_In_ void* pvParam, _In_ bool bTimerOrWaitFired)
{
	WaitForSingleObject(_Light->heBlink, INFINITE);
	static bool bBlink = false;
	bBlink ? bBlink = false : bBlink = true;

	_Light->ThreadSafe_Begin();
	_Light->OnRender(bBlink);
	_Light->ifDXGISwapChain4->Present1(0, NULL, &_Light->dxgiPresent);
	_Light->ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::COLightV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																										_In_ STDeviceResources* pstDeviceResources)
{
	COGraphicV(vmMemory, pcRePag_Light, pcWindowName, uiIDElementA, pstDeviceResources);

	crfLight_1 = D2D1::ColorF(RGB(0, 0, 255), 1.0f);
	crfLight_2 = D2D1::ColorF(RGB(0, 255, 255), 1.0f);

	fEllipseRadius_X = 0.0f;
	fEllipseRadius_Y = 0.0f;

	bRadialGradient = true;
	ptfRadialGradientCenter = {0.0f, 0.0f};
	ptfRadialGradientOriginOffset = {0.0f, 0.0f};
	fRadialGradientRadius_X = 0.0f;
	fRadialGradientRadius_Y = 0.0f;

	bLight_On = true;
	heBlink = CreateEvent(nullptr, true, true, nullptr);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COLight::COFreiV(void)
{
	SetEvent(heBlink); CloseHandle(heBlink);
	SafeRelease(&ifLight_1); SafeRelease(&ifLight_2); SafeRelease(&ifEllipseLight);
	return ((COGraphic*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::OnRender(_In_ bool bBlink)
{
	ifD2D1Context6->BeginDraw();
	ifD2D1Context6->Clear(crfBackground);

	if(!bBlink){
		ellipse = D2D1::Ellipse(D2D1::Point2F((float)lWidth / 2.0f, (float)lHeight / 2.0f), fEllipseRadius_X, fEllipseRadius_Y);
		SafeRelease(&ifEllipseLight);
		pstDeviceResources->ifd2d1Factory7->CreateEllipseGeometry(ellipse, &ifEllipseLight);
		ifD2D1Context6->DrawGeometry(ifEllipseLight, ifLight_1, 0);

		if(!bRadialGradient) ifD2D1Context6->FillGeometry(ifEllipseLight, ifLight_1);
		else{
			D2D1_GRADIENT_STOP gradientStops[2];
			gradientStops[0].color = crfLight_1; gradientStops[0].position = 0.0f;
			gradientStops[1].color = crfLight_2; gradientStops[1].position = 1.0f;

			ID2D1GradientStopCollection* ifGradientStops;
			ifD2D1Context6->CreateGradientStopCollection(gradientStops, 2, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &ifGradientStops);

			ID2D1RadialGradientBrush* ifRadialGradientBrush;
			ifD2D1Context6->CreateRadialGradientBrush(
				D2D1::RadialGradientBrushProperties(ptfRadialGradientCenter, ptfRadialGradientOriginOffset,
																						fRadialGradientRadius_X, fRadialGradientRadius_Y),
																						ifGradientStops, &ifRadialGradientBrush);

			ifD2D1Context6->FillGeometry(ifEllipseLight, ifRadialGradientBrush);
			SafeRelease(&ifGradientStops); SafeRelease(&ifRadialGradientBrush);
		}
	}
	ifD2D1Context6->EndDraw();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::OnPaint(void)
{
	ThreadSafe_Begin();
	OnRender(false);
	rclDirty.left = 0; rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::WM_Create(void)
{
	ifD2D1Context6->CreateSolidColorBrush(crfLight_1, &ifLight_1);
	ifD2D1Context6->CreateSolidColorBrush(crfLight_2, &ifLight_2);

	if(fEllipseRadius_X == NULL && fEllipseRadius_Y == NULL){
		fEllipseRadius_X = (float)lWidth / 2.0f;
		fEllipseRadius_Y = (float)lHeight / 2.0f;
	}

	if(ptfRadialGradientCenter.x == NULL && ptfRadialGradientCenter.y == NULL){
		ptfRadialGradientCenter.x = (float)lWidth / 2.0f;
		ptfRadialGradientCenter.y = (float)lHeight / 2.0f;
	}

	if(fRadialGradientRadius_X == NULL && fRadialGradientRadius_Y == NULL){
		fRadialGradientRadius_X = (float)lWidth / 2.0f;
		fRadialGradientRadius_Y = (float)lHeight / 2.0f;
	}
	OnRender(!bLight_On);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);

	CreateTimerQueueTimer(&htEffect_Timer, TimerQueue(), (WAITORTIMERCALLBACK)Timer_Light, this, 0, 500, 0);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::WM_Size(_In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	if(lHeight != HIWORD(lParam) || lWidth != LOWORD(lParam)){
		lHeight = HIWORD(lParam); lWidth = LOWORD(lParam);
		CreateWindowSizeDependentResources();
		OnRender(false);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::Lightcolor_1(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfLight_1 = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::Lightcolor_1(_In_ D2D1_COLOR_F& crfLightColor)
{
	ThreadSafe_Begin();
	crfLight_1 = crfLightColor;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::Lightcolor_2(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfLight_2 = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::Lightcolor_2(_In_ D2D1_COLOR_F& crfLightColor)
{
	ThreadSafe_Begin();
	crfLight_2 = crfLightColor;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::EllipseRadius(_In_ float fRadius_X, _In_ float fRadius_Y)
{
	ThreadSafe_Begin();
	fEllipseRadius_X = fRadius_X;
	fEllipseRadius_Y = fRadius_Y;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall  RePag::DirectX::COLight::RadialGradientCenter(_In_ float fCenter_X, _In_ float fCenter_Y)
{
	ThreadSafe_Begin();
	ptfRadialGradientCenter.x = fCenter_X;
	ptfRadialGradientCenter.y = fCenter_Y;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall  RePag::DirectX::COLight::RadialGradientCenter(_In_ D2D1_POINT_2F ptfCenter)
{
	ThreadSafe_Begin();
	ptfRadialGradientCenter = ptfCenter;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::RadialGradientOriginOffset(_In_ float fOffset_X, _In_ float fOffset_Y)
{
	ThreadSafe_Begin();
	ptfRadialGradientOriginOffset.x = fOffset_X;
	ptfRadialGradientOriginOffset.y = fOffset_Y;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::RadialGradientOriginOffset(_In_ D2D1_POINT_2F ptfOffset)
{
	ThreadSafe_Begin();
	ptfRadialGradientOriginOffset = ptfOffset;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::RadialGradientRadius(_In_ float fRadiusA_X, _In_ float fRadiusA_Y)
{
	ThreadSafe_Begin();
	fRadialGradientRadius_X = fRadiusA_X;
	fRadialGradientRadius_Y = fRadiusA_Y;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::RadialGradient(_In_ bool bRadialGradientA)
{
	ThreadSafe_Begin();
	bRadialGradient = bRadialGradientA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::BlinkPeriod(_In_ unsigned long ulPeriod_msec)
{
	ThreadSafe_Begin();
	if(htEffect_Timer) ChangeTimerQueueTimer(TimerQueue(), htEffect_Timer, 0, ulPeriod_msec);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::Blink_Stop(void)
{
	ThreadSafe_Begin();
	if(hWndElement){
		ResetEvent(heBlink);
		OnRender(true);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::Blink_Go(void)
{
	ThreadSafe_Begin();
	if(hWndElement){
		SetEvent(heBlink);
		OnRender(false);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::Light_On(_In_ bool bLight_OnA)
{
	ThreadSafe_Begin();
	if(hWndElement){
		ResetEvent(heBlink);
		OnRender(!bLight_OnA);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	else{	ResetEvent(heBlink); bLight_On = bLight_OnA; }
	ThreadSafe_End();
}