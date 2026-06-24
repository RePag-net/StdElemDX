/******************************************************************************
MIT License

Copyright(c) 2025 Ren� Pagel

Filename: OEditBox.cpp
For more information see https://github.com/RePag-net/StdElem

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
#define _Line ((COStringA*)pvLine)
#define _ZeileAktuell ((COStringA*)vliText->Element(pvLineAktuell))
#define _ZeileErste ((COStringA*)vliText->Element(pvLineErste))
#define _EditBox ((RePag::DirectX::COEditBox*)pvParam)
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
		case WM_HSCROLL       : ((COEditBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_HScroll(wParam);
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
		//case WM_PAINT         : ((COEditBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Paint();
		//												return NULL;
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

  STScrollInfo siCharacter; siCharacter.ucMask = SBI_POS;
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
	pvLine = nullptr;
	lSelectLine = lLine = 0;
	pfnWM_Char_ShiftReturn = nullptr;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::OnRender(_In_ bool bCaret)
{
	COTextBox::OnRender(bCaret, lLine, lSelectLine);
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
			STScrollInfo siLine, siCharacter; siLine.ucMask = siCharacter.ucMask = SBI_POS;
			GetScrollBar(SB_VERT, siLine); GetScrollBar(SB_HORZ, siCharacter);
			if(!ptfCaret.x && !ptfCaret.y && !siLine.fPos && !siCharacter.fPos){ ulCharacterPos = lLine = 0; pvLine = vliText->Element(lLine); }
		}
    rcfSelect = D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);
		if(!htCaret) CreateTimerQueueTimer(&htCaret, TimerQueue(), (WAITORTIMERCALLBACK)Timer_Caret_EditBox, this, 0, 500, 0);
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::WM_HScroll(_In_ WPARAM wParam)
{
	ThreadSafe_Begin();
	UNREFERENCED_PARAMETER(wParam);

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
	STScrollInfo siLine; siLine.ucMask = SBI_PAGE | SBI_POS | SBI_MAX | SBI_CHARACTER_HEIGHT; GetScrollBar(SB_VERT, siLine);
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
												if(bScrollChanged){
													ptfCaret.y += szfCharacter.height;
													if(cSelect){ rcfSelect.top += szfCharacter.height; rcfSelect.bottom += szfCharacter.height; }
												}
												break;
		case SB_LINEDOWN	: if(!lParam && siLine.fPos + siLine.fPage < siLine.fMax){
													siLine.fPos += szfCharacter.height;
													if(siLine.fPos + siLine.fPage > siLine.fMax) siLine.fPos = siLine.fMax - siLine.fPage;
													if(siLine.fPos < 0.0f) siLine.fPos = 0.0f;
													siLine.ucMask = SBI_POS;
													SetScrollBar(SB_VERT, siLine);
													bScrollChanged = true;
												}
												if(bScrollChanged){
													ptfCaret.y -= szfCharacter.height;
													if(cSelect){ rcfSelect.top -= szfCharacter.height; rcfSelect.bottom -= szfCharacter.height; }
												}
												break;
		case SB_PAGEUP		: if(!lParam && siLine.fPos > 0.0f){
													siLine.fPos -= fScrollPage;
													if(siLine.fPos < 0.0f) siLine.fPos = 0.0f;
													siLine.ucMask = SBI_POS;
													SetScrollBar(SB_VERT, siLine);
												}
												if(cSelect){
													rcfSelect.top = (float)lLine * szfCharacter.height - siLine.fPos;
													rcfSelect.bottom = rcfSelect.top + szfCharacter.height; }
												break;
		case SB_PAGEDOWN	: if(!lParam && siLine.fPos + siLine.fPage < siLine.fMax){
													siLine.fPos += fScrollPage;
													if(siLine.fPos + siLine.fPage > siLine.fMax) siLine.fPos = siLine.fMax - siLine.fPage;
													if(siLine.fPos < 0.0f) siLine.fPos = 0.0f;
													siLine.ucMask = SBI_POS;
													SetScrollBar(SB_VERT, siLine);
												}
												if(cSelect){
													rcfSelect.top = (float)lLine * szfCharacter.height - siLine.fPos;
													rcfSelect.bottom = rcfSelect.top + szfCharacter.height; }
												break;
	}

	rclDirty.left = rclDirty.top = 0;
	rclDirty.right = lWidth; rclDirty.bottom = lHeight;
	OnRender(true);
	ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::WM_KeyDown(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
  D2D_SIZE_F szfTextPoint; STScrollInfo siLine, siCharacter; siCharacter.ucMask = SBI_POS; siLine.ucMask = SBI_PAGE; RECT rcl2Dirty[2];
	void* pvLineTemp = nullptr; float fCharacterPos_old; D2D_POINT_2F ptfCaret_old; long lLines;
	unsigned long ulCharacterPos_old; long lLine_old;

	auto MoveCaretToLine = [&](long lLineA){
		pvLine = vliText->Element(lLineA);
		ulCharacterPos = 0;
		if(ptfCaret.x > 0.0f && _Line->Length()){
			do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
			while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _Line->Length());
			ptfCaret.x = szfTextPoint.width;
		}
		else ptfCaret.x = 0.0f;
	};

	auto UpdateSelection = [&](){
		D2D_SIZE_F szfSelectPoint; COStringA* pSelectLine = (COStringA*)vliText->Element(lSelectLine);
		GetTextPoint(pSelectLine->c_Str(), ulSelectPos, szfSelectPoint);
		float fSelectY = ptfCaret.y + (float)(lSelectLine - lLine) * szfCharacter.height;

		if(lLine == lSelectLine && ulCharacterPos == ulSelectPos){
			cSelect = 0; SetEvent(heCaret);
			rcfSelect = D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);
			rclDirty.left = rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
			OnRender(true);
			return;
		}

		if(lLine < lSelectLine) cSelect = -2;
		else if(lLine > lSelectLine) cSelect = 2;
		else if(ulCharacterPos < ulSelectPos) cSelect = -1;
		else cSelect = 1;

		if(ptfCaret.x < szfSelectPoint.width){ rcfSelect.left = ptfCaret.x; rcfSelect.right = szfSelectPoint.width; }
		else{ rcfSelect.left = szfSelectPoint.width; rcfSelect.right = ptfCaret.x; }
		if(ptfCaret.y < fSelectY){ rcfSelect.top = ptfCaret.y; rcfSelect.bottom = fSelectY + szfCharacter.height; }
		else{ rcfSelect.top = fSelectY; rcfSelect.bottom = ptfCaret.y + szfCharacter.height; }

		ResetEvent(heCaret);
		rclDirty.left = rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
		OnRender(false);
	};

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
											GetTextPoint(_Line->c_Str(), _Line->Length(), szfTextPoint);
											ulCharacterPos = _Line->Length();

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
													GetTextPoint(_Line->c_Str(), --ulCharacterPos, szfTextPoint);
													GetScrollBar(SB_HORZ, siCharacter);
													rcl2Dirty[1].left = FloatToLong(ptfCaret.x - siCharacter.fPos) - 1;
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

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
													if(cSelect < 0){
														rclDirty.top = FloatToLong(rcfSelect.top); rclDirty.bottom = FloatToLong(rcfSelect.bottom);
														GetTextPoint(_Line->c_Str(), --ulCharacterPos, szfTextPoint);
														if(cSelect == -2){ // Up
															rcfSelect.left = ptfCaret.x = szfTextPoint.width;
															GetTextPoint(_Line->c_Str(), ulSelectPos, szfTextPoint);
															rcfSelect.right = szfTextPoint.width;
															rclDirty.right = FloatToLong(rcfSelect.right);
															rclDirty.left = FloatToLong(rcfSelect.left);
															OnRender(false);
															cSelect = -1;
														}
														else{	// LEFT
															rclDirty.right = FloatToLong(ptfCaret.x);
															ptfCaret.x = szfTextPoint.width;
															rclDirty.left = FloatToLong(ptfCaret.x);
															if(ulSelectPos > ulCharacterPos){	rcfSelect.left = ptfCaret.x; OnRender(false); }
															else if(ulSelectPos < ulCharacterPos){ rcfSelect.right = ptfCaret.x; OnRender(false);	}
															else if(lLine == lSelectLine){ cSelect = 0; SetEvent(heCaret); OnRender(true); }
														}
													}
													else if(cSelect > 0){
                            rclDirty.top = FloatToLong(rcfSelect.top); rclDirty.bottom = FloatToLong(rcfSelect.bottom);
                            rclDirty.right = FloatToLong(ptfCaret.x);
														GetTextPoint(_Line->c_Str(), --ulCharacterPos, szfTextPoint);
														ptfCaret.x = szfTextPoint.width;
														rclDirty.left = FloatToLong(ptfCaret.x);

														if(cSelect == 2){ // DOWN
															UpdateSelection();
														}
														else{ // RIGHT
															if(ulSelectPos < ulCharacterPos){ rcfSelect.right = ptfCaret.x; OnRender(false); cSelect = -1; }
															else if(ulSelectPos > ulCharacterPos){ rcfSelect.left = ptfCaret.x; OnRender(false); cSelect = -1; }
															else if(lLine == lSelectLine){ cSelect = 0; SetEvent(heCaret); OnRender(true); }
														}
													}
													else{ cSelect = -1; // LEFT
														ulSelectPos = ulCharacterPos + 1;
														GetTextPoint(_Line->c_Str(), ulSelectPos, szfTextPoint);
														rcfSelect.right = szfTextPoint.width; rcfSelect.left = ptfCaret.x;
														rclDirty.right = FloatToLong(rcfSelect.right);
														rcfSelect.top = ptfCaret.y; rcfSelect.bottom = rcfSelect.top + szfCharacter.height;
														ResetEvent(heCaret);

														rclDirty.top = FloatToLong(rcfSelect.top); rclDirty.bottom = FloatToLong(rcfSelect.bottom);
														rclDirty.left =	FloatToLong(rcfSelect.left);
														OnRender(false);
													}
													rclDirty.left = rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
													ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
												}
												else if(cSelect) DeSelect();
											}
											ThreadSafe_End();
											break;
		case VK_RIGHT		: ThreadSafe_Begin();
											if(ulCharacterPos < _Line->Length()){
												if(!cSelect){
													GetScrollBar(SB_HORZ, siCharacter);
													rcl2Dirty[0].left = FloatToLong(ptfCaret.x - siCharacter.fPos) - 1;
													GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint);
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

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
													if(cSelect > 0){
														rclDirty.top = FloatToLong(rcfSelect.top); rclDirty.bottom = FloatToLong(rcfSelect.bottom);
														rclDirty.left = FloatToLong(ptfCaret.x);
														GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint);
														ptfCaret.x = szfTextPoint.width;
														rclDirty.right = FloatToLong(ptfCaret.x);

														if(cSelect == 2){ // DOWN
															UpdateSelection();
														}
														else{ // RIGHT
															if(ulSelectPos < ulCharacterPos){	rcfSelect.right = ptfCaret.x; OnRender(false); }
															else if(ulSelectPos > ulCharacterPos){ rcfSelect.left = ptfCaret.x; OnRender(false); }
															else if(lLine == lSelectLine){ cSelect = 0; SetEvent(heCaret); OnRender(true); }
														}
													}
													else if(cSelect < 0){
														rclDirty.top = FloatToLong(rcfSelect.top); rclDirty.bottom = FloatToLong(rcfSelect.bottom);
														rclDirty.left = FloatToLong(ptfCaret.x);
														GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint);
														ptfCaret.x = szfTextPoint.width;
														rclDirty.right = FloatToLong(ptfCaret.x);

														if(cSelect == -2){ // Up
															UpdateSelection();
														}
														else{ // LEFT
															if(ulSelectPos < ulCharacterPos){	rcfSelect.right = ptfCaret.x;	OnRender(false); cSelect = 1;	}
															else if(ulSelectPos > ulCharacterPos){ rcfSelect.left = ptfCaret.x;	OnRender(false); cSelect = 1;	}
															else if(lLine == lSelectLine){ cSelect = 0; SetEvent(heCaret); OnRender(true); }
														}
													}
													else{ cSelect = 1; // RIGHT
														ulSelectPos = ulCharacterPos - 1;
														GetTextPoint(_Line->c_Str(), ulSelectPos, szfTextPoint);
														rcfSelect.left = szfTextPoint.width; rcfSelect.right = ptfCaret.x;
														rcfSelect.top = ptfCaret.y; rcfSelect.bottom = rcfSelect.top + szfCharacter.height;
														ResetEvent(heCaret);

														rclDirty.top = FloatToLong(rcfSelect.top); rclDirty.bottom = FloatToLong(rcfSelect.bottom);
														rclDirty.right = FloatToLong(rcfSelect.right); rclDirty.left = FloatToLong(rcfSelect.left);
														OnRender(false);
													}
													rclDirty.left = rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
													ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
												}
												else if(cSelect) DeSelect();
											}
											ThreadSafe_End();
											break;
		case VK_UP			: ThreadSafe_Begin();
											if(lLine){
												if(!cSelect){
													siCharacter.ucMask = SBI_POS;	GetScrollBar(SB_HORZ, siCharacter);
													fCharacterPos_old = siCharacter.fPos;
													ptfCaret_old = ptfCaret;
													lLine_old = lLine; ulCharacterPos_old = ulCharacterPos;

													MoveCaretToLine(--lLine);

													siCharacter.ucMask = SBI_PAGE | SBI_POS;
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

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
													if(cSelect){
														MoveCaretToLine(--lLine);
														if(!ptfCaret.y){
															siLine.ucMask = SBI_POS;
															GetScrollBar(SB_VERT, siLine);
															if(siLine.fPos > 0.0f){
																siLine.fPos -= szfCharacter.height;
																if(siLine.fPos < 0.0f) siLine.fPos = 0.0f;
																SetScrollBar(SB_VERT, siLine);
															}
														}
														else ptfCaret.y -= szfCharacter.height;
													}
													else{ lSelectLine = lLine_old; ulSelectPos = ulCharacterPos_old; }
													UpdateSelection();
													ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
												}
												else if(cSelect) DeSelect();
											}
											ThreadSafe_End();
											break;
		case VK_DOWN		: ThreadSafe_Begin();
											if(lLine < (long)vliText->Number() - 1){
												if(!cSelect){
													siCharacter.ucMask = SBI_POS;	GetScrollBar(SB_HORZ, siCharacter);
													fCharacterPos_old = siCharacter.fPos;
													ptfCaret_old = ptfCaret;
													lLine_old = lLine; ulCharacterPos_old = ulCharacterPos;

													MoveCaretToLine(++lLine);

													siCharacter.ucMask = SBI_PAGE | SBI_POS;
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
													if(ptfCaret.y + szfCharacter.height >= siLine.fPage){
														siLine.fPos += szfCharacter.height;
														SetScrollBar(SB_VERT, siLine);

														rclDirty.left = rclDirty.top = 0;
														rclDirty.right = lWidth; rclDirty.bottom = lHeight;
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

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
													if(cSelect){
														MoveCaretToLine(++lLine);
														siLine.ucMask = SBI_PAGE | SBI_POS;
														GetScrollBar(SB_VERT, siLine);
														if(ptfCaret.y + szfCharacter.height >= siLine.fPage){
															siLine.fPos += szfCharacter.height;
															SetScrollBar(SB_VERT, siLine);
														}
														else ptfCaret.y += szfCharacter.height;
													}
													else{ lSelectLine = lLine_old; ulSelectPos = ulCharacterPos_old; }
													UpdateSelection();
                          ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
												}
												else if(cSelect) DeSelect();
											}
											ThreadSafe_End();
											break;
		case VK_DELETE	: ThreadSafe_Begin();
											if(!ucZeichenVorgabe){ ThreadSafe_End(); break; }
											if(ulCharacterPos == _Line->Length() && lLine == vliText->Number() - 1){ ThreadSafe_End(); break; }
											if(cSelect) Select_Delete();
											else{
												siCharacter.ucMask = SBI_MAX | SBI_PAGE | SBI_POS;
												GetScrollBar(SB_HORZ, siCharacter);
												GetTextPoint(_Line->c_Str(), ulCharacterPos + 1, szfTextPoint);
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

													GetTextPoint(_Line->c_Str(), _Line->Length(), szfTextPoint);
													if(siCharacter.fMax < szfTextPoint.width){ siCharacter.fMax = szfTextPoint.width; SetScrollBar(SB_HORZ, siCharacter); }

													rclDirty.left = 0; rclDirty.right = FloatToLong(siCharacter.fPage);
													rclDirty.top = FloatToLong(ptfCaret.y); rclDirty.bottom = FloatToLong(siLine.fPage);
													OnRender(true);
													ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
												}
												else{
													pvLine = vliText->Element(pvLineTemp);
													GetTextPoint(_Line->c_Str(), _Line->Length(), szfTextPoint);

													siCharacter.ucMask = SBI_MAX | SBI_PAGE | SBI_POS;
													GetScrollBar(SB_HORZ, siCharacter);
													if(siCharacter.fMax < szfTextPoint.width){
														siCharacter.fMax = szfTextPoint.width;
														SetScrollBar(SB_HORZ, siCharacter);
													}
													_Line->Delete(ulCharacterPos, 1);

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
	STScrollInfo siCharacter, siLine; siCharacter.ucMask = SBI_ALL; siLine.ucMask = SBI_POS | SBI_PAGE | SBI_MAX; GetScrollBar(SB_HORZ, siCharacter);
	switch(wParam){
		case VK_ESCAPE		: ThreadSafe_Begin();
												if(pfnWM_Char_Escape) pfnWM_Char_Escape(this);
												ThreadSafe_End();
												break;
		case VK_TAB				: ThreadSafe_Begin();
												if(!ucZeichenVorgabe){ ThreadSafe_End(); break; }
												ulTab = 0;
												do{ SendMessage(hWndElement, WM_CHAR, ' ', NULL); }
												while(++ulTab < 4);
												ThreadSafe_End();
												break;
		case VK_BACK			: ThreadSafe_Begin();
												if(!ucZeichenVorgabe){ ThreadSafe_End(); break; }
												if(cSelect){ Select_Delete(); ThreadSafe_End(); break; }
												else if(ulCharacterPos){
													_Line->SubString(vbCharacter, ulCharacterPos, ulCharacterPos);
													GetTextPoint(vbCharacter, 1, szfTextPoint); VMFrei(vbCharacter);
													if(ptfCaret.x < szfTextPoint.width){ ThreadSafe_End(); break; }

													pvLine = vliText->IteratorToBegin();
													for(lLines = 0; lLines <= lLine; lLines++) vliText->NextElement(pvLine, pvLineTemp);
													GetTextPoint(((COStringA*)vliText->Element(pvLineTemp))->c_Str(), ((COStringA*)vliText->Element(pvLineTemp))->Length(), szfTextPoint);
													pvLine = vliText->Element(pvLineTemp);
													_Line->Delete(--ulCharacterPos, 1);
													if(siCharacter.fMax == szfTextPoint.width){
														pvLineTemp = vliText->IteratorToBegin();
														while(pvLineTemp){
															GetTextPoint(((COStringA*)vliText->Element(pvLineTemp))->c_Str(), ((COStringA*)vliText->Element(pvLineTemp))->Length(), szfTextPoint);
															if(szfTextPoint.width == siCharacter.fMax) break;
															vliText->NextElement(pvLineTemp);
														}

														GetTextPoint(_Line->c_Str(), _Line->Length(), szfTextPoint);

														if(!pvLineTemp){ siCharacter.fMax = szfTextPoint.width; SetScrollBar(SB_HORZ, siCharacter); }
													}

													GetTextPoint(_Line->c_Str(), ulCharacterPos, szfTextPoint);

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

														GetTextPoint(_Line->c_Str(), _Line->Length(), szfTextPoint);
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
														GetTextPoint(_Line->c_Str(), _Line->Length(), szfTextPoint);
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
												else if(!ucZeichenVorgabe){ ThreadSafe_End(); break; }
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
												if(ZeichenVorgabe(wParam)){
													if(cSelect){ Select_Delete(); ThreadSafe_End(); break; }
													(ulCharacterPos == _Line->Length() ? *_Line += (char*)&wParam : _Line->Insert((char*)&wParam, ulCharacterPos));

													ulCharacterPos++;

													siCharacter.ucMask = SBI_MAX | SBI_PAGE | SBI_POS;
													GetScrollBar(SB_HORZ, siCharacter);
													GetTextPoint(_Line->c_Str(), _Line->Length(), szfTextPoint);
													if(siCharacter.fMax < szfTextPoint.width){
														siCharacter.fMax = szfTextPoint.width;
														SetScrollBar(SB_HORZ, siCharacter);
													}

													GetTextPoint(_Line->c_Str(), ulCharacterPos, szfTextPoint);
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
bool __vectorcall RePag::DirectX::COEditBox::WM_Command(_In_ WPARAM wParam)
{
	HGLOBAL hGlobal; char* pcClipboard;

	auto CopySelection = [&]() -> bool{
		if(!cSelect) return false;

		long lStartLine = lLine, lEndLine = lSelectLine;
		ULONG ulStartPos = ulCharacterPos, ulEndPos = ulSelectPos;
		if(lStartLine > lEndLine || lStartLine == lEndLine && ulStartPos > ulEndPos){
			long lLineTemp = lStartLine; lStartLine = lEndLine; lEndLine = lLineTemp;
			ULONG ulPosTemp = ulStartPos; ulStartPos = ulEndPos; ulEndPos = ulPosTemp;
		}

		COStringA* vasClipboard = COStringAV(vmMemory);
		VMBLOCK vbCharacter = nullptr;
		if(lStartLine == lEndLine){
			COStringA* pLine = (COStringA*)vliText->Element(lStartLine);
			if(ulEndPos > ulStartPos){
				pLine->SubString(vbCharacter, ulStartPos + 1, ulEndPos);
				*vasClipboard = vbCharacter; VMFrei(vbCharacter);
			}
		}
		else{
			COStringA* pLine = (COStringA*)vliText->Element(lStartLine);
			if(ulStartPos < pLine->Length()){
				pLine->SubString(vbCharacter, ulStartPos + 1, pLine->Length());
				*vasClipboard += vbCharacter; VMFrei(vbCharacter);
			}
			*vasClipboard += "\n";

			for(long lCurrentLine = lStartLine + 1; lCurrentLine < lEndLine; lCurrentLine++){
				*vasClipboard += *((COStringA*)vliText->Element(lCurrentLine));
				*vasClipboard += "\n";
			}

			pLine = (COStringA*)vliText->Element(lEndLine);
			if(ulEndPos){
				pLine->SubString(vbCharacter, 1, ulEndPos);
				*vasClipboard += vbCharacter; VMFrei(vbCharacter);
			}
		}

		if(!OpenClipboard(hWndElement)){ VMFreiV(vasClipboard); return false; }
		EmptyClipboard();
		hGlobal = GlobalAlloc(GMEM_MOVEABLE, vasClipboard->Length() + 1);
		if(!hGlobal){ CloseClipboard(); VMFreiV(vasClipboard); return false; }
		pcClipboard = (char*)GlobalLock(hGlobal);
		if(!pcClipboard){ GlobalFree(hGlobal); CloseClipboard(); VMFreiV(vasClipboard); return false; }
		MemCopy(pcClipboard, vasClipboard->c_Str(), vasClipboard->Length());
		pcClipboard[vasClipboard->Length()] = 0;
		GlobalUnlock(hGlobal);
		if(!SetClipboardData(CF_TEXT, hGlobal)) GlobalFree(hGlobal);
		CloseClipboard(); VMFreiV(vasClipboard);
		return true;
	};

	auto UpdateDirectXState = [&](){
		D2D_SIZE_F szfTextPoint; STScrollInfo siCharacter, siLine;
		siCharacter.ucMask = SBI_ALL; GetScrollBar(SB_HORZ, siCharacter);
		siCharacter.fMax = 0.0f;
		void* pvIterator = vliText->IteratorToBegin();
		while(pvIterator){
			COStringA* pLine = (COStringA*)vliText->Element(pvIterator);
			GetTextPoint(pLine->c_Str(), pLine->Length(), szfTextPoint);
			if(szfTextPoint.width > siCharacter.fMax) siCharacter.fMax = szfTextPoint.width;
			vliText->NextElement(pvIterator);
		}

		siLine.ucMask = SBI_ALL; GetScrollBar(SB_VERT, siLine);
		siLine.fMax = (float)vliText->Number() * szfCharacter.height;
		if((float)lLine * szfCharacter.height < siLine.fPos) siLine.fPos = (float)lLine * szfCharacter.height;
		else if((float)(lLine + 1) * szfCharacter.height - siLine.fPos > siLine.fPage)
			siLine.fPos = (float)(lLine + 1) * szfCharacter.height - siLine.fPage;
		if(siLine.fPos + siLine.fPage > siLine.fMax) siLine.fPos = siLine.fMax - siLine.fPage;
		if(siLine.fPos < 0.0f) siLine.fPos = 0.0f;

		GetTextPoint(_Line->c_Str(), ulCharacterPos, szfTextPoint);
		if(szfTextPoint.width < siCharacter.fPos) siCharacter.fPos = szfTextPoint.width;
		else if(szfTextPoint.width - siCharacter.fPos >= siCharacter.fPage)
			siCharacter.fPos = szfTextPoint.width - siCharacter.fPage + (float)ucCaretStrength;
		if(siCharacter.fPos + siCharacter.fPage > siCharacter.fMax) siCharacter.fPos = siCharacter.fMax - siCharacter.fPage;
		if(siCharacter.fPos < 0.0f) siCharacter.fPos = 0.0f;

		SetScrollBar(SB_HORZ, siCharacter); SetScrollBar(SB_VERT, siLine);
		ptfCaret.x = szfTextPoint.width - siCharacter.fPos;
		ptfCaret.y = (float)lLine * szfCharacter.height - siLine.fPos;
		rclDirty.left = rclDirty.top = 0;
		rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
		SetEvent(heCaret); OnRender(true);
		ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
	};

	switch(LOWORD(wParam)){
		case IDM_KOPIEREN     : ThreadSafe_Begin();
														if(!CopySelection()){ ThreadSafe_End(); return true; }
														ThreadSafe_End(); return false;
		case IDM_AUSSCHNEIDEN : ThreadSafe_Begin();
														if(!ucZeichenVorgabe){ ThreadSafe_End(); return false; }
														if(!CopySelection()){ ThreadSafe_End(); return true; }
														Select_Delete();
														ThreadSafe_End(); return false;
		case IDM_EINFUGEN     : ThreadSafe_Begin();
														if(!IsClipboardFormatAvailable(CF_TEXT) || !ucZeichenVorgabe){ ThreadSafe_End(); return false; }
														if(cSelect) Select_Delete();
														if(!vliText->Number()){
															COStringA* vasLine = COStringAV(vmMemory);
															vliText->ToEnd(vasLine); pvLine = vasLine; lLine = 0; ulCharacterPos = 0;
														}
														if(!OpenClipboard(hWndElement)){ ThreadSafe_End(); return false; }
														hGlobal = GetClipboardData(CF_TEXT);
														pcClipboard = hGlobal ? (char*)GlobalLock(hGlobal) : nullptr;
														if(!pcClipboard){ CloseClipboard(); ThreadSafe_End(); return false; }

														{
															COStringA* vasClipboard = COStringAV(vmMemory);
															ULONG ulInsertedLines = 0, ulLastLineCharacters = 0;
															char acCharacter[2] = { 0, 0 };
															for(ULONG ulClipboardPos = 0; pcClipboard[ulClipboardPos]; ulClipboardPos++){
																if(pcClipboard[ulClipboardPos] == '\r') continue;
																if(pcClipboard[ulClipboardPos] == '\n'){
																	*vasClipboard += "\n"; ulInsertedLines++; ulLastLineCharacters = 0;
																}
																else{
																	acCharacter[0] = pcClipboard[ulClipboardPos];
																	*vasClipboard += acCharacter; ulLastLineCharacters++;
																}
															}
															GlobalUnlock(hGlobal); CloseClipboard();

															if(vasClipboard->Length()){
																COStringA* vasText = COStringAV(vmMemory);
																ULONG ulInsertPos = ulCharacterPos;
																for(long lCurrentLine = 0; lCurrentLine < (long)vliText->Number(); lCurrentLine++){
																	COStringA* pLine = (COStringA*)vliText->Element(lCurrentLine);
																	if(lCurrentLine < lLine) ulInsertPos += pLine->Length() + 1;
																	*vasText += *pLine;
																	if(lCurrentLine + 1 < (long)vliText->Number()) *vasText += "\n";
																}

																long lPasteLine = lLine; ULONG ulPastePos = ulCharacterPos;
																if(ulInsertPos == vasText->Length()) *vasText += *vasClipboard;
																else vasText->Insert(vasClipboard, ulInsertPos);
																Text(vasText->c_Str());
																lLine = lPasteLine + (long)ulInsertedLines;
																ulInsertedLines ? ulCharacterPos = ulLastLineCharacters : ulCharacterPos = ulPastePos + vasClipboard->Length();
																pvLine = vliText->Element(lLine);
																lSelectLine = lLine; ulSelectPos = ulCharacterPos; cSelect = 0;
																rcfSelect = D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);
																VMFreiV(vasText); UpdateDirectXState();
															}
															VMFreiV(vasClipboard);
														}
														ThreadSafe_End(); return false;
		default               : return true;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::WM_ContexMenu(_In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	if(!IsWindowEnabled(hWndElement)){
		EnableMenuItem(hMenu, IDM_CUT, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_COPY, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_PASTE, MF_BYCOMMAND | MF_GRAYED);
	}
	else{
		EnableMenuItem(hMenu, IDM_CUT, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(hMenu, IDM_COPY, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(hMenu, IDM_PASTE, MF_BYCOMMAND | MF_ENABLED);
	}

	POINT ptPosition;
	ptPosition.x = LOWORD(lParam); ptPosition.y = HIWORD(lParam);
	if(ptPosition.x == USHRT_MAX && ptPosition.y == USHRT_MAX) ClientToScreen(GetParent(hWndElement), &Position(ptPosition));
	TrackPopupMenuEx(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, ptPosition.x, ptPosition.y, hWndElement, nullptr);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::WM_MouseMove(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	if(hWndElement == GetFocus() && wParam == MK_LBUTTON){
		ThreadSafe_Begin();
		if((short)GET_X_LPARAM(lParam) < ptfCaret.x - szfCharacter.width) SendMessage(hWndElement, WM_KEYDOWN, VK_LEFT, NULL);
		else if((short)GET_X_LPARAM(lParam) > ptfCaret.x + szfCharacter.width) SendMessage(hWndElement, WM_KEYDOWN, VK_RIGHT, NULL);

		if((short)GET_Y_LPARAM(lParam) < ptfCaret.y - szfCharacter.height) SendMessage(hWndElement, WM_KEYDOWN, VK_UP, NULL);
		else if((short)GET_Y_LPARAM(lParam) > ptfCaret.y + szfCharacter.height) SendMessage(hWndElement, WM_KEYDOWN, VK_DOWN, NULL);
		ThreadSafe_End();
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::WM_LButtonDown(_In_ LPARAM lParam)
{
	SetCapture(hWndElement);
	ThreadSafe_Begin();
	if(hWndElement != GetFocus()) SetFocus(hWndElement);
	if(cSelect) DeSelect();

	D2D_SIZE_F szfTextPoint; RECT rcl2Dirty[2];
	STScrollInfo siLine; siLine.ucMask = SBI_POS | SBI_MAX;
	GetScrollBar(SB_VERT, siLine);

	rcl2Dirty[0].right = FloatToLong(ptfCaret.x) + 2; rcl2Dirty[0].left = rcl2Dirty[0].right - ucCaretStrength - 4;
	ptfCaret.y ? rcl2Dirty[0].top = FloatToLong(ptfCaret.y) - 1 : rcl2Dirty[0].top = 0;
	rcl2Dirty[0].bottom = rcl2Dirty[0].top + FloatToLong(szfCharacter.height) + 2;

	ptfCaret.x = 0.0f; ulCharacterPos = 0;
	if(!vliText->Number()){
		COStringA* vasZeile = COStringAV(vmMemory);
		vliText->ToEnd(vasZeile);
	}

	lLine = (long)(((float)GET_Y_LPARAM(lParam) + siLine.fPos) / szfCharacter.height);
	if(lLine < 0) lLine = 0;
	else if(lLine >= (long)vliText->Number()) lLine = (long)vliText->Number() - 1;
  lSelectLine = lLine;

	ptfCaret.y = (float)lLine * szfCharacter.height - siLine.fPos;
	pvLine = vliText->Element(lLine);

	STScrollInfo siCharacter; siCharacter.ucMask = SBI_POS;
	GetScrollBar(SB_HORZ, siCharacter);
	if(_Line->Length()){
		do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
		while(szfTextPoint.width - siCharacter.fPos < (float)GET_X_LPARAM(lParam) && ulCharacterPos < _Line->Length());
		ptfCaret.x = szfTextPoint.width;
	}
	else ptfCaret.x = 0.0f;
  ulSelectPos = ulCharacterPos;

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
	D2D_SIZE_F szfTextPoint; STScrollInfo siCharacter, siLine;
	long lStartLine = lLine, lEndLine = lSelectLine; ULONG ulStartPos = ulCharacterPos, ulEndPos = ulSelectPos;

	if(lStartLine > lEndLine || lStartLine == lEndLine && ulStartPos > ulEndPos){
		long lLineTemp = lStartLine; lStartLine = lEndLine; lEndLine = lLineTemp;
		ULONG ulPosTemp = ulStartPos; ulStartPos = ulEndPos; ulEndPos = ulPosTemp;
	}

	if(lStartLine == lEndLine){
		pvLine = vliText->Element(lStartLine);
		if(ulEndPos > ulStartPos) _Line->Delete(ulStartPos, ulEndPos - ulStartPos);
	}
	else{
		COStringA* pStartLine = (COStringA*)vliText->Element(lStartLine);
		COStringA* pEndLine = (COStringA*)vliText->Element(lEndLine);

		VMBLOCK vbRest = nullptr;
		if(ulEndPos < pEndLine->Length()) pEndLine->SubString(vbRest, ulEndPos + 1, pEndLine->Length());
		pStartLine->Delete(ulStartPos, pStartLine->Length() - ulStartPos);
		if(vbRest){ *pStartLine += vbRest; VMFrei(vbRest); }

		void* pvDelete = vliText->IteratorToBegin(); void* pvPrevious = nullptr;
		for(long lLines = 0; lLines <= lStartLine; lLines++) vliText->NextElement(pvDelete, pvPrevious);
		for(long lLines = lStartLine + 1; pvDelete && lLines <= lEndLine; lLines++){
			VMFreiV((COStringA*)vliText->Element(pvDelete));
			vliText->DeleteElement(pvDelete, pvPrevious, false);
			vliText->NextElement(pvDelete);
		}
	}

	lLine = lSelectLine = lStartLine;
	ulCharacterPos = ulSelectPos = ulStartPos;
	pvLine = vliText->Element(lLine);
	cSelect = 0;
	rcfSelect = D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);

	siCharacter.ucMask = SBI_ALL; GetScrollBar(SB_HORZ, siCharacter);
	siCharacter.fMax = 0.0f;
	void* pvIterator = vliText->IteratorToBegin();
	while(pvIterator){
		COStringA* pLine = (COStringA*)vliText->Element(pvIterator);
		GetTextPoint(pLine->c_Str(), pLine->Length(), szfTextPoint);
		if(szfTextPoint.width > siCharacter.fMax) siCharacter.fMax = szfTextPoint.width;
		vliText->NextElement(pvIterator);
	}
	if(siCharacter.fPos + siCharacter.fPage > siCharacter.fMax) siCharacter.fPos = siCharacter.fMax - siCharacter.fPage;
	if(siCharacter.fPos < 0.0f) siCharacter.fPos = 0.0f;

	siLine.ucMask = SBI_ALL; GetScrollBar(SB_VERT, siLine);
	siLine.fMax = (float)vliText->Number() * szfCharacter.height;
	if(siLine.fPos + siLine.fPage > siLine.fMax) siLine.fPos = siLine.fMax - siLine.fPage;
	if(siLine.fPos < 0.0f) siLine.fPos = 0.0f;

	GetTextPoint(_Line->c_Str(), ulCharacterPos, szfTextPoint);
	ptfCaret.x = szfTextPoint.width;
	ptfCaret.y = (float)lLine * szfCharacter.height - siLine.fPos;
	if(ptfCaret.x < siCharacter.fPos) siCharacter.fPos = ptfCaret.x;
	else if(ptfCaret.x - siCharacter.fPos >= siCharacter.fPage){
		siCharacter.fPos = ptfCaret.x - siCharacter.fPage + (float)ucCaretStrength;
		if(siCharacter.fPos + siCharacter.fPage > siCharacter.fMax) siCharacter.fPos = siCharacter.fMax - siCharacter.fPage;
		if(siCharacter.fPos < 0.0f) siCharacter.fPos = 0.0f;
	}
	SetScrollBar(SB_HORZ, siCharacter);
	SetScrollBar(SB_VERT, siLine);

	rclDirty.left = rclDirty.top = 0;
	rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
	SetEvent(heCaret);
	OnRender(true);
	ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
}
//---------------------------------------------------------------------------------------------------------------------------------------
