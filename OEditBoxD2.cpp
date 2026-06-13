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
	lLine = 0;
	pfnWM_Char_ShiftReturn = nullptr;
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
	STScrollInfo siCharacter; siCharacter.ucMask = SIF_PAGE | SIF_POS; GetScrollBar(SB_HORZ, siCharacter); 
	switch(LOWORD(wParam)){
		case SB_LINELEFT	: if(cSelect){ rcfSelect.left -= szfCharacter.width; rcfSelect.right -= szfCharacter.width; }
												break;
		case SB_LINERIGHT	: if(cSelect){ rcfSelect.left += szfCharacter.width; rcfSelect.right += szfCharacter.width; }
												break;
		case SB_PAGELEFT	:
		case SB_PAGERIGHT	:	ptfCaret.x = (float)lLine * szfCharacter.width - siCharacter.fPos;
												if(cSelect){
													rcfSelect.left = (float)lLine * szfCharacter.width - siCharacter.fPos;
													rcfSelect.right = rcfSelect.left + szfCharacter.width;
												}
												break;
	}

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
  VMBLOCK vbZeichen = nullptr; D2D_SIZE_F szfTextPoint; STScrollInfo siLine, siCharacter; siCharacter.ucMask = SBI_POS; siLine.ucMask = SBI_PAGE; RECT rcl2Dirty[2];
	void* pvLineTemp = nullptr; float fCharacterPos_old; D2D_POINT_2F ptfCaret_old; long lLines;
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

											if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
												//if(cSelect < 0) rcfSelect.left = ptfCaret.x;
												//else if(cSelect > 0){
												//	if(cSelect == 2 || ulSelectPos < ulCharacterPos) rcfSelect.right = ptfCaret.x;
												//	else{ cSelect = 0; ShowCaret(hWndElement);}
												//}
												//else{ cSelect = -1;
												//	ulSelectPos = ulCharacterPos + 1;
												//	rcfSelect.top = ptfCaret.y; rcfSelect.bottom = rcfSelect.top + szfCharacter.height;
 											//		rcfSelect.left = ptfCaret.x; rcfSelect.right = rcZeichnen.right;
												//	HideCaret(hWndElement);
												//}
												//rcZeichnen.left = ptfCaret.x;
												//rcZeichnen.top = ptfCaret.y; rcZeichnen.bottom = rcZeichnen.top + szfCharacter.height;
												//UpdateFenster(&rcZeichnen, true, false);
											}
											else if(cSelect) DeSelect();

											//while(ptfCaret.x < 0) SendMessage(hWndElement, WM_HSCROLL, SB_LINELEFT, NULL);
                      //if(ptfCaret.x < 0) SetScrollBar(SB_HORZ, siCharacter);
											}
											ThreadSafe_End();
											break;
		case VK_RIGHT		: ThreadSafe_Begin();
											if(ulCharacterPos < _Line->Length()){
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

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
													//if(cSelect > 0) rcfSelect.right = ptfCaret.x;
													//else if(cSelect < 0){
													//	if(cSelect == -2 || ulSelectPos > ulCharacterPos) rcfSelect.left = ptfCaret.x;
													//	else cSelect = 0;
													//}
													//else{ cSelect = 1;
													//	ulSelectPos = ulCharacterPos - 1;
													//	rcfSelect.top = ptfCaret.y; rcfSelect.bottom = rcfSelect.top + szfCharacter.height;
													//	rcfSelect.left = rcl2Dirty[0].left; rcfSelect.right = ptfCaret.x;
													//}
												}
												else if(cSelect) DeSelect();
											}
											ThreadSafe_End();
											break;	
		case VK_UP			: ThreadSafe_Begin();
											if(lLine){
												siCharacter.ucMask = SBI_POS;	GetScrollBar(SB_HORZ, siCharacter);
												fCharacterPos_old = siCharacter.fPos;
												ptfCaret_old = ptfCaret;

												pvLine = vliText->Element(--lLine);
												ulCharacterPos = 0;
												if(ptfCaret.x > 0.0f && _Line->Length()){
													do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
													while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _Line->Length());
													ptfCaret.x = szfTextPoint.width;
												}
												else ptfCaret.x = 0.0f;

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

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
													//if(cSelect < 0){
													//	rcSelect.top = ptCaret.y - lZeichen_hohe;
													//	if(cSelect == -1) cSelect--;
													//}
													//else if(cSelect > 0){
													//	if(cSelect == 1){ cSelect = -2; rcSelect.top -= lZeichen_hohe; rcSelect.left = ptCaret.x; }
													//	else if(rcSelect.top < rcSelect.bottom - lZeichen_hohe * 2) rcSelect.bottom -= lZeichen_hohe;
													//	else{ cSelect = 0; ShowCaret(hWndElement); }
													//}
													//else{
													//	cSelect = -2;
													//	ulSelectPos = rcZeichnen.right;
													//	rcSelect.top = ptCaret.y - lZeichen_hohe; rcSelect.bottom = ptCaret.y + lZeichen_hohe;
													//	rcSelect.left = ptCaret.x; rcSelect.right = rcZeichnen.left;
													//	HideCaret(hWndElement);
													//}
													//rcZeichnen.left = 0; rcZeichnen.right = lRand_rechts;
													//rcZeichnen.top = ptCaret.y - lZeichen_hohe; rcZeichnen.bottom = ptCaret.y + lZeichen_hohe;
													//UpdateFenster(&rcZeichnen, true, false);
												}
												else if(cSelect) DeSelect();
											}
											ThreadSafe_End();
											break;
		case VK_DOWN		: ThreadSafe_Begin();
											if(lLine < (long)vliText->Number() - 1){
												siCharacter.ucMask = SBI_POS;	GetScrollBar(SB_HORZ, siCharacter);
												fCharacterPos_old = siCharacter.fPos;
												ptfCaret_old = ptfCaret;

												pvLine = vliText->Element(++lLine);
												ulCharacterPos = 0;
												if(ptfCaret.x > 0.0f && _Line->Length()){
													do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
													while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _Line->Length());
													ptfCaret.x = szfTextPoint.width;
												}
												else ptfCaret.x = 0.0f;

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

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
												//	if(cSelect > 0){
												//		rcSelect.bottom = ptCaret.y + lZeichen_hohe * 2;
												//		if(cSelect == 1) cSelect++;
												//	}
												//	else if(cSelect < 0){
												//		if(cSelect == -1){ cSelect = 2; rcSelect.bottom += lZeichen_hohe; rcSelect.right = ptCaret.x; }
												//		else if(rcSelect.top < rcSelect.bottom - lZeichen_hohe * 2) rcSelect.top += lZeichen_hohe;
												//		else{ cSelect = 0; ShowCaret(hWndElement); }
												//	}
												//	else{
												//		cSelect = 2;
												//		ulSelectPos = rcZeichnen.right;
												//		rcSelect.top = ptCaret.y; rcSelect.bottom = ptCaret.y + lZeichen_hohe * 2;
												//		rcSelect.left = rcZeichnen.left; rcSelect.right = ptCaret.x;
												//		HideCaret(hWndElement);
												//	}
												//	rcZeichnen.left = 0; rcZeichnen.right = lRand_rechts;
												//	rcZeichnen.top = ptCaret.y; rcZeichnen.bottom = ptCaret.y + lZeichen_hohe * 2;
												//	UpdateFenster(&rcZeichnen, true, false);
												}
												else if(cSelect) DeSelect();
											}
											ThreadSafe_End();
											break;
		case VK_DELETE	: ThreadSafe_Begin();
											if(!ucZeichenVorgabe){ ThreadSafe_End(); break; }
											if(ulCharacterPos == _Line->Length() && lLine == vliText->Number() - 1){ ThreadSafe_End(); break; }
											//if(cSelect) Select_Loschen(hdc);
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
	VMBLOCK vbZeichen = nullptr; ULONG ulTab; D2D_SIZE_F szfTextPoint; long lLines; void* pvLineTemp = nullptr; COStringA* vasZeile;
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
												//if(cSelect){ Select_Loschen(hdc); ThreadSafe_End(); break; }
												else if(ulCharacterPos){
													_Line->SubString(vbZeichen, ulCharacterPos, ulCharacterPos);
													GetTextPoint(vbZeichen, 1, szfTextPoint); VMFrei(vbZeichen);
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
													//if(cSelect) Select_Loschen(hdc);
													vasZeile = COStringAV(vmMemory);
													pvLine = vliText->IteratorToBegin();
													if(pvLine){
														for(lLines = 0; lLines <= lLine; lLines++) vliText->NextElement(pvLine, pvLineTemp);
														if(ulCharacterPos != ((COStringA*)vliText->Element(pvLineTemp))->Length()){
															((COStringA*)vliText->Element(pvLineTemp))->SubString(vbZeichen, ulCharacterPos + 1, ((COStringA*)vliText->Element(pvLineTemp))->Length());
															*vasZeile = vbZeichen; VMFrei(vbZeichen);
															((COStringA*)vliText->Element(pvLineTemp))->Delete(ulCharacterPos, ((COStringA*)vliText->Element(pvLineTemp))->Length() - ulCharacterPos);
														}
													}
													if(!pvLine) pvLine = vliText->ToEnd(vasZeile);
													else vliText->Insert(pvLine, pvLineTemp, vasZeile);
													pvLine = vasZeile;

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
													//if(cSelect){ Select_Loschen(hdc); ThreadSafe_End(); break; }
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
	//HDC hdc; VMBLOCK vbZeichen; HGLOBAL hGlobal; char* pcAblage; SIZE stZeichengrosse; RECT rcZeichnen; void* pvLineAktuell = nullptr; void* pvLineErste = nullptr;
	//SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO); ULONG ulZeichen; long lLinen; COStringA* vasZeile = nullptr;
	switch(LOWORD(wParam)){
	//	case IDM_KOPIEREN     : ThreadSafe_Begin();
	//													*vasInhalt = ""; OpenClipboard(hWndElement); EmptyClipboard(); 
	//													switch(cSelect){
	//														case  1 : _Zeile->SubString(vasInhalt, ulSelectPos + 1, ulCharacterPos);
	//																			ulZeichen = vasInhalt->Length();
	//																			break;
	//														case -1 : _Zeile->SubString(vasInhalt, ulCharacterPos + 1, ulSelectPos);
	//																			ulZeichen = vasInhalt->Length();
	//																			break;
	//														case  2 : rcZeichnen.top = lLine - (rcSelect.bottom - rcSelect.top) / szfCharacter.height + 1;
	//																			rcZeichnen.bottom = rcSelect.top + szfCharacter.height;
	//																			pvLineAktuell = vliText->IteratorToBegin();
	//																			for(lLinen = 0; lLinen < rcZeichnen.top; lLinen++) vliText->NextElement(pvLineAktuell);
	//																			ulZeichen = _ZeileAktuell->SubString(vbZeichen, ulSelectPos + 1, _ZeileAktuell->Length());
	//																			*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//																			vliText->NextElement(pvLineAktuell);
	//																			rcZeichnen.bottom += szfCharacter.height;
	//																			*vasInhalt += "\n"; ulZeichen++;
	//																			while(pvLineAktuell && rcZeichnen.bottom < rcSelect.bottom){
	//																				ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, _ZeileAktuell->Length());
	//																				*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//																				vliText->NextElement(pvLineAktuell);
	//																				rcZeichnen.bottom += szfCharacter.height;
	//																				*vasInhalt += "\n"; ulZeichen++;
	//																			}
	//																			ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, ulCharacterPos);
	//																			*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//																			break;
	//														case -2 : rcZeichnen.bottom = rcSelect.top + szfCharacter.height;
	//																			pvLineAktuell = vliText->IteratorToBegin();
	//																			for(lLinen = 0; lLinen < lLine; lLinen++) vliText->NextElement(pvLineAktuell);
	//																			ulZeichen = _ZeileAktuell->SubString(vbZeichen, ulCharacterPos + 1, _ZeileAktuell->Length());
	//																			*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//																			vliText->NextElement(pvLineAktuell);
	//																			rcZeichnen.bottom += szfCharacter.height;
	//																			*vasInhalt += "\n"; ulZeichen++;
	//																			while(pvLineAktuell && rcZeichnen.bottom < rcSelect.bottom){
	//																				ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, _ZeileAktuell->Length());
	//																				*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//																				vliText->NextElement(pvLineAktuell);
	//																				rcZeichnen.bottom += szfCharacter.height;
	//																				*vasInhalt += "\n"; ulZeichen++;
	//																			}
	//																			ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, ulSelectPos);
	//																			*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//																			break;
	//														default  : CloseClipboard(); ThreadSafe_End(); return true;
	//													}

	//													hGlobal = GlobalAlloc(GMEM_MOVEABLE, ulZeichen + 1); 
	//													pcAblage = (char*)GlobalLock(hGlobal); 
	//													MemCopy(pcAblage, vasInhalt->c_Str(), ulZeichen); 
	//													pcAblage[ulZeichen] = 0;
	//													GlobalUnlock(hGlobal); 

	//									 				SetClipboardData(CF_TEXT, hGlobal); CloseClipboard(); 
	//													ThreadSafe_End(); return false;
	//	case IDM_AUSSCHNEIDEN : ThreadSafe_Begin();
	//													if(!ucZeichenVorgabe){ ThreadSafe_End(); return false;}
	//													OpenClipboard(hWndElement);	EmptyClipboard(); 
	//													hdc = GetDC(hWndElement);	SelectObject(hdc, hFont);	
	//													switch(cSelect){
	//														case  1 : GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
	//																			_Zeile->SubString(vasInhalt, ulSelectPos + 1, ulCharacterPos);
	//																			ulZeichen = vasInhalt->Length();
	//																			_Zeile->Delete(ulSelectPos, ulZeichen);
	//																			ulCharacterPos -= ulZeichen;

	//																			if(stZeichengrosse.cx == lBreitesteZeile) BreitesteZeile(hdc);

	//																			rcZeichnen.top = rcSelect.top; rcZeichnen.bottom = rcSelect.bottom;
	//																			rcZeichnen.left = rcSelect.right; rcZeichnen.right = lRand_rechts;

	//																			if(stZeichengrosse.cx - lTextPos < rcZeichnen.right) rcZeichnen.right = stZeichengrosse.cx - lTextPos;
	//																			GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichen, &stZeichengrosse);
	//																			ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
	//																			rcZeichnen.left = rcSelect.left + rcZeichnen.right - rcZeichnen.left;
	//																			cSelect = 0; ShowCaret(hWndElement);
	//																			UpdateFenster(&rcZeichnen, true, false);
	//																			ptfCaret.x -= stZeichengrosse.cx;
	//																			SetCaretPos(ptfCaret.x, ptfCaret.y);
	//																			break;
	//														case -1 : GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
	//																			_Zeile->SubString(vasInhalt, ulCharacterPos + 1, ulSelectPos);
	//																			ulZeichen = vasInhalt->Length();
	//																			_Zeile->Delete(ulCharacterPos, ulZeichen);

	//																			if(stZeichengrosse.cx == lBreitesteZeile) BreitesteZeile(hdc);

	//																			rcZeichnen.top = rcSelect.top; rcZeichnen.bottom = rcSelect.bottom;
	//																			rcZeichnen.left = rcSelect.right;	rcZeichnen.right = lRand_rechts;

	//																			if(stZeichengrosse.cx - lTextPos < rcZeichnen.right) rcZeichnen.right = stZeichengrosse.cx - lTextPos;
	//																			GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichen, &stZeichengrosse);
	//																			ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
	//																			rcZeichnen.left = rcSelect.left + rcZeichnen.right - rcZeichnen.left;
	//																			cSelect = 0; ShowCaret(hWndElement);
	//																			UpdateFenster(&rcZeichnen, true, false);
	//																			break;
	//														case  2 : rcZeichnen.top = lLine - (rcSelect.bottom - rcSelect.top) / szfCharacter.height + 1;
	//																			rcZeichnen.bottom = rcSelect.top + szfCharacter.height;
	//																			pvLineAktuell = vliText->IteratorToBegin();
	//																			for(lLinen = 0; lLinen < rcZeichnen.top; lLinen++) vliText->NextElement(pvLineAktuell, pvLineErste);
	//																			ulZeichen = _ZeileAktuell->SubString(vbZeichen, ulSelectPos + 1, _ZeileAktuell->Length());
	//																			*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//																			_ZeileAktuell->Delete(ulSelectPos, ulZeichen);
	//																			rcZeichnen.bottom += szfCharacter.height;
	//																			*vasInhalt += "\n"; ulZeichen++;
	//																			vliText->NextElement(pvLineAktuell, pvLineErste);
	//																			while(pvLineAktuell && rcZeichnen.bottom < rcSelect.bottom){
	//																				ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, _ZeileAktuell->Length());
	//																				*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//																				VMFreiV((COStringA*)vliText->Element(pvLineAktuell));
	//																				vliText->DeleteElement(pvLineAktuell, pvLineErste, false);
	//																				vliText->NextElement(pvLineAktuell);
	//																				rcZeichnen.bottom += szfCharacter.height;
	//																				*vasInhalt += "\n"; ulZeichen++;
	//																			}
	//																			ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, ulCharacterPos);
	//																			*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//																			_ZeileAktuell->SubString(vbZeichen, ulCharacterPos + 1, _ZeileAktuell->Length());
	//																			*((COStringA*)vliText->Element(pvLineErste)) += vbZeichen; VMFrei(vbZeichen);
	//																			vliText->DeleteElement(pvLineAktuell, pvLineErste, false);

	//																			pvLine = vliText->Element(pvLineErste);
	//																			lLinen = (rcSelect.bottom - rcSelect.top) / szfCharacter.height - 1;
	//																			lLine -= lLinen;
	//																			ptfCaret.y -= szfCharacter.height * lLinen;
	//																			SetzScrollVert(stScrollInfo);

	//																			GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
	//																			if(stZeichengrosse.cx > lBreitesteZeile){ lBreitesteZeile = stZeichengrosse.cx; SetzScrollHorz(stScrollInfo); }
	//																			else BreitesteZeile(hdc);

	//																			ulCharacterPos = ulSelectPos;
	//																			GetTextExtentPoint32(hdc, _Zeile->c_Str(), ulCharacterPos, &stZeichengrosse);
	//																			ptfCaret.x = stZeichengrosse.cx;
	//																			SetCaretPos(ptfCaret.x, ptfCaret.y);
 //                            
	//																			GetClientRect(hWndElement, &rcZeichnen);
	//																			rcZeichnen.top = rcSelect.top;
	//																			cSelect = 0; ShowCaret(hWndElement);
	//																			UpdateFenster(&rcZeichnen, true, false);
	//																			break;
	//														case -2 : rcZeichnen.bottom = rcSelect.top + szfCharacter.height;
	//																			pvLineAktuell = vliText->IteratorToBegin();
	//																			for(lLinen = 0; lLinen < lLine; lLinen++) vliText->NextElement(pvLineAktuell, pvLineErste);
	//																			ulZeichen = _ZeileAktuell->SubString(vbZeichen, ulCharacterPos + 1, _ZeileAktuell->Length());
	//																			*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//																			_ZeileAktuell->Delete(ulCharacterPos, ulZeichen);
	//																			rcZeichnen.bottom += szfCharacter.height;
	//																			*vasInhalt += "\n"; ulZeichen++;
	//																			vliText->NextElement(pvLineAktuell, pvLineErste);
	//																			while(pvLineAktuell && rcZeichnen.bottom < rcSelect.bottom){
	//																				ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, _ZeileAktuell->Length());
	//																				*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//																				VMFreiV((COStringA*)vliText->Element(pvLineAktuell));
	//																				vliText->DeleteElement(pvLineAktuell, pvLineErste, false);
	//																				vliText->NextElement(pvLineAktuell);
	//																				rcZeichnen.bottom += szfCharacter.height;
	//																				*vasInhalt += "\n"; ulZeichen++;
	//																			}
	//																			ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, ulSelectPos);
	//																			*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//																			_ZeileAktuell->SubString(vbZeichen, ulSelectPos + 1, _ZeileAktuell->Length());
	//																			*((COStringA*)vliText->Element(pvLineErste)) += vbZeichen; VMFrei(vbZeichen);
	//																			vliText->DeleteElement(pvLineAktuell, pvLineErste, false);

	//																			lLinen = (rcSelect.bottom - rcSelect.top) / szfCharacter.height - 1;
	//																			SetzScrollVert(stScrollInfo);

	//																			GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
	//																			if(stZeichengrosse.cx > lBreitesteZeile){ lBreitesteZeile = stZeichengrosse.cx; SetzScrollHorz(stScrollInfo);}
	//																			else BreitesteZeile(hdc);
 //                            
	//																			GetClientRect(hWndElement, &rcZeichnen);
	//																			rcZeichnen.top = rcSelect.top;
	//																			cSelect = 0; ShowCaret(hWndElement);
	//																			UpdateFenster(&rcZeichnen, true, false);
	//																			break;
	//														default  : CloseClipboard(); ReleaseDC(hWndElement, hdc); ThreadSafe_End(); return true;
	//													}
	//													ReleaseDC(hWndElement, hdc);

	//													hGlobal = GlobalAlloc(GMEM_MOVEABLE, ulZeichen + 1); 
	//													pcAblage = (char*)GlobalLock(hGlobal); 
	//													MemCopy(pcAblage, vasInhalt->c_Str(), ulZeichen);
	//													pcAblage[ulZeichen] = 0;
	//													GlobalUnlock(hGlobal); 
	//									 				SetClipboardData(CF_TEXT, hGlobal); CloseClipboard(); 	
	//													ThreadSafe_End(); return false;
	//	case IDM_EINFUGEN     :	ThreadSafe_Begin();
	//													if(!IsClipboardFormatAvailable(CF_TEXT) || !ucZeichenVorgabe){ ThreadSafe_End(); return false;}
	//													if(!vliText->Number()){ COStringA* vasZeile = COStringAV(vmSpeicher); vliText->ToEnd(vasZeile); pvLine = vasZeile; }
	//													hdc = GetDC(hWndElement);	SelectObject(hdc, hFont);
	//													if(cSelect) Select_Loschen(hdc);

	//													OpenClipboard(hWndElement); 
	//													hGlobal = GetClipboardData(CF_TEXT);
	//													*vasInhalt = (char*)GlobalLock(hGlobal);
	//													rcZeichnen.bottom = vasInhalt->Length();
	//													GlobalUnlock(hGlobal); 
	//													CloseClipboard(); 

	//													rcZeichnen.top = 0; ulZeichen = 1; 
	//													vliText->IteratorToBegin();
	//													do{ ulZeichen++;
	//														if((*vasInhalt)[++rcZeichnen.top] == 0x0A){
	//															if(!pvLineAktuell){
	//																pvLineAktuell = vliText->IteratorToBegin();
	//																for(lLinen = 0; lLinen <= lLine; lLinen++) vliText->NextElement(pvLineAktuell, pvLineErste);

	//																vasZeile = COStringAV(vmSpeicher);
	//																if(ulCharacterPos != ((COStringA*)vliText->Element(pvLineErste))->Length()){
	//																	_ZeileErste->SubString(vbZeichen, ulCharacterPos + 1, _ZeileErste->Length());
	//																	*vasZeile = vbZeichen; VMFrei(vbZeichen);
	//																	_ZeileErste->Delete(ulCharacterPos, _ZeileErste->Length() - ulCharacterPos);
	//																}
	//																if(!pvLineAktuell) pvLineAktuell = vliText->ToEnd(vasZeile);
	//																else pvLineAktuell = vliText->Insert(pvLineAktuell, pvLineErste, vasZeile);

	//																vasInhalt->SubString(vbZeichen, rcZeichnen.top - ulZeichen + 2, rcZeichnen.top);
	//																_Zeile->Insert(vbZeichen, ulCharacterPos); VMFrei(vbZeichen);
	//																GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
	//															}
	//															else{
	//																vasZeile = COStringAV(vmSpeicher);
	//																vasInhalt->SubString(vasZeile, rcZeichnen.top - ulZeichen + 2, rcZeichnen.top);
	//																vliText->Insert(pvLineAktuell, pvLineErste, vasZeile);
	//																vliText->NextElement(pvLineErste);
	//																GetTextExtentPoint32(hdc, vasZeile->c_Str(), vasZeile->Length(), &stZeichengrosse);
	//															}		
	//															if(lBreitesteZeile < stZeichengrosse.cx){ lBreitesteZeile = stZeichengrosse.cx; SetzScrollHorz(stScrollInfo); }
	//															lLine++;	ptfCaret.y += szfCharacter.height;
	//															ulZeichen = 0;
	//														}
	//													}
	//													while(rcZeichnen.top < rcZeichnen.bottom);

	//													if(pvLineAktuell){
	//														vasInhalt->SubString(vbZeichen, rcZeichnen.top - ulZeichen + 2, rcZeichnen.top);
	//														_ZeileAktuell->Insert(vbZeichen, 0); VMFrei(vbZeichen);
	//														pvLine = vliText->Element(pvLineAktuell);

	//														GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
	//														if(lBreitesteZeile < stZeichengrosse.cx){ lBreitesteZeile = stZeichengrosse.cx; SetzScrollHorz(stScrollInfo); }
	//														SetzScrollVert(stScrollInfo);

	//														ulCharacterPos = --ulZeichen;
	//														GetTextExtentPoint32(hdc, _Zeile->c_Str(), ulCharacterPos, &stZeichengrosse);
	//														ptfCaret.x = stZeichengrosse.cx;
	//														SetCaretPos(ptfCaret.x, ptfCaret.y);

	//														GetClientRect(hWndElement, &rcZeichnen);
	//														rcZeichnen.top = ptfCaret.y - rcSelect.bottom + rcSelect.top + szfCharacter.height * 2;
	//														ScrollWindow(hWndElement, 0, rcSelect.bottom - rcSelect.top - szfCharacter.height, &rcZeichnen, nullptr);
	//														rcZeichnen.top -= szfCharacter.height; rcZeichnen.bottom = rcZeichnen.top + rcSelect.bottom - rcSelect.top;
	//														UpdateFenster(&rcZeichnen, true, false);
	//													}
	//													else{ 
	//														_Zeile->Insert(vasInhalt, ulCharacterPos);
	//														ulCharacterPos += --ulZeichen;	

	//														GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
	//														if(lBreitesteZeile < stZeichengrosse.cx){ lBreitesteZeile = stZeichengrosse.cx; SetzScrollHorz(stScrollInfo); }

	//														GetClientRect(hWndElement, &rcZeichnen);
	//														rcZeichnen.top = ptfCaret.y; rcZeichnen.bottom = rcZeichnen.top + szfCharacter.height;
	//														GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichen, &stZeichengrosse);			
	//														if(ptfCaret.x + stZeichengrosse.cx <= rcZeichnen.right){
	//															rcZeichnen.left = ptfCaret.x; rcZeichnen.right -= stZeichengrosse.cx;
	//															ScrollWindow(hWndElement, stZeichengrosse.cx, 0, &rcZeichnen, nullptr);
	//															rcZeichnen.right = ptfCaret.x + stZeichengrosse.cx;
	//															UpdateFenster(&rcZeichnen, true, false);
	//															ptfCaret.x = rcZeichnen.right;
	//															SetCaretPos(ptfCaret.x, ptfCaret.y);
	//														}
	//														else{ ptfCaret.x += stZeichengrosse.cx;
	//															while(ptfCaret.x > rcZeichnen.right) SendMessage(hWndElement, WM_HSCROLL, SB_LINERIGHT, NULL);
	//														}
	//													}
	//													ReleaseDC(hWndElement, hdc);
	//													ThreadSafe_End(); return false;
		default               : return true;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::WM_ContexMenu(_In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	if(!IsWindowEnabled(hWndElement)){
		EnableMenuItem(hMenu, IDM_AUSSCHNEIDEN, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_KOPIEREN, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_EINFUGEN, MF_BYCOMMAND | MF_GRAYED);
	}
	else{
		EnableMenuItem(hMenu, IDM_AUSSCHNEIDEN, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(hMenu, IDM_KOPIEREN, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(hMenu, IDM_EINFUGEN, MF_BYCOMMAND | MF_ENABLED);
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
		//if((short)LOWORD(lParam) < ptfCaret.x - lZeichen_mittel) SendMessage(hWndElement, WM_KEYDOWN, VK_LEFT, NULL);
		//else if((short)LOWORD(lParam) > ptfCaret.x + lZeichen_mittel) SendMessage(hWndElement, WM_KEYDOWN, VK_RIGHT, NULL);

		//if((short)HIWORD(lParam) < ptfCaret.y - szfCharacter.height) SendMessage(hWndElement, WM_KEYDOWN, VK_UP, NULL);
		//else if((short)HIWORD(lParam) > ptfCaret.y + szfCharacter.height) SendMessage(hWndElement, WM_KEYDOWN, VK_DOWN, NULL);
		ThreadSafe_End();
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::WM_LButtonDown(_In_ LPARAM lParam)
{
	SetCapture(hWndElement);
	ThreadSafe_Begin();
	if(hWndElement != GetFocus()) SetFocus(hWndElement);
	//if(cSelect) DeSelect();

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
void __vectorcall RePag::DirectX::COEditBox::Select_Loschen(_In_ HDC hdc)
{
	//VMBLOCK vbZeichen; SIZE stZeichengrosse; RECT rcZeichnen; void* pvLineAktuell = nullptr; void* pvLineErste = nullptr;
	//SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO); ULONG ulZeichen; long lLinen; COStringA* vasZeile = nullptr;
	//*vasInhalt = NULL;

	//switch(cSelect){
	//	case  1 : GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
	//						_Zeile->SubString(vasInhalt, ulSelectPos + 1, ulCharacterPos);
	//						ulZeichen = vasInhalt->Length();
	//						_Zeile->Delete(ulSelectPos, ulZeichen);
	//						ulCharacterPos -= ulZeichen;

	//						if(stZeichengrosse.cx == lBreitesteZeile) BreitesteZeile(hdc);

	//						rcZeichnen.top = rcSelect.top; rcZeichnen.bottom = rcSelect.bottom;
	//						rcZeichnen.left = rcSelect.right; rcZeichnen.right = lRand_rechts;

	//						if(stZeichengrosse.cx - lTextPos < rcZeichnen.right) rcZeichnen.right = stZeichengrosse.cx - lTextPos;
	//						GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichen, &stZeichengrosse);
	//						ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
	//						rcZeichnen.left = rcSelect.left + rcZeichnen.right - rcZeichnen.left;
	//						cSelect = 0; ShowCaret(hWndElement);
	//						UpdateFenster(&rcZeichnen, true, false);
	//						ptfCaret.x -= stZeichengrosse.cx;
	//						SetCaretPos(ptfCaret.x, ptfCaret.y);
	//						break;
	//	case -1 : GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
	//						_Zeile->SubString(vasInhalt, ulCharacterPos + 1, ulSelectPos);
	//						ulZeichen = vasInhalt->Length();
	//						_Zeile->Delete(ulCharacterPos, ulZeichen);

	//						if(stZeichengrosse.cx == lBreitesteZeile) BreitesteZeile(hdc);

	//						rcZeichnen.top = rcSelect.top; rcZeichnen.bottom = rcSelect.bottom;
	//						rcZeichnen.left = rcSelect.right;	rcZeichnen.right = lRand_rechts;

	//						if(stZeichengrosse.cx - lTextPos < rcZeichnen.right) rcZeichnen.right = stZeichengrosse.cx - lTextPos;
	//						GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichen, &stZeichengrosse);
	//						ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
	//						rcZeichnen.left = rcSelect.left + rcZeichnen.right - rcZeichnen.left;
	//						cSelect = 0; ShowCaret(hWndElement);
	//						UpdateFenster(&rcZeichnen, true, false);
	//						break;
	//	case  2 : rcZeichnen.top = lLine - (rcSelect.bottom - rcSelect.top) / szfCharacter.height + 1;
	//						rcZeichnen.bottom = rcSelect.top + szfCharacter.height;
	//						pvLineAktuell = vliText->IteratorToBegin();
	//						for(lLinen = 0; lLinen < rcZeichnen.top; lLinen++) vliText->NextElement(pvLineAktuell, pvLineErste);
	//						ulZeichen = _ZeileAktuell->SubString(vbZeichen, ulSelectPos + 1, _ZeileAktuell->Length());
	//						*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//						_ZeileAktuell->Delete(ulSelectPos, ulZeichen);
	//						rcZeichnen.bottom += szfCharacter.height;
	//						*vasInhalt += "\n"; ulZeichen++;
	//						vliText->NextElement(pvLineAktuell, pvLineErste);
	//						while(pvLineAktuell && rcZeichnen.bottom < rcSelect.bottom){
	//							ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, _ZeileAktuell->Length());
	//							*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//							VMFreiV((COStringA*)vliText->Element(pvLineAktuell));
	//							vliText->DeleteElement(pvLineAktuell, pvLineErste, false);
	//							vliText->NextElement(pvLineAktuell);
	//							rcZeichnen.bottom += szfCharacter.height;
	//							*vasInhalt += "\n"; ulZeichen++;
	//						}
	//						ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, ulCharacterPos);
	//						*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//						_ZeileAktuell->SubString(vbZeichen, ulCharacterPos + 1, _ZeileAktuell->Length());
	//						*((COStringA*)vliText->Element(pvLineErste)) += vbZeichen; VMFrei(vbZeichen);
	//						vliText->DeleteElement(pvLineAktuell, pvLineErste, false);

	//						pvLine = vliText->Element(pvLineErste);
	//						lLinen = (rcSelect.bottom - rcSelect.top) / szfCharacter.height - 1;
	//						lLine -= lLinen;
	//						ptfCaret.y -= szfCharacter.height * lLinen;
	//						SetzScrollVert(stScrollInfo);

	//						GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
	//						if(stZeichengrosse.cx > lBreitesteZeile){ lBreitesteZeile = stZeichengrosse.cx; SetzScrollHorz(stScrollInfo); }
	//						else BreitesteZeile(hdc);

	//						ulCharacterPos = ulSelectPos;
	//						GetTextExtentPoint32(hdc, _Zeile->c_Str(), ulCharacterPos, &stZeichengrosse);
	//						ptfCaret.x = stZeichengrosse.cx;
	//						SetCaretPos(ptfCaret.x, ptfCaret.y);
 //
	//						GetClientRect(hWndElement, &rcZeichnen);
	//						rcZeichnen.top = rcSelect.top;
	//						cSelect = 0; ShowCaret(hWndElement);
	//						UpdateFenster(&rcZeichnen, true, false);
	//						break;
	//	case -2 : rcZeichnen.bottom = rcSelect.top + szfCharacter.height;
	//						pvLineAktuell = vliText->IteratorToBegin();
	//						for(lLinen = 0; lLinen < lLine; lLinen++) vliText->NextElement(pvLineAktuell, pvLineErste);
	//						ulZeichen = _ZeileAktuell->SubString(vbZeichen, ulCharacterPos + 1, _ZeileAktuell->Length());
	//						*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//						_ZeileAktuell->Delete(ulCharacterPos, ulZeichen);
	//						rcZeichnen.bottom += szfCharacter.height;
	//						*vasInhalt += "\n"; ulZeichen++;
	//						vliText->NextElement(pvLineAktuell, pvLineErste);
	//						while(pvLineAktuell && rcZeichnen.bottom < rcSelect.bottom){
	//							ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, _ZeileAktuell->Length());
	//							*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//							VMFreiV((COStringA*)vliText->Element(pvLineAktuell));
	//							vliText->DeleteElement(pvLineAktuell, pvLineErste, false);
	//							vliText->NextElement(pvLineAktuell);
	//							rcZeichnen.bottom += szfCharacter.height;
	//							*vasInhalt += "\n"; ulZeichen++;
	//						}
	//						ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, ulSelectPos);
	//						*vasInhalt += vbZeichen; VMFrei(vbZeichen);
	//						_ZeileAktuell->SubString(vbZeichen, ulSelectPos + 1, _ZeileAktuell->Length());
	//						*((COStringA*)vliText->Element(pvLineErste)) += vbZeichen; VMFrei(vbZeichen);
	//						vliText->DeleteElement(pvLineAktuell, pvLineErste, false);

	//						lLinen = (rcSelect.bottom - rcSelect.top) / szfCharacter.height - 1;
	//						SetzScrollVert(stScrollInfo);

	//						GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
	//						if(stZeichengrosse.cx > lBreitesteZeile){ lBreitesteZeile = stZeichengrosse.cx; SetzScrollHorz(stScrollInfo); }
	//						else BreitesteZeile(hdc);
 //
	//						GetClientRect(hWndElement, &rcZeichnen);
	//						rcZeichnen.top = rcSelect.top;
	//						cSelect = 0; ShowCaret(hWndElement);
	//						UpdateFenster(&rcZeichnen, true, false);
	//						break;
	//}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::BreitesteZeile(_In_ HDC hdc)
{
	//void* pvLinen = vliText->IteratorToBegin(); long lLinenbreite = 0; SIZE stZeichengrosse;
	//while(pvLinen){
	//	GetTextExtentPoint32(hdc, ((COStringA*)vliText->Element(pvLinen))->c_Str(), ((COStringA*)vliText->Element(pvLinen))->Length(), &stZeichengrosse);
	//	if(stZeichengrosse.cx == lBreitesteZeile) break;
	//	else if(stZeichengrosse.cx > lLinenbreite) lLinenbreite = stZeichengrosse.cx;
	//	vliText->NextElement(pvLinen);
	//}
	//if(!pvLinen){ SCROLLINFO stScrollHorz; stScrollHorz.cbSize = sizeof(SCROLLINFO); lBreitesteZeile = lLinenbreite; SetzScrollHorz(stScrollHorz); }
}
//---------------------------------------------------------------------------------------------------------------------------------------
