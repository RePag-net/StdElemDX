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
RePag::DirectX::COScrollBar* __vectorcall RePag::DirectX::COScrollBarV(_In_ const char* pcFensterName, _In_ unsigned int uiIDElement,
																														 _In_ STDeviceResources* pstDeviceResources, bool bHorizontal)
{
	COScrollBar* vScrollBar = (COScrollBar*)VMBlock(VMDialog(), sizeof(COScrollBar));
	vScrollBar->COScrollBarV(VMDialog(), pcFensterName, uiIDElement, pstDeviceResources, bHorizontal);
	return vScrollBar;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COScrollBar* __vectorcall RePag::DirectX::COScrollBarV(_In_ const VMEMORY vmSpeicher, _In_ const char* pcFensterName, _In_ unsigned int uiIDElement,
																														 _In_ STDeviceResources* pstDeviceResources, bool bHorizontal)
{
	COScrollBar* vScrollBar = (COScrollBar*)VMBlock(vmSpeicher, sizeof(COScrollBar));
	vScrollBar->COScrollBarV(vmSpeicher, pcFensterName, uiIDElement, pstDeviceResources, bHorizontal);
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
																														_In_ unsigned int uiIDElementA,	_In_ STDeviceResources* pstDeviceResourcesA, bool bHorizontalA)
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
	rcfThumb = {0};
	fThumbSize = 0;
	bMouseTracking = false;
	ucDirty = NONCLIENT;
	bHorizontal = bHorizontalA;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::COScrollBarV(_In_ const VMEMORY vmSpeicher, _In_ const char* pcFensterName, _In_ unsigned int uiIDElementA,
																														_In_ STDeviceResources* pstDeviceResourcesA, bool bHorizontalA)
{
	COScrollBarV(vmSpeicher, pcRePag_ScrollBar, pcFensterName, uiIDElementA, pstDeviceResourcesA, bHorizontalA);
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

	D2D1_POINT_2F ptf3Arrow[3]; D2D1_RECT_F rcfButton; ID2D1GeometrySink* ifSink;
	float fWidth = lWidth; float fHeight = lHeight;
	if(bHorizontal){
		ptf3Arrow[0] = D2D1::Point2F(fWidth - fWidth * fScaleArrowThumb / 100.0f, fWidth * fScaleArrowThumb / 100.0f);
		ptf3Arrow[1] = D2D1::Point2F(fWidth * fScaleArrowThumb / 100.0f, fWidth * fScaleArrowThumb / 100.0f);
		ptf3Arrow[2] = D2D1::Point2F(fWidth / 2, fWidth - fWidth * fScaleArrowThumb / 100.0f);

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

		pstDeviceResources->ifd2d1Factory7->CreatePathGeometry(&ifArrow_Down);
		ifArrow_Down->Open(&ifSink);
		ifSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		ifSink->BeginFigure(ptf3Arrow[0], D2D1_FIGURE_BEGIN_FILLED);

		ifSink->AddLines(ptf3Arrow, 3);
		ifSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		ifSink->Close();
		SafeRelease(&ifSink);

		rcfButton = {0, 0, fWidth, fWidth};
		pstDeviceResources->ifd2d1Factory7->CreateRectangleGeometry(rcfButton, &ifButton_Up);
		rcfButton = {0, fHeight - fWidth, fWidth, fHeight};
		pstDeviceResources->ifd2d1Factory7->CreateRectangleGeometry(rcfButton, &ifButton_Down);
	}
	else{
		ptf3Arrow[0] = D2D1::Point2F(fHeight * fScaleArrowThumb / 100.0f, fHeight - fHeight * fScaleArrowThumb / 100.0f);
		ptf3Arrow[1] = D2D1::Point2F(fHeight * fScaleArrowThumb / 100.0f, fHeight * fScaleArrowThumb / 100.0f);
		ptf3Arrow[2] = D2D1::Point2F(fHeight - fHeight * fScaleArrowThumb / 100.0f, fHeight / 2);

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

		pstDeviceResources->ifd2d1Factory7->CreatePathGeometry(&ifArrow_Down);
		ifArrow_Down->Open(&ifSink);
		ifSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		ifSink->BeginFigure(ptf3Arrow[0], D2D1_FIGURE_BEGIN_FILLED);

		ifSink->AddLines(ptf3Arrow, 3);
		ifSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		ifSink->Close();
		SafeRelease(&ifSink);

		rcfButton = {0, 0, fHeight, fHeight};
		pstDeviceResources->ifd2d1Factory7->CreateRectangleGeometry(rcfButton, &ifButton_Up);
		rcfButton = {fWidth - fHeight, 0,  fWidth, fHeight};
		pstDeviceResources->ifd2d1Factory7->CreateRectangleGeometry(rcfButton, &ifButton_Down);
	}

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
	POINTS ptlCursor = MAKEPOINTS(lParam); RECT rcl2Dirty[2];	D2D1_ROUNDED_RECT rrcfThumb;
	static char cCursor_Move = 0; static long lCursor = 0; static char cThumbDirection = 0;
	ThreadSicher_Anfang();

	if(!bMouseTracking){ bMouseTracking = true;	TrackMouseEvent(&stTrackMouseEvent); }
	else{
		switch(ucDirty){
			case ARROW_UP			: if(ptlCursor.y > lWidth){
														ifButtonColor->SetColor(crfButton);
														ifArrowColor->SetColor(crfArrow);
														rclDirty.left = 0; rclDirty.top = 0;
														rclDirty.right = lWidth; rclDirty.bottom = lWidth;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = SLIDER_VERT;
													}
													break;
			case ARROW_DOWN		: if(ptlCursor.y < lHeight - lWidth){
														ifButtonColor->SetColor(crfButton);
														ifArrowColor->SetColor(crfArrow);
														rclDirty.left = 0; rclDirty.top = lHeight - lWidth;
														rclDirty.right = lWidth; rclDirty.bottom = lHeight;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = SLIDER_VERT;
													}
													break;
			case ARROW_LEFT		:	if(ptlCursor.x > lHeight){
														ifButtonColor->SetColor(crfButton);
														ifArrowColor->SetColor(crfArrow);
														rclDirty.left = 0; rclDirty.top = 0;
														rclDirty.right = lHeight; rclDirty.bottom = lHeight;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = SLIDER_HORZ;
													}
													break;
			case ARROW_RIGHT	: if(ptlCursor.x < lWidth - lHeight){
														ifButtonColor->SetColor(crfButton);
														ifArrowColor->SetColor(crfArrow);
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
														if(cCursor_Move > (char)fThumbSize && stScrollInfo.lPos < stScrollInfo.lMax - stScrollInfo.ulPage){
															cCursor_Move = 0;
															stScrollInfo.lPos++;
															dxgiPresent.DirtyRectsCount = 2;
															dxgiPresent.pDirtyRects = rcl2Dirty;

															rcl2Dirty[0].top = (long)(rcfThumb.top - 2.0f); rcl2Dirty[0].bottom = (long)rcfThumb.bottom;
															rcl2Dirty[0].left = (long)rcfThumb.left; rcl2Dirty[0].right = (long)rcfThumb.right;
															rcfThumb.top += fThumbSize; rcfThumb.bottom += fThumbSize;
															rrcfThumb = {rcfThumb, 2.0f, 2.0f};
															SafeRelease(&ifThumb);
															pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
															rcl2Dirty[1].top = (long)rcfThumb.top; rcl2Dirty[1].bottom = (long)rcfThumb.bottom;
															rcl2Dirty[1].left = (long)rcfThumb.left; rcl2Dirty[1].right = (long)rcfThumb.right;

															OnRender();
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															dxgiPresent.DirtyRectsCount = 1;
															dxgiPresent.pDirtyRects = &rclDirty;
														}
														else if(cCursor_Move < (char)fThumbSize * -1 && stScrollInfo.lPos){
															cCursor_Move = 0;
															stScrollInfo.lPos--;
															dxgiPresent.DirtyRectsCount = 2;
															dxgiPresent.pDirtyRects = rcl2Dirty;

															rcl2Dirty[0].top = (long)rcfThumb.top; rcl2Dirty[0].bottom = (long)(rcfThumb.bottom + 2.0f);
															rcl2Dirty[0].left = (long)rcfThumb.left; rcl2Dirty[0].right = (long)rcfThumb.right;
															rcfThumb.top -= fThumbSize;	rcfThumb.bottom -= fThumbSize;
															rrcfThumb = {rcfThumb, 2.0f, 2.0f};
															SafeRelease(&ifThumb);
															pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
															rcl2Dirty[1].top = (long)rcfThumb.top; rcl2Dirty[1].bottom = (long)rcfThumb.bottom;
															rcl2Dirty[1].left = (long)rcfThumb.left; rcl2Dirty[1].right = (long)rcfThumb.right;

															OnRender();
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															dxgiPresent.DirtyRectsCount = 1;
															dxgiPresent.pDirtyRects = &rclDirty;
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
														if(cCursor_Move > (char)fThumbSize && stScrollInfo.lPos < stScrollInfo.lMax - stScrollInfo.ulPage){
															cCursor_Move = 0;
															stScrollInfo.lPos++;
															dxgiPresent.DirtyRectsCount = 2;
															dxgiPresent.pDirtyRects = rcl2Dirty;

															rcl2Dirty[0].top = (long)rcfThumb.top; rcl2Dirty[0].bottom = (long)rcfThumb.bottom;
															rcl2Dirty[0].left = (long)(rcfThumb.left - 2.0f); rcl2Dirty[0].right = (long)rcfThumb.right;
															rcfThumb.left += fThumbSize; rcfThumb.right += fThumbSize;
															rrcfThumb = {rcfThumb, 2.0f, 2.0f};
															SafeRelease(&ifThumb);
															pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
															rcl2Dirty[1].top = (long)rcfThumb.top; rcl2Dirty[1].bottom = (long)rcfThumb.bottom;
															rcl2Dirty[1].left = (long)rcfThumb.left; rcl2Dirty[1].right = (long)rcfThumb.right;

															OnRender();
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															dxgiPresent.DirtyRectsCount = 1;
															dxgiPresent.pDirtyRects = &rclDirty;
														}
														else if(cCursor_Move < (char)fThumbSize * -1 && stScrollInfo.lPos){
															cCursor_Move = 0;
															stScrollInfo.lPos--;
															dxgiPresent.DirtyRectsCount = 2;
															dxgiPresent.pDirtyRects = rcl2Dirty;

															rcl2Dirty[0].top = (long)rcfThumb.top; rcl2Dirty[0].bottom = (long)rcfThumb.bottom;
															rcl2Dirty[0].left = (long)rcfThumb.left; rcl2Dirty[0].right = (long)(rcfThumb.right + 2.0f);
															rcfThumb.left -= fThumbSize; rcfThumb.right -= fThumbSize;
															rrcfThumb = {rcfThumb, 2.0f, 2.0f};
															SafeRelease(&ifThumb);
															pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
															rcl2Dirty[1].top = (long)rcfThumb.top; rcl2Dirty[1].bottom = (long)rcfThumb.bottom;
															rcl2Dirty[1].left = (long)rcfThumb.left; rcl2Dirty[1].right = (long)rcfThumb.right;

															OnRender();
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															dxgiPresent.DirtyRectsCount = 1;
															dxgiPresent.pDirtyRects = &rclDirty;
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
														ifButtonColor->SetColor(crfButton_Move);
														ifArrowColor->SetColor(crfArrow_Move);
														rclDirty.left = 0; rclDirty.top = 0;
														rclDirty.right = lWidth; rclDirty.bottom = lWidth;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = ARROW_UP;
													}
													else if(ptlCursor.y >= lHeight - lWidth){
														ifButtonColor->SetColor(crfButton_Move);
														ifArrowColor->SetColor(crfArrow_Move);
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
														ifButtonColor->SetColor(crfButton_Move);
														ifArrowColor->SetColor(crfArrow_Move);
														rclDirty.left = 0; rclDirty.top = 0;
														rclDirty.right = lHeight; rclDirty.bottom = lHeight;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = ARROW_LEFT;
													}
													else if(ptlCursor.x >= lWidth - lHeight){
														ifButtonColor->SetColor(crfButton_Move);
														ifArrowColor->SetColor(crfArrow_Move);
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
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_MouseOver(WPARAM wParam, LPARAM lParam)
{
	POINTS ptlCursor = MAKEPOINTS(lParam);
	ThreadSicher_Anfang();
	if(bHorizontal){
		if(ptlCursor.y <= lWidth){
			ifButtonColor->SetColor(crfButton_Move);
			ifArrowColor->SetColor(crfArrow_Move);
			rclDirty.left = 0; rclDirty.top = 0;
			rclDirty.right = lWidth; rclDirty.bottom = lWidth;
			OnRender();
			ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
			ucDirty = ARROW_UP;
		}
		else if(ptlCursor.y >= lHeight - lWidth){
			ifButtonColor->SetColor(crfButton_Move);
			ifArrowColor->SetColor(crfArrow_Move);
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
			ifButtonColor->SetColor(crfButton_Move);
			ifArrowColor->SetColor(crfArrow_Move);
			rclDirty.left = 0; rclDirty.top = 0;
			rclDirty.right = lHeight; rclDirty.bottom = lHeight;
			OnRender();
			ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
			ucDirty = ARROW_LEFT;
		}
		else if(ptlCursor.x >= lWidth - lHeight){
			ifButtonColor->SetColor(crfButton_Move);
			ifArrowColor->SetColor(crfArrow_Move);
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
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_MouseLeave(void)
{
	ThreadSicher_Anfang();
	switch(ucDirty){
		case ARROW_UP			: ifButtonColor->SetColor(crfButton);
												ifArrowColor->SetColor(crfArrow);
												rclDirty.left = 0; rclDirty.top = 0;
												rclDirty.right = lWidth; rclDirty.bottom = lWidth;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												break;
		case ARROW_DOWN	 :	ifButtonColor->SetColor(crfButton);
												ifArrowColor->SetColor(crfArrow);
												rclDirty.left = 0; rclDirty.top = lHeight - lWidth;
												rclDirty.right = lWidth; rclDirty.bottom = lHeight;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												break;
		case ARROW_LEFT		: ifButtonColor->SetColor(crfButton);
												ifArrowColor->SetColor(crfArrow);
												rclDirty.left = 0; rclDirty.top = 0;
												rclDirty.right = lHeight; rclDirty.bottom = lHeight;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												break;
		case ARROW_RIGHT	:	ifButtonColor->SetColor(crfButton);
												ifArrowColor->SetColor(crfArrow);
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
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_LButtonDown(WPARAM wParam, LPARAM lParam)
{
	POINTS ptlCursor = MAKEPOINTS(lParam);
	ThreadSicher_Anfang();
	switch(ucDirty){
		case	ARROW_UP		:	if(ptlCursor.y <= lWidth){
													ifButtonColor->SetColor(crfButton_Click);
													ifArrowColor->SetColor(crfArrow_Click);
													rclDirty.left = 0; rclDirty.top = 0;
													rclDirty.right = lWidth; rclDirty.bottom = lWidth;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
												break;
		case ARROW_DOWN		:	if(ptlCursor.y >= lHeight - lWidth){
													ifButtonColor->SetColor(crfButton_Click);
													ifArrowColor->SetColor(crfArrow_Click);
													rclDirty.left = 0; rclDirty.top = lHeight - lWidth;
													rclDirty.right = lWidth; rclDirty.bottom = lHeight;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
												break;
		case	ARROW_LEFT	:	if(ptlCursor.x <= lHeight){
													ifButtonColor->SetColor(crfButton_Click);
													ifArrowColor->SetColor(crfArrow_Click);
													rclDirty.left = 0; rclDirty.top = 0;
													rclDirty.right = lHeight; rclDirty.bottom = lHeight;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
												break;
		case ARROW_RIGHT	:	if(ptlCursor.x >=  lWidth - lHeight){
													ifButtonColor->SetColor(crfButton_Click);
													ifArrowColor->SetColor(crfArrow_Click);
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
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::WM_LButtonUp(WPARAM wParam, LPARAM lParam)
{
	POINTS ptlCursor = MAKEPOINTS(lParam); RECT rcl3Dirty[3]; D2D1_ROUNDED_RECT rrcfThumb;
	ThreadSicher_Anfang();
	switch(ucDirty){
		case ARROW_UP			:	if(stScrollInfo.lPos > 0){
													stScrollInfo.lPos--;
													dxgiPresent.DirtyRectsCount = 3;
													dxgiPresent.pDirtyRects = rcl3Dirty;

													rcl3Dirty[2].top = (long)rcfThumb.top; rcl3Dirty[2].bottom = (long)(rcfThumb.bottom + 2.0f);
													rcl3Dirty[2].left = (long)rcfThumb.left; rcl3Dirty[2].right = (long)rcfThumb.right;
													rcfThumb.top -= fThumbSize;	rcfThumb.bottom -= fThumbSize;
													rrcfThumb = {rcfThumb, 2.0f, 2.0f};
													SafeRelease(&ifThumb);
													pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
													rcl3Dirty[1].top = (long)rcfThumb.top; rcl3Dirty[1].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[1].left = (long)rcfThumb.left; rcl3Dirty[1].right = (long)rcfThumb.right;

													ifArrowColor->SetColor(crfArrow_Move);
													ifButtonColor->SetColor(crfButton_Move);
													rcl3Dirty[0].left = 0; rcl3Dirty[0].top = 0;
													rcl3Dirty[0].right = lWidth; rcl3Dirty[0].bottom = lWidth;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													dxgiPresent.DirtyRectsCount = 1;
													dxgiPresent.pDirtyRects = &rclDirty;

													SendMessage(GetParent(hWndElement), WM_VSCROLL, SB_LINEUP, NULL);
												}
												else{
													ifButtonColor->SetColor(crfButton_Move);
													ifArrowColor->SetColor(crfArrow_Move);
													rclDirty.left = 0; rclDirty.top = 0;
													rclDirty.right = lWidth; rclDirty.bottom = lWidth;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
												break;
		case	ARROW_DOWN	:	if(stScrollInfo.lPos + stScrollInfo.ulPage < stScrollInfo.lMax){
													stScrollInfo.lPos++;
													dxgiPresent.DirtyRectsCount = 3;
													dxgiPresent.pDirtyRects = rcl3Dirty;

													rcl3Dirty[2].top = (long)(rcfThumb.top - 2.0f); rcl3Dirty[2].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[2].left = (long)rcfThumb.left; rcl3Dirty[2].right = (long)rcfThumb.right;
													rcfThumb.top += fThumbSize; rcfThumb.bottom += fThumbSize;
													rrcfThumb = {rcfThumb, 2.0f, 2.0f};
													SafeRelease(&ifThumb);
													pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
													rcl3Dirty[1].top = (long)rcfThumb.top; rcl3Dirty[1].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[1].left = (long)rcfThumb.left; rcl3Dirty[1].right = (long)rcfThumb.right;

													ifArrowColor->SetColor(crfArrow_Move);
													ifButtonColor->SetColor(crfButton_Move);
													rcl3Dirty[0].left = 0; rcl3Dirty[0].top = lHeight - lWidth;
													rcl3Dirty[0].right = lWidth; rcl3Dirty[0].bottom = lHeight;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													dxgiPresent.DirtyRectsCount = 1;
													dxgiPresent.pDirtyRects = &rclDirty;

													SendMessage(GetParent(hWndElement), WM_VSCROLL, SB_LINEDOWN, NULL);
												}
												else{
													ifButtonColor->SetColor(crfButton_Move);
													ifArrowColor->SetColor(crfArrow_Move);
													rclDirty.left = 0; rclDirty.top = lHeight - lWidth;
													rclDirty.right = lWidth; rclDirty.bottom = lHeight;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
												break;
		case ARROW_LEFT		:	if(stScrollInfo.lPos > 0){
													stScrollInfo.lPos--;
													dxgiPresent.DirtyRectsCount = 3;
													dxgiPresent.pDirtyRects = rcl3Dirty;

													rcl3Dirty[2].top = (long)rcfThumb.top; rcl3Dirty[2].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[2].left = (long)rcfThumb.left; rcl3Dirty[2].right = (long)(rcfThumb.right + 2.0f);
													rcfThumb.left -= fThumbSize; rcfThumb.right -= fThumbSize;
													rrcfThumb = {rcfThumb, 2.0f, 2.0f};
													SafeRelease(&ifThumb);
													pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
													rcl3Dirty[1].top = (long)rcfThumb.top; rcl3Dirty[1].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[1].left = (long)rcfThumb.left; rcl3Dirty[1].right = (long)rcfThumb.right;

													ifButtonColor->SetColor(crfButton_Move);
													ifArrowColor->SetColor(crfArrow_Move);
													rcl3Dirty[0].left = 0; rcl3Dirty[0].top = 0;
													rcl3Dirty[0].right = lHeight; rcl3Dirty[0].bottom = lHeight;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													dxgiPresent.DirtyRectsCount = 1;
													dxgiPresent.pDirtyRects = &rclDirty;

													SendMessage(GetParent(hWndElement), WM_VSCROLL, SB_LINEUP, NULL);
												}
												else{
													ifButtonColor->SetColor(crfButton_Move);
													ifArrowColor->SetColor(crfArrow_Move);
													rclDirty.left = 0; rclDirty.top = 0;
													rclDirty.right = lHeight; rclDirty.bottom = lHeight;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
												break;
		case	ARROW_RIGHT	:	if(stScrollInfo.lPos < stScrollInfo.lMax - (long)stScrollInfo.ulPage){
													stScrollInfo.lPos++;
													dxgiPresent.DirtyRectsCount = 3;
													dxgiPresent.pDirtyRects = rcl3Dirty;

													rcl3Dirty[2].top = (long)rcfThumb.top; rcl3Dirty[2].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[2].left = (long)(rcfThumb.left - 2.0f); rcl3Dirty[2].right = (long)rcfThumb.right;
													rcfThumb.left += fThumbSize; rcfThumb.right += fThumbSize;
													rrcfThumb = {rcfThumb, 2.0f, 2.0f};
													SafeRelease(&ifThumb);
													pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
													rcl3Dirty[1].top = (long)rcfThumb.top; rcl3Dirty[1].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[1].left = (long)rcfThumb.left; rcl3Dirty[1].right = (long)rcfThumb.right;

													ifButtonColor->SetColor(crfButton_Move);
													ifArrowColor->SetColor(crfArrow_Move);
													rcl3Dirty[0].left = lWidth - lHeight; rcl3Dirty[0].top = 0;
													rcl3Dirty[0].right = lWidth; rcl3Dirty[0].bottom = lHeight;
													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													dxgiPresent.DirtyRectsCount = 1;
													dxgiPresent.pDirtyRects = &rclDirty;

													SendMessage(GetParent(hWndElement), WM_VSCROLL, SB_LINEDOWN, NULL);
												}
												else{
													ifButtonColor->SetColor(crfButton_Move);
													ifArrowColor->SetColor(crfArrow_Move);
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
													if(stScrollInfo.lPos + (long)stScrollInfo.ulPage * 2 < stScrollInfo.lMax) stScrollInfo.lPos += stScrollInfo.ulPage;
													else stScrollInfo.lPos = stScrollInfo.lMax - stScrollInfo.ulPage;
													dxgiPresent.DirtyRectsCount = 2;
													dxgiPresent.pDirtyRects = rcl3Dirty;

													rcl3Dirty[1].top = (long)(rcfThumb.top - 2.0f); rcl3Dirty[1].bottom = (long)(rcfThumb.bottom + 2.0f);
													rcl3Dirty[1].left = (long)(rcfThumb.left - 2.0f); rcl3Dirty[1].right = (long)(rcfThumb.right + 2.0f);
													rcfThumb.top = fThumbSize * (float)stScrollInfo.lPos + (float)lWidth;
													rcfThumb.bottom = rcfThumb.top + fThumbSize;
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
													if(stScrollInfo.lPos > (long)stScrollInfo.ulPage) stScrollInfo.lPos -= stScrollInfo.ulPage;
													else stScrollInfo.lPos = 0;
													dxgiPresent.DirtyRectsCount = 2;
													dxgiPresent.pDirtyRects = rcl3Dirty;

													rcl3Dirty[1].top = (long)(rcfThumb.top - 2.0f); rcl3Dirty[1].bottom = (long)(rcfThumb.bottom + 2.0f);
													rcl3Dirty[1].left = (long)(rcfThumb.left - 2.0f); rcl3Dirty[1].right = (long)(rcfThumb.right + 2.0f);
													rcfThumb.top = fThumbSize * (float)stScrollInfo.lPos + (float)lWidth;
													rcfThumb.bottom = rcfThumb.top + fThumbSize;
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
													if(stScrollInfo.lPos + (long)stScrollInfo.ulPage * 2 < stScrollInfo.lMax) stScrollInfo.lPos += stScrollInfo.ulPage;
													else stScrollInfo.lPos = stScrollInfo.lMax - stScrollInfo.ulPage;
													dxgiPresent.DirtyRectsCount = 2;
													dxgiPresent.pDirtyRects = rcl3Dirty;

													rcl3Dirty[1].top = (long)(rcfThumb.top - 2.0f); rcl3Dirty[1].bottom = (long)(rcfThumb.bottom + 2.0f);
													rcl3Dirty[1].left = (long)(rcfThumb.left - 2.0f); rcl3Dirty[1].right = (long)(rcfThumb.right + 2.0f);
													rcfThumb.left = fThumbSize * (float)stScrollInfo.lPos + (float)lHeight;
													rcfThumb.right = rcfThumb.left + fThumbSize;
													rrcfThumb = {rcfThumb, 2.0f, 2.0f};
													SafeRelease(&ifThumb);
													pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
													rcl3Dirty[0].top = (long)rcfThumb.top; rcl3Dirty[0].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[0].left = (long)rcfThumb.left; rcl3Dirty[0].right = (long)rcfThumb.right;

													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													dxgiPresent.DirtyRectsCount = 1;
													dxgiPresent.pDirtyRects = &rclDirty;

													SendMessage(GetParent(hWndElement), WM_VSCROLL, SB_PAGERIGHT, NULL);
												}
												else if(ptlCursor.x < (long)rcfThumb.left && ptlCursor.x > lHeight){
													if(stScrollInfo.lPos > (long)stScrollInfo.ulPage) stScrollInfo.lPos -= stScrollInfo.ulPage;
													else stScrollInfo.lPos = 0;
													dxgiPresent.DirtyRectsCount = 2;
													dxgiPresent.pDirtyRects = rcl3Dirty;

													rcl3Dirty[1].top = (long)(rcfThumb.top - 2.0f); rcl3Dirty[1].bottom = (long)(rcfThumb.bottom + 2.0f);
													rcl3Dirty[1].left = (long)(rcfThumb.left - 2.0f); rcl3Dirty[1].right = (long)(rcfThumb.right + 2.0f);
													rcfThumb.left = fThumbSize * (float)stScrollInfo.lPos + (float)lHeight;
													rcfThumb.right = rcfThumb.left + fThumbSize;
													rrcfThumb = {rcfThumb, 2.0f, 2.0f};
													SafeRelease(&ifThumb);
													pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfThumb, &ifThumb);
													rcl3Dirty[0].top = (long)rcfThumb.top; rcl3Dirty[0].bottom = (long)rcfThumb.bottom;
													rcl3Dirty[0].left = (long)rcfThumb.left; rcl3Dirty[0].right = (long)rcfThumb.right;

													OnRender();
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													dxgiPresent.DirtyRectsCount = 1;
													dxgiPresent.pDirtyRects = &rclDirty;

													SendMessage(GetParent(hWndElement), WM_VSCROLL, SB_PAGELEFT, NULL);
												}
	}
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COScrollBar::CreateThumb(bool bRender)
{
	if(bHorizontal){
		if(stScrollInfo.lMax > 0 && stScrollInfo.ulPage && stScrollInfo.lMax > (long)stScrollInfo.ulPage){
			rcfThumb.left = (float)lWidth - (float)lWidth * fScaleArrowThumb / 100.0f; rcfThumb.right = (float)lWidth * fScaleArrowThumb / 100.0f;
			fThumbSize = (float)(lHeight - lWidth * 2) / (float)(stScrollInfo.lMax - stScrollInfo.ulPage + 1);
			rcfThumb.top = fThumbSize * (float)stScrollInfo.lPos + (float)lWidth;
			rcfThumb.bottom = rcfThumb.top + fThumbSize;
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
		if(stScrollInfo.lMax > 0 && stScrollInfo.ulPage && stScrollInfo.lMax > (long)stScrollInfo.ulPage){
			rcfThumb.top = (float)lHeight - (float)lHeight * fScaleArrowThumb / 100.0f; rcfThumb.bottom = (float)lHeight * fScaleArrowThumb / 100.0f;
			fThumbSize = (float)(lWidth - lHeight * 2) / (float)(stScrollInfo.lMax - stScrollInfo.ulPage + 1);
			rcfThumb.left = fThumbSize * (float)stScrollInfo.lPos + (float)lHeight;
			rcfThumb.right = rcfThumb.left + fThumbSize;
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