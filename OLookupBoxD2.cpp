/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename : OLookupBoxD2.cpp
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
#include "OLookupBoxD2.h"

constexpr BYTE BUTTON = 1;
constexpr BYTE TEXT = 2;
constexpr BYTE NONCLIENT = 3;
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COLookupBox * __vectorcall RePag::DirectX::COLookupBoxV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																				_In_ STDeviceResources* pstDeviceResources)
{
	COLookupBox* vLookupBox = (COLookupBox*)VMBlock(VMDialog(), sizeof(COLookupBox));
	vLookupBox->COLookupBoxV(VMDialog(), pcWindowName, uiIDElement, pstDeviceResources);
	return vLookupBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COLookupBox* __vectorcall RePag::DirectX::COLookupBoxV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																			 _In_ STDeviceResources* pstDeviceResources)
{
	COLookupBox* vLookupBox = (COLookupBox*)VMBlock(vmMemory, sizeof(COLookupBox));
	vLookupBox->COLookupBoxV(vmMemory, pcWindowName, uiIDElement, pstDeviceResources);
	return vLookupBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_LookupBox(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	COLookupBox* pLookupBox;
	switch(uiMessage){
		case WM_CREATE			: ((COLookupBox*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
													((COLookupBox*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
													((COLookupBox*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_LookupBox(hWnd);
													return NULL;
		case WM_SIZE				: pLookupBox = (COLookupBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pLookupBox) pLookupBox->WM_Size(lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_SETFOCUS		: ((COLookupBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_SetFocus();
													return NULL;
		case WM_KILLFOCUS		: pLookupBox = (COLookupBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													pLookupBox->ThreadSafe_Begin();
													if(IsWindowVisible(hWnd)) pLookupBox->WM_LButtonUp_LookupBox();
													if(pLookupBox->pfnWM_KillFocus) pLookupBox->pfnWM_KillFocus(pLookupBox);
													pLookupBox->ThreadSafe_End();
													return NULL;
		case WM_VSCROLL			: ((COLookupBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_VScroll(wParam);
													return NULL;
		case WM_HSCROLL			: ((COLookupBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_HScroll(wParam);
													return NULL;
		case WM_KEYDOWN			: ((COLookupBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KeyDown(wParam);
													return NULL;
		case WM_CHAR				: ((COLookupBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Char(wParam);
													return NULL;
		case WM_COMMAND			: pLookupBox = (COLookupBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pLookupBox->pfnWM_Command){
														pLookupBox->ThreadSafe_Begin();
														if(!pLookupBox->pfnWM_Command(pLookupBox, wParam)){ pLookupBox->ThreadSafe_End(); return NULL; }
														pLookupBox->ThreadSafe_End();
													}
													else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													break;
		case WM_LBUTTONDOWN	: pLookupBox = (COLookupBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pLookupBox->pfnWM_LButtonDown){ pLookupBox->ThreadSafe_Begin(); pLookupBox->pfnWM_LButtonDown(pLookupBox); pLookupBox->ThreadSafe_End(); }
													return NULL;
		case WM_LBUTTONUP		: pLookupBox = (COLookupBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													pLookupBox->ThreadSafe_Begin();
													pLookupBox->WM_LButtonUp(lParam);
													pLookupBox->WM_LButtonUp_LookupBox();
													if(pLookupBox->pfnWM_LButtonUp) pLookupBox->pfnWM_LButtonUp(pLookupBox);
													else PostMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWnd, GWLP_ID), wParam), lParam);
													pLookupBox->ThreadSafe_End();
													return NULL;
		case WM_MOUSEWHEEL	: ((COLookupBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseWheel(wParam, lParam);
													return NULL;
		case WM_NCDESTROY		: pLookupBox = (COLookupBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pLookupBox->htEffect_Timer) DeleteTimerQueueTimer(TimerQueue(), pLookupBox->htEffect_Timer, INVALID_HANDLE_VALUE);
													VMFreiV(pLookupBox);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//---------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_Entry(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	COLookupBox::COEntry* pEntry;
	switch(uiMessage){
		case WM_CREATE			: ((COLookupBox::COEntry*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
													((COLookupBox::COEntry*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
													return NULL;
		case WM_SIZE				: pEntry = (COLookupBox::COEntry*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pEntry) pEntry->WM_Size_Element(hWnd, lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_SETFOCUS		: ((COLookupBox::COEntry*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_SetFocus();
													return NULL;
		case WM_KILLFOCUS		: ((COLookupBox::COEntry*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KillFocus();
													return NULL;
		case WM_CHAR				: pEntry = (COLookupBox::COEntry*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(IsWindowEnabled(hWnd)) pEntry->WM_Char(wParam);
													return NULL;
		case WM_COMMAND			: PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													return NULL;
		case WM_MOUSEHOVER	: ((COLookupBox::COEntry*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseOver(lParam);
													return NULL;
		case WM_MOUSELEAVE	: ((COLookupBox::COEntry*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseLeave();
													return NULL;
		case WM_MOUSEMOVE		: ((COLookupBox::COEntry*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseMove(lParam);
													return NULL;
		case WM_LBUTTONDOWN	: ((COLookupBox::COEntry*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDown(lParam);
													return NULL;
		case WM_LBUTTONUP		: ((COLookupBox::COEntry*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonUp(wParam, lParam);
													return NULL;
		case WM_NCDESTROY		: pEntry = (COLookupBox::COEntry*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pEntry->htEffect_Timer) DeleteTimerQueueTimer(TimerQueue(), pEntry->htEffect_Timer, INVALID_HANDLE_VALUE);
													pEntry->pLookupBox->eEntry = NULL;	VMFreiV(pEntry);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::COLookupBoxV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																														_In_ STDeviceResources* pstDeviceResourcesA)
{
	COListBoxV(vmMemory, pcRePag_LookupBox, pcWindowName, uiIDElementA, pstDeviceResourcesA);

	WindowStyle(WS_CHILDWINDOW | WS_CLIPSIBLINGS);

	COStringA asTemp = pcWindowName; asTemp += "_lbLookupBoxEntry";
	eEntry = (COEntry*)VMBlock(vmMemory, sizeof(COEntry));

  // Note: four numbers uiIDElement, because COScrollBars by COTextBox and COEnum !!!
	eEntry->COEntryV(vmMemory, asTemp.c_Str(), ++uiIDElementA, this, pstDeviceResourcesA);

	eEntry->SetBackgroundColor(crfBackground);
	eEntry->SetTextColor(crfText);

	STFont stFont;
	stFont.fontFamilyName = L"Arial";
	stFont.fontCollection = NULL;
	stFont.fontWeight = DWRITE_FONT_WEIGHT_MEDIUM;
	stFont.fontSytle = DWRITE_FONT_STYLE_NORMAL;
	stFont.fontStretch = DWRITE_FONT_STRETCH_MEDIUM;
	stFont.fontSize = 15.0f;
	stFont.localeName = L"en-en";
	SetFont(stFont);
	ucHeight_Entry = 20;
}
//---------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COLookupBox::COFreiV(void)
{
	if(eEntry) DestroyWindow(eEntry->HWND_Element());
	return ((COListBox*)this)->COFreiV();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::COEntry::COEntryV(_In_ VMEMORY vmSpeicher, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																															_In_ COLookupBox * pLookupBoxA, _In_ STDeviceResources* pstDeviceResourcesA)
{
	COButtonV(vmSpeicher, pcRePag_LookupEntry, pcWindowName, uiIDElementA, pstDeviceResourcesA);

	pLookupBox = pLookupBoxA;

	ucTextAlignment = TXA_CENTERVERTICAL | TXA_LEFT;

	crfButton = D2D1::ColorF(RGB(125, 125, 125), 1.0f);
	crfArrow = D2D1::ColorF(RGB(255, 255, 0), 1.0f);

	crfButton_Move = D2D1::ColorF(RGB(255, 255, 0), 1.0f);
	crfArrow_Move = D2D1::ColorF(RGB(255, 50, 50), 1.f);

	crfButton_Click = D2D1::ColorF(RGB(255, 50, 50), 1.0f);
	crfArrow_Click = D2D1::ColorF(RGB(255, 255, 0), 1.0f);

	bMouseTracking = false;
	ucDirty = NONCLIENT;
}
//---------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COLookupBox::COEntry::COFreiV(void)
{
	SafeRelease(&ifButton); SafeRelease(&ifButtonColor);
	SafeRelease(&ifArrow); SafeRelease(&ifArrowColor);

	return ((COTextLine*)this)->COFreiV();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::COEntry::OnRender(void)
{
	IDWriteTextLayout* ifTextLayout; WCHAR wcContent[255]; size_t szBytes_Text; float fTextWidth;	D2D1_RECT_F rcfText;

	WaitForSingleObjectEx(heRender, INFINITE, false);
	if(mbstowcs_s(&szBytes_Text, wcContent, 255, vasContent->c_Str(), vasContent->Length())) goto Error;
	if(pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wcContent, (UINT32)szBytes_Text, ifText, (float)lWidth, (float)lHeight, &ifTextLayout)) goto Error;
	TextAlignment(ifTextLayout, fTextWidth, rcfText);
	SafeRelease(&ifTextLayout);

	ifD2D1Context6->BeginDraw();
	ifD2D1Context6->Clear(crfBackground);
	ifD2D1Context6->DrawText(wcContent, (UINT32)szBytes_Text, ifText, rcfText, ifTextColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);

	ifD2D1Context6->DrawGeometry(ifButton, ifButtonColor, 1);
	ifD2D1Context6->FillGeometry(ifButton, ifButtonColor);
	ifD2D1Context6->DrawGeometry(ifArrow, ifArrowColor, 1);
	ifD2D1Context6->FillGeometry(ifArrow, ifArrowColor);
	ifD2D1Context6->EndDraw();

Error:
	SetEvent(heRender);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::COEntry::Geometry(void)
{
	ID2D1GeometrySink* ifSink = nullptr;
	D2D1_POINT_2F ptfArrow[3] = {
		D2D1::Point2F(fButton_left + fButtonSize * 0.25f, fButton_top + fButtonSize * 0.25f),
		D2D1::Point2F(fButton_left + fButtonSize * 0.75f, fButton_top + fButtonSize * 0.25f),
		D2D1::Point2F(fButton_left + fButtonSize * 0.50f, fButton_top + fButtonSize * 0.75f)
	};

	SafeRelease(&ifArrow);
	pstDeviceResources->ifd2d1Factory7->CreatePathGeometry(&ifArrow);
	ifArrow->Open(&ifSink);
	ifSink->SetFillMode(D2D1_FILL_MODE_WINDING);
	ifSink->BeginFigure(ptfArrow[0], D2D1_FIGURE_BEGIN_FILLED);
	ifSink->AddLines(ptfArrow, 3);
	ifSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	ifSink->Close();
	SafeRelease(&ifSink);

	D2D1_RECT_F rcfButton = D2D1::RectF(fButton_left, fButton_top, (float)lWidth, fButton_top + fButtonSize);
	SafeRelease(&ifButton);
	pstDeviceResources->ifd2d1Factory7->CreateRectangleGeometry(rcfButton, &ifButton);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::COEntry::OnPaint(void)
{
	ThreadSafe_Begin();
	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::COEntry::WM_Create(void)
{
	CharacterMetric();

	stTrackMouseEvent.cbSize = sizeof(stTrackMouseEvent);
	stTrackMouseEvent.dwFlags = TME_HOVER | TME_LEAVE;
	stTrackMouseEvent.hwndTrack = hWndElement;
	stTrackMouseEvent.dwHoverTime = 10;

	float fTextHeight = (float)lHeight;
	float fTextWidth = (float)lWidth;
	fButtonSize = fTextHeight < fTextWidth ? fTextHeight : fTextWidth;
	fButton_left = fTextWidth - fButtonSize;
	fButton_top = (fTextHeight - fButtonSize) / 2.0f;

	ifD2D1Context6->CreateSolidColorBrush(crfText, &ifTextColor);
	ifD2D1Context6->CreateSolidColorBrush(crfButton, &ifButtonColor);
	ifD2D1Context6->CreateSolidColorBrush(crfArrow, &ifArrowColor);

	Geometry();

	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::COEntry::WM_MouseMove(_In_ LPARAM lParam)
{
	POINTS ptlCursor = MAKEPOINTS(lParam);
	ThreadSafe_Begin();
	if(!bMouseTracking){ bMouseTracking = true;	TrackMouseEvent(&stTrackMouseEvent); }
	else{
		switch(ucDirty){
			case BUTTON	: if(ptlCursor.x < fButton_left){
											ifButtonColor->SetColor(crfButton);
											ifArrowColor->SetColor(crfArrow);
											rclDirty.left = (long)fButton_left; rclDirty.top = 0;
											rclDirty.right = lWidth; rclDirty.bottom = pLookupBox->ucHeight_Entry;
											OnRender();
											ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											ucDirty = TEXT;
										}
										break;
			case TEXT		: if(ptlCursor.x >= fButton_left){
											ifButtonColor->SetColor(crfButton_Move);
											ifArrowColor->SetColor(crfArrow_Move);
											rclDirty.left = (long)fButton_left; rclDirty.top = 0;
											rclDirty.right = lWidth; rclDirty.bottom = pLookupBox->ucHeight_Entry;
											OnRender();
											ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											ucDirty = BUTTON;
										}
		}
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::COEntry::WM_MouseOver(_In_ LPARAM lParam)
{
	POINTS ptlCursor = MAKEPOINTS(lParam);
	ThreadSafe_Begin();
	if(ptlCursor.x >= fButton_left){
		ifButtonColor->SetColor(crfButton_Move);
		ifArrowColor->SetColor(crfArrow_Move);
		rclDirty.left = (long)fButton_left; rclDirty.top = 0;  
		rclDirty.right = lWidth; rclDirty.bottom = pLookupBox->ucHeight_Entry; 
		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		ucDirty = BUTTON;
	}
	else ucDirty = TEXT;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::COEntry::WM_MouseLeave(void)
{
	ThreadSafe_Begin();
	ifButtonColor->SetColor(crfButton);
	ifArrowColor->SetColor(crfArrow);
	rclDirty.left = (long)fButton_left; rclDirty.top = 0;
	rclDirty.right = lWidth; rclDirty.bottom = pLookupBox->ucHeight_Entry;
	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ucDirty = NONCLIENT;
	bMouseTracking = false;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::COEntry::WM_LButtonDown(_In_ LPARAM lParam)
{
	POINTS ptlCursor = MAKEPOINTS(lParam);
	ThreadSafe_Begin();
	if(pfnWM_LButtonUp) pfnWM_LButtonUp(this);

	if(ptlCursor.x >= fButton_left){
		ifButtonColor->SetColor(crfButton_Click);
		ifArrowColor->SetColor(crfArrow_Click);
		rclDirty.left = (long)fButton_left; rclDirty.top = 0;
		rclDirty.right = lWidth; rclDirty.bottom = pLookupBox->ucHeight_Entry;
		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::COEntry::WM_LButtonUp(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
  POINTS ptlCursor = MAKEPOINTS(lParam); BYTE ucIndex = 0;
	ThreadSafe_Begin();
	if(ptlCursor.x >= fButton_left){
		SetWindowPos(pLookupBox->HWND_Element(), HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
		SetFocus(pLookupBox->HWND_Element());
		pLookupBox->ulCharacterPos = vasContent->Length();
		pLookupBox->SearchAndSetEntry(vasContent, ucIndex);;

		ifButtonColor->SetColor(crfButton_Move);
		ifArrowColor->SetColor(crfArrow_Move);
		rclDirty.left = (long)fButton_left; rclDirty.top = 0;
		rclDirty.right = lWidth; rclDirty.bottom = pLookupBox->ucHeight_Entry;
		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);

		if(pfnWM_LButtonUp) pfnWM_LButtonUp(this);
		else PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), lParam);
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::COEntry::Text(_In_z_ char* pcText)
{
	ThreadSafe_Begin();
	*vasContent = pcText;
	if(hWndElement){
		rclDirty.left = rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::COEntry::Lock(void)
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
void __vectorcall RePag::DirectX::COLookupBox::COEntry::Release(void)
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
void __vectorcall RePag::DirectX::COLookupBox::WM_Create_LookupBox(_In_ HWND hWnd)
{
	hWndElement = hWnd;

	eEntry->CreateWindowGraphic(GetParent(hWndElement), ucHeight_Entry, lWidth, ptPosition.x, ptPosition.y - ucHeight_Entry);
	NewWindow(lHeight - ucHeight_Entry, lWidth, ptPosition.x, ptPosition.y);

	if(vliText->Number()) eEntry->Text(SetAndSearchEnum(0, vasContent)->c_Str());
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::WM_LButtonUp_LookupBox(void)
{
	ThreadSafe_Begin();
	eEntry->Text(SelectEnum(vasContent)->c_Str());
	ShowWindow(hWndElement, SW_HIDE);
	HWND hWndZeichnen_1, hWndZeichnen_2 = GetParent(hWndElement);
	while(IsChild(hWndZeichnen_1 = GetParent(hWndZeichnen_2), hWndZeichnen_2)) hWndZeichnen_2 = hWndZeichnen_1;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
long& __vectorcall RePag::DirectX::COLookupBox::GetHeight(_Out_ long& lHeightA)
{
	ThreadSafe_Begin();
	lHeightA = lHeight + ucHeight_Entry;
	ThreadSafe_End();
	return lHeightA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
RECT& __vectorcall RePag::DirectX::COLookupBox::GetWindow(_Out_ RECT& rcWindow)
{
	ThreadSafe_Begin();
	rcWindow.left = ptPosition.x;
	rcWindow.top = ptPosition.y - ucHeight_Entry;
	rcWindow.right = ptPosition.x + lWidth;
	rcWindow.bottom = lHeight + ptPosition.y;
	ThreadSafe_End();
	return rcWindow;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::NewWindowPosition(_In_ long lPos_x, _In_ long lPos_y)
{
	ThreadSafe_Begin();
	ptPosition.x = lPos_x; ptPosition.y = lPos_y + ucHeight_Entry;
	MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, IsWindowVisible(hWndElement));

	eEntry->NewWindowPosition(ptPosition.x, ptPosition.y - ucHeight_Entry);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::NewWindowPosition(_In_ POINT& ptPositionA)
{
	ThreadSafe_Begin();
	ptPositionA.y += ucHeight_Entry;
	ptPosition = ptPositionA;
	MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, IsWindowVisible(hWndElement));

	eEntry->NewWindowPosition(ptPosition.x, ptPosition.y - ucHeight_Entry);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::NewWindow(_In_ long lHeightA, _In_ long lWidthA, _In_ long lPos_x, _In_ long lPos_y)
{
	ThreadSafe_Begin();
	lHeight = lHeightA; lWidth = lWidthA; ptPosition.x = lPos_x; ptPosition.y = lPos_y + ucHeight_Entry;

	MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, IsWindowVisible(hWndElement));
	CreateWindowSizeDependentResources();

  SetScrollBarPos(SB_HORZ, 0, ptPosition.y - ucScrollBarSize - ucHeight_Entry);

	STScrollInfo siScrollInfo;
	siScrollInfo.ucMask = SBI_POS | SBI_PAGE;
	siScrollInfo.fPos = 0;
	siScrollInfo.fPage = (float)lHeight - ucScrollBarSize;
	SetScrollBar(SB_VERT, siScrollInfo);

	siScrollInfo.fPage = (float)lWidth - ucScrollBarSize;
	SetScrollBar(SB_HORZ, siScrollInfo);

	ChangeSizeVisibleScrollBars();

	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);

	eEntry->NewWindow(ucHeight_Entry, lWidth, lPos_x, lPos_y);
	eEntry->Geometry();
	eEntry->OnPaint();
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::NewWindowSize(_In_ long lHeightA, _In_ long lWidthA)
{
	ThreadSafe_Begin();
	lHeight = lHeightA - ucHeight_Entry; lWidth = lWidthA;

	MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, IsWindowVisible(hWndElement));
	CreateWindowSizeDependentResources();

	eEntry->NewWindowSize(ucHeight_Entry, lWidthA);
	eEntry->Geometry();
	eEntry->OnPaint();
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::NewWindowHeight(_In_ long lHeightA)
{
	ThreadSafe_Begin();
	lHeight = lHeightA - ucHeight_Entry;

	MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, IsWindowVisible(hWndElement));
	CreateWindowSizeDependentResources();
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::NewWindowWidth(_In_ long lWidthA)
{
	ThreadSafe_Begin();
	lWidth = lWidthA;

	MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, IsWindowVisible(hWndElement));
	CreateWindowSizeDependentResources();

	eEntry->NewWindowWidth(lWidthA);
	eEntry->Geometry();
	eEntry->OnPaint();
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::ChangeWindowSize(_In_ long lHeightA, _In_ long lWidthA)
{
	ThreadSafe_Begin();
	lHeight += lHeightA; lWidth += lWidthA;

	MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, IsWindowVisible(hWndElement));
	CreateWindowSizeDependentResources();

	eEntry->ChangeWindowSize(0, lWidthA);
	eEntry->Geometry();
	eEntry->OnPaint();
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::ChangeWindowPosition(_In_ long lPos_x, _In_ long lPos_y)
{
	ThreadSafe_Begin();
	ptPosition.x += lPos_x; ptPosition.y += lPos_y;
	MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, IsWindowVisible(hWndElement));

	eEntry->ChangeWindowPosition(lPos_x, lPos_y);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::SetFont(_In_ STFont& stFont)
{
	ThreadSafe_Begin(); 
	SafeRelease(&ifText);
	pstDeviceResources->ifdwriteFactory7->CreateTextFormat(stFont.fontFamilyName, stFont.fontCollection, stFont.fontWeight, stFont.fontSytle,
																												 stFont.fontStretch, stFont.fontSize, stFont.localeName, &ifText);
	eEntry->SetFont(stFont);;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::SetHeight_Entry(_In_ unsigned char ucHeight)
{
	ThreadSafe_Begin();
	ucHeight_Entry = ucHeight;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::SetTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfText = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	eEntry->SetTextColor(crfText);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::SetBackgroundColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfBackground = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	eEntry->SetBackgroundColor(crfBackground);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::SetButtonColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	eEntry->crfButton = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::SetButtonColor(_In_ D2D1_COLOR_F& crfButtonA)
{
	ThreadSafe_Begin();
	eEntry->crfButton = crfButtonA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::SetButtonColor_Move(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	eEntry->crfButton_Move = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::SetButtonColor_Move(_In_ D2D1_COLOR_F& crfButton_MoveA)
{
	ThreadSafe_Begin();
	eEntry->crfButton_Move = crfButton_MoveA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::SetButtonColor_Click(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	eEntry->crfButton_Click = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::SetButtonColor_Click(_In_ D2D1_COLOR_F& crfButton_ClickA)
{
	ThreadSafe_Begin();
	eEntry->crfButton_Click = crfButton_ClickA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::SetArrowColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	eEntry->crfArrow = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::SetArrowColor(_In_ D2D1_COLOR_F& crfArrowA)
{
	ThreadSafe_Begin();
	eEntry->crfArrow = crfArrowA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::SetArrowColor_Move(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	eEntry->crfArrow_Move = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::SetArrowColor_Move(_In_ D2D1_COLOR_F& crfArrow_MoveA)
{
	ThreadSafe_Begin();
	eEntry->crfArrow_Move = crfArrow_MoveA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::SetArrowColor_Click(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	eEntry->crfArrow_Click = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::SetArrowColor_Click(_In_ D2D1_COLOR_F& crfArrow_ClickA)
{
	ThreadSafe_Begin();
	eEntry->crfArrow_Click = crfArrow_ClickA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::Lock(void)
{
	eEntry->Lock();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::Release(void)
{
	eEntry->Release();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COLookupBox::Text(_In_z_ char* pcText)
{
	ThreadSafe_Begin();
	((COTextBox*)this)->Text(pcText);
	if(eEntry->Active() && vliText->Number()) eEntry->Text(SetAndSearchEnum(0, vasContent)->c_Str());
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COLookupBox::SetSelectIndex(_In_ unsigned char ucIndex)
{
	ThreadSafe_Begin();
	if(((COListBox*)this)->SetSelectIndex(ucIndex)){
		eEntry->Text(SetAndSearchEnum(ucIndex, vasContent)->c_Str());
		ThreadSafe_End();
		return true;
	}
	ThreadSafe_End();
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COLookupBox::SearchAndSetEntry(_In_ COStringA* vasEnum, _In_ unsigned char& ucIndexA)
{
	ThreadSafe_Begin();
	bool bReturn = false;
	if(SearchEnum(vasEnum, ucIndexA)) bReturn = SetSelectIndex(ucIndexA);
	ThreadSafe_End();
	return bReturn;
}
//---------------------------------------------------------------------------------------------------------------------------------------
