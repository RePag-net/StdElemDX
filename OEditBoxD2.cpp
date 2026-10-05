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
#define _Line ((COStringA*)pvLine)
#define _SelectLine ((COStringA*)vliText->Element(pvIterator))
#define _PasteLine ((COStringA*)vliText->Element(pvIterator))
#define _DeleteLine ((COStringA*)vliText->Element(pvIterator))
#define _InsertLine ((COStringA*)liText.Element(pvIterator_insert))
#define _ZeileAktuell ((COStringA*)vliText->Element(pvLineAktuell))
#define _ZeileErste ((COStringA*)vliText->Element(pvLineErste))
#define _EditBox ((RePag::DirectX::COEditBox*)pvParam)

constexpr CHAR LEFT = -1;
constexpr CHAR RIGHT = 1;
constexpr CHAR UP = -2;
constexpr	CHAR DOWN = 2;
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
	pvLine = nullptr;
	lLine = 0;
	pfnWM_Char_ShiftReturn = nullptr;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::OnPaint(void)
{
	ThreadSafe_Begin();
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
    rcfSelect = D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);
		if(!htCaret) CreateTimerQueueTimer(&htCaret, TimerQueue(), (WAITORTIMERCALLBACK)Timer_Caret_EditBox, this, 0, 500, 0);
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditBox::WM_KillFocus(void)
{
	ThreadSafe_Begin();
	DeleteTimerQueueTimer(TimerQueue(), htCaret, NULL); htCaret = nullptr;

	//rclDirty.left = FloatToLong(ptfCaret.x); rclDirty.right = FloatToLong(ptfCaret.x) + ucCaretStrength;
	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);

	if(pfnWM_KillFocus) pfnWM_KillFocus(this);
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

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
													siCharacter.ucMask = SBI_POS | SBI_PAGE;
													GetScrollBar(SB_HORZ, siCharacter);
													GetTextPoint(_Line->c_Str(), ulCharacterPos - 1, szfTextPoint);
													if(szfTextPoint.width - siCharacter.fPos < 0.0f){
														siCharacter.fPos = szfTextPoint.width;
														SetScrollBar(SB_HORZ, siCharacter);
														rclDirty.left = rclDirty.top = 0;
														GetScrollBar(SB_VERT, siLine);
														rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
													}
													
													switch(cSelect){ // left
														case	0			:	ResetEvent(heCaret);
																					stSelect_top.lLine = stSelect_bottom.lLine = lLine;
																					stSelect_top.fPosition = ptfCaret.x;
																					stSelect_top.ulCharacterPos = ulCharacterPos;
																					stSelect_bottom.fPosition = ptfCaret_old.x;
																					stSelect_bottom.ulCharacterPos = ulCharacterPos + 1;

																					rclDirty.left = FloatToLong(stSelect_top.fPosition - siCharacter.fPos);
																					rclDirty.right = FloatToLong(stSelect_bottom.fPosition - siCharacter.fPos);
																					rclDirty.top = FloatToLong(ptfCaret.y);
																					rclDirty.bottom = FloatToLong(ptfCaret.y + szfCharacter.height);
																					break;
														case	RIGHT	:
														case	LEFT	:	ulCharacterPos--;
																					if(stSelect_top.lLine == stSelect_bottom.lLine){
																						if(ulCharacterPos == stSelect_top.ulCharacterPos){
																							stSelect_bottom.fPosition = ptfCaret.x = stSelect_top.fPosition;
																							DeSelect(); ThreadSafe_End(); return;
																						}
																						else if(ulCharacterPos > stSelect_top.ulCharacterPos){
																							rclDirty.right = FloatToLong(stSelect_bottom.fPosition);
																							GetTextPoint(_Line->c_Str(), ulCharacterPos, szfTextPoint);
																							stSelect_bottom.fPosition = ptfCaret.x = szfTextPoint.width;
																							stSelect_bottom.ulCharacterPos = ulCharacterPos;
																						}
																						else{
																							GetTextPoint(_Line->c_Str(), ulCharacterPos, szfTextPoint);
																							stSelect_top.fPosition = ptfCaret.x = szfTextPoint.width;
																							stSelect_top.ulCharacterPos = ulCharacterPos;
																							rclDirty.left = FloatToLong(stSelect_top.fPosition - siCharacter.fPos);
																						}
																					}
																					else if(lLine == stSelect_top.lLine){
																						GetTextPoint(_Line->c_Str(), ulCharacterPos, szfTextPoint);
																						stSelect_top.fPosition = ptfCaret.x = szfTextPoint.width;
																						stSelect_top.ulCharacterPos = ulCharacterPos;
																					}
																					else{
																						GetTextPoint(_Line->c_Str(), ulCharacterPos, szfTextPoint);
																						stSelect_bottom.fPosition = ptfCaret.x = szfTextPoint.width;
																						stSelect_bottom.ulCharacterPos = ulCharacterPos;
																					}
																					break;
														case	DOWN	: 
														case	UP		:	ulCharacterPos--; 
																					GetTextPoint(_Line->c_Str(), ulCharacterPos, szfTextPoint);
																					ptfCaret.x = szfTextPoint.width;
																					if(stSelect_top.lLine == stSelect_bottom.lLine){
																						if(ptfCaret.x == stSelect_top.fPosition){
																							stSelect_bottom.fPosition = ptfCaret.x = stSelect_top.fPosition;
																							DeSelect(); ThreadSafe_End(); return;
																						}
																						else if(stSelect_bottom.fPosition < stSelect_top.fPosition){
																							STSelect stSelect_temp = stSelect_top;
																							stSelect_top = stSelect_bottom;
																							stSelect_bottom = stSelect_temp;
																						}
																						else{
																							stSelect_bottom.fPosition = ptfCaret.x;
																							stSelect_bottom.ulCharacterPos = ulCharacterPos;
																						}
																					}
																					else if(lLine <= stSelect_top.lLine){
																						stSelect_top.fPosition = ptfCaret.x;
																						stSelect_top.ulCharacterPos = ulCharacterPos;
																					}
																					else{
																						stSelect_bottom.fPosition = ptfCaret.x;
																						stSelect_bottom.ulCharacterPos = ulCharacterPos;
																					}
																					break;
													}
													cSelect = -1;
													OnRender(false);
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

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
													siCharacter.ucMask = SBI_PAGE | SBI_POS; siLine.ucMask = SBI_PAGE;
													GetScrollBar(SB_HORZ, siCharacter);
													GetTextPoint(_Line->c_Str(), ulCharacterPos + 1, szfTextPoint);
													if(szfTextPoint.width - siCharacter.fPos > siCharacter.fPage){
														siCharacter.fPos = szfTextPoint.width - siCharacter.fPage + (float)ucCaretStrength;
														SetScrollBar(SB_HORZ, siCharacter);
														rclDirty.left = rclDirty.top = 0;
														GetScrollBar(SB_VERT, siLine);
														rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
													}
													
													switch(cSelect){	// right
														case	0			: ResetEvent(heCaret);
																					stSelect_top.lLine = stSelect_bottom.lLine = lLine;
																					stSelect_top.fPosition = ptfCaret_old.x;
																					stSelect_top.ulCharacterPos = ulCharacterPos - 1;
																					stSelect_bottom.fPosition = ptfCaret.x;
																					stSelect_bottom.ulCharacterPos = ulCharacterPos;

																					rclDirty.left = FloatToLong(stSelect_top.fPosition - siCharacter.fPos);
																					rclDirty.right = FloatToLong(stSelect_bottom.fPosition - siCharacter.fPos);
																					rclDirty.top = FloatToLong(ptfCaret.y);
																					rclDirty.bottom = FloatToLong(ptfCaret.y + szfCharacter.height);
																					break;
														case RIGHT	:
														case	LEFT	: ulCharacterPos++;
																					if(stSelect_top.lLine == stSelect_bottom.lLine){
																						if(ulCharacterPos == stSelect_bottom.ulCharacterPos){
																							stSelect_top.fPosition = ptfCaret.x = stSelect_bottom.fPosition;
																							DeSelect(); ThreadSafe_End(); return;
																						}
																						else if(ulCharacterPos > stSelect_bottom.ulCharacterPos){
																							GetTextPoint(_Line->c_Str(), ulCharacterPos, szfTextPoint);
																							stSelect_bottom.fPosition = ptfCaret.x = szfTextPoint.width;
																							stSelect_bottom.ulCharacterPos = ulCharacterPos;
																							stSelect_bottom.fPosition > siCharacter.fPage ? rclDirty.right = FloatToLong(siCharacter.fPage)
																																														: rclDirty.right = FloatToLong(stSelect_bottom.fPosition);
																						}
																						else{
																							rclDirty.left = FloatToLong(stSelect_top.fPosition);
																							GetTextPoint(_Line->c_Str(), ulCharacterPos, szfTextPoint);
																							stSelect_top.fPosition = ptfCaret.x = szfTextPoint.width;
																							stSelect_top.ulCharacterPos = ulCharacterPos;
																						}
																					}
																					else if(lLine == stSelect_top.lLine){
																						GetTextPoint(_Line->c_Str(), ulCharacterPos, szfTextPoint);
																						stSelect_top.fPosition = ptfCaret.x = szfTextPoint.width;
																						stSelect_top.ulCharacterPos = ulCharacterPos;
																					}
																					else{
																						GetTextPoint(_Line->c_Str(), ulCharacterPos, szfTextPoint);
																						stSelect_bottom.fPosition = ptfCaret.x = szfTextPoint.width;
																						stSelect_bottom.ulCharacterPos = ulCharacterPos;
																					}
																					break;
														case	DOWN	: 
														case	UP		: ulCharacterPos++;
																					GetTextPoint(_Line->c_Str(), ulCharacterPos, szfTextPoint);
																					ptfCaret.x = szfTextPoint.width;
																					if(stSelect_top.lLine == stSelect_bottom.lLine){
																						if(ptfCaret.x == stSelect_top.fPosition){
																							stSelect_bottom.fPosition = ptfCaret.x = stSelect_top.fPosition;
																							DeSelect(); ThreadSafe_End(); return;
																						}
																						else if(stSelect_bottom.fPosition < stSelect_top.fPosition){
																							STSelect stSelect_temp = stSelect_top;
																							stSelect_top = stSelect_bottom;
																							stSelect_bottom = stSelect_temp;
																						}
																						else{
																							stSelect_bottom.fPosition = ptfCaret.x;
																							stSelect_bottom.ulCharacterPos = ulCharacterPos;
																						}
																					}
																					else if(lLine <= stSelect_top.lLine){
																						stSelect_top.fPosition = ptfCaret.x;
																						stSelect_top.ulCharacterPos = ulCharacterPos;
																					}
																					else{
																						stSelect_bottom.fPosition = ptfCaret.x;
																						stSelect_bottom.ulCharacterPos = ulCharacterPos;
																					}
																					break;
													}
													cSelect = 1;
													OnRender(false);
													ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
												}
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
													if(ptfCaret.x > 0.0f && _Line->Length()){
														do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
														while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _Line->Length());
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

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
													siLine.ucMask = SBI_PAGE | SBI_POS;
													GetScrollBar(SB_VERT, siLine);
													if(siLine.fPos && ptfCaret.y < szfCharacter.height && cSelect){
														siLine.fPos -= szfCharacter.height;
														SetScrollBar(SB_VERT, siLine);
														rclDirty.left = rclDirty.top = 0;
														GetScrollBar(SB_HORZ, siCharacter);
														rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
													}

													switch(cSelect){ // up
														case	0			:	ResetEvent(heCaret);
																					stSelect_top.lLine = lLine;
																					stSelect_top.fPosition = ptfCaret.x;
																					stSelect_top.ulCharacterPos = ulCharacterPos;
																					stSelect_bottom.lLine = stSelect_top.lLine + 1;
																					stSelect_bottom.fPosition = ptfCaret_old.x;
																					stSelect_bottom.ulCharacterPos = ulCharacterPos_old;

																					rclDirty.left = 0;
																					rclDirty.right = FloatToLong(siCharacter.fPage);
																					rclDirty.top = FloatToLong(ptfCaret.y);
																					rclDirty.bottom = FloatToLong(ptfCaret_old.y + szfCharacter.height);
																					break;
														case	DOWN	:	
														case	UP		:	pvLine = vliText->Element(--lLine);
																					if(lLine == stSelect_top.lLine){
																						stSelect_bottom.lLine--;
																						ulCharacterPos = 0;
																						do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
																						while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _Line->Length());
																						stSelect_bottom.fPosition = ptfCaret.x = szfTextPoint.width;
																						stSelect_bottom.ulCharacterPos = ulCharacterPos;
																						ptfCaret.y ? ptfCaret.y -= szfCharacter.height : ptfCaret.y = 0.0f;
																						if(stSelect_top.fPosition == stSelect_bottom.fPosition){
																							DeSelect(); ThreadSafe_End(); return;
																						}
																						else if(stSelect_bottom.fPosition < stSelect_top.fPosition){
																							STSelect stSelect_temp = stSelect_top;
																							stSelect_top = stSelect_bottom;
																							stSelect_bottom = stSelect_temp;
																						}
																					}
																					else if(lLine < stSelect_top.lLine){
																						stSelect_top.lLine--;
																						ulCharacterPos = 0;
																						do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
																						while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _Line->Length());
																						stSelect_top.fPosition = ptfCaret.x = szfTextPoint.width;
																						stSelect_top.ulCharacterPos = ulCharacterPos;
																						ptfCaret.y ? ptfCaret.y -= szfCharacter.height : ptfCaret.y = 0.0f;
																						rclDirty.top = FloatToLong(ptfCaret.y);
																					}
																					else{
																						stSelect_bottom.lLine--;
																						ulCharacterPos = 0;
																						do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
																						while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _Line->Length());
																						stSelect_bottom.fPosition = ptfCaret.x = szfTextPoint.width;
																						stSelect_bottom.ulCharacterPos = ulCharacterPos;
																						ptfCaret.y -= szfCharacter.height;
																					}
																					break;
														case	LEFT	: 
														case	RIGHT	: pvLine = vliText->Element(--lLine);
																					if(lLine == stSelect_top.lLine){
																						stSelect_bottom.lLine--;
																						ulCharacterPos = 0;
																						do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
																						while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _Line->Length());
																						stSelect_bottom.fPosition = ptfCaret.x = szfTextPoint.width;
																						stSelect_bottom.ulCharacterPos = ulCharacterPos;
																						ptfCaret.y -= szfCharacter.height;
																						if(stSelect_top.fPosition == stSelect_bottom.fPosition){
																							DeSelect(); ThreadSafe_End(); return;
																						}
																						else if(stSelect_bottom.fPosition < stSelect_top.fPosition){
																							STSelect stSelect_temp = stSelect_top;
																							stSelect_top = stSelect_bottom;
																							stSelect_bottom = stSelect_temp;
																						}
																					}
																					else if(lLine < stSelect_top.lLine){
																						stSelect_top.lLine--;
																						ulCharacterPos = 0;
																						do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
																						while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _Line->Length());
																						stSelect_top.fPosition = ptfCaret.x = szfTextPoint.width;
																						stSelect_top.ulCharacterPos = ulCharacterPos;
																						ptfCaret.y ? ptfCaret.y -= szfCharacter.height : ptfCaret.y = 0.0f;
																						rclDirty.top = FloatToLong(ptfCaret.y);
																						rclDirty.left = 0;
																						rclDirty.right = FloatToLong(siCharacter.fPage);
																					}
																					else{
																						stSelect_bottom.lLine--;
																						ulCharacterPos = 0;
																						do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
																						while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _Line->Length());
																						stSelect_bottom.fPosition = ptfCaret.x = szfTextPoint.width;
																						stSelect_bottom.ulCharacterPos = ulCharacterPos;
																						ptfCaret.y -= szfCharacter.height;
																					}
																					break;
													}
													cSelect = -2;
													OnRender(false);
													ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
												}
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
													if(ptfCaret.x > 0.0f && _Line->Length()){
														do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
														while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _Line->Length());
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

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
													siLine.ucMask = SBI_PAGE | SBI_POS;
													GetScrollBar(SB_VERT, siLine);
													if(cSelect && ptfCaret.y + szfCharacter.height * 2 > siLine.fPage){
														siLine.fPos += szfCharacter.height;
														SetScrollBar(SB_VERT, siLine);
														rclDirty.left = rclDirty.top = 0;
														GetScrollBar(SB_HORZ, siCharacter);
														rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
													}

													switch(cSelect){ // down
														case	0			: ResetEvent(heCaret);
																					stSelect_bottom.lLine = lLine;
																					stSelect_bottom.fPosition = ptfCaret.x;
																					stSelect_bottom.ulCharacterPos = ulCharacterPos;
																					stSelect_top.lLine = stSelect_bottom.lLine - 1;
																					stSelect_top.fPosition = ptfCaret_old.x;
																					stSelect_top.ulCharacterPos = ulCharacterPos_old;

																					rclDirty.left = 0;
																					rclDirty.right = FloatToLong(siCharacter.fPage);
																					ptfCaret_old.y ? rclDirty.top = FloatToLong(ptfCaret_old.y - szfCharacter.height) : rclDirty.top = 0;
																					rclDirty.bottom = FloatToLong(ptfCaret.y + szfCharacter.height);
																					break;
														case	DOWN	: 
														case	UP		: 	pvLine = vliText->Element(++lLine);
																					if(lLine == stSelect_bottom.lLine){
																						stSelect_top.lLine++;
																						ulCharacterPos = 0;
																						do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
																						while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _Line->Length());
																						stSelect_top.fPosition = ptfCaret.x = szfTextPoint.width;
																						stSelect_top.ulCharacterPos = ulCharacterPos;
																						ptfCaret.y += szfCharacter.height;
																						if(stSelect_bottom.fPosition == stSelect_top.fPosition){
																							DeSelect(); ThreadSafe_End(); return;
																						}
																						else if(stSelect_bottom.fPosition < stSelect_top.fPosition){
																							STSelect stSelect_temp = stSelect_top;
																							stSelect_top = stSelect_bottom;
																							stSelect_bottom = stSelect_temp;
																						}
																					}
																					else if(lLine < stSelect_bottom.lLine){
																						stSelect_top.lLine++;
																						ulCharacterPos = 0;
																						do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
																						while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _Line->Length());
																						stSelect_top.fPosition = ptfCaret.x = szfTextPoint.width;
																						stSelect_top.ulCharacterPos = ulCharacterPos;
																						rclDirty.top = FloatToLong(ptfCaret.y);
																						ptfCaret.y += szfCharacter.height;
																					}
																					else{
																						stSelect_bottom.lLine++;
																						ulCharacterPos = 0;
																						do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
																						while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _Line->Length());
																						stSelect_bottom.fPosition = ptfCaret.x = szfTextPoint.width;
																						stSelect_bottom.ulCharacterPos = ulCharacterPos;
																						if(ptfCaret.y + szfCharacter.height < siLine.fPage) ptfCaret.y += szfCharacter.height;
																						rclDirty.bottom = FloatToLong(ptfCaret.y + szfCharacter.height);
																						if(rclDirty.bottom > FloatToLong(siLine.fPage)) rclDirty.bottom = FloatToLong(siLine.fPage);
																					}
																					break;
														case	LEFT	: 
														case	RIGHT	:	pvLine = vliText->Element(++lLine);
																					if(lLine == stSelect_bottom.lLine){
																						stSelect_top.lLine++;
																						ulCharacterPos = 0;
																						do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
																						while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _Line->Length());
																						stSelect_top.fPosition = ptfCaret.x = szfTextPoint.width;
																						stSelect_top.ulCharacterPos = ulCharacterPos;
																						ptfCaret.y += szfCharacter.height;
																						if(stSelect_top.fPosition == stSelect_bottom.fPosition){
																							DeSelect(); ThreadSafe_End(); return;
																						}
																						else if(stSelect_bottom.fPosition < stSelect_top.fPosition){
																							STSelect stSelect_temp = stSelect_top;
																							stSelect_top = stSelect_bottom;
																							stSelect_bottom = stSelect_temp;
																						}
																					}
																					else if(lLine < stSelect_bottom.lLine){
																						stSelect_top.lLine++;
																						ulCharacterPos = 0;
																						do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
																						while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _Line->Length());
																						stSelect_top.fPosition = ptfCaret.x = szfTextPoint.width;
																						stSelect_top.ulCharacterPos = ulCharacterPos;
																						ptfCaret.y += szfCharacter.height;
																					}
																					else{
																						stSelect_bottom.lLine++;
																						ulCharacterPos = 0;
																						do{ GetTextPoint(_Line->c_Str(), ++ulCharacterPos, szfTextPoint); }
																						while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _Line->Length());
																						stSelect_bottom.fPosition = ptfCaret.x = szfTextPoint.width;
																						stSelect_bottom.ulCharacterPos = ulCharacterPos;
																						ptfCaret.y += szfCharacter.height;
																						rclDirty.bottom += FloatToLong(szfCharacter.height);
																						if(rclDirty.bottom > FloatToLong(siLine.fPage)) rclDirty.bottom = FloatToLong(siLine.fPage);
																						rclDirty.left = 0;
																						rclDirty.right = FloatToLong(siCharacter.fPage);
																					}
																					break;
													}
													cSelect = 2;
													OnRender(false);
													ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
												}
												else if(cSelect) DeSelect();
											}
											ThreadSafe_End();
											break;
		case VK_DELETE	: ThreadSafe_Begin();
											if(!ucCharacterSpecification){ ThreadSafe_End(); break; }
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
HGLOBAL hGlobal; char* pcClipboard;	*vasContent = NULL; ULONG ulCharacter; long lSelectLine = 0; VMBLOCK vbSelectLine;
void* pvIterator; void* pvDelete; void* pvPreIterator; void* pvIterator_insert;
STScrollInfo siLine{}; siLine.ucMask = SBI_POS | SBI_PAGE | SBI_CHARACTER_HEIGHT;
STScrollInfo siCharacter{}; siCharacter.ucMask = SBI_PAGE | SBI_MAX;
D2D_SIZE_F szfTextPoint; COStringA* vasLine; float fWidestLine = 0; ULONG ulWidth, ulSign = 0, ulSign_right = 1; COList liText(false);

	switch(LOWORD(wParam)){
		case IDM_COPY		:	ThreadSafe_Begin();
											OpenClipboard(hWndElement); EmptyClipboard();
											if(stSelect_top.lLine == stSelect_bottom.lLine){
												((COStringA*)vliText->Element(stSelect_top.lLine))->SubString(vbSelectLine, stSelect_top.ulCharacterPos + 1, stSelect_bottom.ulCharacterPos);
												ulCharacter = stSelect_bottom.ulCharacterPos - stSelect_top.ulCharacterPos + 1;
												hGlobal = GlobalAlloc(GMEM_MOVEABLE, ulCharacter);
												pcClipboard = (char*)GlobalLock(hGlobal);
												MemCopy(pcClipboard, vbSelectLine, ulCharacter); VMFrei(vbSelectLine);
												pcClipboard[ulCharacter] = 0;
												GlobalUnlock(hGlobal);
											}
											else{
												sbVertical->GetScrollInfo(siLine);
												pvIterator = vliText->IteratorToBegin(); rcfSelect = {0};
												while(pvIterator && rcfSelect.top < siLine.fPos){ 
													vliText->NextElement(pvIterator); rcfSelect.top += siLine.szfCharacter.height; lSelectLine++; }
												while(pvIterator && lSelectLine++ < stSelect_top.lLine) vliText->NextElement(pvIterator);

												_SelectLine->SubString(vbSelectLine, stSelect_top.ulCharacterPos + 1, _SelectLine->Length());
												*vasContent += vbSelectLine; *vasContent += "\n"; VMFrei(vbSelectLine);
												vliText->NextElement(pvIterator);

												if(lSelectLine++ == stSelect_bottom.lLine) goto LastLineCopy;

												do{
													_SelectLine->SubString(vbSelectLine, 1, _SelectLine->Length());
													*vasContent += vbSelectLine; *vasContent += "\n"; VMFrei(vbSelectLine);
													vliText->NextElement(pvIterator);
												}
												while(pvIterator && lSelectLine++ < stSelect_bottom.lLine);
LastLineCopy:
												_SelectLine->SubString(vbSelectLine, 1, stSelect_bottom.ulCharacterPos);
												*vasContent += vbSelectLine; VMFrei(vbSelectLine);

												hGlobal = GlobalAlloc(GMEM_MOVEABLE, vasContent->Length() + 1);
												pcClipboard = (char*)GlobalLock(hGlobal);
												MemCopy(pcClipboard, vasContent->c_Str(), vasContent->Length());
												pcClipboard[vasContent->Length()] = 0;
												GlobalUnlock(hGlobal);
												*vasContent = NULL;
											}
											SetClipboardData(CF_TEXT, hGlobal); CloseClipboard();
											ThreadSafe_End(); return false;
		case IDM_CUT		: ThreadSafe_Begin();
											OpenClipboard(hWndElement); EmptyClipboard();
											if(stSelect_top.lLine == stSelect_bottom.lLine){
												((COStringA*)vliText->Element(stSelect_top.lLine))->SubString(vbSelectLine, stSelect_top.ulCharacterPos + 1, stSelect_bottom.ulCharacterPos);
												ulCharacter = stSelect_bottom.ulCharacterPos - stSelect_top.ulCharacterPos + 1;
												hGlobal = GlobalAlloc(GMEM_MOVEABLE, ulCharacter);
												pcClipboard = (char*)GlobalLock(hGlobal);
												MemCopy(pcClipboard, vbSelectLine, ulCharacter); VMFrei(vbSelectLine);
												pcClipboard[ulCharacter] = 0;
												GlobalUnlock(hGlobal);
												
												((COStringA*)vliText->Element(stSelect_top.lLine))->Delete(stSelect_top.ulCharacterPos, ulCharacter - 1);
												sbHorizontal->GetScrollInfo(siCharacter);
												rclDirty.left = FloatToLong(stSelect_top.fPosition); rclDirty.right = FloatToLong(siCharacter.fPage);
												stSelect_bottom = stSelect_top;
												OnRender(true);
												ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
											}
											else{
												sbVertical->GetScrollInfo(siLine);
												pvIterator = vliText->IteratorToBegin(); pvDelete = nullptr;  rcfSelect = {0};
												while(pvIterator && rcfSelect.top < siLine.fPos){
													vliText->NextElement(pvIterator, pvDelete); rcfSelect.top += siLine.szfCharacter.height; lSelectLine++;
												}
												while(pvIterator && lSelectLine++ < stSelect_top.lLine) vliText->NextElement(pvIterator);

												_SelectLine->SubString(vbSelectLine, stSelect_top.ulCharacterPos + 1, _SelectLine->Length());
												*vasContent += vbSelectLine; *vasContent += "\n"; VMFrei(vbSelectLine);
												((COStringA*)vliText->Element(pvIterator))->Delete(stSelect_top.ulCharacterPos, _SelectLine->Length() - stSelect_top.ulCharacterPos);
												pvPreIterator = pvIterator;
												vliText->NextElement(pvIterator, pvDelete);

												if(lSelectLine++ == stSelect_bottom.lLine) goto LastLineCut;

												do{
													_SelectLine->SubString(vbSelectLine, 1, _SelectLine->Length());
													*vasContent += vbSelectLine; *vasContent += "\n"; VMFrei(vbSelectLine);
													VMFrei(vliText->Element(pvIterator));
													vliText->DeleteElement(pvIterator, pvDelete, false);
												}
												while(pvIterator && lSelectLine++ < stSelect_bottom.lLine);
LastLineCut:
												_SelectLine->SubString(vbSelectLine, 1, stSelect_bottom.ulCharacterPos);
												*vasContent += vbSelectLine; VMFrei(vbSelectLine);

												hGlobal = GlobalAlloc(GMEM_MOVEABLE, vasContent->Length() + 1);
												pcClipboard = (char*)GlobalLock(hGlobal);
												MemCopy(pcClipboard, vasContent->c_Str(), vasContent->Length());
												pcClipboard[vasContent->Length()] = 0;
												GlobalUnlock(hGlobal);

												((COStringA*)vliText->Element(pvIterator))->Delete(0, stSelect_bottom.ulCharacterPos);
                        *((COStringA*)vliText->Element(pvPreIterator)) += *((COStringA*)vliText->Element(pvIterator));
												VMFrei(vliText->Element(pvIterator));
												vliText->DeleteElement(pvIterator, pvDelete, false);

												stSelect_bottom = stSelect_top; cSelect = 0; SetEvent(heCaret);
												sbHorizontal->GetScrollInfo(siCharacter);
												rclDirty.left = 0; rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
												stSelect_bottom = stSelect_top;
												OnRender(true);
												ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
											}
											SetClipboardData(CF_TEXT, hGlobal); CloseClipboard();
											*vasContent = NULL;
											ThreadSafe_End(); return false;
		case IDM_PASTE	: ThreadSafe_Begin();
											if(!IsClipboardFormatAvailable(CF_TEXT) || !ucCharacterSpecification){ ThreadSafe_End(); return false; }
											OpenClipboard(hWndElement);
											hGlobal = GetClipboardData(CF_TEXT);
											*vasContent = (char*)GlobalLock(hGlobal);
											GlobalUnlock(hGlobal);
											CloseClipboard();

											if(!vasContent->Length()){ ThreadSafe_End(); return false; }
											if((*vasContent)[vasContent->Length() - 1] != 0x0A) *vasContent += "\n";

											ulWidth = vasContent->Length();
											do{
												ulSign_right++;
												if((*vasContent)[++ulSign] == 0x0A){
													vasLine = COStringAV(vmMemory);
													vasContent->SubString(vasLine, ulSign - ulSign_right + 2, ulSign);
													liText.ToEnd(vasLine);
													ulSign_right = 0;
												}
											}
											while(ulSign < ulWidth);
											
											if(liText.Number() == 1){
												vasContent->ShortRightOne();
												pvIterator = vliText->IteratorToBegin();
												for(long lPastLine = 0; lPastLine < lLine; lPastLine++) vliText->NextElement(pvIterator);
												_PasteLine->Insert(vasContent, ulCharacterPos);

												sbHorizontal->GetScrollInfo(siCharacter);
                        GetTextPoint(_PasteLine->c_Str(), _PasteLine->Length(), szfTextPoint);
                        if(siCharacter.fMax < szfTextPoint.width){
                          siCharacter.fMax = szfTextPoint.width;
                          SetScrollBar(SB_HORZ, siCharacter);
													ChangeSizeVisibleScrollBars();
                        }
												VMFrei(vliText->Element(lLine));

												rclDirty.left = ulCharacterPos; rclDirty.right = FloatToLong(siCharacter.fPage);
												OnRender(true);
												ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
											}
											else{
												pvIterator = vliText->IteratorToBegin(); pvPreIterator = nullptr;
												for(long lPastLine = 0; lPastLine < lLine; lPastLine++) vliText->NextElement(pvIterator, pvPreIterator);
												_PasteLine->SubString(vasContent, ulCharacterPos + 1, _PasteLine->Length());
												_PasteLine->Delete(ulCharacterPos, _PasteLine->Length() - ulCharacterPos);

												pvIterator_insert = liText.IteratorToBegin();
												_PasteLine->Insert(_InsertLine->c_Str(), ulCharacterPos + 1);
												GetTextPoint(_PasteLine->c_Str(), _PasteLine->Length(), szfTextPoint);
												if(fWidestLine < szfTextPoint.width) fWidestLine = szfTextPoint.width;

                        VMFrei(liText.Element(pvIterator_insert));
												liText.DeleteFirstElement(pvIterator_insert, false);
                        vliText->NextElement(pvIterator, pvPreIterator);
												if(liText.Number() == 1) goto LastLinePaste;

												do{
                          vliText->Insert(pvIterator, pvPreIterator, liText.Element(pvIterator_insert));

													GetTextPoint(_PasteLine->c_Str(), _PasteLine->Length(), szfTextPoint);
													if(fWidestLine < szfTextPoint.width) fWidestLine = szfTextPoint.width;

													VMFrei(liText.Element(pvIterator_insert));
													liText.DeleteFirstElement(pvIterator_insert, false);
                          vliText->NextElement(pvPreIterator);
												}
												while(liText.Number() > 1);
LastLinePaste:
												vliText->Insert(pvIterator, pvPreIterator, liText.Element(pvIterator_insert));
												vliText->NextElement(pvPreIterator);
												*(COStringA*)vliText->Element(pvPreIterator) += *vasContent;
												GetTextPoint(((COStringA*)vliText->Element(pvPreIterator))->c_Str(), ((COStringA*)vliText->Element(pvPreIterator))->Length(), szfTextPoint);
												if(fWidestLine < szfTextPoint.width) fWidestLine = szfTextPoint.width;

												sbHorizontal->GetScrollInfo(siCharacter);
												if(siCharacter.fMax < fWidestLine){
													siCharacter.fMax = fWidestLine;
													SetScrollBar(SB_HORZ, siCharacter);
													ChangeSizeVisibleScrollBars();
												}

												sbVertical->GetScrollInfo(siLine);
                        rclDirty.left = 0; rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
												OnRender(true);
												ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
											}
											*vasContent = NULL;
											ThreadSafe_End(); return false;
		default         : return true;
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
		if((float)GET_X_LPARAM(lParam) < ptfCaret.x - szfCharacter.width) SendMessage(hWndElement, WM_KEYDOWN, VK_LEFT, NULL);
		else if((float)GET_X_LPARAM(lParam) > ptfCaret.x + szfCharacter.width) SendMessage(hWndElement, WM_KEYDOWN, VK_RIGHT, NULL);

		if((float)GET_Y_LPARAM(lParam) < ptfCaret.y - szfCharacter.height) SendMessage(hWndElement, WM_KEYDOWN, VK_UP, NULL);
		else if((float)GET_Y_LPARAM(lParam) > ptfCaret.y + szfCharacter.height) SendMessage(hWndElement, WM_KEYDOWN, VK_DOWN, NULL);
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
	STScrollInfo siLine{}; siLine.ucMask = SBI_POS | SBI_MAX;
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

	STScrollInfo siCharacter{}; siCharacter.ucMask = SBI_POS;
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
