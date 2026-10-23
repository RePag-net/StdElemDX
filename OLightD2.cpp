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
												VMFreiV(pLight);
												return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
HANDLE htTimer_Light = nullptr;
void CALLBACK RePag::DirectX::Timer_Light(_In_ void* pvParam, _In_ bool bTimerOrWaitFired)
{
	_Light->ThreadSafe_Begin();
	SetEvent(_Light->he2Render[LIGHT_TRIGGER]);
	_Light->ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::COLightV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																										_In_ STDeviceResources* pstDeviceResources)
{
	COGraphicV(vmMemory, pcRePag_Light, pcWindowName, uiIDElementA, pstDeviceResources);

	crfLight_1_0 = D2D1::ColorF(RGB(0, 0, 255), 1.0f);
	crfLight_2_1 = D2D1::ColorF(RGB(0, 255, 255), 1.0f);

	fEllipseRadius_X = 0.0f;
	fEllipseRadius_Y = 0.0f;

	bRadialGradient = true;
	ptfRadialGradientCenter = D2D1::Point2F(0.0f, 0.0f);
	ptfRadialGradientOriginOffset = D2D1::Point2F(0.0f, 0.0f);
	fRadialGradientRadius_X = 0.0f;
	fRadialGradientRadius_Y = 0.0f;

	crfStroke = D2D1::ColorF(RGB(100, 100, 100), 1.0f);
	fStroke_Width = 0.0f;

	bLight_On = true;
	bThreadEnd = false;
	he2Render[LIGHT_TRIGGER] = CreateEvent(nullptr, false, false, nullptr);
	he2Render[LIGHT_BYPASS] = CreateEvent(nullptr, true, false, nullptr);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COLight::COFreiV(void)
{
	bThreadEnd = true; SetEvent(he2Render[LIGHT_TRIGGER]); SetEvent(he2Render[LIGHT_BYPASS]);
	DeleteTimerQueueTimer(TimerQueue(), htTimer_Light, INVALID_HANDLE_VALUE);
	CloseHandle(he2Render[LIGHT_TRIGGER]); CloseHandle(he2Render[LIGHT_BYPASS]);
	CloseHandle(hRenderThread);
	SafeRelease(&ifLight_1_0); SafeRelease(&ifLight_2_1); SafeRelease(&ifEllipseLight); SafeRelease(&ifStroke);
	return ((COGraphic*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::OnRender(_In_ bool bLight_On)
{
	ifD2D1Context6->BeginDraw();
	ifD2D1Context6->Clear(crfBackground);

	if(bLight_On){
		ellipse = D2D1::Ellipse(D2D1::Point2F((float)lWidth / 2.0f, (float)lHeight / 2.0f), fEllipseRadius_X, fEllipseRadius_Y);
		SafeRelease(&ifEllipseLight);
		pstDeviceResources->ifd2d1Factory7->CreateEllipseGeometry(ellipse, &ifEllipseLight);
		ifD2D1Context6->DrawGeometry(ifEllipseLight, ifStroke, fStroke_Width);

		if(!bRadialGradient) ifD2D1Context6->FillGeometry(ifEllipseLight, ifLight_1_0);
		else{
			D2D1_GRADIENT_STOP gradientStops[2];
			gradientStops[0].color = crfLight_1_0; gradientStops[0].position = 0.0f;
			gradientStops[1].color = crfLight_2_1; gradientStops[1].position = 1.0f;

			ID2D1GradientStopCollection* ifGradientStops = nullptr;
			ifD2D1Context6->CreateGradientStopCollection(gradientStops, 2, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &ifGradientStops);

			ID2D1RadialGradientBrush* ifRadialGradientBrush = nullptr;
			ifD2D1Context6->CreateRadialGradientBrush(
				D2D1::RadialGradientBrushProperties(ptfRadialGradientCenter, ptfRadialGradientOriginOffset,
																						fRadialGradientRadius_X, fRadialGradientRadius_Y),
											D2D1::BrushProperties(1.0f, D2D1::IdentityMatrix()),
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
	OnRender(true);
	rclDirty.left = 0; rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::WM_Create(void)
{
	ifD2D1Context6->CreateSolidColorBrush(crfLight_1_0, &ifLight_1_0);
	ifD2D1Context6->CreateSolidColorBrush(crfLight_2_1, &ifLight_2_1);
	ifD2D1Context6->CreateSolidColorBrush(crfStroke, &ifStroke);

	if(fEllipseRadius_X == NULL && fEllipseRadius_Y == NULL){
		fEllipseRadius_X = (float)lWidth / 2.0f - fStroke_Width;
		fEllipseRadius_Y = (float)lHeight / 2.0f - fStroke_Width;
	}

	if(ptfRadialGradientCenter.x == 0.0f && ptfRadialGradientCenter.y == 0.0f)
		ptfRadialGradientCenter = {(float)lWidth / 2.0f, (float)lHeight / 2.0f};

	if(fRadialGradientRadius_X == 0.0f && fRadialGradientRadius_Y == 0.0f){
		fRadialGradientRadius_X = (float)lWidth / 2.0f;
		fRadialGradientRadius_Y = (float)lHeight / 2.0f;
	}

	if(crfBackground.a == 0.0f){
		D2D1_COLOR_F crfBackground_Main;
		((CODialog*)pMainElement)->GetBackgroundColor(crfBackground_Main);
		crfBackground = crfBackground_Main;
	}

	OnRender(bLight_On);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);

	hRenderThread = CreateThread(NULL, 0, thRender_Light, this, NULL, NULL);
	CreateTimerQueueTimer(&htTimer_Light, TimerQueue(), (WAITORTIMERCALLBACK)Timer_Light, this, 0, 500, 0);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::WM_Size(_In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	if(lHeight != HIWORD(lParam) || lWidth != LOWORD(lParam)){
		lHeight = HIWORD(lParam); lWidth = LOWORD(lParam);
		CreateWindowSizeDependentResources();
		OnRender(true);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::Lightcolor_1_0(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfLight_1_0 = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::Lightcolor_1_0(_In_ D2D1_COLOR_F& crfLightColor)
{
	ThreadSafe_Begin();
	crfLight_1_0 = crfLightColor;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::Lightcolor_2_1(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfLight_2_1 = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::Lightcolor_2_1(_In_ D2D1_COLOR_F& crfLightColor)
{
	ThreadSafe_Begin();
	crfLight_2_1 = crfLightColor;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::Strokecolor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfStroke = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::Strokecolor(_In_ D2D1_COLOR_F& crfStrokeColor)
{
	ThreadSafe_Begin();
	crfStroke = crfStrokeColor;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::StokeWidth(_In_ float fStroke)
{
	ThreadSafe_Begin();
	fStroke_Width = fStroke;
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
	if(htTimer_Light) ChangeTimerQueueTimer(TimerQueue(), htTimer_Light, 0, ulPeriod_msec);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::Blink_Stop(void)
{
	ThreadSafe_Begin();
	if(hWndElement){
		ResetEvent(he2Render[LIGHT_BYPASS]);
		OnRender(true);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	else ResetEvent(he2Render[LIGHT_BYPASS]);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::Blink_Go(void)
{
	ThreadSafe_Begin();
	SetEvent(he2Render[LIGHT_BYPASS]);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLight::Light_On(_In_ bool bLight_OnA)
{
	ThreadSafe_Begin();
	if(hWndElement){
		ResetEvent(he2Render[LIGHT_BYPASS]);
		OnRender(bLight_OnA);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	else{ ResetEvent(he2Render[LIGHT_BYPASS]); bLight_On = bLight_OnA; }
	ThreadSafe_End();
}