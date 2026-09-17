/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OProgressBarD2.cpp
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
#include "OProgressBarD2.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COProgressBar* __vectorcall RePag::DirectX::COProgressBarV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																					 _In_ STDeviceResources* pstDeviceResources)
{
	COProgressBar* vProgressBar = (COProgressBar*)VMBlock(VMDialog(), sizeof(COProgressBar));
	vProgressBar->COProgressBarV(VMDialog(), pcWindowName, uiIDElement, pstDeviceResources);
	return vProgressBar;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COProgressBar* __vectorcall RePag::DirectX::COProgressBarV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																					 _In_ STDeviceResources* pstDeviceResources)
{
	COProgressBar* vProgressBar = (COProgressBar*)VMBlock(vmMemory, sizeof(COProgressBar));
	vProgressBar->COProgressBarV(vmMemory, pcWindowName, uiIDElement, pstDeviceResources);
	return vProgressBar;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_ProgressBar(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	COProgressBar* vProgressBar;
	switch(uiMessage){
		case WM_CREATE			: ((COProgressBar*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
													((COProgressBar*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
													return NULL;
		case WM_SIZE				: vProgressBar = (COProgressBar*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(vProgressBar) vProgressBar->WM_Size(lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_LBUTTONDOWN	: ((COProgressBar*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDown(wParam, lParam);
													return NULL;
		case WM_LBUTTONUP		: PostMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWnd, GWLP_ID), wParam), lParam);
													return NULL;
		case WM_MOUSEMOVE		: ((COProgressBar*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseMove(wParam, lParam);
													return NULL;
		case WM_NCDESTROY		: vProgressBar = (COProgressBar*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(vProgressBar->htEffect_Timer) DeleteTimerQueueTimer(TimerQueue(), vProgressBar->htEffect_Timer, INVALID_HANDLE_VALUE);
													VMFreiV(vProgressBar);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COProgressBar::COProgressBarV(_In_ VMEMORY vmSpeicher, _In_z_ const char* pcFensterName, _In_ unsigned int uiIDElementA,
																																_In_ STDeviceResources* pstDeviceResources)
{
	COGraphicV(vmSpeicher, pcRePag_ProgressBar, pcFensterName, uiIDElementA, pstDeviceResources);

	crfBackground = {RGB(100, 100, 100), 1.0f};

	crfFillColor = D2D1::ColorF(RGB(140, 85, 0), 1.0f);
	dStep = ulPosition_max = ulPosition_min = ulPosition = 0;

	pfnWM_LButtonDown = nullptr;
	pfnWM_MouseMove = nullptr;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COProgressBar::COFreiV(void)
{
	SafeRelease(&ifFillColor); SafeRelease(&ifProgressBar);
	return ((COElement*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COProgressBar::OnRender(void)
{
	WaitForSingleObjectEx(heRender, INFINITE, false);
	ifD2D1Context6->BeginDraw();
	ifD2D1Context6->Clear(crfBackground);

	if(ulPosition){ D2D1_RECT_F rcfButton;
		rcfButton = {5, 5, (float)(ulPosition + 5), (float)(lHeight - 5)};

		pstDeviceResources->ifd2d1Factory7->CreateRectangleGeometry(rcfButton, &ifProgressBar);
		ifD2D1Context6->DrawGeometry(ifProgressBar, ifFillColor, 1);
		ifD2D1Context6->FillGeometry(ifProgressBar, ifFillColor);
		SafeRelease(&ifProgressBar);
	}
	ifD2D1Context6->EndDraw();
	SetEvent(heRender);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COProgressBar::OnPaint(void)
{
	ThreadSafe_Begin();
	OnRender();
  rclDirty.left = 0; rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COProgressBar::WM_Create(void)
{
	ifD2D1Context6->CreateSolidColorBrush(crfFillColor, &ifFillColor);

	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COProgressBar::WM_Size(_In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	if(lHeight != HIWORD(lParam) || lWidth != LOWORD(lParam)){
		lHeight = HIWORD(lParam); lWidth = LOWORD(lParam);
		CreateWindowSizeDependentResources();

		if(ulPosition_max > ulPosition_min){ dStep = (double)lWidth / (double)(ulPosition_max - ulPosition_min); }
		else dStep = 0;

		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COProgressBar::WM_LButtonDown(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	if(ulPosition_max){
		ulPosition = LOWORD(lParam);
		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	if(pfnWM_LButtonDown) pfnWM_LButtonDown(this, wParam, lParam);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COProgressBar::WM_MouseMove(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	if(ulPosition_max && wParam == MK_LBUTTON){
		ulPosition = LOWORD(lParam);
		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	if(pfnWM_MouseMove) pfnWM_MouseMove(this, wParam, lParam);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COProgressBar::SetPosition_Max(_In_ unsigned long ulPosition)
{
	ThreadSafe_Begin();
	ulPosition_max = ulPosition;
	if(ulPosition_max > ulPosition_min){ dStep = (double)lWidth / (double)(ulPosition_max - ulPosition_min); }
	else dStep = 0;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COProgressBar::SetPosition_Min(_In_ unsigned long ulPosition)
{
	ThreadSafe_Begin();
	ulPosition_min = ulPosition;
	if(ulPosition_max > ulPosition_min){ dStep = (double)lWidth / (double)(ulPosition_max - ulPosition_min); }
	else dStep = 0;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COProgressBar::SetPosition(_In_ unsigned long ulPositionA)
{
	ThreadSafe_Begin();
	if(!dStep) dStep = (double)lWidth / (double)(ulPosition_max - ulPosition_min); 
	ulPosition = (ULONG)((double)ulPositionA * dStep);
	if(hWndElement){
		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COProgressBar::FillColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfFillColor = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	if(ifFillColor) ifFillColor->SetColor(crfFillColor);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COProgressBar::FillColor(_In_ D2D1_COLOR_F& crfFillColorA)
{
	ThreadSafe_Begin();
	crfFillColor = crfFillColorA;
	if(ifFillColor) ifFillColor->SetColor(crfFillColor);
	ThreadSafe_End();
}