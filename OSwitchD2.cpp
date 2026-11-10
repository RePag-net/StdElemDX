/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OSwitchD2.cpp
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
#include "OSwitchD2.h"
#include "OSwitchGroupD2.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COSwitch* __vectorcall RePag::DirectX::COSwitchV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																 _In_ STDeviceResources* pstDeviceResources)
{
	COSwitch* vSwitch = (COSwitch*)VMBlock(VMDialog(), sizeof(COSwitch));
	vSwitch->COSwitchV(VMDialog(), pcWindowName, uiIDElement, pstDeviceResources);
	return vSwitch;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COSwitch* __vectorcall RePag::DirectX::COSwitchV(_In_ VMEMORY vmMemory,_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																 _In_ STDeviceResources* pstDeviceResources)
{
	COSwitch* vSwitch = (COSwitch*)VMBlock(vmMemory, sizeof(COSwitch));
	vSwitch->COSwitchV(vmMemory, pcWindowName, uiIDElement, pstDeviceResources);
	return vSwitch;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_Switch(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	COSwitch* pSwitch;
	switch(uiMessage){
		case WM_CREATE			: ((COSwitch*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
													((COSwitch*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
													return NULL;
		case WM_SIZE				: pSwitch = (COSwitch*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pSwitch) pSwitch->WM_Size(lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_INITPAINT		: ((COSwitch*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_InitPaint();
													return NULL;
		case WM_COMMAND			: PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													break;
		case WM_LBUTTONDOWN	: pSwitch = (COSwitch*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													SetCapture(hWnd);
													if(pSwitch->pfnWM_LButtonDown){ pSwitch->ThreadSafe_Begin(); pSwitch->pfnWM_LButtonDown(pSwitch); pSwitch->ThreadSafe_End(); }
													return NULL;
		case WM_LBUTTONUP		: ((COSwitch*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonUp(wParam, lParam);
													return NULL;
		case WM_NCDESTROY		: VMFreiV((COSwitch*)GetWindowLongPtr(hWnd, GWLP_USERDATA));
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitch::COSwitchV(_In_ VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName,
																											_In_ unsigned int uiIDElementA,	_In_ STDeviceResources* pstDeviceResourcesA)
{
	COButtonV(vmMemory, pcClassName, pcWindowName, uiIDElementA, pstDeviceResourcesA);

	asLabel_On = "Switch On";
	asLabel_Off = "Switch Off";
	bOn = false; *vasContent = asLabel_Off;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitch::COSwitchV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName,	_In_ unsigned int uiIDElementA,
																											_In_ STDeviceResources* pstDeviceResourcesA)
{
	COButtonV(vmMemory, pcRePag_Switch, pcWindowName, uiIDElementA, pstDeviceResourcesA);

	asLabel_On = "Switch On";
	asLabel_Off = "Switch Off";
	bOn = false; *vasContent = asLabel_Off;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitch::OnRender(_In_ bool bBlinkA)
{
	ifD2D1Context6->BeginDraw();
	Background();
	OnRender_Rectangle(bBlinkA);
	ifD2D1Context6->EndDraw();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitch::WM_InitPaint(void)
{
	ifD2D1Context6->CreateSolidColorBrush(crfText, &ifTextColor);
	ifD2D1Context6->CreateSolidColorBrush(crfButton_1_0, &ifButtonColor);

	if(ptfStartPoint.x == 0.0f && ptfStartPoint.y == 0.0f && ptfEndPoint.x == 0.0f && ptfEndPoint.y == 0.0f){
		ptfStartPoint = {(float)lWidth / 2.0f, 0.0f}; ptfEndPoint = {(float)lWidth / 2.0f, (float)lHeight};
	};

	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);

	hRenderThread = CreateThread(NULL, 0, thRender_Button, this, NULL, NULL);
	CreateTimerQueueTimer(&htTimer_Button, TimerQueue(), (WAITORTIMERCALLBACK)Timer_Button, this, 0, 500, 0);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitch::Background(void)
{
	ID2D1GradientStopCollection* ifGradientStops = nullptr;
	D2D1_POINT_2F ptfStartPoint = D2D1::Point2F(0.0f, 0.0f); D2D1_POINT_2F ptfEndPoint = D2D1::Point2F(0.0f, 0.0f);
	if(bSwitchGroup){
		COSwitchGroup* pSwitchGroup = (COSwitchGroup*)GetWindowLongPtr(GetParent(hWndElement), GWLP_USERDATA);
		if(!pSwitchGroup->GetLinearGradient()){	D2D1_COLOR_F crfBackgroundA;
			ifD2D1Context6->Clear(pSwitchGroup->GetBackgroundColor(crfBackgroundA)); return;
		}
		else{
			D2D1_GRADIENT_STOP gs2GradientStops[2];
			pSwitchGroup->GetGradientStops(gs2GradientStops);
			if(FAILED(ifD2D1Context6->CreateGradientStopCollection(gs2GradientStops, 2, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &ifGradientStops))) goto Error;
			pSwitchGroup->LinearGradientStartPoint(ptfStartPoint);
			ptfStartPoint.x -= (float)ptPosition.x; ptfStartPoint.y -= (float)ptPosition.y;
			pSwitchGroup->LinearGradientEndPoint(ptfEndPoint);
			ptfEndPoint.x -= (float)ptPosition.x; ptfEndPoint.y -= (float)ptPosition.y;
		}
	}
  else if(!Background_Dialog(ifGradientStops, ptfStartPoint, ptfEndPoint)) goto Error;
	Background_Rectangle(ifGradientStops, ptfStartPoint, ptfEndPoint);
Error:
	SafeRelease(&ifGradientStops);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitch::WM_LButtonUp(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	SetFocus(hWndElement);
	Switch(!bOn);
	if(pfnWM_LButtonUp) pfnWM_LButtonUp(this);
	else PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), lParam);
	ReleaseCapture();
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitch::Switch(_In_ bool	bOnA)
{
	if(bOnA){
		bOn = true;	*vasContent = asLabel_On;
		if(!bLinearGradient) ifButtonColor->SetColor(crfButton_2_0); 
		else{	crfGradientStop_0 = crfButton_2_0;	crfGradientStop_1 = crfButton_2_1; }
	}
	else{
		bOn = false; *vasContent = asLabel_Off;
		if(!bLinearGradient) ifButtonColor->SetColor(crfButton_1_0);
		else{ crfGradientStop_0 = crfButton_1_0;	crfGradientStop_1 = crfButton_1_1; }
	}
	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COSwitch::IsOn(void)
{
	ThreadSafe_Begin();
	bool bOnA = bOn;
	ThreadSafe_End();
	return bOnA;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitch::SetSwitch(_In_ bool bOnA)
{
	ThreadSafe_Begin();
	Switch(bOnA);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitch::Label(_In_z_ const char* pcLabel)
{
	ThreadSafe_Begin();
	asLabel_On = pcLabel;
	asLabel_Off = pcLabel;
	*vasContent = pcLabel;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitch::Label_On(_In_z_ const char* pcLabel)
{
	ThreadSafe_Begin();
	asLabel_On = pcLabel;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitch::Label_Off(_In_z_ const char* pcLabel)
{
	ThreadSafe_Begin();
	asLabel_Off = pcLabel;
	*vasContent = pcLabel;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
