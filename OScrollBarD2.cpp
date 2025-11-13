/******************************************************************************
MIT License

Copyright(c) 2025 René Pagel

Filename : OScrollBarD2.cpp
For more information see https ://github.com/RePag-net/StdElemDX

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
#include "OScrollBarD2.h"

constexpr BYTE ARROW_UP = 1;
constexpr BYTE ARROW_DOWN = 2;
constexpr BYTE ARROW_LEFT = 3;
constexpr BYTE ARROW_RIGHT = 4;
constexpr BYTE THUMB_VERT = 5;
constexpr BYTE THUMB_HORZ = 6;
constexpr BYTE SLIDER_VERT = 7;
constexpr BYTE SLIDER_HORZ = 8;
constexpr BYTE NONCLIENT = 9;
constexpr CHAR THUMB_UP = -1;
constexpr CHAR THUMB_DOWN = 1;
constexpr CHAR THUMB_LEFT = -1;
constexpr CHAR THUMB_RIGHT = -1;
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COScrollBar* __vectorcall RePag::DirectX::COScrollBarV(_In_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																			 _In_ STDeviceResources* pstDeviceResources, bool bHorizontal)
{
	COScrollBar* vScrollBar = (COScrollBar*)VMBlock(VMDialog(), sizeof(COScrollBar));
	vScrollBar->COScrollBarV(VMDialog(), pcWindowName, uiIDElement, pstDeviceResources, bHorizontal);
	return vScrollBar;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COScrollBar* __vectorcall RePag::DirectX::COScrollBarV(_In_ const VMEMORY vmMemory, _In_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																			 _In_ STDeviceResources* pstDeviceResources, bool bHorizontal)
{
	COScrollBar* vScrollBar = (COScrollBar*)VMBlock(vmMemory, sizeof(COScrollBar));
	vScrollBar->COScrollBarV(vmMemory, pcWindowName, uiIDElement, pstDeviceResources, bHorizontal);
	return vScrollBar;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_ScrollBar(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	COScrollBar* pScrollBar;
	switch(uiMessage){
		case WM_CREATE			: ((COScrollBar*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
													((COScrollBar*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
													return NULL;
		case WM_SIZE				: pScrollBar = (COScrollBar*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pScrollBar) pScrollBar->WM_Size_Element(hWnd, lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_VSCROLL			: ((COScrollBar*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_VScroll(wParam);
													return NULL;
		case WM_HSCROLL			: ((COScrollBar*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_HScroll(wParam);
													return NULL;
		case WM_MOUSEHOVER	: ((COScrollBar*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseOver(wParam, lParam);
													return NULL;
		case WM_MOUSELEAVE	: ((COScrollBar*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseLeave();
													return NULL;
		case WM_MOUSEMOVE		: ((COScrollBar*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseMove(wParam, lParam);
													return NULL;
		case WM_LBUTTONDOWN	: ((COScrollBar*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDown(wParam, lParam);
													return NULL;
		case WM_LBUTTONUP		: ((COScrollBar*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonUp(wParam, lParam);
													return NULL;
		case WM_NCDESTROY		: pScrollBar = (COScrollBar*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													VMFreiV(pScrollBar);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::COScrollBarV(_In_ const VMEMORY vmMemory, _In_ const char* pcClassName, _In_ const char* pcWindowName, 
																														_In_ unsigned int uiIDElementA,	_In_ STDeviceResources* pstDeviceResourcesA, bool bHorizontalA)
{
	COGraphicV(vmMemory, pcClassName, pcWindowName, uiIDElementA, pstDeviceResourcesA);

	crfBackground = D2D1::ColorF(RGB(255, 255, 0), 1.0f);

	crfArrow = D2D1::ColorF(RGB(255, 255, 0), 1.0f);
	crfButton = D2D1::ColorF(RGB(125, 125, 125), 1.0f);
	crfThumb = D2D1::ColorF(RGB(125, 125, 125), 1.0f);

	crfButton_Move = D2D1::ColorF(RGB(255, 255, 0), 1.0f);
	crfArrow_Move = D2D1::ColorF(RGB(255, 50, 50), 1.f);
	crfThumb_Move = D2D1::ColorF(RGB(255, 100, 50), 1.0f);

	crfButton_Click = D2D1::ColorF(RGB(255, 50, 50), 1.0f);
	crfArrow_Click = D2D1::ColorF(RGB(255, 255, 0), 1.0f);
	crfThumb_Click = D2D1::ColorF(RGB(255, 50, 50), 1.0f);

	fScaleArrowThumb = 75.0f;
	siScrollInfo = {0};
	rcfThumb = {0};
	fThumb = 0;
	fStep = 0;
	bMouseTracking = false;
	ucDirty = NONCLIENT;
	bHorizontal = bHorizontalA;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::COScrollBarV(_In_ const VMEMORY vmMemory, _In_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																														_In_ STDeviceResources* pstDeviceResourcesA, bool bHorizontalA)
{
	COScrollBarV(vmMemory, pcRePag_ScrollBar, pcWindowName, uiIDElementA, pstDeviceResourcesA, bHorizontalA);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COScrollBar::COFreiV(void)
{
	SafeRelease(&ifButton_Up); SafeRelease(&ifButton_Down); SafeRelease(&ifThumb); SafeRelease(&ifArrow_Up); SafeRelease(&ifArrow_Down);
	SafeRelease(&ifButtonColor_Up); SafeRelease(&ifArrowColor_Up); SafeRelease(&ifButtonColor_Down); SafeRelease(&ifArrowColor_Down); 
	SafeRelease(&ifThumbColor);
	return ((COGraphic*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::OnRender(void)
{
	ifD2D1Context6->BeginDraw();
	ifD2D1Context6->Clear(crfBackground);
	ifD2D1Context6->DrawGeometry(ifButton_Up, ifButtonColor_Up, 1);
	ifD2D1Context6->FillGeometry(ifButton_Up, ifButtonColor_Up);

	ifD2D1Context6->DrawGeometry(ifButton_Down, ifButtonColor_Down, 1);
	ifD2D1Context6->FillGeometry(ifButton_Down, ifButtonColor_Down);

	ifD2D1Context6->DrawGeometry(ifArrow_Up, ifArrowColor_Up, 1);
	ifD2D1Context6->FillGeometry(ifArrow_Up, ifArrowColor_Up);

	ifD2D1Context6->DrawGeometry(ifArrow_Down, ifArrowColor_Down, 1);
	ifD2D1Context6->FillGeometry(ifArrow_Down, ifArrowColor_Down);

	if(ifThumb){
		ifD2D1Context6->DrawGeometry(ifThumb, ifThumbColor, 1);
		ifD2D1Context6->FillGeometry(ifThumb, ifThumbColor);
	}
	ifD2D1Context6->EndDraw();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::OnPaint(void)
{
	ThreadSafe_Begin();
	OnRender();
	rclDirty.left = 0; rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_Create(void)
{
	stTrackMouseEvent.cbSize = sizeof(stTrackMouseEvent);
	stTrackMouseEvent.dwFlags = TME_HOVER | TME_LEAVE;
	stTrackMouseEvent.hwndTrack = hWndElement;
	stTrackMouseEvent.dwHoverTime = 10;

	ifD2D1Context6->CreateSolidColorBrush(crfButton, &ifButtonColor_Up);
	ifD2D1Context6->CreateSolidColorBrush(crfArrow, &ifArrowColor_Up);
	ifD2D1Context6->CreateSolidColorBrush(crfButton, &ifButtonColor_Down);
	ifD2D1Context6->CreateSolidColorBrush(crfArrow, &ifArrowColor_Down);
	ifD2D1Context6->CreateSolidColorBrush(crfThumb, &ifThumbColor);

	Geometry();

	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_MouseMove(WPARAM wParam, LPARAM lParam)
{
	POINTS ptlCursor = MAKEPOINTS(lParam); RECT rcl2Dirty[2];	D2D1_ROUNDED_RECT rrcfThumb;
	static char cCursor_Move = 0; static long lCursor = 0; static char cThumbDirection = 0;
	ThreadSafe_Begin();

	if(!bMouseTracking){ bMouseTracking = true;	TrackMouseEvent(&stTrackMouseEvent); }
	else{
		switch(ucDirty){
			case ARROW_UP			: if(ptlCursor.y > lWidth){
														ifButtonColor_Up->SetColor(crfButton);
														ifArrowColor_Up->SetColor(crfArrow);
														rclDirty.left = 0; rclDirty.top = 0;
														rclDirty.right = lWidth; rclDirty.bottom = lWidth;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = SLIDER_VERT;
													}
													break;
			case ARROW_DOWN		: if(ptlCursor.y < lHeight - lWidth){
														ifButtonColor_Down->SetColor(crfButton);
														ifArrowColor_Down->SetColor(crfArrow);
														rclDirty.left = 0; rclDirty.top = lHeight - lWidth;
														rclDirty.right = lWidth; rclDirty.bottom = lHeight;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = SLIDER_VERT;
													}
													break;
			case ARROW_LEFT		:	if(ptlCursor.x > lHeight){
														ifButtonColor_Up->SetColor(crfButton);
														ifArrowColor_Up->SetColor(crfArrow);
														rclDirty.left = 0; rclDirty.top = 0;
														rclDirty.right = lHeight; rclDirty.bottom = lHeight;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = SLIDER_HORZ;
													}
													break;
			case ARROW_RIGHT	: if(ptlCursor.x < lWidth - lHeight){
														ifButtonColor_Down->SetColor(crfButton);
														ifArrowColor_Down->SetColor(crfArrow);
														rclDirty.left = lWidth - lHeight; rclDirty.top = 0;
														rclDirty.right = lWidth; rclDirty.bottom = lHeight;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = SLIDER_HORZ;
													}
													break;
			case THUMB_VERT		:	if(wParam == MK_LBUTTON){
														if(ptlCursor.y < lCursor){
															if(cThumbDirection == THUMB_UP) cCursor_Move -= lCursor - ptlCursor.y;
															else{ cThumbDirection = THUMB_UP; cCursor_Move = 0; }
														}
														else if(ptlCursor.y > lCursor){
															if(cThumbDirection == THUMB_DOWN) cCursor_Move += ptlCursor.y - lCursor;
															else{ cThumbDirection = THUMB_DOWN; cCursor_Move = 0; }
														}
														lCursor = ptlCursor.y;
														if(cCursor_Move > (char)fStep && siScrollInfo.fPos < siScrollInfo.fMax - siScrollInfo.fPage){
															cCursor_Move = 0;
															siScrollInfo.fPos += siScrollInfo.szfCharacter.height;
															dxgiPresent.DirtyRectsCount = 2;
															dxgiPresent.pDirtyRects = rcl2Dirty;

															rcl2Dirty[0].top = (long)(rcfThumb.top - 2.0f); rcl2Dirty[0].bottom = (long)rcfThumb.bottom;
															rcl2Dirty[0].left = (long)rcfThumb.left; rcl2Dirty[0].right = (long)rcfThumb.right;
															if(siScrollInfo.fPos < siScrollInfo.fMax - siScrollInfo.fPage){ 
																		rcfThumb.top += fStep; rcfThumb.bottom += fStep; }
															else{ rcfThumb.bottom = (float)(lHeight - lWidth); rcfThumb.top = rcfThumb.bottom - fThumb; }
															rrcfThumb = {rcfThumb, 2.0f, 2.0f};
															SafeRelease(&ifThumb);
															pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
															rcl2Dirty[1].top = (long)rcfThumb.top; rcl2Dirty[1].bottom = (long)rcfThumb.bottom;
															rcl2Dirty[1].left = (long)rcfThumb.left; rcl2Dirty[1].right = (long)rcfThumb.right;

															OnRender();
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															dxgiPresent.DirtyRectsCount = 1;
															dxgiPresent.pDirtyRects = &rclDirty;

															SendMessage(GetParent(hWndElement), WM_VSCROLL, SB_LINEDOWN, NULL);
														}
														else if(cCursor_Move < (char)fStep * -1 && siScrollInfo.fPos > 0){
															cCursor_Move = 0;
															siScrollInfo.fPos -= siScrollInfo.szfCharacter.height;
															dxgiPresent.DirtyRectsCount = 2;
															dxgiPresent.pDirtyRects = rcl2Dirty;

															rcl2Dirty[0].top = (long)rcfThumb.top; rcl2Dirty[0].bottom = (long)(rcfThumb.bottom + 2.0f);
															rcl2Dirty[0].left = (long)rcfThumb.left; rcl2Dirty[0].right = (long)rcfThumb.right;
															if(siScrollInfo.fPos > 0){
																		rcfThumb.top -= fStep; rcfThumb.bottom -= fStep; }
															else{ rcfThumb.top = (float)lWidth; rcfThumb.bottom = rcfThumb.top + fThumb; }
															rrcfThumb = {rcfThumb, 2.0f, 2.0f};
															SafeRelease(&ifThumb);
															pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
															rcl2Dirty[1].top = (long)rcfThumb.top; rcl2Dirty[1].bottom = (long)rcfThumb.bottom;
															rcl2Dirty[1].left = (long)rcfThumb.left; rcl2Dirty[1].right = (long)rcfThumb.right;

															OnRender();
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															dxgiPresent.DirtyRectsCount = 1;
															dxgiPresent.pDirtyRects = &rclDirty;

															SendMessage(GetParent(hWndElement), WM_VSCROLL, SB_LINEUP, NULL);
														}
													}
													else if(ptlCursor.y < (long)rcfThumb.top || ptlCursor.y > (long)rcfThumb.bottom){
														ifThumbColor->SetColor(crfThumb);
														rclDirty.top = (long)rcfThumb.top; rclDirty.bottom = (long)rcfThumb.bottom;
														rclDirty.left = (long)rcfThumb.left; rclDirty.right = (long)rcfThumb.right;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = SLIDER_VERT;
													}
													break;
			case THUMB_HORZ		:	if(wParam == MK_LBUTTON){
														if(ptlCursor.x < lCursor){
															if(cThumbDirection == THUMB_LEFT) cCursor_Move -= lCursor - ptlCursor.x;
															else{ cThumbDirection = THUMB_LEFT; cCursor_Move = 0; }
														}
														else if(ptlCursor.x > lCursor){
															if(cThumbDirection == THUMB_RIGHT) cCursor_Move += ptlCursor.x - lCursor;
															else{ cThumbDirection = THUMB_RIGHT; cCursor_Move = 0; }
														}
														lCursor = ptlCursor.x;
														if(cCursor_Move > (char)fStep && siScrollInfo.fPos < siScrollInfo.fMax - siScrollInfo.fPage){
															cCursor_Move = 0;
															siScrollInfo.fPos += siScrollInfo.szfCharacter.width;
															dxgiPresent.DirtyRectsCount = 2;
															dxgiPresent.pDirtyRects = rcl2Dirty;

															rcl2Dirty[0].top = (long)rcfThumb.top; rcl2Dirty[0].bottom = (long)rcfThumb.bottom;
															rcl2Dirty[0].left = (long)(rcfThumb.left - 2.0f); rcl2Dirty[0].right = (long)rcfThumb.right;
															if(siScrollInfo.fPos < siScrollInfo.fMax - siScrollInfo.fPage){
																		rcfThumb.left += fStep; rcfThumb.right += fStep; }
															else{ rcfThumb.right = (float)(lWidth - lHeight); rcfThumb.left = rcfThumb.right - fThumb; }
															rrcfThumb = {rcfThumb, 2.0f, 2.0f};
															SafeRelease(&ifThumb);
															pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
															rcl2Dirty[1].top = (long)rcfThumb.top; rcl2Dirty[1].bottom = (long)rcfThumb.bottom;
															rcl2Dirty[1].left = (long)rcfThumb.left; rcl2Dirty[1].right = (long)rcfThumb.right;

															OnRender();
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															dxgiPresent.DirtyRectsCount = 1;
															dxgiPresent.pDirtyRects = &rclDirty;

															SendMessage(GetParent(hWndElement), WM_HSCROLL, SB_LINERIGHT, NULL);
														}
														else if(cCursor_Move < (char)fStep * -1 && siScrollInfo.fPos > 0){
															cCursor_Move = 0;
															siScrollInfo.fPos -= siScrollInfo.szfCharacter.width;
															dxgiPresent.DirtyRectsCount = 2;
															dxgiPresent.pDirtyRects = rcl2Dirty;

															rcl2Dirty[0].top = (long)rcfThumb.top; rcl2Dirty[0].bottom = (long)rcfThumb.bottom;
															rcl2Dirty[0].left = (long)rcfThumb.left; rcl2Dirty[0].right = (long)(rcfThumb.right + 2.0f);
															if(siScrollInfo.fPos > 0){
																		rcfThumb.left -= fStep; rcfThumb.right -= fStep; }
															else{ rcfThumb.left = (float)lHeight; rcfThumb.right = rcfThumb.left + fThumb; }
															rrcfThumb = {rcfThumb, 2.0f, 2.0f};
															SafeRelease(&ifThumb);
															pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
															rcl2Dirty[1].top = (long)rcfThumb.top; rcl2Dirty[1].bottom = (long)rcfThumb.bottom;
															rcl2Dirty[1].left = (long)rcfThumb.left; rcl2Dirty[1].right = (long)rcfThumb.right;

															OnRender();
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															dxgiPresent.DirtyRectsCount = 1;
															dxgiPresent.pDirtyRects = &rclDirty;

															SendMessage(GetParent(hWndElement), WM_HSCROLL, SB_LINELEFT, NULL);
														}
													}
													else if(ptlCursor.x < (long)rcfThumb.left || ptlCursor.x > (long)rcfThumb.right){
														ifThumbColor->SetColor(crfThumb);
														rclDirty.top = (long)rcfThumb.top; rclDirty.bottom = (long)rcfThumb.bottom;
														rclDirty.left = (long)rcfThumb.left; rclDirty.right = (long)rcfThumb.right;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = SLIDER_HORZ;
													}
													break;
			case SLIDER_VERT	:	if(ptlCursor.y <= lWidth){
														ifButtonColor_Up->SetColor(crfButton_Move);
														ifArrowColor_Up->SetColor(crfArrow_Move);
														rclDirty.left = 0; rclDirty.top = 0;
														rclDirty.right = lWidth; rclDirty.bottom = lWidth;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = ARROW_UP;
													}
													else if(ptlCursor.y >= lHeight - lWidth){
														ifButtonColor_Down->SetColor(crfButton_Move);
														ifArrowColor_Down->SetColor(crfArrow_Move);
														rclDirty.left = 0; rclDirty.top = lHeight - lWidth;
														rclDirty.right = lWidth; rclDirty.bottom = lHeight;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = ARROW_DOWN;
													}
													else if(ptlCursor.y >= (long)rcfThumb.top && ptlCursor.y <= (long)rcfThumb.bottom){
														ifThumbColor->SetColor(crfThumb_Move);
														rclDirty.top = (long)rcfThumb.top; rclDirty.bottom = (long)rcfThumb.bottom;
														rclDirty.left = (long)rcfThumb.left; rclDirty.right = (long)rcfThumb.right;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = THUMB_VERT;
													}
													break;
			case SLIDER_HORZ	: if(ptlCursor.x <= lHeight){
														ifButtonColor_Up->SetColor(crfButton_Move);
														ifArrowColor_Up->SetColor(crfArrow_Move);
														rclDirty.left = 0; rclDirty.top = 0;
														rclDirty.right = lHeight; rclDirty.bottom = lHeight;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = ARROW_LEFT;
													}
													else if(ptlCursor.x >= lWidth - lHeight){
														ifButtonColor_Down->SetColor(crfButton_Move);
														ifArrowColor_Down->SetColor(crfArrow_Move);
														rclDirty.left = lWidth - lHeight; rclDirty.top = 0;
														rclDirty.right = lWidth; rclDirty.bottom = lHeight;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = ARROW_RIGHT;
													}
													else if(ptlCursor.x >= (long)rcfThumb.left && ptlCursor.x <= (long)rcfThumb.right){
														ifThumbColor->SetColor(crfThumb_Move);
														rclDirty.top = (long)rcfThumb.top; rclDirty.bottom = (long)rcfThumb.bottom;
														rclDirty.left = (long)rcfThumb.left; rclDirty.right = (long)rcfThumb.right;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = THUMB_HORZ;
													}
		}
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_MouseOver(WPARAM wParam, LPARAM lParam)
{
	POINTS ptlCursor = MAKEPOINTS(lParam);
	ThreadSafe_Begin();
	if(!bHorizontal){
		if(ptlCursor.y <= lWidth){
			ifButtonColor_Up->SetColor(crfButton_Move);
			ifArrowColor_Up->SetColor(crfArrow_Move);
			rclDirty.left = 0; rclDirty.top = 0;
			rclDirty.right = lWidth; rclDirty.bottom = lWidth;
			OnRender();
			ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
			ucDirty = ARROW_UP;
		}
		else if(ptlCursor.y >= lHeight - lWidth){
			ifButtonColor_Down->SetColor(crfButton_Move);
			ifArrowColor_Down->SetColor(crfArrow_Move);
			rclDirty.left = 0; rclDirty.top = lHeight - lWidth;
			rclDirty.right = lWidth; rclDirty.bottom = lHeight;
			OnRender();
			ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
			ucDirty = ARROW_DOWN;
		}
		else if(ptlCursor.y >= (long)rcfThumb.top && ptlCursor.y <= (long)rcfThumb.bottom){
			ifThumbColor->SetColor(crfThumb_Move);
			rclDirty.top = (long)rcfThumb.top; rclDirty.bottom = (long)rcfThumb.bottom;
			rclDirty.left = (long)rcfThumb.left; rclDirty.right = (long)rcfThumb.right;
			OnRender();
			ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
			ucDirty = THUMB_VERT;
		}
		else if(ptlCursor.y < (long)rcfThumb.top || ptlCursor.y > (long)rcfThumb.bottom) ucDirty = SLIDER_VERT;
	}
	else{
		if(ptlCursor.x <= lHeight){
			ifButtonColor_Up->SetColor(crfButton_Move);
			ifArrowColor_Up->SetColor(crfArrow_Move);
			rclDirty.left = 0; rclDirty.top = 0;
			rclDirty.right = lHeight; rclDirty.bottom = lHeight;
			OnRender();
			ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
			ucDirty = ARROW_LEFT;
		}
		else if(ptlCursor.x >= lWidth - lHeight){
			ifButtonColor_Down->SetColor(crfButton_Move);
			ifArrowColor_Down->SetColor(crfArrow_Move);
			rclDirty.left = lWidth - lHeight; rclDirty.top = 0;
			rclDirty.right = lWidth; rclDirty.bottom = lHeight;
			OnRender();
			ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
			ucDirty = ARROW_RIGHT;
		}
		else if(ptlCursor.x >= (long)rcfThumb.left && ptlCursor.x <= (long)rcfThumb.right){
			ifThumbColor->SetColor(crfThumb_Move);
			rclDirty.top = (long)rcfThumb.top; rclDirty.bottom = (long)rcfThumb.bottom;
			rclDirty.left = (long)rcfThumb.left; rclDirty.right = (long)rcfThumb.right;
			OnRender();
			ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
			ucDirty = THUMB_HORZ;
		}
		else if(ptlCursor.x < (long)rcfThumb.left || ptlCursor.x > (long)rcfThumb.right) ucDirty = SLIDER_HORZ;
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_MouseLeave(void)
{
	ThreadSafe_Begin();
	switch(ucDirty){
		case ARROW_UP			: ifButtonColor_Up->SetColor(crfButton);
												ifArrowColor_Up->SetColor(crfArrow);
												rclDirty.left = 0; rclDirty.top = 0;
												rclDirty.right = lWidth; rclDirty.bottom = lWidth;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												break;
		case ARROW_DOWN	 :	ifButtonColor_Down->SetColor(crfButton);
												ifArrowColor_Down->SetColor(crfArrow);
												rclDirty.left = 0; rclDirty.top = lHeight - lWidth;
												rclDirty.right = lWidth; rclDirty.bottom = lHeight;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												break;
		case ARROW_LEFT		: ifButtonColor_Up->SetColor(crfButton);
												ifArrowColor_Up->SetColor(crfArrow);
												rclDirty.left = 0; rclDirty.top = 0;
												rclDirty.right = lHeight; rclDirty.bottom = lHeight;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												break;
		case ARROW_RIGHT	:	ifButtonColor_Down->SetColor(crfButton);
												ifArrowColor_Down->SetColor(crfArrow);
												rclDirty.left = lWidth - lHeight; rclDirty.top = 0;
												rclDirty.right = lWidth; rclDirty.bottom = lHeight;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												break;
		case THUMB_HORZ		:
		case THUMB_VERT		:	ifThumbColor->SetColor(crfThumb);
												rclDirty.top = (long)rcfThumb.top; rclDirty.bottom = (long)rcfThumb.bottom;
												rclDirty.left = (long)rcfThumb.left; rclDirty.right = (long)rcfThumb.right;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ucDirty = NONCLIENT;
	bMouseTracking = false;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_LButtonDown(WPARAM wParam, LPARAM lParam)
{
	POINTS ptlCursor = MAKEPOINTS(lParam);
	ThreadSafe_Begin();
	switch(ucDirty){
		case ARROW_UP			:	if(ptlCursor.y <= lWidth){
													ifButtonColor_Up->SetColor(crfButton_Click);
													ifArrowColor_Up->SetColor(crfArrow_Click);
													rclDirty.left = 0; rclDirty.top = 0;
													rclDirty.right = lWidth; rclDirty.bottom = lWidth;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
												break;
		case ARROW_DOWN		:	if(ptlCursor.y >= lHeight - lWidth){
													ifButtonColor_Down->SetColor(crfButton_Click);
													ifArrowColor_Down->SetColor(crfArrow_Click);
													rclDirty.left = 0; rclDirty.top = lHeight - lWidth;
													rclDirty.right = lWidth; rclDirty.bottom = lHeight;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
												break;
		case ARROW_LEFT		:	if(ptlCursor.x <= lHeight){
													ifButtonColor_Up->SetColor(crfButton_Click);
													ifArrowColor_Up->SetColor(crfArrow_Click);
													rclDirty.left = 0; rclDirty.top = 0;
													rclDirty.right = lHeight; rclDirty.bottom = lHeight;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
												break;
		case ARROW_RIGHT	:	if(ptlCursor.x >=  lWidth - lHeight){
													ifButtonColor_Down->SetColor(crfButton_Click);
													ifArrowColor_Down->SetColor(crfArrow_Click);
													rclDirty.left = lWidth - lHeight; rclDirty.top = 0;
													rclDirty.right = lWidth; rclDirty.bottom = lHeight;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
												break;
		case THUMB_VERT		: if(ptlCursor.y >= (long)rcfThumb.top && ptlCursor.y <= (long)rcfThumb.bottom){
													ifThumbColor->SetColor(crfThumb_Click);
													rclDirty.top = (long)rcfThumb.top; rclDirty.bottom = (long)rcfThumb.bottom;
													rclDirty.left = (long)rcfThumb.left; rclDirty.right = (long)rcfThumb.right;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
												break;
		case THUMB_HORZ		: if(ptlCursor.x >= (long)rcfThumb.left && ptlCursor.x <= (long)rcfThumb.right){
													ifThumbColor->SetColor(crfThumb_Click);
													rclDirty.top = (long)rcfThumb.top; rclDirty.bottom = (long)rcfThumb.bottom;
													rclDirty.left = (long)rcfThumb.left; rclDirty.right = (long)rcfThumb.right;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_LButtonUp(WPARAM wParam, LPARAM lParam)
{
	POINTS ptlCursor = MAKEPOINTS(lParam); RECT rcl3Dirty[3]; D2D1_ROUNDED_RECT rrcfThumb;
	ThreadSafe_Begin();
	switch(ucDirty){
		case ARROW_UP			:	if(siScrollInfo.fPos > 0){
													siScrollInfo.fPos -= siScrollInfo.szfCharacter.height;
													dxgiPresent.DirtyRectsCount = 3;
													dxgiPresent.pDirtyRects = rcl3Dirty;

													rcl3Dirty[2].top = (long)rcfThumb.top; rcl3Dirty[2].bottom = (long)(rcfThumb.bottom + 2.0f);
													rcl3Dirty[2].left = (long)rcfThumb.left; rcl3Dirty[2].right = (long)rcfThumb.right;
													if(siScrollInfo.fPos > 0){	rcfThumb.bottom -= fStep; rcfThumb.top -= fStep; }
													else{ rcfThumb.top = (float)lWidth; rcfThumb.bottom = rcfThumb.top + fThumb;  }
													rrcfThumb = {rcfThumb, 2.0f, 2.0f};
													SafeRelease(&ifThumb);
													pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
													rcl3Dirty[1].top = (long)rcfThumb.top; rcl3Dirty[1].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[1].left = (long)rcfThumb.left; rcl3Dirty[1].right = (long)rcfThumb.right;

													ifArrowColor_Up->SetColor(crfArrow_Move);
													ifButtonColor_Up->SetColor(crfButton_Move);
													rcl3Dirty[0].left = 0; rcl3Dirty[0].top = 0;
													rcl3Dirty[0].right = lWidth; rcl3Dirty[0].bottom = lWidth;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													dxgiPresent.DirtyRectsCount = 1;
													dxgiPresent.pDirtyRects = &rclDirty;

													SendMessage(GetParent(hWndElement), WM_VSCROLL, SB_LINEUP, NULL);
												}
												else{
													ifButtonColor_Up->SetColor(crfButton_Move);
													ifArrowColor_Up->SetColor(crfArrow_Move);
													rclDirty.left = 0; rclDirty.top = 0;
													rclDirty.right = lWidth; rclDirty.bottom = lWidth;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
												break;
		case	ARROW_DOWN	:	if(siScrollInfo.fPos + siScrollInfo.fPage < siScrollInfo.fMax){
													siScrollInfo.fPos += siScrollInfo.szfCharacter.height;
													dxgiPresent.DirtyRectsCount = 3;
													dxgiPresent.pDirtyRects = rcl3Dirty;

													rcl3Dirty[2].top = (long)(rcfThumb.top - 2.0f); rcl3Dirty[2].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[2].left = (long)rcfThumb.left; rcl3Dirty[2].right = (long)rcfThumb.right;
													if(siScrollInfo.fPos + siScrollInfo.fPage < siScrollInfo.fMax){
																rcfThumb.bottom += fStep; rcfThumb.top += fStep; }
													else{ rcfThumb.bottom = (float)(lHeight - lWidth); rcfThumb.top = rcfThumb.bottom - fThumb; }
													rrcfThumb = {rcfThumb, 2.0f, 2.0f};
													SafeRelease(&ifThumb);
													pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
													rcl3Dirty[1].top = (long)rcfThumb.top; rcl3Dirty[1].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[1].left = (long)rcfThumb.left; rcl3Dirty[1].right = (long)rcfThumb.right;

													ifArrowColor_Down->SetColor(crfArrow_Move);
													ifButtonColor_Down->SetColor(crfButton_Move);
													rcl3Dirty[0].left = 0; rcl3Dirty[0].top = lHeight - lWidth;
													rcl3Dirty[0].right = lWidth; rcl3Dirty[0].bottom = lHeight;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													dxgiPresent.DirtyRectsCount = 1;
													dxgiPresent.pDirtyRects = &rclDirty;

													SendMessage(GetParent(hWndElement), WM_VSCROLL, SB_LINEDOWN, NULL);
												}
												else{
													ifButtonColor_Down->SetColor(crfButton_Move);
													ifArrowColor_Down->SetColor(crfArrow_Move);
													rclDirty.left = 0; rclDirty.top = lHeight - lWidth;
													rclDirty.right = lWidth; rclDirty.bottom = lHeight;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
												break;
		case ARROW_LEFT		:	if(siScrollInfo.fPos > 0){
													siScrollInfo.fPos -= siScrollInfo.szfCharacter.width;
													dxgiPresent.DirtyRectsCount = 3;
													dxgiPresent.pDirtyRects = rcl3Dirty;

													rcl3Dirty[2].top = (long)rcfThumb.top; rcl3Dirty[2].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[2].left = (long)rcfThumb.left; rcl3Dirty[2].right = (long)(rcfThumb.right + 2.0f);
													if(siScrollInfo.fPos > 0){ rcfThumb.right -= fStep; rcfThumb.left -= fStep;	}
													else{ rcfThumb.left = (float)lHeight; rcfThumb.right = rcfThumb.left + fThumb; }
													rrcfThumb = {rcfThumb, 2.0f, 2.0f};
													SafeRelease(&ifThumb);
													pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
													rcl3Dirty[1].top = (long)rcfThumb.top; rcl3Dirty[1].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[1].left = (long)rcfThumb.left; rcl3Dirty[1].right = (long)rcfThumb.right;

													ifButtonColor_Up->SetColor(crfButton_Move);
													ifArrowColor_Up->SetColor(crfArrow_Move);
													rcl3Dirty[0].left = 0; rcl3Dirty[0].top = 0;
													rcl3Dirty[0].right = lHeight; rcl3Dirty[0].bottom = lHeight;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													dxgiPresent.DirtyRectsCount = 1;
													dxgiPresent.pDirtyRects = &rclDirty;

													SendMessage(GetParent(hWndElement), WM_HSCROLL, SB_LINELEFT, NULL);
												}
												else{
													ifButtonColor_Up->SetColor(crfButton_Move);
													ifArrowColor_Up->SetColor(crfArrow_Move);
													rclDirty.left = 0; rclDirty.top = 0;
													rclDirty.right = lHeight; rclDirty.bottom = lHeight;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
												break;
		case	ARROW_RIGHT	:	if(siScrollInfo.fPos + siScrollInfo.fPage < siScrollInfo.fMax){
													siScrollInfo.fPos += siScrollInfo.szfCharacter.width;
													dxgiPresent.DirtyRectsCount = 3;
													dxgiPresent.pDirtyRects = rcl3Dirty;

													rcl3Dirty[2].top = (long)rcfThumb.top; rcl3Dirty[2].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[2].left = (long)(rcfThumb.left - 2.0f); rcl3Dirty[2].right = (long)rcfThumb.right;
													if(siScrollInfo.fPos + siScrollInfo.fPage < siScrollInfo.fMax){
																rcfThumb.right += fStep; rcfThumb.left += fStep; }
													else{ rcfThumb.right = (float)(lWidth - lHeight); rcfThumb.left = rcfThumb.right - fThumb; }
													rrcfThumb = {rcfThumb, 2.0f, 2.0f};
													SafeRelease(&ifThumb);
													pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
													rcl3Dirty[1].top = (long)rcfThumb.top; rcl3Dirty[1].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[1].left = (long)rcfThumb.left; rcl3Dirty[1].right = (long)rcfThumb.right;

													ifButtonColor_Down->SetColor(crfButton_Move);
													ifArrowColor_Down->SetColor(crfArrow_Move);
													rcl3Dirty[0].left = lWidth - lHeight; rcl3Dirty[0].top = 0;
													rcl3Dirty[0].right = lWidth; rcl3Dirty[0].bottom = lHeight;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													dxgiPresent.DirtyRectsCount = 1;
													dxgiPresent.pDirtyRects = &rclDirty;

													SendMessage(GetParent(hWndElement), WM_HSCROLL, SB_LINERIGHT, NULL);
												}
												else{
													ifButtonColor_Down->SetColor(crfButton_Move);
													ifArrowColor_Down->SetColor(crfArrow_Move);
													rclDirty.left = lWidth - lHeight; rclDirty.top = 0;
													rclDirty.right = lWidth; rclDirty.bottom = lHeight;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
												break;
		case THUMB_HORZ		:
		case THUMB_VERT		:	rclDirty.top = (long)rcfThumb.top; rclDirty.bottom = (long)rcfThumb.bottom;
												rclDirty.left = (long)rcfThumb.left; rclDirty.right = (long)rcfThumb.right;
												ifThumbColor->SetColor(crfThumb_Move);
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												break;
		case SLIDER_VERT:		if(ptlCursor.y > (long)rcfThumb.bottom && ptlCursor.y < lHeight - lWidth){
													if(siScrollInfo.fPos + siScrollInfo.fPage * 2 < siScrollInfo.fMax) siScrollInfo.fPos += siScrollInfo.fPage;
													else siScrollInfo.fPos = siScrollInfo.fMax - siScrollInfo.fPage;
													dxgiPresent.DirtyRectsCount = 2;
													dxgiPresent.pDirtyRects = rcl3Dirty;

													rcl3Dirty[1].top = (long)(rcfThumb.top - 2.0f); rcl3Dirty[1].bottom = (long)(rcfThumb.bottom + 2.0f);
													rcl3Dirty[1].left = (long)(rcfThumb.left - 2.0f); rcl3Dirty[1].right = (long)(rcfThumb.right + 2.0f);
													if(siScrollInfo.fPos + siScrollInfo.fPage < siScrollInfo.fMax){
																rcfThumb.top += fThumb; rcfThumb.bottom += fThumb; }
													else{	rcfThumb.bottom = (float)(lHeight - lWidth); rcfThumb.top = rcfThumb.bottom - fThumb;	}
													rrcfThumb = {rcfThumb, 2.0f, 2.0f};
													SafeRelease(&ifThumb);
													pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
													rcl3Dirty[0].top = (long)rcfThumb.top; rcl3Dirty[0].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[0].left = (long)rcfThumb.left; rcl3Dirty[0].right = (long)rcfThumb.right;

													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													dxgiPresent.DirtyRectsCount = 1;
													dxgiPresent.pDirtyRects = &rclDirty;

													SendMessage(GetParent(hWndElement), WM_VSCROLL, SB_PAGEDOWN, NULL);
												}
												else if(ptlCursor.y < (long)rcfThumb.top && ptlCursor.y > lWidth){
													if(siScrollInfo.fPos > siScrollInfo.fPage) siScrollInfo.fPos -= siScrollInfo.fPage;
													else siScrollInfo.fPos = 0;
													dxgiPresent.DirtyRectsCount = 2;
													dxgiPresent.pDirtyRects = rcl3Dirty;

													rcl3Dirty[1].top = (long)(rcfThumb.top - 2.0f); rcl3Dirty[1].bottom = (long)(rcfThumb.bottom + 2.0f);
													rcl3Dirty[1].left = (long)(rcfThumb.left - 2.0f); rcl3Dirty[1].right = (long)(rcfThumb.right + 2.0f);
													if(siScrollInfo.fPos > 0){ rcfThumb.top -= fThumb; rcfThumb.bottom -= fThumb;	}
													else{	rcfThumb.top = (float)lWidth; rcfThumb.bottom = rcfThumb.top + fThumb; }
													rrcfThumb = {rcfThumb, 2.0f, 2.0f};
													SafeRelease(&ifThumb);
													pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
													rcl3Dirty[0].top = (long)rcfThumb.top; rcl3Dirty[0].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[0].left = (long)rcfThumb.left; rcl3Dirty[0].right = (long)rcfThumb.right;

													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													dxgiPresent.DirtyRectsCount = 1;
													dxgiPresent.pDirtyRects = &rclDirty;

													SendMessage(GetParent(hWndElement), WM_VSCROLL, SB_PAGEUP, NULL);
												}
												break;
		case SLIDER_HORZ	:	if(ptlCursor.x > (long)rcfThumb.right && ptlCursor.x < lWidth - lHeight){
													if(siScrollInfo.fPos + siScrollInfo.fPage * 2 < siScrollInfo.fMax) siScrollInfo.fPos += siScrollInfo.fPage;
													else siScrollInfo.fPos = siScrollInfo.fMax - siScrollInfo.fPage;
													dxgiPresent.DirtyRectsCount = 2;
													dxgiPresent.pDirtyRects = rcl3Dirty;

													rcl3Dirty[1].top = (long)(rcfThumb.top - 2.0f); rcl3Dirty[1].bottom = (long)(rcfThumb.bottom + 2.0f);
													rcl3Dirty[1].left = (long)(rcfThumb.left - 2.0f); rcl3Dirty[1].right = (long)(rcfThumb.right + 2.0f);
													if(siScrollInfo.fPos + siScrollInfo.fPage < siScrollInfo.fMax){
																rcfThumb.left += fThumb; rcfThumb.right += fThumb; }
													else{ rcfThumb.right = (float)(lWidth - lHeight); rcfThumb.left = rcfThumb.right - fThumb; }
													rrcfThumb = {rcfThumb, 2.0f, 2.0f};
													SafeRelease(&ifThumb);
													pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
													rcl3Dirty[0].top = (long)rcfThumb.top; rcl3Dirty[0].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[0].left = (long)rcfThumb.left; rcl3Dirty[0].right = (long)rcfThumb.right;

													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													dxgiPresent.DirtyRectsCount = 1;
													dxgiPresent.pDirtyRects = &rclDirty;

													SendMessage(GetParent(hWndElement), WM_HSCROLL, SB_PAGERIGHT, NULL);
												}
												else if(ptlCursor.x < (long)rcfThumb.left && ptlCursor.x > lHeight){
													if(siScrollInfo.fPos > siScrollInfo.fPage) siScrollInfo.fPos -= siScrollInfo.fPage;
													else siScrollInfo.fPos = 0;
													dxgiPresent.DirtyRectsCount = 2;
													dxgiPresent.pDirtyRects = rcl3Dirty;

													rcl3Dirty[1].top = (long)(rcfThumb.top - 2.0f); rcl3Dirty[1].bottom = (long)(rcfThumb.bottom + 2.0f);
													rcl3Dirty[1].left = (long)(rcfThumb.left - 2.0f); rcl3Dirty[1].right = (long)(rcfThumb.right + 2.0f);
													if(siScrollInfo.fPos > 0){ rcfThumb.left -= fThumb; rcfThumb.right -= fThumb; }
													else{ rcfThumb.left = (float)lHeight; rcfThumb.right = rcfThumb.left + fThumb; }
													rrcfThumb = {rcfThumb, 2.0f, 2.0f};
													SafeRelease(&ifThumb);
													pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
													rcl3Dirty[0].top = (long)rcfThumb.top; rcl3Dirty[0].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[0].left = (long)rcfThumb.left; rcl3Dirty[0].right = (long)rcfThumb.right;

													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													dxgiPresent.DirtyRectsCount = 1;
													dxgiPresent.pDirtyRects = &rclDirty;

													SendMessage(GetParent(hWndElement), WM_HSCROLL, SB_PAGELEFT, NULL);
												}
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_VScroll(_In_ WPARAM wParam)
{
	ThreadSafe_Begin();
	switch(LOWORD(wParam)){
		case SB_LINEUP		: if(siScrollInfo.fPos > 0) siScrollInfo.fPos--; break;
		case SB_LINEDOWN	:	if(siScrollInfo.fPos < siScrollInfo.fMax) siScrollInfo.fPos++; break;
		case SB_PAGEUP		:	if(siScrollInfo.fPos - siScrollInfo.fPage > 0) siScrollInfo.fPos -= siScrollInfo.fPage; break;
		case SB_PAGEDOWN	: if(siScrollInfo.fPos + siScrollInfo.fPage < siScrollInfo.fMax) siScrollInfo.fPos += siScrollInfo.fPage; break;
	}
	CreateThumb(true);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_HScroll(_In_ WPARAM wParam)
{
	ThreadSafe_Begin();
	switch(LOWORD(wParam)){
		case SB_LINELEFT	: if(siScrollInfo.fPos > 0) siScrollInfo.fPos--; break;
		case SB_LINERIGHT	: if(siScrollInfo.fPos < siScrollInfo.fMax) siScrollInfo.fPos++; break;
		case SB_PAGELEFT	: if(siScrollInfo.fPos - siScrollInfo.fPage > 0) siScrollInfo.fPos -= siScrollInfo.fPage; break;
		case SB_PAGERIGHT	: if(siScrollInfo.fPos + siScrollInfo.fPage < siScrollInfo.fMax) siScrollInfo.fPos += siScrollInfo.fPage; break;
	}
	CreateThumb(true);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::Geometry(void)
{
	D2D1_POINT_2F ptf3Arrow[3]; D2D1_RECT_F rcfButton; ID2D1GeometrySink* ifSink;
	float fWidth = lWidth; float fHeight = lHeight;
	if(!bHorizontal){
		ptf3Arrow[0] = D2D1::Point2F(fWidth - fWidth * fScaleArrowThumb / 100.0f, fWidth * fScaleArrowThumb / 100.0f);
		ptf3Arrow[1] = D2D1::Point2F(fWidth * fScaleArrowThumb / 100.0f, fWidth * fScaleArrowThumb / 100.0f);
		ptf3Arrow[2] = D2D1::Point2F(fWidth / 2, fWidth - fWidth * fScaleArrowThumb / 100.0f);

		SafeRelease(&ifArrow_Up);
		pstDeviceResources->ifd2d1Factory7->CreatePathGeometry(&ifArrow_Up);
		ifSink = nullptr;
		ifArrow_Up->Open(&ifSink);
		ifSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		ifSink->BeginFigure(ptf3Arrow[0], D2D1_FIGURE_BEGIN_FILLED);

		ifSink->AddLines(ptf3Arrow, 3);
		ifSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		ifSink->Close();
		SafeRelease(&ifSink);

		ptf3Arrow[0] = D2D1::Point2F(fWidth - fWidth * fScaleArrowThumb / 100.0f, fHeight - fWidth * fScaleArrowThumb / 100.0f);
		ptf3Arrow[1] = D2D1::Point2F(fWidth * fScaleArrowThumb / 100.0f, fHeight - lWidth * fScaleArrowThumb / 100.0f);
		ptf3Arrow[2] = D2D1::Point2F(fWidth / 2, fHeight - fWidth + fWidth * fScaleArrowThumb / 100.0f);

		SafeRelease(&ifArrow_Down);
		pstDeviceResources->ifd2d1Factory7->CreatePathGeometry(&ifArrow_Down);
		ifArrow_Down->Open(&ifSink);
		ifSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		ifSink->BeginFigure(ptf3Arrow[0], D2D1_FIGURE_BEGIN_FILLED);

		ifSink->AddLines(ptf3Arrow, 3);
		ifSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		ifSink->Close();
		SafeRelease(&ifSink);

		rcfButton = {0, 0, fWidth, fWidth};
		SafeRelease(&ifButton_Up);
		pstDeviceResources->ifd2d1Factory7->CreateRectangleGeometry(rcfButton, &ifButton_Up);
		rcfButton = {0, fHeight - fWidth, fWidth, fHeight};
		SafeRelease(&ifButton_Down);
		pstDeviceResources->ifd2d1Factory7->CreateRectangleGeometry(rcfButton, &ifButton_Down);
	}
	else{
		ptf3Arrow[0] = D2D1::Point2F(fHeight * fScaleArrowThumb / 100.0f, fHeight - fHeight * fScaleArrowThumb / 100.0f);
		ptf3Arrow[1] = D2D1::Point2F(fHeight * fScaleArrowThumb / 100.0f, fHeight * fScaleArrowThumb / 100.0f);
		ptf3Arrow[2] = D2D1::Point2F(fHeight - fHeight * fScaleArrowThumb / 100.0f, fHeight / 2);

		SafeRelease(&ifArrow_Up);
		pstDeviceResources->ifd2d1Factory7->CreatePathGeometry(&ifArrow_Up);
		ifSink = nullptr;
		ifArrow_Up->Open(&ifSink);
		ifSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		ifSink->BeginFigure(ptf3Arrow[0], D2D1_FIGURE_BEGIN_FILLED);

		ifSink->AddLines(ptf3Arrow, 3);
		ifSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		ifSink->Close();
		SafeRelease(&ifSink);

		ptf3Arrow[0] = D2D1::Point2F(fWidth - fHeight * fScaleArrowThumb / 100.0f, fHeight * fScaleArrowThumb / 100.0f);
		ptf3Arrow[1] = D2D1::Point2F(fWidth - fHeight * fScaleArrowThumb / 100.0f, fHeight - fHeight * fScaleArrowThumb / 100.0f);
		ptf3Arrow[2] = D2D1::Point2F(fWidth - (fHeight - fHeight * fScaleArrowThumb / 100.0f), fHeight / 2);

		SafeRelease(&ifArrow_Down);
		pstDeviceResources->ifd2d1Factory7->CreatePathGeometry(&ifArrow_Down);
		ifArrow_Down->Open(&ifSink);
		ifSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		ifSink->BeginFigure(ptf3Arrow[0], D2D1_FIGURE_BEGIN_FILLED);

		ifSink->AddLines(ptf3Arrow, 3);
		ifSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		ifSink->Close();
		SafeRelease(&ifSink);

		rcfButton = {0, 0, fHeight, fHeight};
		SafeRelease(&ifButton_Up);
		pstDeviceResources->ifd2d1Factory7->CreateRectangleGeometry(rcfButton, &ifButton_Up);
		rcfButton = {fWidth - fHeight, 0,  fWidth, fHeight};
		SafeRelease(&ifButton_Down);
		pstDeviceResources->ifd2d1Factory7->CreateRectangleGeometry(rcfButton, &ifButton_Down);
	}
	CreateThumb(false);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::CreateThumb(bool bRender)
{
	if(!bHorizontal){
		if(siScrollInfo.szfCharacter.height && siScrollInfo.fPage && siScrollInfo.fMax
			 && siScrollInfo.fPage < siScrollInfo.fMax && siScrollInfo.fPos <= siScrollInfo.fMax - siScrollInfo.fPage){
			fThumb = (float)(lHeight - lWidth * 2) * siScrollInfo.fPage / siScrollInfo.fMax;
			fStep = siScrollInfo.szfCharacter.height * fThumb / siScrollInfo.fPage;
			rcfThumb.left = (float)lWidth - (float)lWidth * fScaleArrowThumb / 100.0f; rcfThumb.right = (float)lWidth * fScaleArrowThumb / 100.0f;
			rcfThumb.top = siScrollInfo.fPos * fThumb / siScrollInfo.fPage + (float)lWidth; rcfThumb.bottom = rcfThumb.top + fThumb;
			if(ifThumb) SafeRelease(&ifThumb);
			D2D1_ROUNDED_RECT rrcfThumb = {rcfThumb, 2.0f, 2.0f};
			pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
			if(bRender){
				rclDirty.top = (long)rcfThumb.top; rclDirty.bottom = (long)rcfThumb.bottom;
				rclDirty.left = (long)rcfThumb.left; rclDirty.right = (long)rcfThumb.right;
				OnRender();
				ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
			}
		}
	}
	else{
		if(siScrollInfo.szfCharacter.width && siScrollInfo.fPage && siScrollInfo.fMax
			 && siScrollInfo.fPage < siScrollInfo.fMax && siScrollInfo.fPos <= siScrollInfo.fMax - siScrollInfo.fPage){
			fThumb = (float)(lWidth - lHeight * 2) * siScrollInfo.fPage / siScrollInfo.fMax;
			fStep = siScrollInfo.szfCharacter.width * fThumb / siScrollInfo.fPage;
			rcfThumb.top = (float)lHeight - (float)lHeight * fScaleArrowThumb / 100.0f; rcfThumb.bottom = (float)lHeight * fScaleArrowThumb / 100.0f;
			rcfThumb.left = siScrollInfo.fPos * fThumb / siScrollInfo.fPage + (float)lHeight;	rcfThumb.right = rcfThumb.left + fThumb; 
			if(ifThumb) SafeRelease(&ifThumb);
			D2D1_ROUNDED_RECT rrcfThumb = {rcfThumb, 2.0f, 2.0f};
			pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
			if(bRender){
				rclDirty.top = (long)rcfThumb.top; rclDirty.bottom = (long)rcfThumb.bottom;
				rclDirty.left = (long)rcfThumb.left; rclDirty.right = (long)(rcfThumb.right + 1.0f);
				OnRender();
				ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
			}
		}

	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::GetScrollInfo(_In_ STScrollInfo& siScrollInfoA)
{
	ThreadSafe_Begin();
	if(siScrollInfoA.ucMask & SBI_MAX) siScrollInfoA.fMax = siScrollInfo.fMax;
	if(siScrollInfoA.ucMask & SBI_PAGE) siScrollInfoA.fPage = siScrollInfo.fPage;
	if(siScrollInfoA.ucMask & SBI_POS) siScrollInfoA.fPos = siScrollInfo.fPos;
	if(siScrollInfoA.ucMask & SBI_CHARACTER_WIDTH) siScrollInfoA.szfCharacter.width = siScrollInfo.szfCharacter.width;
	if(siScrollInfoA.ucMask & SBI_CHARACTER_HEIGHT) siScrollInfoA.szfCharacter.height = siScrollInfo.szfCharacter.height;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetScrollInfo(_In_ STScrollInfo& siScrollInfoA)
{
	ThreadSafe_Begin();
	if(siScrollInfoA.ucMask & SBI_MAX) siScrollInfo.fMax = siScrollInfoA.fMax;
	if(siScrollInfoA.ucMask & SBI_PAGE) siScrollInfo.fPage = siScrollInfoA.fPage;
	if(siScrollInfoA.ucMask & SBI_POS) siScrollInfo.fPos = siScrollInfoA.fPos;
	if(siScrollInfoA.ucMask & SBI_CHARACTER_WIDTH) siScrollInfo.szfCharacter.width = siScrollInfoA.szfCharacter.width;
	if(siScrollInfoA.ucMask & SBI_CHARACTER_HEIGHT) siScrollInfo.szfCharacter.height = siScrollInfoA.szfCharacter.height;
	if(hWndElement) CreateThumb(true);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::NewSize(_In_ long lHeightA, _In_ long lWidthA)
{
	ThreadSafe_Begin();
	if(hWndElement){
		NewWindowSize(lHeightA, lWidthA);
		Geometry();
		OnRender();
		rclDirty.top = 0; rclDirty.bottom = lHeight;
		rclDirty.left = 0; rclDirty.right = lWidth;
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::NewHeight(_In_ long lHeightA)
{
	ThreadSafe_Begin();
	if(hWndElement){
		NewWindowHeight(lHeightA);
		Geometry();
		OnRender();
		rclDirty.top = 0; rclDirty.bottom = lHeight;
		rclDirty.left = 0; rclDirty.right = lWidth;
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::NewWidth(_In_ long lWidthA)
{
	ThreadSafe_Begin();
	if(hWndElement){
		NewWindowWidth(lWidthA);
		Geometry();
		OnRender();
		rclDirty.top = 0; rclDirty.bottom = lHeight;
		rclDirty.left = 0; rclDirty.right = lWidth;
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::ScaleArrowThumb(_In_ float fScale)
{
	ThreadSafe_Begin();
	if(fScale > 100) fScale = 100;
	if(fScale < 0) fScale = 0;
	fScaleArrowThumb = fScale;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetButtonColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfButton = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifButtonColor_Up) ifButtonColor_Up->SetColor(crfButton);
	if(ifButtonColor_Down) ifButtonColor_Down->SetColor(crfButton);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetButtonColor(_In_ D2D1_COLOR_F& crfButtonA)
{
	ThreadSafe_Begin();
	crfButton = crfButtonA;
	if(ifButtonColor_Up) ifButtonColor_Up->SetColor(crfButton);
	if(ifButtonColor_Down) ifButtonColor_Down->SetColor(crfButton);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetButtonColor_Move(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfButton_Move = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetButtonColor_Move(_In_ D2D1_COLOR_F& crfButton_MoveA)
{
	ThreadSafe_Begin();
	crfButton_Move = crfButton_MoveA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetButtonColor_Click(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfButton_Click = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetButtonColor_Click(_In_ D2D1_COLOR_F& crfButton_ClickA)
{
	ThreadSafe_Begin();
	crfButton_Click = crfButton_ClickA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetArrowColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfArrow = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifArrowColor_Up) ifArrowColor_Up->SetColor(crfArrow);
	if(ifArrowColor_Down) ifArrowColor_Down->SetColor(crfArrow);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetArrowColor(_In_ D2D1_COLOR_F& crfArrowA)
{
	ThreadSafe_Begin();
	crfArrow = crfArrowA;
	if(ifArrowColor_Up) ifArrowColor_Up->SetColor(crfArrow);
	if(ifArrowColor_Down) ifArrowColor_Down->SetColor(crfArrow);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetArrowColor_Move(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfArrow_Move = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetArrowColor_Move(_In_ D2D1_COLOR_F& crfArrow_MoveA)
{
	ThreadSafe_Begin();
	crfArrow_Move = crfArrow_MoveA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetArrowColor_Click(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfArrow_Click = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetArrowColor_Click(_In_ D2D1_COLOR_F& crfArrow_ClickA)
{
	ThreadSafe_Begin();
	crfArrow_Click = crfArrow_ClickA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetThumbColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfThumb = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifThumbColor) ifThumbColor->SetColor(crfThumb);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetThumbColor(_In_ D2D1_COLOR_F& crfThumbA)
{
	ThreadSafe_Begin();
	crfThumb = crfThumbA;
	if(ifThumbColor) ifThumbColor->SetColor(crfThumb);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetThumbColor_Move(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfThumb_Move = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetThumbColor_Move(_In_ D2D1_COLOR_F& crfThumb_MoveA)
{
	ThreadSafe_Begin();
	crfThumb_Move = crfThumb_MoveA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetThumbColor_Click(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfThumb_Click = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetThumbColor_Click(_In_ D2D1_COLOR_F& crfThumb_ClickA)
{
	ThreadSafe_Begin();
	crfThumb_Click = crfThumb_ClickA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------