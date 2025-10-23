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
constexpr BYTE THUMB = 3;
constexpr BYTE SLIDER = 4;
constexpr BYTE NONCLIENT = 5;
constexpr CHAR THUMB_UP = -1;
constexpr CHAR THUMB_DOWN = 1;
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COScrollBar* __vectorcall RePag::DirectX::COScrollBarV(_In_ const char* pcFensterName, _In_ unsigned int uiIDElement,
																														 _In_ STDeviceResources* pstDeviceResources)
{
	COScrollBar* vScrollBar = (COScrollBar*)VMBlock(VMDialog(), sizeof(COScrollBar));
	vScrollBar->COScrollBarV(VMDialog(), pcFensterName, uiIDElement, pstDeviceResources);
	return vScrollBar;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COScrollBar* __vectorcall RePag::DirectX::COScrollBarV(_In_ const VMEMORY vmSpeicher, _In_ const char* pcFensterName, _In_ unsigned int uiIDElement,
																														 _In_ STDeviceResources* pstDeviceResources)
{
	COScrollBar* vScrollBar = (COScrollBar*)VMBlock(vmSpeicher, sizeof(COScrollBar));
	vScrollBar->COScrollBarV(vmSpeicher, pcFensterName, uiIDElement, pstDeviceResources);
	return vScrollBar;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_ScrollBar(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
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
void __vectorcall RePag::DirectX::COScrollBar::COScrollBarV(_In_ const VMEMORY vmSpeicher, _In_ const char* pcKlassenName, _In_ const char* pcFensterName, 
																														_In_ unsigned int uiIDElementA,	_In_ STDeviceResources* pstDeviceResourcesA)
{
	COGraphicV(vmSpeicher, pcKlassenName, pcFensterName, uiIDElementA, pstDeviceResourcesA);

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
	stScrollInfo = {0};
	rclThumb = {0};
	lThumb_Hohe = 0;
	bMouseTracking = false;
	ucDirty = NONCLIENT;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::COScrollBarV(_In_ const VMEMORY vmSpeicher, _In_ const char* pcFensterName, _In_ unsigned int uiIDElementA,
																														_In_ STDeviceResources* pstDeviceResourcesA)
{
	COScrollBarV(vmSpeicher, pcRePag_ScrollBar, pcFensterName, uiIDElementA, pstDeviceResourcesA);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COScrollBar::COFreiV(void)
{
	SafeRelease(&ifButton_Up); SafeRelease(&ifButton_Down); SafeRelease(&ifThumb); SafeRelease(&ifArrow_Up); SafeRelease(&ifArrow_Down);
	SafeRelease(&ifButtonColor); SafeRelease(&ifThumbColor); SafeRelease(&ifArrowColor);
	return ((COGraphic*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::OnRender(void)
{
	ifD2D1Context6->BeginDraw();
	ifD2D1Context6->Clear(crfBackground);
	ifD2D1Context6->DrawGeometry(ifButton_Up, ifButtonColor, 1);
	ifD2D1Context6->FillGeometry(ifButton_Up, ifButtonColor);

	ifD2D1Context6->DrawGeometry(ifButton_Down, ifButtonColor, 1);
	ifD2D1Context6->FillGeometry(ifButton_Down, ifButtonColor);

	ifD2D1Context6->DrawGeometry(ifArrow_Up, ifArrowColor, 1);
	ifD2D1Context6->FillGeometry(ifArrow_Up, ifArrowColor);

	ifD2D1Context6->DrawGeometry(ifArrow_Down, ifArrowColor, 1);
	ifD2D1Context6->FillGeometry(ifArrow_Down, ifArrowColor);

	if(ifThumb){
		ifD2D1Context6->DrawGeometry(ifThumb, ifThumbColor, 1);
		ifD2D1Context6->FillGeometry(ifThumb, ifThumbColor);
	}
	ifD2D1Context6->EndDraw();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::OnPaint(void)
{
	ThreadSicher_Anfang();
	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_Create(void)
{
	ifD2D1Context6->CreateSolidColorBrush(crfButton, &ifButtonColor);
	ifD2D1Context6->CreateSolidColorBrush(crfArrow, &ifArrowColor);
	ifD2D1Context6->CreateSolidColorBrush(crfThumb, &ifThumbColor);

	ptf3Arrow_Up[0] = D2D1::Point2F(lBreite - lBreite * fScaleArrowThumb / 100.0f, lBreite * fScaleArrowThumb / 100.0f);
	ptf3Arrow_Up[1] = D2D1::Point2F(lBreite * fScaleArrowThumb / 100.0f, lBreite * fScaleArrowThumb / 100.0f);
	ptf3Arrow_Up[2] = D2D1::Point2F(lBreite / 2, lBreite - lBreite * fScaleArrowThumb / 100.0f);

	ptf3Arrow_Down[0] = D2D1::Point2F(lBreite - lBreite * fScaleArrowThumb / 100.0f, lHohe - lBreite * fScaleArrowThumb / 100.0f);
	ptf3Arrow_Down[1] = D2D1::Point2F(lBreite * fScaleArrowThumb / 100.0f, lHohe - lBreite * fScaleArrowThumb / 100.0f);
	ptf3Arrow_Down[2] = D2D1::Point2F(lBreite / 2, lHohe - lBreite + lBreite * fScaleArrowThumb / 100.0f);
	
	D2D1_RECT_F rcf_Button_Down, rcfButton_Up;
	rcfButton_Up = {0, 0, (float)lBreite, (float)lBreite};
	pstDeviceResources->ifd2d1Factory7->CreateRectangleGeometry(rcfButton_Up, &ifButton_Up);
	rcf_Button_Down = {0, (float)lHohe - lBreite, (float)lBreite, (float)lHohe};
	pstDeviceResources->ifd2d1Factory7->CreateRectangleGeometry(rcf_Button_Down, &ifButton_Down);

	pstDeviceResources->ifd2d1Factory7->CreatePathGeometry(&ifArrow_Up);
	ID2D1GeometrySink* ifSink = nullptr;
	ifArrow_Up->Open(&ifSink);
	ifSink->SetFillMode(D2D1_FILL_MODE_WINDING);

	ifSink->BeginFigure(ptf3Arrow_Up[0], D2D1_FIGURE_BEGIN_FILLED);

	ifSink->AddLines(ptf3Arrow_Up, 3);
	ifSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	ifSink->Close();
	SafeRelease(&ifSink);

	pstDeviceResources->ifd2d1Factory7->CreatePathGeometry(&ifArrow_Down);
	ifArrow_Down->Open(&ifSink);
	ifSink->SetFillMode(D2D1_FILL_MODE_WINDING);

	ifSink->BeginFigure(ptf3Arrow_Down[0], D2D1_FIGURE_BEGIN_FILLED);

	ifSink->AddLines(ptf3Arrow_Down, 3);
	ifSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	ifSink->Close();
	SafeRelease(&ifSink);

	CreateThumb(false);

	stTrackMouseEvent.cbSize = sizeof(stTrackMouseEvent);
	stTrackMouseEvent.dwFlags = TME_HOVER | TME_LEAVE;
	stTrackMouseEvent.hwndTrack = hWndElement;
	stTrackMouseEvent.dwHoverTime = 10;

	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_MouseMove(WPARAM wParam, LPARAM lParam)
{
	POINTS ptlCursor = MAKEPOINTS(lParam); RECT rcl2Dirty[2];	static char cCursor_Move = 0; static long lCursor = 0; static char cThumbDirection = 0;
	ThreadSicher_Anfang();

	if(!bMouseTracking){
		bMouseTracking = true;
		TrackMouseEvent(&stTrackMouseEvent); 
	}
	else{
		switch(ucDirty){
			case ARROW_UP		: if(ptlCursor.y > lBreite){
													ifButtonColor->SetColor(crfButton);
													ifArrowColor->SetColor(crfArrow);
													rclDirty.left = 0; rclDirty.top = 0;
													rclDirty.right = lBreite; rclDirty.bottom = lBreite;

													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													ucDirty = SLIDER;
												}
												break;
			case ARROW_DOWN	: if(ptlCursor.y < lHohe - lBreite){
													ifButtonColor->SetColor(crfButton);
													ifArrowColor->SetColor(crfArrow);
													rclDirty.left = 0; rclDirty.top = lHohe - lBreite;
													rclDirty.right = lBreite; rclDirty.bottom = lHohe;

													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													ucDirty = SLIDER;
												}
												break;
			case THUMB			:	if(wParam == MK_LBUTTON){
													if(ptlCursor.y < lCursor){
														if(cThumbDirection == THUMB_UP) cCursor_Move -= lCursor - ptlCursor.y;
														else{	cThumbDirection = THUMB_UP; cCursor_Move = 0; }
													}
													else if(ptlCursor.y > lCursor){
														if(cThumbDirection == THUMB_DOWN) cCursor_Move += ptlCursor.y - lCursor;
														else{ cThumbDirection = THUMB_DOWN; cCursor_Move = 0; }
													}
													lCursor = ptlCursor.y;
													if(cCursor_Move > lThumb_Hohe && rclThumb.bottom < lHohe - lBreite){
														cCursor_Move = 0;
														dxgiPresent.DirtyRectsCount = 2;
														dxgiPresent.pDirtyRects = rcl2Dirty;
														rcl2Dirty[0] = rclThumb;
														stScrollInfo.lPos++;

														D2D1_RECT_F rcfThumb;
														rclThumb.top = lThumb_Hohe * stScrollInfo.lPos + lBreite;
														rclThumb.bottom = rclThumb.top + lThumb_Hohe;
														rcl2Dirty[1] = rclThumb;
														rcfThumb.left = rclThumb.left; rcfThumb.right = rclThumb.right;
														rcfThumb.top = rclThumb.top; rcfThumb.bottom = rclThumb.bottom;
														D2D1_ROUNDED_RECT rrcfThumb = {rcfThumb, 2.0f, 2.0f};
														SafeRelease(&ifThumb);
														pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);

														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														dxgiPresent.DirtyRectsCount = 1;
														dxgiPresent.pDirtyRects = &rclDirty;
													}
													else if(cCursor_Move < lThumb_Hohe * -1 && rclThumb.top > lBreite){
														cCursor_Move = 0;
														dxgiPresent.DirtyRectsCount = 2;
														dxgiPresent.pDirtyRects = rcl2Dirty;
														rcl2Dirty[0] = rclThumb;
														stScrollInfo.lPos--;

														D2D1_RECT_F rcfThumb;
														rclThumb.top = lThumb_Hohe * stScrollInfo.lPos + lBreite;
														rclThumb.bottom = rclThumb.top + lThumb_Hohe;
														rcl2Dirty[1] = rclThumb;
														rcfThumb.left = rclThumb.left; rcfThumb.right = rclThumb.right;
														rcfThumb.top = rclThumb.top; rcfThumb.bottom = rclThumb.bottom;
														D2D1_ROUNDED_RECT rrcfThumb = {rcfThumb, 2.0f, 2.0f};
														SafeRelease(&ifThumb);
														pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);

														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														dxgiPresent.DirtyRectsCount = 1;
														dxgiPresent.pDirtyRects = &rclDirty;
													}
												}
												else if(ptlCursor.y < rclThumb.top ||  ptlCursor.y > rclThumb.bottom){
													ifThumbColor->SetColor(crfThumb);
													rclDirty = rclThumb;

													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													ucDirty = SLIDER;
												}
												break;
			case SLIDER			:	if(ptlCursor.y <= lBreite){
													ifButtonColor->SetColor(crfButton_Move);
													ifArrowColor->SetColor(crfArrow_Move);
													ucDirty = ARROW_UP;
													rclDirty.left = 0; rclDirty.top = 0;
													rclDirty.right = lBreite; rclDirty.bottom = lBreite;

													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
												else if(ptlCursor.y >= lHohe - lBreite){
													ifButtonColor->SetColor(crfButton_Move);
													ifArrowColor->SetColor(crfArrow_Move);
													ucDirty = ARROW_DOWN;
													rclDirty.left = 0; rclDirty.top = lHohe - lBreite;
													rclDirty.right = lBreite; rclDirty.bottom = lHohe;

													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
												else if(ptlCursor.y >= rclThumb.top && ptlCursor.y <= rclThumb.bottom){
													ifThumbColor->SetColor(crfThumb_Move);
													ucDirty = THUMB;
													rclDirty = rclThumb;

													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
		}
	}
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_MouseOver(WPARAM wParam, LPARAM lParam)
{
	POINTS ptlCursor = MAKEPOINTS(lParam);
	ThreadSicher_Anfang();
	if(ptlCursor.y <= lBreite){
		ifButtonColor->SetColor(crfButton_Move);
		ifArrowColor->SetColor(crfArrow_Move);
		ucDirty = ARROW_UP;
		rclDirty.left = 0; rclDirty.top = 0;
		rclDirty.right = lBreite; rclDirty.bottom = lBreite;

		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	else if(ptlCursor.y >= lHohe - lBreite){
		ifButtonColor->SetColor(crfButton_Move);
		ifArrowColor->SetColor(crfArrow_Move);
		ucDirty = ARROW_DOWN;
		rclDirty.left = 0; rclDirty.top = lHohe - lBreite;
		rclDirty.right = lBreite; rclDirty.bottom = lHohe;

		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	else if(ptlCursor.y >= rclThumb.top && ptlCursor.y <= rclThumb.bottom){
		ifThumbColor->SetColor(crfThumb_Move);
		ucDirty = THUMB;
		rclDirty = rclThumb;

		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	else if(ptlCursor.y < rclThumb.top || ptlCursor.y > rclThumb.bottom){
		ucDirty = SLIDER;
	}
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_MouseLeave(void)
{
	ThreadSicher_Anfang();
	switch(ucDirty){
		case ARROW_UP   : ifButtonColor->SetColor(crfButton);
											ifArrowColor->SetColor(crfArrow);
											rclDirty.left = 0; rclDirty.top = 0;
											rclDirty.right = lBreite; rclDirty.bottom = lBreite;
											OnRender();
											ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											break;
		case ARROW_DOWN :	ifButtonColor->SetColor(crfButton);
											ifArrowColor->SetColor(crfArrow);
											rclDirty.left = 0; rclDirty.top = lHohe - lBreite;
											rclDirty.right = lBreite; rclDirty.bottom = lHohe;
											OnRender();
											ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											break;
		case THUMB			:	ifThumbColor->SetColor(crfThumb);
											rclDirty = rclThumb;
											OnRender();
											ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ucDirty = NONCLIENT;
	bMouseTracking = false;
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_LButtonDown(WPARAM wParam, LPARAM lParam)
{
	POINTS ptlCursor = MAKEPOINTS(lParam);
	ThreadSicher_Anfang();
	switch(ucDirty){
		case	ARROW_UP	:	if(ptlCursor.y <= lBreite){
												ifButtonColor->SetColor(crfButton_Click);
												ifArrowColor->SetColor(crfArrow_Click);
												rclDirty.left = 0; rclDirty.top = 0;
												rclDirty.right = lBreite; rclDirty.bottom = lBreite;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											}
											break;
		case ARROW_DOWN	:	if(ptlCursor.y >= lHohe - lBreite){
												ifButtonColor->SetColor(crfButton_Click);
												ifArrowColor->SetColor(crfArrow_Click);
												rclDirty.left = 0; rclDirty.top = lHohe - lBreite;
												rclDirty.right = lBreite; rclDirty.bottom = lHohe;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											}
											break;
		case THUMB			: if(ptlCursor.y >= rclThumb.top && ptlCursor.y <= rclThumb.bottom){
												ifThumbColor->SetColor(crfThumb_Click);
												rclDirty = rclThumb;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											}
	}
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_LButtonUp(WPARAM wParam, LPARAM lParam)
{
	POINTS ptlCursor = MAKEPOINTS(lParam); RECT rcl3Dirty[3];
	ThreadSicher_Anfang();
	switch(ucDirty){
		case ARROW_UP			:	if(ptlCursor.y <= lBreite){
													if(stScrollInfo.lPos > 0){
														stScrollInfo.lPos--;
														dxgiPresent.DirtyRectsCount = 3;
														dxgiPresent.pDirtyRects = rcl3Dirty;

														rcl3Dirty[2] = rclThumb;

														D2D1_RECT_F rcfThumb;
														rclThumb.top = lThumb_Hohe * stScrollInfo.lPos + lBreite;
														rclThumb.bottom = rclThumb.top + lThumb_Hohe;
														rcl3Dirty[1] = rclThumb;
														rcfThumb.left = rclThumb.left; rcfThumb.right = rclThumb.right;
														rcfThumb.top = rclThumb.top; rcfThumb.bottom = rclThumb.bottom;
														D2D1_ROUNDED_RECT rrcfThumb = {rcfThumb, 2.0f, 2.0f};
														SafeRelease(&ifThumb);
														pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);

														ifButtonColor->SetColor(crfButton_Move);
														ifArrowColor->SetColor(crfArrow_Move);
														rcl3Dirty[0].left = 0; rcl3Dirty[0].top = 0;
														rcl3Dirty[0].right = lBreite; rcl3Dirty[0].bottom = lBreite;

														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														dxgiPresent.DirtyRectsCount = 1;
														dxgiPresent.pDirtyRects = &rclDirty;

													}
													else{
														ifButtonColor->SetColor(crfButton_Move);
														ifArrowColor->SetColor(crfArrow_Move);
														rclDirty.left = 0; rclDirty.top = 0;
														rclDirty.right = lBreite; rclDirty.bottom = lBreite;

														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													}
													SendMessage(GetParent(hWndElement), WM_VSCROLL, SB_LINEUP, NULL);
												}
												break;
		case	ARROW_DOWN	:	if(ptlCursor.y >= lHohe - lBreite){
													if(stScrollInfo.lPos < stScrollInfo.lMax - (long)stScrollInfo.ulPage){
														stScrollInfo.lPos++;
														dxgiPresent.DirtyRectsCount = 3;
														dxgiPresent.pDirtyRects = rcl3Dirty;

														rcl3Dirty[2] = rclThumb;

														D2D1_RECT_F rcfThumb;
														rclThumb.top = lThumb_Hohe * stScrollInfo.lPos + lBreite;
														rclThumb.bottom = rclThumb.top + lThumb_Hohe;
														rcl3Dirty[1] = rclThumb;
														rcfThumb.left = rclThumb.left; rcfThumb.right = rclThumb.right;
														rcfThumb.top = rclThumb.top; rcfThumb.bottom = rclThumb.bottom;
														D2D1_ROUNDED_RECT rrcfThumb = {rcfThumb, 2.0f, 2.0f};
														SafeRelease(&ifThumb);
														pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);

														ifButtonColor->SetColor(crfButton_Move);
														ifArrowColor->SetColor(crfArrow_Move);
														rcl3Dirty[0].left = 0; rcl3Dirty[0].top = lHohe - lBreite;
														rcl3Dirty[0].right = lBreite; rcl3Dirty[0].bottom = lHohe;

														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														dxgiPresent.DirtyRectsCount = 1;
														dxgiPresent.pDirtyRects = &rclDirty;

													}
													else{
														ifButtonColor->SetColor(crfButton_Move);
														ifArrowColor->SetColor(crfArrow_Move);
														rclDirty.left = 0; rclDirty.top = lHohe - lBreite;
														rclDirty.right = lBreite; rclDirty.bottom = lHohe;

														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);

													}
													SendMessage(GetParent(hWndElement), WM_VSCROLL, SB_LINEDOWN, NULL);
												}
												break;
		case	THUMB				:	rclDirty = rclThumb;
												ifThumbColor->SetColor(crfThumb_Move);

												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::CreateThumb(bool bRender)
{
	if(stScrollInfo.lMax > 0 && stScrollInfo.ulPage){
		D2D1_RECT_F rcfThumb;
		rcfThumb.left = lBreite - lBreite * fScaleArrowThumb / 100.0f; rcfThumb.right = lBreite * fScaleArrowThumb / 100.0f;
		rclThumb.left = (long)rcfThumb.left; rclThumb.right = (long)rcfThumb.right;

		if(stScrollInfo.lMax > (long)stScrollInfo.ulPage){
			rcfThumb.top = lBreite; rcfThumb.bottom = (lHohe - lBreite * 2) / (stScrollInfo.lMax - stScrollInfo.ulPage + 1) + lBreite;
			rclThumb.top = (long)rcfThumb.top; rclThumb.bottom = (long)rcfThumb.bottom;
			lThumb_Hohe = rcfThumb.bottom - rcfThumb.top;

			if(ifThumb) SafeRelease(&ifThumb);
			D2D1_ROUNDED_RECT rrcfThumb = {rcfThumb, 2.0f, 2.0f};
			pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);

			if(bRender){
				rclDirty = rclThumb;
				OnRender();
				ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
			}		
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::GetScrollInfo(_In_ STScrollInfo& stScrollInfoA)
{
	ThreadSicher_Anfang();
	stScrollInfoA = stScrollInfo;
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetScrollInfo(_In_ STScrollInfo& stScrollInfoA)
{
	ThreadSicher_Anfang();
	stScrollInfo = stScrollInfoA;
	if(hWndElement) CreateThumb(true);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::ScaleArrowThumb(_In_ float fScale)
{
	ThreadSicher_Anfang();
	if(fScale > 100) fScale = 100;
	if(fScale < 0) fScale = 0;
	fScaleArrowThumb = fScale;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetButtonColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSicher_Anfang();
	crfButton = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifButtonColor) ifButtonColor->SetColor(crfButton);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetButtonColor(_In_ D2D1_COLOR_F& crfButtonA)
{
	ThreadSicher_Anfang();
	crfButton = crfButtonA;
	if(ifButtonColor) ifButtonColor->SetColor(crfButton);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetButtonColor_Move(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSicher_Anfang();
	crfButton_Move = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetButtonColor_Move(_In_ D2D1_COLOR_F& crfButton_MoveA)
{
	ThreadSicher_Anfang();
	crfButton_Move = crfButton_MoveA;
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetButtonColor_Click(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSicher_Anfang();
	crfButton_Click = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetButtonColor_Click(_In_ D2D1_COLOR_F& crfButton_ClickA)
{
	ThreadSicher_Anfang();
	crfButton_Click = crfButton_ClickA;
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetArrowColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSicher_Anfang();
	crfArrow = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifArrowColor) ifArrowColor->SetColor(crfArrow);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetArrowColor(_In_ D2D1_COLOR_F& crfArrowA)
{
	ThreadSicher_Anfang();
	crfArrow = crfArrowA;
	if(ifArrowColor) ifArrowColor->SetColor(crfArrow);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetArrowColor_Move(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSicher_Anfang();
	crfArrow_Move = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetArrowColor_Move(_In_ D2D1_COLOR_F& crfArrow_MoveA)
{
	ThreadSicher_Anfang();
	crfArrow_Move = crfArrow_MoveA;
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetArrowColor_Click(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSicher_Anfang();
	crfArrow_Click = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetArrowColor_Click(_In_ D2D1_COLOR_F& crfArrow_ClickA)
{
	ThreadSicher_Anfang();
	crfArrow_Click = crfArrow_ClickA;
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetThumbColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSicher_Anfang();
	crfThumb = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifThumbColor) ifThumbColor->SetColor(crfThumb);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetThumbColor(_In_ D2D1_COLOR_F& crfThumbA)
{
	ThreadSicher_Anfang();
	crfThumb = crfThumbA;
	if(ifThumbColor) ifThumbColor->SetColor(crfThumb);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetThumbColor_Move(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSicher_Anfang();
	crfThumb_Move = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetThumbColor_Move(_In_ D2D1_COLOR_F& crfThumb_MoveA)
{
	ThreadSicher_Anfang();
	crfThumb_Move = crfThumb_MoveA;
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetThumbColor_Click(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSicher_Anfang();
	crfThumb_Click = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::SetThumbColor_Click(_In_ D2D1_COLOR_F& crfThumb_ClickA)
{
	ThreadSicher_Anfang();
	crfThumb_Click = crfThumb_ClickA;
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------