/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OEditBoxD2.cpp
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
#include "OEditBoxD2.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
#define _CurrentLine ((COStringA*)pvLine)
#define _EditBox ((RePag::DirectX::COEditBox*)pvParam)
#define _DeleteLine ((COStringA*)vliText->Element(pvIterator))
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COEditBox* __vectorcall RePag::DirectX::COEditBoxV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																	 _In_ STDeviceResources* pstDeviceResources)
{
	COEditBox* vEditBox = (COEditBox*)VMBlock(VMDialog(), sizeof(COEditBox));
	vEditBox->COEditBoxV(VMDialog(), pcWindowName, uiIDElement, pstDeviceResources);
	return vEditBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COEditBox* __vectorcall RePag::DirectX::COEditBoxV(_In_ VMEMORY vmSpeicher, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																	 _In_ STDeviceResources* pstDeviceResources)
{
	COEditBox* vEditBox = (COEditBox*)VMBlock(vmSpeicher, sizeof(COEditBox));
	vEditBox->COEditBoxV(vmSpeicher, pcWindowName, uiIDElement, pstDeviceResources);
	return vEditBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_EditBox(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	COEditBox* pEditBox;
	switch(uiMessage){
		case WM_CREATE				: ((COEditBox*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
														((COEditBox*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
														return NULL;
		case WM_SIZE					: pEditBox = (COEditBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
														if(pEditBox) pEditBox->WM_Size(lParam);
														else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
														return NULL;
		case WM_VSCROLL       : ((COEditBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_VScroll(wParam, lParam);
														return NULL;
		case WM_HSCROLL       : ((COEditBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_HScroll();
														return NULL;
		case WM_SETFOCUS      : ((COEditBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_SetFocus();
														return NULL;
		case WM_KILLFOCUS     : ((COEditBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KillFocus();
														return NULL;
		case WM_KEYDOWN       : ((COEditBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KeyDown(wParam, lParam);
														return NULL;
		case WM_CHAR          :	((COEditBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Char(wParam);
														return NULL;
		case WM_COMMAND       : pEditBox = (COEditBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
														if(!pEditBox->WM_Command(wParam)) return NULL;
														else if(pEditBox->pfnWM_Command){
															pEditBox->ThreadSafe_Begin();
															if(!pEditBox->pfnWM_Command(pEditBox, wParam)){ pEditBox->ThreadSafe_End(); return NULL; }
															pEditBox->ThreadSafe_End();
														}
														else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
														break;
		case WM_CONTEXTMENU   : ((COEditBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_ContexMenu(lParam);
														return NULL;
		case WM_MOUSEMOVE     : ((COEditBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseMove(wParam, lParam);
														return NULL;
		case WM_LBUTTONDOWN   : ((COEditBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDown(lParam);
														return NULL;
		case WM_LBUTTONUP     : ((COEditBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonUp(wParam, lParam);
														return NULL;
		case WM_MOUSEWHEEL    : ((COEditBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseWheel(wParam, lParam);
														return NULL;
		case WM_NCDESTROY     : pEditBox = (COEditBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
														if(pEditBox->htEffect_Timer) DeleteTimerQueueTimer(TimerQueue(), pEditBox->htEffect_Timer, INVALID_HANDLE_VALUE);
														VMFreiV(pEditBox);
														return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void CALLBACK RePag::DirectX::Timer_Caret_EditBox(_In_ void* pvParam, _In_ bool bTimerOrWaitFired)
{
	WaitForSingleObject(_EditBox->heCaret, INFINITE);
	static bool bCaret = false;
	bCaret ? bCaret = false : bCaret = true;

  STScrollInfo siCharacter{}; siCharacter.ucMask = SBI_POS;
	_EditBox->ThreadSafe_Begin();
	_EditBox->GetScrollBar(SB_HORZ, siCharacter);
	_EditBox->rclDirty.left = _EditBox->FloatToLong(_EditBox->ptfCaret.x - siCharacter.fPos);
	_EditBox->rclDirty.right = _EditBox->rclDirty.left + _EditBox->ucCaretStrength;
	_EditBox->rclDirty.top = _EditBox->FloatToLong(_EditBox->ptfCaret.y);
	_EditBox->rclDirty.bottom = _EditBox->FloatToLong(_EditBox->ptfCaret.y + _EditBox->szfCharacter.height);

	_EditBox->OnRender(bCaret);
	_EditBox->ifDXGISwapChain4->Present1(0, NULL, &_EditBox->dxgiPresent);
	_EditBox->ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::COEditBoxV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																												_In_ STDeviceResources* pstDeviceResources)
{
	// Note: three numbers uiIDElement, because COScrollBars by COTextBox!!!
	COTextBoxV(vmMemory, pcRePag_EditBox, pcWindowName, uiIDElementA, pstDeviceResources);

	heCaret = CreateEvent(nullptr, true, true, nullptr);
	pfnWM_Char_ShiftReturn = nullptr;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::OnPaint(void)
{
	ThreadSafe_Begin();
	rclDirty.left = 0; rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::WM_Size(_In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	if(lHeight != HIWORD(lParam) || lWidth != LOWORD(lParam)){
		lHeight = HIWORD(lParam); lWidth = LOWORD(lParam);
		CreateWindowSizeDependentResources();
		STScrollInfo siScrollInfo{}; siScrollInfo.ucMask = SBI_PAGE;

		siScrollInfo.fPage = (float)lHeight;
		sbVertical->SetScrollInfo(siScrollInfo);
		sbVertical->NewWindow(lHeight - ucScrollBarSize, ucScrollBarSize, lWidth - ucScrollBarSize, 0);

		siScrollInfo.fPage = (float)lWidth;
		sbHorizontal->SetScrollInfo(siScrollInfo);
		sbHorizontal->NewWindow(ucScrollBarSize, lWidth - ucScrollBarSize, 0, lHeight - ucScrollBarSize);

		ChangeSizeVisibleScrollBars();
		OnRender(false);
		ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::WM_SetFocus(void)
{
	ThreadSafe_Begin();
	if(!cSelect){
		if(!vliText->Number()){
			COStringA* vasZeile = COStringAV(vmMemory); vliText->ToEnd(vasZeile); pvLine = vasZeile;
			ptfCaret.x = ptfCaret.y = 0.0f; ulCharacterPos = lLine = 0;
		}
		else{
			STScrollInfo siLine{}, siCharacter{}; siLine.ucMask = siCharacter.ucMask = SBI_POS;
			GetScrollBar(SB_VERT, siLine); GetScrollBar(SB_HORZ, siCharacter);
			if(!ptfCaret.x && !ptfCaret.y && !siLine.fPos && !siCharacter.fPos){ ulCharacterPos = lLine = 0; pvLine = vliText->Element(lLine); }
		}
		if(!htCaret) CreateTimerQueueTimer(&htCaret, TimerQueue(), (WAITORTIMERCALLBACK)Timer_Caret_EditBox, this, 0, 500, 0);
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::WM_KillFocus(void)
{
	ThreadSafe_Begin();
	DeleteTimerQueueTimer(TimerQueue(), htCaret, NULL); htCaret = nullptr;

	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);

	if(pfnWM_KillFocus) pfnWM_KillFocus(this);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::WM_HScroll(void)
{
	ThreadSafe_Begin();

	rclDirty.left = rclDirty.top = 0;
	rclDirty.right = lWidth; rclDirty.bottom = lHeight;
	OnRender(true);
	ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::WM_VScroll(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	STScrollInfo siLine{}; siLine.ucMask = SBI_PAGE | SBI_POS | SBI_MAX | SBI_CHARACTER_HEIGHT; GetScrollBar(SB_VERT, siLine);
	bool bScrollChanged = lParam != NULL;
	float fScrollPage = siLine.fPage;
	if(siLine.szfCharacter.height > 0.0f){
		long lScrollLines = (long)(siLine.fPage / siLine.szfCharacter.height) + 1;
		fScrollPage = (float)lScrollLines * siLine.szfCharacter.height;
	}
	switch(LOWORD(wParam)){
		case SB_LINEUP		: if(!lParam && siLine.fPos > 0.0f){
													siLine.fPos -= szfCharacter.height;
													if(siLine.fPos < 0.0f) siLine.fPos = 0.0f;
													siLine.ucMask = SBI_POS;
													SetScrollBar(SB_VERT, siLine);
													bScrollChanged = true;
												}
												if(bScrollChanged) ptfCaret.y += szfCharacter.height;
												break;
		case SB_LINEDOWN	: if(!lParam && siLine.fPos + siLine.fPage < siLine.fMax){
													siLine.fPos += szfCharacter.height;
													if(siLine.fPos + siLine.fPage > siLine.fMax) siLine.fPos = siLine.fMax - siLine.fPage;
													if(siLine.fPos < 0.0f) siLine.fPos = 0.0f;
													siLine.ucMask = SBI_POS;
													SetScrollBar(SB_VERT, siLine);
													bScrollChanged = true;
												}
												if(bScrollChanged) ptfCaret.y -= szfCharacter.height;
												break;
		case SB_PAGEUP		: if(!lParam && siLine.fPos > 0.0f){
													siLine.fPos -= fScrollPage;
													if(siLine.fPos < 0.0f) siLine.fPos = 0.0f;
													siLine.ucMask = SBI_POS;
													SetScrollBar(SB_VERT, siLine);
												}
												break;
		case SB_PAGEDOWN	: if(!lParam && siLine.fPos + siLine.fPage < siLine.fMax){
													siLine.fPos += fScrollPage;
													if(siLine.fPos + siLine.fPage > siLine.fMax) siLine.fPos = siLine.fMax - siLine.fPage;
													if(siLine.fPos < 0.0f) siLine.fPos = 0.0f;
													siLine.ucMask = SBI_POS;
													SetScrollBar(SB_VERT, siLine);
												}
												break;
	}

	rclDirty.left = rclDirty.top = 0;
	rclDirty.right = lWidth; rclDirty.bottom = FloatToLong(siLine.fPage);
	OnRender(true);
	ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::WM_KeyDown(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
  D2D_SIZE_F szfTextPoint; STScrollInfo siLine, siCharacter{}; siCharacter.ucMask = SBI_POS; siLine.ucMask = SBI_PAGE; RECT rcl2Dirty[2];
	void* pvLineTemp = nullptr; float fCharacterPos_old; D2D_POINT_2F ptfCaret_old; long lLines;
	unsigned long ulCharacterPos_old; long lLine_old;

	switch(wParam){
		case VK_HOME		:	ThreadSafe_Begin();
											ulCharacterPos = 0;
											GetScrollBar(SB_HORZ, siCharacter);
											if(!siCharacter.fPos){
												rcl2Dirty[0].left = FloatToLong(ptfCaret.x);
												rcl2Dirty[0].right = rcl2Dirty[0].left + ucCaretStrength + 2;
												rcl2Dirty[0].top = FloatToLong(ptfCaret.y);
												rcl2Dirty[0].bottom = FloatToLong(ptfCaret.y + szfCharacter.height);

												ptfCaret.x = 0;
												rcl2Dirty[1].left = FloatToLong(ptfCaret.x);
												rcl2Dirty[1].right = rcl2Dirty[1].left + ucCaretStrength + 2;
												rcl2Dirty[1].top = FloatToLong(ptfCaret.y);
												rcl2Dirty[1].bottom = rcl2Dirty[1].top + FloatToLong(szfCharacter.height);

												dxgiPresent.DirtyRectsCount = 2;
												dxgiPresent.pDirtyRects = rcl2Dirty;
												OnRender(true);
												ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
												dxgiPresent.DirtyRectsCount = 1;
												dxgiPresent.pDirtyRects = &rclDirty;
											}
											else{
												ptfCaret.x = 0;
												siCharacter.fPos = 0;
                        SetScrollBar(SB_HORZ, siCharacter);
												rclDirty.left = rclDirty.top = 0;
												siCharacter.ucMask = SBI_PAGE;
												GetScrollBar(SB_HORZ, siCharacter);
												GetScrollBar(SB_VERT, siLine);
												rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
												OnRender(true);
												ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
											}
											ThreadSafe_End();
											break;
		case VK_END			: ThreadSafe_Begin();
											GetTextPoint(_CurrentLine->c_Str(), _CurrentLine->Length(), szfTextPoint);
											ulCharacterPos = _CurrentLine->Length();

											siCharacter.ucMask |= SIF_PAGE;
											GetScrollBar(SB_HORZ, siCharacter);
											if(siCharacter.fPage > szfTextPoint.width){
												rcl2Dirty[0].left = FloatToLong(ptfCaret.x);
												rcl2Dirty[0].right = rcl2Dirty[0].left + ucCaretStrength + 2;
												rcl2Dirty[0].top = FloatToLong(ptfCaret.y);
												rcl2Dirty[0].bottom = FloatToLong(ptfCaret.y + szfCharacter.height);

												ptfCaret.x = szfTextPoint.width;
												rcl2Dirty[1].left = FloatToLong(ptfCaret.x);
												rcl2Dirty[1].right = rcl2Dirty[1].left + ucCaretStrength + 2;
												rcl2Dirty[1].top = FloatToLong(ptfCaret.y);
												rcl2Dirty[1].bottom = rcl2Dirty[1].top + FloatToLong(szfCharacter.height);

												dxgiPresent.DirtyRectsCount = 2;
												dxgiPresent.pDirtyRects = rcl2Dirty;
												OnRender(true);
												ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
												dxgiPresent.DirtyRectsCount = 1;
												dxgiPresent.pDirtyRects = &rclDirty;
											}
											else{
												siCharacter.fPos = szfTextPoint.width - (float)lWidth + (float)ucScrollBarSize;
												ptfCaret.x = szfTextPoint.width;
												SetScrollBar(SB_HORZ, siCharacter);

												GetScrollBar(SB_VERT, siLine);
												rclDirty.left = rclDirty.top = 0;
												rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
												OnRender(true);
												ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
											}

											ThreadSafe_End();
											break;
		case VK_LEFT		: ThreadSafe_Begin();
											if(ulCharacterPos){
												if(!cSelect){
													GetTextPoint(_CurrentLine->c_Str(), --ulCharacterPos, szfTextPoint);
													GetScrollBar(SB_HORZ, siCharacter);
													rcl2Dirty[1].left = FloatToLong(ptfCaret.x - siCharacter.fPos) - 1;
													ptfCaret_old = ptfCaret;
													ptfCaret.x = szfTextPoint.width;
													if(szfTextPoint.width - siCharacter.fPos > 0.0f){
														rcl2Dirty[1].top = rcl2Dirty[0].top = rclDirty.top;
														rcl2Dirty[1].bottom = rcl2Dirty[0].bottom = rclDirty.bottom;
														rcl2Dirty[1].right = rcl2Dirty[1].left + ucCaretStrength + 2;
														rcl2Dirty[0].left = FloatToLong(ptfCaret.x - siCharacter.fPos);
														rcl2Dirty[0].right = rcl2Dirty[0].left + ucCaretStrength;

														dxgiPresent.DirtyRectsCount = 2;
														dxgiPresent.pDirtyRects = rcl2Dirty;
														OnRender(true);
														ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
														dxgiPresent.DirtyRectsCount = 1;
														dxgiPresent.pDirtyRects = &rclDirty;
													}
													else{
														siCharacter.fPos = szfTextPoint.width;
														SetScrollBar(SB_HORZ, siCharacter);
														rclDirty.left = rclDirty.top = 0;
														siCharacter.ucMask = SBI_PAGE; GetScrollBar(SB_HORZ, siCharacter);
														GetScrollBar(SB_VERT, siLine);
														rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
														OnRender(true);
														ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
													}
												}

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam) SelectText_Left(ptfCaret_old);
												else if(cSelect) DeSelect();
											}
											ThreadSafe_End();
											break;
		case VK_RIGHT		: ThreadSafe_Begin();
											if(ulCharacterPos < _CurrentLine->Length()){
												if(!cSelect){
													GetScrollBar(SB_HORZ, siCharacter);
													rcl2Dirty[0].left = FloatToLong(ptfCaret.x - siCharacter.fPos) - 1;
													GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint);
													ptfCaret_old = ptfCaret;
													ptfCaret.x = szfTextPoint.width;

													siCharacter.ucMask = SBI_PAGE | SBI_POS;
													GetScrollBar(SB_HORZ, siCharacter);
													if(szfTextPoint.width - siCharacter.fPos < siCharacter.fPage){
														rcl2Dirty[0].top = rcl2Dirty[1].top = FloatToLong(ptfCaret.y);
														rcl2Dirty[0].bottom = rcl2Dirty[1].bottom = FloatToLong(ptfCaret.y + szfCharacter.height);
														rcl2Dirty[0].right = rcl2Dirty[0].left + ucCaretStrength + 2;
														rcl2Dirty[1].left = FloatToLong(ptfCaret.x - siCharacter.fPos);
														rcl2Dirty[1].right = rcl2Dirty[1].left + ucCaretStrength;

														dxgiPresent.DirtyRectsCount = 2;
														dxgiPresent.pDirtyRects = rcl2Dirty;
														OnRender(true);
														ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
														dxgiPresent.DirtyRectsCount = 1;
														dxgiPresent.pDirtyRects = &rclDirty;
													}
													else{
														siCharacter.fPos = szfTextPoint.width - siCharacter.fPage + (float)ucCaretStrength;
														if(siCharacter.fPos < 0.0f) siCharacter.fPos = 0.0f;
														siCharacter.ucMask = SBI_POS;
														SetScrollBar(SB_HORZ, siCharacter);
														rclDirty.left = rclDirty.top = 0;
														siCharacter.ucMask = SBI_PAGE; GetScrollBar(SB_HORZ, siCharacter);
														GetScrollBar(SB_VERT, siLine);
														rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
														OnRender(true);
														ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
													}
												}

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam) SelectText_Right(ptfCaret_old);
												else if(cSelect) DeSelect();
											}
											ThreadSafe_End();
											break;
		case VK_UP			: ThreadSafe_Begin();
											if(lLine){
												siCharacter.ucMask = SBI_PAGE | SBI_POS; GetScrollBar(SB_HORZ, siCharacter);
												if(!cSelect){
													fCharacterPos_old = siCharacter.fPos;
													ptfCaret_old = ptfCaret; ulCharacterPos_old = ulCharacterPos;
													lLine_old = lLine; 

													pvLine = vliText->Element(--lLine);
													ulCharacterPos = 0;
													if(ptfCaret.x > 0.0f && _CurrentLine->Length()){
														do{ GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint); }
														while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _CurrentLine->Length());
														ptfCaret.x = szfTextPoint.width;
													}
													else ptfCaret.x = 0.0f;

													GetScrollBar(SB_HORZ, siCharacter);
													if(ptfCaret.x < siCharacter.fPos){
														siCharacter.fPos = ptfCaret.x;
														siCharacter.ucMask = SBI_POS;
														SetScrollBar(SB_HORZ, siCharacter);
													}
													else if(ptfCaret.x - siCharacter.fPos >= siCharacter.fPage){
														siCharacter.fPos = ptfCaret.x - siCharacter.fPage + (float)ucCaretStrength;
														if(siCharacter.fPos < 0.0f) siCharacter.fPos = 0.0f;
														siCharacter.ucMask = SBI_POS;
														SetScrollBar(SB_HORZ, siCharacter);
													}

													if(!ptfCaret.y){
														siLine.ucMask = SBI_POS;
														GetScrollBar(SB_VERT, siLine);
														if(siLine.fPos > 0.0f){
															siLine.fPos -= szfCharacter.height;
															if(siLine.fPos < 0.0f) siLine.fPos = 0.0f;
															SetScrollBar(SB_VERT, siLine);

															rclDirty.left = rclDirty.top = 0;
															rclDirty.right = lWidth; rclDirty.bottom = lHeight;
															OnRender(true);
															ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
														}
													}
													else{
														rcl2Dirty[0].left = FloatToLong(ptfCaret_old.x - fCharacterPos_old);
														rcl2Dirty[0].right = rcl2Dirty[0].left + ucCaretStrength + 2;
														rcl2Dirty[0].top = FloatToLong(ptfCaret_old.y);
														rcl2Dirty[0].bottom = FloatToLong(ptfCaret_old.y + szfCharacter.height);

														ptfCaret.y -= szfCharacter.height;
														siCharacter.ucMask = SBI_POS;
														GetScrollBar(SB_HORZ, siCharacter);
														rcl2Dirty[1].left = FloatToLong(ptfCaret.x - siCharacter.fPos);
														rcl2Dirty[1].right = rcl2Dirty[1].left + ucCaretStrength + 2;
														rcl2Dirty[1].top = FloatToLong(ptfCaret.y);
														rcl2Dirty[1].bottom = FloatToLong(ptfCaret.y + szfCharacter.height);

														dxgiPresent.DirtyRectsCount = 2;
														dxgiPresent.pDirtyRects = rcl2Dirty;
														OnRender(true);
														ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
														dxgiPresent.DirtyRectsCount = 1;
														dxgiPresent.pDirtyRects = &rclDirty;
													}
												}

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam) SelectText_Up(ptfCaret_old, ulCharacterPos_old);
												else if(cSelect) DeSelect();
											}
											ThreadSafe_End();
											break;
		case VK_DOWN		: ThreadSafe_Begin();
											if(lLine < (long)vliText->Number() - 1){
												siCharacter.ucMask = SBI_PAGE | SBI_POS; GetScrollBar(SB_HORZ, siCharacter);
												if(!cSelect){
													fCharacterPos_old = siCharacter.fPos;
													ptfCaret_old = ptfCaret;
													ulCharacterPos_old = ulCharacterPos;

													pvLine = vliText->Element(++lLine);
													ulCharacterPos = 0;
													if(ptfCaret.x > 0.0f && _CurrentLine->Length()){
														do{ GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint); }
														while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _CurrentLine->Length());
														ptfCaret.x = szfTextPoint.width;
													}
													else ptfCaret.x = 0.0f;

													GetScrollBar(SB_HORZ, siCharacter);
													if(ptfCaret.x < siCharacter.fPos){
														siCharacter.fPos = ptfCaret.x;
														siCharacter.ucMask = SBI_POS;
														SetScrollBar(SB_HORZ, siCharacter);
													}
													else if(ptfCaret.x - siCharacter.fPos >= siCharacter.fPage){
														siCharacter.fPos = ptfCaret.x - siCharacter.fPage + (float)ucCaretStrength;
														if(siCharacter.fPos < 0.0f) siCharacter.fPos = 0.0f;
														siCharacter.ucMask = SBI_POS;
														SetScrollBar(SB_HORZ, siCharacter);
													}

													siLine.ucMask = SBI_PAGE | SBI_POS;
													GetScrollBar(SB_VERT, siLine);
													if(ptfCaret.y + szfCharacter.height * 2 >= siLine.fPage){
														siLine.fPos += szfCharacter.height;
														SetScrollBar(SB_VERT, siLine);

														rclDirty.left = rclDirty.top = 0;
														rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
														OnRender(true);
														ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
													}
													else{
														rcl2Dirty[0].left = FloatToLong(ptfCaret_old.x - fCharacterPos_old);
														rcl2Dirty[0].right = rcl2Dirty[0].left + ucCaretStrength + 2;
														rcl2Dirty[0].top = FloatToLong(ptfCaret_old.y);
														rcl2Dirty[0].bottom = FloatToLong(ptfCaret_old.y + szfCharacter.height);

														ptfCaret.y += szfCharacter.height;
														siCharacter.ucMask = SBI_POS;
														GetScrollBar(SB_HORZ, siCharacter);
														rcl2Dirty[1].left = FloatToLong(ptfCaret.x - siCharacter.fPos);
														rcl2Dirty[1].right = rcl2Dirty[1].left + ucCaretStrength + 2;
														rcl2Dirty[1].top = FloatToLong(ptfCaret.y);
														rcl2Dirty[1].bottom = FloatToLong(ptfCaret.y + szfCharacter.height);

														dxgiPresent.DirtyRectsCount = 2;
														dxgiPresent.pDirtyRects = rcl2Dirty;
														OnRender(true);
														ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
														dxgiPresent.DirtyRectsCount = 1;
														dxgiPresent.pDirtyRects = &rclDirty;
													}
												}

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam) SelectText_Down(ptfCaret_old, ulCharacterPos_old);
												else if(cSelect) DeSelect();
											}
											ThreadSafe_End();
											break;
		case VK_DELETE	: ThreadSafe_Begin();
											if(!ucCharacterSpecification){ ThreadSafe_End(); break; }
											if(ulCharacterPos == _CurrentLine->Length() && lLine == vliText->Number() - 1){ ThreadSafe_End(); break; }
											if(cSelect) Select_Delete();
											else{
												siCharacter.ucMask = SBI_MAX | SBI_PAGE | SBI_POS;
												GetScrollBar(SB_HORZ, siCharacter);
												GetTextPoint(_CurrentLine->c_Str(), ulCharacterPos + 1, szfTextPoint);
												if(szfTextPoint.width - siCharacter.fPos >= siCharacter.fPage){ ThreadSafe_End(); break; }

												pvLine = vliText->IteratorToBegin();
												for(lLines = 0; lLines <= lLine; lLines++) vliText->NextElement(pvLine, pvLineTemp);
												if(ulCharacterPos == ((COStringA*)vliText->Element(pvLineTemp))->Length()){
													*((COStringA*)vliText->Element(pvLineTemp)) += *((COStringA*)vliText->Element(pvLine));
													VMFreiV((COStringA*)vliText->Element(pvLine));
													vliText->DeleteElement(pvLine, pvLineTemp, false);
													pvLine = vliText->Element(pvLineTemp);

													siLine.ucMask = SBI_POS | SBI_MAX | SBI_PAGE;
													GetScrollBar(SB_VERT, siLine);
													siLine.fMax = (float)vliText->Number() * szfCharacter.height;
													if(siLine.fPos + siLine.fPage > siLine.fMax){
														siLine.fPos = siLine.fMax - siLine.fPage;
														if(siLine.fPos < 0.0f) siLine.fPos = 0.0f;
													}
													SetScrollBar(SB_VERT, siLine);

													GetTextPoint(_CurrentLine->c_Str(), _CurrentLine->Length(), szfTextPoint);
													if(siCharacter.fMax < szfTextPoint.width){ siCharacter.fMax = szfTextPoint.width; SetScrollBar(SB_HORZ, siCharacter); }

													rclDirty.left = 0; rclDirty.right = FloatToLong(siCharacter.fPage);
													rclDirty.top = FloatToLong(ptfCaret.y); rclDirty.bottom = FloatToLong(siLine.fPage);
													OnRender(true);
													ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
												}
												else{
													pvLine = vliText->Element(pvLineTemp);
													GetTextPoint(_CurrentLine->c_Str(), _CurrentLine->Length(), szfTextPoint);

													siCharacter.ucMask = SBI_MAX | SBI_PAGE | SBI_POS;
													GetScrollBar(SB_HORZ, siCharacter);
													if(siCharacter.fMax < szfTextPoint.width){
														siCharacter.fMax = szfTextPoint.width;
														SetScrollBar(SB_HORZ, siCharacter);
													}
													_CurrentLine->Delete(ulCharacterPos, 1);

													if(szfTextPoint.width - siCharacter.fPos < siCharacter.fPage) rclDirty.right = FloatToLong(szfTextPoint.width - siCharacter.fPos);
                          else rclDirty.right = FloatToLong(siCharacter.fPage);
                          rclDirty.left = FloatToLong(ptfCaret.x - siCharacter.fPos);

													rclDirty.top = FloatToLong(ptfCaret.y); rclDirty.bottom = FloatToLong(ptfCaret.y + szfCharacter.height);
													OnRender(true);
													ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
												}
											}
											ThreadSafe_End();
											break;
		case VK_PRIOR		: SendMessage(hWndElement, WM_VSCROLL, SB_PAGEUP, NULL); break;
		case VK_NEXT		: SendMessage(hWndElement, WM_VSCROLL, SB_PAGEDOWN, NULL); break;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::WM_Char(_In_ WPARAM wParam)
{
	VMBLOCK vbCharacter = nullptr; ULONG ulTab; D2D_SIZE_F szfTextPoint; long lLines; void* pvLineTemp = nullptr; COStringA* vasLine;
	STScrollInfo siCharacter, siLine{}; siCharacter.ucMask = SBI_ALL; siLine.ucMask = SBI_POS | SBI_PAGE | SBI_MAX; GetScrollBar(SB_HORZ, siCharacter);
	switch(wParam){
		case VK_ESCAPE		: ThreadSafe_Begin();
												if(pfnWM_Char_Escape) pfnWM_Char_Escape(this);
												ThreadSafe_End();
												break;
		case VK_TAB				: ThreadSafe_Begin();
												if(!ucCharacterSpecification){ ThreadSafe_End(); break; }
												ulTab = 0;
												do{ SendMessage(hWndElement, WM_CHAR, ' ', NULL); }
												while(++ulTab < 4);
												ThreadSafe_End();
												break;
		case VK_BACK			: ThreadSafe_Begin();
												if(!ucCharacterSpecification){ ThreadSafe_End(); break; }
												if(cSelect){ Select_Delete(); ThreadSafe_End(); break; }
												else if(ulCharacterPos){
													_CurrentLine->SubString(vbCharacter, ulCharacterPos, ulCharacterPos);
													GetTextPoint(vbCharacter, 1, szfTextPoint); VMFrei(vbCharacter);
													if(ptfCaret.x < szfTextPoint.width){ ThreadSafe_End(); break; }

													pvLine = vliText->IteratorToBegin();
													for(lLines = 0; lLines <= lLine; lLines++) vliText->NextElement(pvLine, pvLineTemp);
													GetTextPoint(((COStringA*)vliText->Element(pvLineTemp))->c_Str(), ((COStringA*)vliText->Element(pvLineTemp))->Length(), szfTextPoint);
													pvLine = vliText->Element(pvLineTemp);
													_CurrentLine->Delete(--ulCharacterPos, 1);
													if(siCharacter.fMax == szfTextPoint.width){
														pvLineTemp = vliText->IteratorToBegin();
														while(pvLineTemp){
															GetTextPoint(((COStringA*)vliText->Element(pvLineTemp))->c_Str(), ((COStringA*)vliText->Element(pvLineTemp))->Length(), szfTextPoint);
															if(szfTextPoint.width == siCharacter.fMax) break;
															vliText->NextElement(pvLineTemp);
														}

														GetTextPoint(_CurrentLine->c_Str(), _CurrentLine->Length(), szfTextPoint);

														if(!pvLineTemp){ siCharacter.fMax = szfTextPoint.width; SetScrollBar(SB_HORZ, siCharacter); }
													}

													GetTextPoint(_CurrentLine->c_Str(), ulCharacterPos, szfTextPoint);

													rclDirty.top = FloatToLong(ptfCaret.y); rclDirty.bottom = FloatToLong(ptfCaret.y + szfCharacter.height);
													rclDirty.right = FloatToLong(siCharacter.fPage);
													ptfCaret.x = szfTextPoint.width - siCharacter.fPos;
													rclDirty.left = FloatToLong(ptfCaret.x);
													OnRender(true);
													ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
												}
												else{
													GetScrollBar(SB_VERT, siLine);
													if(!lLine && !siLine.fPos){ ThreadSafe_End(); break; }
													pvLine = vliText->IteratorToBegin();
													for(lLines = 0; lLines < lLine; lLines++) vliText->NextElement(pvLine, pvLineTemp);

													GetTextPoint(((COStringA*)vliText->Element(pvLineTemp))->c_Str(), ((COStringA*)vliText->Element(pvLineTemp))->Length(), szfTextPoint);
													ulCharacterPos = ((COStringA*)vliText->Element(pvLineTemp))->Length();
													*((COStringA*)vliText->Element(pvLineTemp)) += *((COStringA*)vliText->Element(pvLine));
													VMFreiV((COStringA*)vliText->Element(pvLine));
													vliText->DeleteElement(pvLine, pvLineTemp, false);
													pvLine = vliText->Element(pvLineTemp);
													lLine--;
                          siLine.fMax -= szfCharacter.height;
                          SetScrollBar(SB_VERT, siLine);
													ptfCaret.y -= siCharacter.szfCharacter.height;

													if(szfTextPoint.width - siCharacter.fPos > siCharacter.fPage){
														siCharacter.fPos = szfTextPoint.width - siCharacter.fPage;

														GetTextPoint(_CurrentLine->c_Str(), _CurrentLine->Length(), szfTextPoint);
                            if(szfTextPoint.width > siCharacter.fMax) siCharacter.fMax = szfTextPoint.width;
														SetScrollBar(SB_HORZ, siCharacter);
														ptfCaret.x = siCharacter.fPage - (float)ucCaretStrength + siCharacter.fPos;

														rclDirty.top = rclDirty.left = 0;
														rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
														OnRender(true);
														ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
													}
													else{
														ptfCaret.x = szfTextPoint.width;
														GetTextPoint(_CurrentLine->c_Str(), _CurrentLine->Length(), szfTextPoint);
                            if(szfTextPoint.width > siCharacter.fMax){ siCharacter.fMax = szfTextPoint.width; SetScrollBar(SB_HORZ, siCharacter); }

														rclDirty.top = FloatToLong(ptfCaret.y); rclDirty.left = 0;
														rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
														OnRender(true);
														ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
													}
												}
												ThreadSafe_End();
												break;
		case VK_RETURN		: ThreadSafe_Begin();
												if(pfnWM_Char_ShiftReturn && GetKeyState(VK_SHIFT) & SHIFTED) pfnWM_Char_ShiftReturn(this);
												else if(!ucCharacterSpecification){ ThreadSafe_End(); break; }
												else{
													if(cSelect) Select_Delete();
													vasLine = COStringAV(vmMemory);
													pvLine = vliText->IteratorToBegin();
													if(pvLine){
														for(lLines = 0; lLines <= lLine; lLines++) vliText->NextElement(pvLine, pvLineTemp);
														if(ulCharacterPos != ((COStringA*)vliText->Element(pvLineTemp))->Length()){
															((COStringA*)vliText->Element(pvLineTemp))->SubString(vbCharacter, ulCharacterPos + 1, ((COStringA*)vliText->Element(pvLineTemp))->Length());
															*vasLine = vbCharacter; VMFrei(vbCharacter);
															((COStringA*)vliText->Element(pvLineTemp))->Delete(ulCharacterPos, ((COStringA*)vliText->Element(pvLineTemp))->Length() - ulCharacterPos);
														}
													}
													if(!pvLine) pvLine = vliText->ToEnd(vasLine);
													else vliText->Insert(pvLine, pvLineTemp, vasLine);
													pvLine = vasLine;

													lLine++; ulCharacterPos = 0;

													siCharacter.ucMask = SBI_MAX | SBI_PAGE | SBI_POS;
													GetScrollBar(SB_HORZ, siCharacter);
													siCharacter.fMax = 0.0f;
													pvLineTemp = vliText->IteratorToBegin();
													while(pvLineTemp){
														GetTextPoint(((COStringA*)vliText->Element(pvLineTemp))->c_Str(), ((COStringA*)vliText->Element(pvLineTemp))->Length(), szfTextPoint);
														if(szfTextPoint.width > siCharacter.fMax) siCharacter.fMax = szfTextPoint.width;
														vliText->NextElement(pvLineTemp);
													}
													siCharacter.fPos = 0.0f;
													SetScrollBar(SB_HORZ, siCharacter);

													siLine.ucMask = SBI_POS | SBI_PAGE | SBI_MAX;
													GetScrollBar(SB_VERT, siLine);
													siLine.fMax = (float)vliText->Number() * szfCharacter.height;
													if((float)(lLine + 1) * szfCharacter.height - siLine.fPos > siLine.fPage){
														siLine.fPos = (float)(lLine + 1) * szfCharacter.height - siLine.fPage;
														if(siLine.fPos + siLine.fPage > siLine.fMax) siLine.fPos = siLine.fMax - siLine.fPage;
														if(siLine.fPos < 0.0f) siLine.fPos = 0.0f;
													}
													SetScrollBar(SB_VERT, siLine);

													ptfCaret.x = 0.0f;
													ptfCaret.y = (float)lLine * szfCharacter.height - siLine.fPos;

													siCharacter.ucMask = SBI_PAGE; GetScrollBar(SB_HORZ, siCharacter);
													siLine.ucMask = SBI_PAGE; GetScrollBar(SB_VERT, siLine);
													rclDirty.left = rclDirty.top = 0;
													rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
													OnRender(true);
													ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
												}
												ThreadSafe_End();
												break;
		default					  : ThreadSafe_Begin();
												if(CharacterCheck(wParam)){
													if(cSelect){ Select_Delete(); ThreadSafe_End(); break; }
													(ulCharacterPos == _CurrentLine->Length() ? *_CurrentLine += (char*)&wParam : _CurrentLine->Insert((char*)&wParam, ulCharacterPos));

													ulCharacterPos++;

													siCharacter.ucMask = SBI_MAX | SBI_PAGE | SBI_POS;
													GetScrollBar(SB_HORZ, siCharacter);
													GetTextPoint(_CurrentLine->c_Str(), _CurrentLine->Length(), szfTextPoint);
													if(siCharacter.fMax < szfTextPoint.width){
														siCharacter.fMax = szfTextPoint.width;
														SetScrollBar(SB_HORZ, siCharacter);
													}

													GetTextPoint(_CurrentLine->c_Str(), ulCharacterPos, szfTextPoint);
													if(szfTextPoint.width - siCharacter.fPos >= siCharacter.fPage){
														siCharacter.fPos = szfTextPoint.width - siCharacter.fPage + (float)ucCaretStrength;
														if(siCharacter.fPos + siCharacter.fPage > siCharacter.fMax) siCharacter.fPos = siCharacter.fMax - siCharacter.fPage;
														if(siCharacter.fPos < 0.0f) siCharacter.fPos = 0.0f;
														SetScrollBar(SB_HORZ, siCharacter);
														rclDirty.top = 0;
														ptfCaret.x = siCharacter.fPage - (float)ucCaretStrength + siCharacter.fPos;
													}
													else{
														rclDirty.top = FloatToLong(ptfCaret.y);
														ptfCaret.x = szfTextPoint.width - siCharacter.fPos;
													}

													siLine.ucMask = SBI_PAGE; GetScrollBar(SB_VERT, siLine);
													rclDirty.left = 0; rclDirty.right = FloatToLong(siCharacter.fPage);
													rclDirty.bottom = FloatToLong(siLine.fPage);
													OnRender(true);
													ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
												}
												ThreadSafe_End();
												break;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::WM_LButtonUp(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	ReleaseCapture();
	ThreadSafe_Begin();
	if(pfnWM_LButtonUp) pfnWM_LButtonUp(this, wParam, lParam);
	else PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), WM_LBUTTONUP);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::WM_LButtonDown(_In_ LPARAM lParam)
{
	SetCapture(hWndElement);
	ThreadSafe_Begin();
	if(hWndElement != GetFocus()) SetFocus(hWndElement);
	if(cSelect) DeSelect();

	RECT rcl2Dirty[2];
	STScrollInfo siLine{}; siLine.ucMask = SBI_POS | SBI_MAX;	GetScrollBar(SB_VERT, siLine);

	rcl2Dirty[0].left = FloatToLong(ptfCaret.x); rcl2Dirty[0].right = rcl2Dirty[0].left + ucCaretStrength;
	rcl2Dirty[0].top = FloatToLong(ptfCaret.y);	rcl2Dirty[0].bottom = rcl2Dirty[0].top + FloatToLong(szfCharacter.height);

	ptfCaret.x = 0.0f; ulCharacterPos = 0;
	if(!vliText->Number()){
		COStringA* vasZeile = COStringAV(vmMemory);
		vliText->ToEnd(vasZeile);
	}
	LButtonDown(lParam);

	rcl2Dirty[1].left = FloatToLong(ptfCaret.x); rcl2Dirty[1].right = rcl2Dirty[1].left + ucCaretStrength;
  rcl2Dirty[1].top = FloatToLong(ptfCaret.y); rcl2Dirty[1].bottom = rcl2Dirty[1].top + FloatToLong(szfCharacter.height);

  dxgiPresent.DirtyRectsCount = 2;
  dxgiPresent.pDirtyRects = rcl2Dirty;
	OnRender(true);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
  dxgiPresent.DirtyRectsCount = 1;
  dxgiPresent.pDirtyRects = &rclDirty;

	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::DirectX::COEditBox::Content(_Out_ COStringA* vasInhaltA)
{
	ThreadSafe_Begin();
	*vasInhaltA = NULL;
	void* pvLinen = vliText->IteratorToBegin();
	while(pvLinen){
		*vasInhaltA += *((COStringA*)vliText->Element(pvLinen));
		*vasInhaltA += "\n";
		vliText->NextElement(pvLinen);
	}
	if(vasInhaltA->Length() == 1 && (*vasInhaltA)[0] == 0x0A) *vasInhaltA = NULL;
	ThreadSafe_End();
	return vasInhaltA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::Select_Delete(void)
{
	STScrollInfo siCharacter{}; siCharacter.ucMask = SBI_PAGE | SBI_MAX; STScrollInfo siLine{}; siLine.ucMask = SBI_PAGE | SBI_MAX;
	if(stSelect_top.lLine == stSelect_bottom.lLine){
		((COStringA*)vliText->Element(stSelect_top.lLine))->Delete(stSelect_top.ulCharacterPos, stSelect_bottom.ulCharacterPos - stSelect_top.ulCharacterPos);

		sbHorizontal->GetScrollInfo(siCharacter);
    rclDirty.left = FloatToLong(stSelect_top.fPosition); rclDirty.right = FloatToLong(siCharacter.fPage);
    cSelect = 0; stSelect_bottom = stSelect_top; SetEvent(heCaret);
		OnRender(true);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	else{
		void* pvIterator = vliText->IteratorToBegin(); void* pvDelete = nullptr; void* pvIterator_firstline;
		long lSelectLine = stSelect_top.lLine; VMBLOCK vbLastLine; D2D_SIZE_F szfTextPoint; float fWidestLine = 0;
		for(long lDeleteLine = 0; lDeleteLine < stSelect_top.lLine; lDeleteLine++) vliText->NextElement(pvIterator, pvDelete);
    _DeleteLine->Delete(stSelect_top.ulCharacterPos, _DeleteLine->Length() - stSelect_top.ulCharacterPos);
    pvIterator_firstline = pvIterator;
		
		if(++lSelectLine < stSelect_bottom.lLine){
			do{
				vliText->NextElement(pvIterator, pvDelete);
				VMFrei(vliText->Element(pvIterator));
				vliText->DeleteElement(pvIterator, pvDelete, false);
			}
			while(pvIterator && ++lSelectLine < stSelect_bottom.lLine);
    }
		else vliText->NextElement(pvIterator, pvDelete);

    _DeleteLine->SubString(vbLastLine, stSelect_bottom.ulCharacterPos + 1, _DeleteLine->Length());
		*((COStringA*)vliText->Element(pvIterator_firstline)) += vbLastLine; VMFrei(vbLastLine);
		VMFrei(vliText->Element(pvIterator));
		vliText->DeleteElement(pvIterator, pvDelete, false);

		pvIterator = vliText->IteratorToBegin();
		do{
			GetTextPoint(_DeleteLine->c_Str(), _DeleteLine->Length(), szfTextPoint);
			if(fWidestLine < szfTextPoint.width) fWidestLine = szfTextPoint.width;
      vliText->NextElement(pvIterator); 
		}
		while(pvIterator);

		sbHorizontal->GetScrollInfo(siCharacter); sbVertical->GetScrollInfo(siLine);
		siCharacter.fMax = fWidestLine;
		sbHorizontal->SetScrollInfo(siCharacter);
		siLine.fMax = vliText->Number() * szfCharacter.height;
		sbVertical->SetScrollInfo(siLine);
		ChangeSizeVisibleScrollBars();

		rclDirty.left = 0; rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
		cSelect = 0; stSelect_bottom = stSelect_top; SetEvent(heCaret);
		OnRender(true);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
