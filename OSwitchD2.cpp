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
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COSwitch* __vectorcall RePag::DirectX::COSwitchV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																 _In_ STDeviceResources* pstDeviceResources)
{
	COSwitch* vSwitch = (COSwitch*)VMBlock(VMDialog(), sizeof(COSwitch));
	vSwitch->COSwitchV(VMDialog(), pcWindowName, uiIDElement, pstDeviceResources);
	return vSwitch;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COSwitch* __vectorcall RePag::DirectX::COSwitchV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
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
		case WM_COMMAND			: PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													break;
		case WM_LBUTTONDOWN	: pSwitch = (COSwitch*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													SetCapture(hWnd);
													if(pSwitch->pfnWM_LButtonDown){ pSwitch->ThreadSafe_Begin(); pSwitch->pfnWM_LButtonDown(pSwitch); pSwitch->ThreadSafe_End(); }
													return NULL;
		case WM_LBUTTONUP		: ((COSwitch*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonUp(wParam, lParam);
													return NULL;
		case WM_NCDESTROY		: pSwitch = (COSwitch*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pSwitch->htEffect_Timer) DeleteTimerQueueTimer(TimerQueue(), pSwitch->htEffect_Timer, INVALID_HANDLE_VALUE);
													VMFreiV(pSwitch);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitch::COSwitchV(_In_ VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																											_In_ STDeviceResources* pstDeviceResources)
{
	COButtonV(vmMemory, pcClassName, pcWindowName, uiIDElementA, pstDeviceResources);

	bOn = false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitch::COSwitchV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																											_In_ STDeviceResources* pstDeviceResources)
{
	COButtonV(vmMemory, pcRePag_Switch, pcWindowName, uiIDElementA, pstDeviceResources);

	bOn = false;
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
	if(bOnA){ bOn = true;	*vasContent = asLabel_On;
						crfBackground = crfBackground_2; if(ifBackColor) ifBackColor->SetColor(crfBackground); }
	else{			bOn = false; *vasContent = asLabel_Off;
						crfBackground = crfBackground_1; if(ifBackColor) ifBackColor->SetColor(crfBackground); }
	bActive = true;

	OnRender();
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
