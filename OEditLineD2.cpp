/******************************************************************************
MIT License

Copyright(c) 2025 René Pagel

Filename : OEditLineD2.cpp
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
#include "OEditLineD2.h"

#define _EditLine ((RePag::DirectX::COEditLine*)pvParam)
constexpr LONG CARET_PIXEL = 2;
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COEditLine* __vectorcall RePag::DirectX::COEditLineV(_In_ const char* pcFensterName, _In_ unsigned int uiIDElement,
																														 _In_ STDeviceResources* pstDeviceResources)
{
	COEditLine* vEditLine = (COEditLine*)VMBlock(VMDialog(), sizeof(COEditLine));
	vEditLine->COEditLineV(VMDialog(), pcFensterName, uiIDElement, pstDeviceResources);
	return vEditLine;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COEditLine* __vectorcall RePag::DirectX::COEditLineV(_In_ const VMEMORY vmSpeicher, _In_ const char* pcFensterName, _In_ unsigned int uiIDElement,
																														 _In_ STDeviceResources* pstDeviceResources)
{
	COEditLine* vEditLine = (COEditLine*)VMBlock(vmSpeicher, sizeof(COEditLine));
	vEditLine->COEditLineV(vmSpeicher, pcFensterName, uiIDElement, pstDeviceResources);
	return vEditLine;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_EditLine(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	COEditLine* pEditLine;
	switch(uiMessage){
		case WM_CREATE        : ((COEditLine*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
														((COEditLine*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
														return NULL;
		case WM_SIZE          : pEditLine = (COEditLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
														if(pEditLine) pEditLine->WM_Size_Element(hWnd, lParam);
														else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
														return NULL;
		case WM_SETFOCUS      : ((COEditLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_SetFocus();
														return NULL;
		case WM_KILLFOCUS     : ((COEditLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KillFocus();
														return NULL;
		case WM_KEYDOWN       :	((COEditLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KeyDown(wParam, lParam);
														return NULL;
		case WM_CHAR          :	((COEditLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Char(wParam);
														return NULL;
		case WM_COMMAND       : pEditLine = (COEditLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
														if(!pEditLine->WM_Command(wParam)) return NULL;
														else if(pEditLine->pfnWM_Command){
															pEditLine->ThreadSafe_Begin();
															if(!pEditLine->pfnWM_Command(pEditLine, wParam)){ pEditLine->ThreadSafe_End(); return NULL; }
															pEditLine->ThreadSafe_End();
														}
														else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
														break;
		case WM_CONTEXTMENU   : ((COEditLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_ContexMenu(lParam);
														return NULL;
		case WM_MOUSEMOVE     : ((COEditLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseMove(wParam, lParam);
														return NULL;
		case WM_LBUTTONDOWN   : ((COEditLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDown(wParam, lParam);
														return NULL;
		case WM_LBUTTONUP     : ((COEditLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonUp(wParam, lParam);
														return NULL;
		case WM_LBUTTONDBLCLK : ((COEditLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDBClick(wParam, lParam);
														return NULL;
		case WM_NCDESTROY     : pEditLine = (COEditLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
														if(pEditLine->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pEditLine->htEffekt_Timer, INVALID_HANDLE_VALUE);
														VMFreiV(pEditLine);
														return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void CALLBACK RePag::DirectX::Timer_Caret(void* pvParam, bool bTimerOrWaitFired)
{
	WaitForSingleObject(_EditLine->heCaret, INFINITE);
	static bool bCaret = false;
	bCaret ? bCaret = false : bCaret = true;

	_EditLine->ThreadSafe_Begin();
	_EditLine->rclDirty.left = _EditLine->ptlCaret.x;
	_EditLine->rclDirty.right = _EditLine->rclDirty.left + CARET_PIXEL;

	_EditLine->OnRender(bCaret);
	_EditLine->ifDXGISwapChain4->Present1(0, NULL, &_EditLine->dxgiPresent);
	_EditLine->ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::COEditLineV(_In_ const VMEMORY vmSpeicher, _In_ const char* pcKlassenName, _In_ const char* pcFensterName, _In_ unsigned int uiIDElementA,
																											_In_ STDeviceResources* pstDeviceResourcesA)
{
	 COTextLineV(vmSpeicher, pcKlassenName, pcFensterName, uiIDElementA, pstDeviceResourcesA);

	 pfnWM_Char_Return = nullptr;
	 pfnWM_Char_Escape = nullptr;
	 pfnWM_KillFocus = nullptr;
	 pfnWM_Command = nullptr;
	 pfnWM_LButtonDown = nullptr;
	 pfnWM_LButtonUp = nullptr;
	 pfnWM_LButtonDBClick = nullptr;
	 ptlCaret = {0};
	 cSelect = 0;
	 rclSelect = {0};
	 lTextPos = 0;
	 ulZeichenPos = 0;
	 ulSelectPos = 0;
	 ulZeichen_max = 0x7fffffff;
	 ucZeichenVorgabe = ZV_ALLE; 
	 crfSelectText = D2D1::ColorF(RGB(0, 0, 0), 1.0f);
	 crfSelectBack = D2D1::ColorF(RGB(215, 215, 0), 1.0f);
	 crfCaret = D2D1::ColorF(RGB(255, 255, 255), 1.0f);
	 htCaret = nullptr;
	 heCaret = CreateEvent(nullptr, true, true, nullptr);

	 vasZeichenMaske = COStringAV(vmSpeicher);

	 hMenu = CreatePopupMenu();
	 AppendMenu(hMenu, MF_STRING, IDM_AUSSCHNEIDEN, "Ausschneiden		Strg+X");
	 AppendMenu(hMenu, MF_STRING, IDM_KOPIEREN, "Kopieren		Strg+C");
	 AppendMenu(hMenu, MF_STRING, IDM_EINFUGEN, "Einfügen		Strg+V");
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::COEditLineV(_In_ const VMEMORY vmSpeicher, _In_ const char* pcFensterName, _In_ unsigned int uiIDElementA,
																											_In_ STDeviceResources* pstDeviceResourcesA)
{
	COEditLineV(vmSpeicher, pcRePag_EditLine, pcFensterName, uiIDElementA, pstDeviceResourcesA);
}
//---------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COEditLine::COFreiV(void)
{
	SafeRelease(&ifSelectBackColor);
	SafeRelease(&ifCaretColor);
	CloseHandle(heCaret);
	if(htCaret) DeleteTimerQueueTimer(TimerQueue(), htCaret, NULL);
	VMFreiV(vasZeichenMaske);
	DestroyMenu(hMenu);
	return ((COTextLine*)this)->COFreiV();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::OnRender(_In_ bool bCaret)
{
	IDWriteTextLayout* ifTextLayout; D2D1_RECT_F rcfText; float fTextWidth; size_t szBytes_Text; WCHAR wcInhalt[255]; 

	WaitForSingleObjectEx(heRender, INFINITE, false);
	ifTextColor->SetColor(crfText);
	mbstowcs_s(&szBytes_Text, wcInhalt, vasInhalt->c_Str(), vasInhalt->Length());
	pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wcInhalt, szBytes_Text, ifText, (float)lWidth, (float)lHeight, &ifTextLayout);
	TextAlignment(ifTextLayout, fTextWidth, rcfText);
	SafeRelease(&ifTextLayout);

	if(lTextPos || (long)fTextWidth > lWidth) rcfText.left = static_cast<float>(lTextPos * -1);

	ifD2D1Context6->BeginDraw();
	ifD2D1Context6->Clear(crfBackground);
	ifD2D1Context6->DrawText(wcInhalt, szBytes_Text, ifText, rcfText, ifTextColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);
		
	if(!cSelect){
		if(bCaret){	D2D1_POINT_2F ptfTop, ptfBottom;
			ptfTop.x = (float)ptlCaret.x;
			ptfTop.y = static_cast<float>(ptlCaret.y + lZeichen_hohe);
			ptfBottom.x = (float)ptlCaret.x;
			ptfBottom.y = (float)ptlCaret.y;
			ifD2D1Context6->DrawLine(ptfTop, ptfBottom, ifCaretColor, (float)CARET_PIXEL, nullptr);
		}
	}
	else{	VMBLOCK vbZeichen; ULONG ulZeichen; D2D1_RECT_F rcfSelect;
		if(cSelect > 0)	ulZeichen = vasInhalt->SubString(vbZeichen, ulSelectPos + 1, ulZeichenPos);
		else ulZeichen = vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulSelectPos);

		rcfSelect.top = rcfText.top; rcfSelect.bottom = rcfText.bottom;
		rcfSelect.left = (float)rclSelect.left; rcfSelect.right = (float)rclSelect.right;

		ifD2D1Context6->FillRectangle(&rcfSelect, ifSelectBackColor);

		ifTextColor->SetColor(crfSelectText);
		mbstowcs_s(&szBytes_Text, wcInhalt, vbZeichen, ulZeichen); VMFrei(vbZeichen);
		ifD2D1Context6->DrawText(wcInhalt, szBytes_Text, ifText, rcfSelect, ifTextColor);
	}

	ifD2D1Context6->EndDraw();
	SetEvent(heRender);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::OnPaint(void)
{
	ThreadSafe_Begin();
	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_Create(void)
{
	CharacterMetric();
	//rclSelect.bottom = lHeight;
	ifD2D1Context6->CreateSolidColorBrush(crfText, &ifTextColor);
	ifD2D1Context6->CreateSolidColorBrush(crfSelectBack, &ifSelectBackColor);
	ifD2D1Context6->CreateSolidColorBrush(crfCaret, &ifCaretColor);

	ifText->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	SIZE szZeichen_Inhalt;
	if(ucSchriftausrichtung & TXA_RECHTS){
		GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), szZeichen_Inhalt);
		if(szZeichen_Inhalt.cx > lWidth) lTextPos = szZeichen_Inhalt.cx - lWidth;
	}
	else if(ucSchriftausrichtung & TXA_MITTEHORIZONTAL){
		GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), szZeichen_Inhalt);
		if(szZeichen_Inhalt.cx > lWidth){
			lTextPos = szZeichen_Inhalt.cx - lWidth;
			lTextPos /= 2;
		}
	}

	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_SetFocus(void)
{
	ThreadSafe_Begin();
	IDWriteTextLayout* ifTextLayout; size_t szBytes_Text; WCHAR wcInhalt[255]; D2D1_POINT_2F ptfText = {0}; float fTextWidth;

	mbstowcs_s(&szBytes_Text, wcInhalt, vasInhalt->c_Str(), vasInhalt->Length());
	pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wcInhalt, szBytes_Text, ifText, (float)lWidth, (float)lHeight, &ifTextLayout);

	TextAlignment(ifTextLayout, fTextWidth, ptfText);
	SafeRelease(&ifTextLayout);

	ptlCaret.y = (long)ptfText.y;
	ptfText.y -= (float)ptlCaret.y;
	if(ptfText.y >= 0.5f) ptlCaret.y += 1;

	rclScroll.top = rclDirty.top = lHeight - lZeichen_hohe - ptlCaret.y;
	rclScroll.bottom = rclDirty.bottom = lHeight - ptlCaret.y;
	ptlScrollOffset.y = 0;

	if(vasZeichenMaske->Length()){
		ptlCaret.x = (long)ptfText.x;
		ptfText.x -= ptlCaret.x;
		if(ptfText.x >= 0.5f) ptlCaret.x += 1;
		if(!cSelect) ulZeichenPos = 0;
		EnableMenuItem(hMenu, IDM_AUSSCHNEIDEN, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_EINFUGEN, MF_BYCOMMAND | MF_GRAYED);
	}
	else{	ptlCaret.x = static_cast<long>(ptfText.x + fTextWidth);
		if(!cSelect) ulZeichenPos = vasInhalt->Length();
	}

	if(!cSelect){
		if(!htCaret) CreateTimerQueueTimer(&htCaret, TimerQueue(), (WAITORTIMERCALLBACK)Timer_Caret, this, 0, 500, 0);
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_KillFocus(void)
{
	ThreadSafe_Begin();
	DeleteTimerQueueTimer(TimerQueue(), htCaret, NULL); htCaret = nullptr;
	
	rclDirty.left = ptlCaret.x; rclDirty.right = ptlCaret.x + CARET_PIXEL;
	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);

	if(pfnWM_KillFocus) pfnWM_KillFocus(this);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_KeyDown(WPARAM wParam, LPARAM lParam)
{
	SIZE stZeichengrosse, stZeichenPos_Caret; VMBLOCK vbZeichen; RECT rcl2Dirty[2];
	switch(wParam){
		case VK_HOME    : ThreadSafe_Begin();
											DeleteCaretPos();
											if(cSelect) DeSelect();
											ulZeichenPos = 0;
											if(lTextPos){
												lTextPos = 0; ptlCaret.x = 0;
												rclDirty.left = 0; rclDirty.right = lWidth;
												OnRender(true);
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											}
											else if(ucSchriftausrichtung & TXA_LINKS) ptlCaret.x = 0; 
											else{ 
												GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
												if(stZeichengrosse.cx > lWidth) ptlCaret.x = 0;
												else if(ucSchriftausrichtung & TXA_RECHTS) ptlCaret.x = lWidth - stZeichengrosse.cx;
												else ptlCaret.x = (lWidth - stZeichengrosse.cx) / 2;
											} 
											ThreadSafe_End();
											break;
		case VK_END     : ThreadSafe_Begin();
											DeleteCaretPos();
											if(cSelect) DeSelect();
											ulZeichenPos = vasInhalt->Length();
											GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
											if(stZeichengrosse.cx > lWidth){
												lTextPos = stZeichengrosse.cx - lWidth; ptlCaret.x = lWidth - CARET_PIXEL;
												rclDirty.left = 0; rclDirty.right = lWidth;
												OnRender(true);
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											}
											else if(ucSchriftausrichtung & TXA_LINKS) ptlCaret.x = stZeichengrosse.cx;
											else if(ucSchriftausrichtung & TXA_RECHTS) ptlCaret.x = lWidth - CARET_PIXEL;
											else ptlCaret.x = (lWidth - stZeichengrosse.cx) / 2 + stZeichengrosse.cx;
											ThreadSafe_End();
											break;
		case VK_LEFT		: ThreadSafe_Begin();											
											if(ulZeichenPos){
												if(cSelect) DeleteCaretPos();
												rclDirty.right = ptlCaret.x;
												GetTextPoint(vasInhalt->c_Str(), --ulZeichenPos, stZeichengrosse);
												if(stZeichengrosse.cx - lTextPos < 0)	ScrollLeft(stZeichengrosse);
												else if(ucSchriftausrichtung & TXA_LINKS){
													if(vasZeichenMaske->Length()){
														if(ZeichenMaske_FestLinks()) GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
													}
													rclDirty.left = ptlCaret.x = stZeichengrosse.cx - lTextPos;
													rclDirty.right += CARET_PIXEL;
												}
												else{
													if(vasZeichenMaske->Length()){
														if(ZeichenMaske_FestLinks()){
															GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
															ucSchriftausrichtung & TXA_RECHTS ? ptlCaret.x = lWidth - stZeichengrosse.cx
																: ptlCaret.x = (lWidth - stZeichengrosse.cx) / 2;
															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
															ptlCaret.x += stZeichengrosse.cx;
														}
														else{
															vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
															GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
															ptlCaret.x -= stZeichengrosse.cx;
														}
													}
													else{
														stZeichenPos_Caret = stZeichengrosse;
														GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
														if(stZeichengrosse.cx < lWidth){
															if(ucSchriftausrichtung & TXA_RECHTS)	ptlCaret.x = lWidth - stZeichengrosse.cx + stZeichenPos_Caret.cx;
															else ptlCaret.x = (lWidth - stZeichengrosse.cx) / 2 + stZeichenPos_Caret.cx;
														}
														else ptlCaret.x = stZeichenPos_Caret.cx - lTextPos;
														rclDirty.left = ptlCaret.x;
														rclDirty.right += CARET_PIXEL;
													}
												}

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
													if(cSelect < 0)	rclSelect.left = rclDirty.left = ptlCaret.x;
													else if(cSelect > 0){
														if(ulSelectPos < ulZeichenPos){
															rclSelect.right = ptlCaret.x;
															rclDirty.right += CARET_PIXEL;
															rclDirty.left = ptlCaret.x;
														}
														else{ cSelect = 0; SetEvent(heCaret);
															rclDirty.left = ptlCaret.x;	}
													}
													else{
														cSelect = -1;
														ulSelectPos = ulZeichenPos + 1;
														rclSelect.top = rclDirty.top; rclSelect.bottom = rclDirty.bottom;
														if(!ptlCaret.x) rclDirty.right = rclSelect.right;
														else{
															rclSelect.left = rclDirty.left = ptlCaret.x;
															rclSelect.right = rclDirty.right;
														}
														ResetEvent(heCaret);
													}
												}
												else if(cSelect) DeSelect();

												OnRender(true);
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											}
											ThreadSafe_End();
											break;
		case VK_RIGHT		: ThreadSafe_Begin();
											if(ulZeichenPos < vasInhalt->Length()){
												if(!cSelect) DeleteCaretPos();
												rclDirty.left = ptlCaret.x;
												GetTextPoint(vasInhalt->c_Str(), ++ulZeichenPos, stZeichengrosse);
												if(stZeichengrosse.cx - lTextPos > lWidth)	ScrollRight(stZeichengrosse);
												else if(stZeichengrosse.cx - lTextPos == lWidth)	ptlCaret.x = lWidth - CARET_PIXEL;
												else if(ucSchriftausrichtung & TXA_LINKS){
													if(vasZeichenMaske->Length()){
														if(ZeichenMaske_FestRechts()) GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
													}
													rclDirty.left -= CARET_PIXEL;
													rclDirty.right = ptlCaret.x = stZeichengrosse.cx - lTextPos;
												}
												else{
 													if(vasZeichenMaske->Length()){
														if(ZeichenMaske_FestRechts()){
															GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
															ucSchriftausrichtung & TXA_RECHTS ? ptlCaret.x = lWidth - stZeichengrosse.cx
																: ptlCaret.x = (lWidth - stZeichengrosse.cx) / 2;
															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
															ptlCaret.x += stZeichengrosse.cx;
														}
	 													else{
															vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
															GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
															ptlCaret.x = lWidth - stZeichengrosse.cx;
														}
													}
													else{
														stZeichenPos_Caret = stZeichengrosse;
														GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
														if(stZeichengrosse.cx < lWidth){
															if(ucSchriftausrichtung & TXA_RECHTS)	ptlCaret.x = lWidth - stZeichengrosse.cx + stZeichenPos_Caret.cx;
															else ptlCaret.x = (lWidth - stZeichengrosse.cx) / 2 + stZeichenPos_Caret.cx;
															if(ptlCaret.x == lWidth) ptlCaret.x -= CARET_PIXEL;
														}
														else ptlCaret.x = stZeichenPos_Caret.cx - lTextPos;
														rclDirty.left -= CARET_PIXEL;
														rclDirty.right = ptlCaret.x;
													}
												}

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){ 
													if(cSelect > 0)	rclSelect.right = rclDirty.right = ptlCaret.x + CARET_PIXEL;
													else if(cSelect < 0){
														if(ulSelectPos > ulZeichenPos){
															rclSelect.left = ptlCaret.x;
															if(ptlCaret.x > rclDirty.right){
																rclDirty.left = rclDirty.right;
																rclDirty.right = ptlCaret.x;
															}
														}
														else{ cSelect = 0; SetEvent(heCaret); }
													}
													else{ 
														cSelect = 1;
														ulSelectPos = ulZeichenPos - 1;
														rclSelect.top = rclDirty.top; rclSelect.bottom = rclDirty.bottom;
														if(stZeichengrosse.cx - lTextPos == lWidth){
															rclDirty.left = rclSelect.left;
															rclSelect.right = lWidth;
														}
														else{
															rclSelect.left = rclDirty.left;
															rclSelect.right = rclDirty.right = ptlCaret.x + CARET_PIXEL;
														}
														ResetEvent(heCaret);
													}											 
												}
												else if(cSelect) DeSelect();
												OnRender(true);
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											}
											ThreadSafe_End();
											break;
		case VK_DELETE	: ThreadSafe_Begin();
											if(!ucZeichenVorgabe){ ThreadSafe_End(); break; }
											if(!vasZeichenMaske->Length()){	
												if(cSelect) Select_Loschen();
												else if(ulZeichenPos < vasInhalt->Length()){
													GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
													if(ucSchriftausrichtung & TXA_LINKS){
														if(!lTextPos){
															if(stZeichengrosse.cx < lWidth) rclDirty.right = stZeichengrosse.cx;
															else rclDirty.right = lWidth;
															vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
															GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
															rclDirty.left = rclDirty.right - stZeichengrosse.cx;

															vasInhalt->Delete(ulZeichenPos, 1);
															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
															ptlCaret.x = stZeichengrosse.cx;
															if(rclDirty.left > ptlCaret.x){
																rclScroll.right = rclDirty.left;
																rclScroll.left = ptlCaret.x;
																ptlScrollOffset.x = rclDirty.left - rclDirty.right;
																dxgiPresent.pScrollRect = &rclScroll;
																dxgiPresent.pScrollOffset = &ptlScrollOffset;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																dxgiPresent.pScrollRect = nullptr;
																dxgiPresent.pScrollOffset = nullptr;
															}
															else{
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
														}
														else{
															if(stZeichengrosse.cx - lTextPos > lWidth){
																rclDirty.right = lWidth;
																vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
																GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																rclDirty.left = rclDirty.right - stZeichengrosse.cx;

																vasInhalt->Delete(ulZeichenPos, 1);
																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																ptlCaret.x = stZeichengrosse.cx - lTextPos;
																rclScroll.right = rclDirty.left;
																rclScroll.left = ptlCaret.x;
																ptlScrollOffset.x = rclDirty.left - rclDirty.right;
																dxgiPresent.pScrollRect = &rclScroll;
																dxgiPresent.pScrollOffset = &ptlScrollOffset;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																dxgiPresent.pScrollRect = nullptr;
																dxgiPresent.pScrollOffset = nullptr;
															}
															else{
																rclDirty.left = 0;
																vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
																GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																rclScroll.left = rclDirty.right = stZeichengrosse.cx;
																rclScroll.right = ptlCaret.x + stZeichengrosse.cx;
																ptlScrollOffset.x = stZeichengrosse.cx;
																vasInhalt->Delete(ulZeichenPos, 1);

																GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
																lTextPos = stZeichengrosse.cx - lWidth;

																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																ptlCaret.x = stZeichengrosse.cx - lTextPos;
																if(lTextPos < 0){	lTextPos = 0;	rclDirty.right = lWidth; }

																dxgiPresent.pScrollRect = &rclScroll;
																dxgiPresent.pScrollOffset = &ptlScrollOffset;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																dxgiPresent.pScrollRect = nullptr;
																dxgiPresent.pScrollOffset = nullptr;
															}
														}
													}
													else if(ucSchriftausrichtung & TXA_RECHTS){ 
														if(stZeichengrosse.cx <= lWidth){
															rclDirty.left = lWidth - stZeichengrosse.cx;
															if(rclDirty.left == ptlCaret.x){
																rclDirty.left = lWidth - stZeichengrosse.cx - CARET_PIXEL;
																vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
																GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																rclDirty.right = rclDirty.left + stZeichengrosse.cx;
																vasInhalt->Delete(ulZeichenPos, 1);
																GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
																ptlCaret.x = lWidth - stZeichengrosse.cx;
																if(ptlCaret.x >= lWidth) ptlCaret.x = lWidth - CARET_PIXEL;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															else{
																vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
																GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																rclScroll.left = rclDirty.right = rclDirty.left + stZeichengrosse.cx;
																if(rclDirty.left < stZeichengrosse.cx) rclDirty.left = 0;
																rclScroll.right = ptlCaret.x + stZeichengrosse.cx;
																if(rclScroll.right > lWidth) rclScroll.right = lWidth;
																ptlScrollOffset.x = stZeichengrosse.cx;
																dxgiPresent.pScrollRect = &rclScroll;
																dxgiPresent.pScrollOffset = &ptlScrollOffset;
																vasInhalt->Delete(ulZeichenPos, 1);
																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																ptlCaret.x = rclDirty.right + stZeichengrosse.cx;
																if(ptlCaret.x >= lWidth) ptlCaret.x = lWidth - CARET_PIXEL;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																dxgiPresent.pScrollRect = nullptr;
																dxgiPresent.pScrollOffset = nullptr;
															}
														}
														else if(!lTextPos){
															rclDirty.left = stZeichengrosse.cx - lWidth;
															vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
															GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
															if(rclDirty.left < stZeichengrosse.cx){
																rcl2Dirty[0].top = rcl2Dirty[1].top = rclDirty.top;
																rcl2Dirty[0].bottom = rcl2Dirty[1].bottom = rclDirty.bottom;
																rcl2Dirty[0].right = lWidth;
																rcl2Dirty[1].left = 0;
																ptlCaret.x = stZeichengrosse.cx - rclDirty.left;
																vasInhalt->Delete(ulZeichenPos, 1);
																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																ptlCaret.x += stZeichengrosse.cx;
																if(ptlCaret.x >= lWidth) ptlCaret.x = lWidth - CARET_PIXEL;
																rcl2Dirty[1].right = ptlCaret.x;
																ptlScrollOffset.x = rclDirty.left * -1;
																rclScroll.right = rcl2Dirty[0].left = rcl2Dirty[0].right - rclDirty.left;
																dxgiPresent.DirtyRectsCount = 2;
																dxgiPresent.pDirtyRects = rcl2Dirty;
																dxgiPresent.pScrollRect = &rclScroll;
																dxgiPresent.pScrollOffset = &ptlScrollOffset;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																dxgiPresent.pScrollRect = nullptr;
																dxgiPresent.pScrollOffset = nullptr;
																dxgiPresent.DirtyRectsCount = 1;
																dxgiPresent.pDirtyRects = &rclDirty;
															}
															else{
																rclDirty.right = lWidth;
																ptlScrollOffset.x = stZeichengrosse.cx * -1;
																rclScroll.left = ptlCaret.x;
																rclScroll.right = rclDirty.left = rclDirty.right - stZeichengrosse.cx;
																dxgiPresent.pScrollRect = &rclScroll;
																dxgiPresent.pScrollOffset = &ptlScrollOffset;
																vasInhalt->Delete(ulZeichenPos, 1);
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																dxgiPresent.pScrollRect = nullptr;
																dxgiPresent.pScrollOffset = nullptr;
															}
														}
														else{
															if(stZeichengrosse.cx - lTextPos > lWidth){
																vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
																GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																vasInhalt->Delete(ulZeichenPos, 1);
																rclDirty.right = lWidth;
																rclScroll.left = ptlCaret.x;
																ptlScrollOffset.x = stZeichengrosse.cx * -1;
																rclScroll.right = rclDirty.left = lWidth - stZeichengrosse.cx;
																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																ptlCaret.x = stZeichengrosse.cx - lTextPos;
															}
															else{
																rclDirty.left = 0;
																vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
																GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																rclScroll.left = rclDirty.right = stZeichengrosse.cx;
																vasInhalt->Delete(ulZeichenPos, 1);

																GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
																lTextPos = stZeichengrosse.cx - lWidth;

																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																ptlCaret.x = stZeichengrosse.cx - lTextPos;
																if(ptlCaret.x >= lWidth)	ptlCaret.x = lWidth - CARET_PIXEL;
																if(lTextPos < 0) lTextPos = 0;

																rclScroll.right = ptlCaret.x;
																ptlScrollOffset.x = rclScroll.left;
															}
															dxgiPresent.pScrollRect = &rclScroll;
															dxgiPresent.pScrollOffset = &ptlScrollOffset;
															OnRender(true);
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															dxgiPresent.pScrollRect = nullptr;
															dxgiPresent.pScrollOffset = nullptr;
														}
													}
													else{ // TXA_Mitte
														if(stZeichengrosse.cx <= lWidth){
															rclDirty.left = (lWidth - stZeichengrosse.cx) / 2;
															rclDirty.right = rclDirty.left + stZeichengrosse.cx;
															vasInhalt->Delete(ulZeichenPos, 1);
															GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
															stZeichengrosse.cx = (lWidth - stZeichengrosse.cx) / 2;
															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichenPos_Caret);
															ptlCaret.x = stZeichenPos_Caret.cx + stZeichengrosse.cx;
															OnRender(true);
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														}
														else if(!lTextPos){
															rclDirty.right = lWidth;
															vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
															GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
															rclScroll.right = rclDirty.left = rclDirty.right - stZeichengrosse.cx;
															vasInhalt->Delete(ulZeichenPos, 1);

															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
															ptlCaret.x = stZeichengrosse.cx;
															rclScroll.left = ptlCaret.x;
															ptlScrollOffset.x = rclDirty.left - rclDirty.right;
														}
														else if(stZeichengrosse.cx - lTextPos > lWidth){
															rclDirty.right = lWidth;
															vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
															GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
															rclScroll.right = rclDirty.left = rclDirty.right - stZeichengrosse.cx;

															vasInhalt->Delete(ulZeichenPos, 1);

															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
															ptlCaret.x = stZeichengrosse.cx - lTextPos;

															rclScroll.left = ptlCaret.x;
															ptlScrollOffset.x = rclDirty.left - rclDirty.right;
														}
														else{
															rclDirty.left = 0;
															vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
															GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
															rclScroll.left = rclDirty.right = stZeichengrosse.cx;

															vasInhalt->Delete(ulZeichenPos, 1);

															GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
															lTextPos = stZeichengrosse.cx - lWidth;

															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
															ptlCaret.x = stZeichengrosse.cx - lTextPos;
															ptlScrollOffset.x = rclDirty.right;
															if(lTextPos < 0){	lTextPos = 0; rclDirty.right = lWidth;	}

															rclScroll.right = ptlCaret.x;
														}
														dxgiPresent.pScrollRect = &rclScroll;
														dxgiPresent.pScrollOffset = &ptlScrollOffset;
														OnRender(true);
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														dxgiPresent.pScrollRect = nullptr;
														dxgiPresent.pScrollOffset = nullptr;
													}
												}
												ThreadSafe_End();
											}
											PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), WM_CHAR);
											break;
		default					: break;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_Char(WPARAM wParam)
{
	VMBLOCK vbZeichen; SIZE stZeichengrosse, stZeichengrosse_1; RECT rcl2Dirty[2]; RECT rcZeichnen = {0};
	switch(wParam){
		case VK_TAB    : ThreadSafe_Begin();
										 if(!ucZeichenVorgabe){ ThreadSafe_End(); break; }
										 rclDirty.right = 0;
										 do{ SendMessage(hWndElement, WM_CHAR, ' ', NULL); }
										 while(++rclDirty.right < 4);
										 ThreadSafe_End();
										 break;
		case VK_RETURN : ThreadSafe_Begin();
										 if(pfnWM_Char_Return) pfnWM_Char_Return(this);
										 ThreadSafe_End();
										 break;
		case VK_ESCAPE : ThreadSafe_Begin();
										 SetFocus(GetParent(hWndElement));
										 if(pfnWM_Char_Escape) pfnWM_Char_Escape(this);
										 ThreadSafe_End();
										 break;
		case VK_BACK   : ThreadSafe_Begin();
											if(!ucZeichenVorgabe){ ThreadSafe_End(); break; }
											if(ulZeichenPos || !ulZeichenPos && cSelect){ 
												if(!vasZeichenMaske->Length()){
													if(cSelect) Select_Loschen();
													else{
														GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
														if(ucSchriftausrichtung & TXA_LINKS){ 
															if(!lTextPos){
																if(stZeichengrosse.cx < lWidth) rclDirty.right = stZeichengrosse.cx;
																else rclDirty.right = lWidth;
																vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
																GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																rclDirty.left = rclDirty.right - stZeichengrosse.cx;

																vasInhalt->Delete(--ulZeichenPos, 1);
																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																ptlCaret.x = stZeichengrosse.cx;
																if(rclDirty.left > ptlCaret.x){
																	rclScroll.right = rclDirty.left;
																	rclScroll.left = ptlCaret.x;
																	ptlScrollOffset.x = rclDirty.left - rclDirty.right;
																	dxgiPresent.pScrollRect = &rclScroll;
																	dxgiPresent.pScrollOffset = &ptlScrollOffset;
																	OnRender(true);
																	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																	dxgiPresent.pScrollRect = nullptr;
																	dxgiPresent.pScrollOffset = nullptr;
																}
																else{
																	rclDirty.right += CARET_PIXEL;
																	OnRender(true);
																	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																}
															}
															else{
																if(stZeichengrosse.cx - lTextPos > lWidth){
																	rclDirty.right = lWidth;
																	vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
																	GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																	rclDirty.left = rclDirty.right - stZeichengrosse.cx;

																	vasInhalt->Delete(--ulZeichenPos, 1);
																	GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																	ptlCaret.x = stZeichengrosse.cx - lTextPos;
																	rclScroll.right = rclDirty.left;
																	rclScroll.left = ptlCaret.x;
																	ptlScrollOffset.x = rclDirty.left - rclDirty.right;
																	dxgiPresent.pScrollRect = &rclScroll;
																	dxgiPresent.pScrollOffset = &ptlScrollOffset;
																	OnRender(true);
																	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																	dxgiPresent.pScrollRect = nullptr;
																	dxgiPresent.pScrollOffset = nullptr;
																}
																else{
																	rclDirty.left = 0;
																	vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
																	GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																	rclScroll.left = rclDirty.right = stZeichengrosse.cx;
																	rclScroll.right = ptlCaret.x;
																	ptlScrollOffset.x = stZeichengrosse.cx;
																	vasInhalt->Delete(--ulZeichenPos, 1);

																	GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
																	lTextPos = stZeichengrosse.cx - lWidth;

																	GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																	ptlCaret.x = stZeichengrosse.cx - lTextPos;
																	if(lTextPos < 0){ lTextPos = 0;	rclDirty.right = lWidth; }

																	dxgiPresent.pScrollRect = &rclScroll;
																	dxgiPresent.pScrollOffset = &ptlScrollOffset;
																	OnRender(true);
																	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																	dxgiPresent.pScrollRect = nullptr;
																	dxgiPresent.pScrollOffset = nullptr;
																}
															}
														}
														else if(ucSchriftausrichtung & TXA_RECHTS){ 
															if(stZeichengrosse.cx <= lWidth){
																rclDirty.left = lWidth - stZeichengrosse.cx;
																vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
																GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																rclScroll.left = rclDirty.right = rclDirty.left + stZeichengrosse.cx;
																if(rclDirty.left < stZeichengrosse.cx) rclDirty.left = 0;
																rclScroll.right = ptlCaret.x;
																vasInhalt->Delete(--ulZeichenPos, 1);
																if(rclScroll.right == rclScroll.left){
																	OnRender(true);
																	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																}
																else{
																	if(rclScroll.right > lWidth) rclScroll.right = lWidth;
																	ptlScrollOffset.x = stZeichengrosse.cx;
																	dxgiPresent.pScrollRect = &rclScroll;
																	dxgiPresent.pScrollOffset = &ptlScrollOffset;
																	GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																	ptlCaret.x = rclDirty.right + stZeichengrosse.cx;
																	if(ptlCaret.x >= lWidth) ptlCaret.x = lWidth - CARET_PIXEL;
																	OnRender(true);
																	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																	dxgiPresent.pScrollRect = nullptr;
																	dxgiPresent.pScrollOffset = nullptr;
																}
															}
															else if(!lTextPos){
																rclDirty.left = stZeichengrosse.cx - lWidth;
																vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
																GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																if(rclDirty.left < stZeichengrosse.cx){
																	rcl2Dirty[0].top = rcl2Dirty[1].top = rclDirty.top;
																	rcl2Dirty[0].bottom = rcl2Dirty[1].bottom = rclDirty.bottom;
																	rcl2Dirty[0].right = lWidth;
																	rcl2Dirty[1].left = 0;
																	ptlCaret.x = stZeichengrosse.cx - rclDirty.left;
																	vasInhalt->Delete(--ulZeichenPos, 1);
																	GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																	ptlCaret.x += stZeichengrosse.cx;
																	if(ptlCaret.x >= lWidth) ptlCaret.x = lWidth - CARET_PIXEL;
																	rcl2Dirty[1].right = ptlCaret.x;
																	ptlScrollOffset.x = rclDirty.left * -1;
																	rclScroll.right = rcl2Dirty[0].left = rcl2Dirty[0].right - rclDirty.left;
																	dxgiPresent.DirtyRectsCount = 2;
																	dxgiPresent.pDirtyRects = rcl2Dirty;
																	dxgiPresent.pScrollRect = &rclScroll;
																	dxgiPresent.pScrollOffset = &ptlScrollOffset;
																	OnRender(true);
																	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																	dxgiPresent.pScrollRect = nullptr;
																	dxgiPresent.pScrollOffset = nullptr;
																	dxgiPresent.DirtyRectsCount = 1;
																	dxgiPresent.pDirtyRects = &rclDirty;
																}
																else{
																	rclDirty.right = lWidth;
																	ptlScrollOffset.x = stZeichengrosse.cx * -1;
																	rclScroll.right = rclDirty.left = rclDirty.right - stZeichengrosse.cx;
																	vasInhalt->Delete(--ulZeichenPos, 1);
																	GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																	rclScroll.left = ptlCaret.x = stZeichengrosse.cx;
																	dxgiPresent.pScrollRect = &rclScroll;
																	dxgiPresent.pScrollOffset = &ptlScrollOffset;
																	OnRender(true);
																	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																	dxgiPresent.pScrollRect = nullptr;
																	dxgiPresent.pScrollOffset = nullptr;
																}
															}
															else{
																if(stZeichengrosse.cx - lTextPos > lWidth){
																	vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
																	GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																	vasInhalt->Delete(--ulZeichenPos, 1);
																	rclDirty.right = lWidth;
																	rclScroll.right = rclDirty.left = lWidth - stZeichengrosse.cx;
																	GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																	ptlCaret.x = stZeichengrosse.cx - lTextPos;
																	rclScroll.left = ptlCaret.x;
																	ptlScrollOffset.x = rclDirty.left - rclDirty.right;
																}
																else{
																	rclDirty.left = 0;
																	vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
																	GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																	rclScroll.left = rclDirty.right = stZeichengrosse.cx;
																	vasInhalt->Delete(--ulZeichenPos, 1);

																	GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
																	lTextPos = stZeichengrosse.cx - lWidth;

																	GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																	ptlCaret.x = stZeichengrosse.cx - lTextPos;
																	if(ptlCaret.x >= lWidth)	ptlCaret.x = lWidth - CARET_PIXEL;
																	if(lTextPos < 0) lTextPos = 0;

																	rclScroll.right = ptlCaret.x;
																	ptlScrollOffset.x = rclScroll.left;
																}
																dxgiPresent.pScrollRect = &rclScroll;
																dxgiPresent.pScrollOffset = &ptlScrollOffset;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																dxgiPresent.pScrollRect = nullptr;
																dxgiPresent.pScrollOffset = nullptr;
															}
														}
														else{ // TXA_Mitte
															if(stZeichengrosse.cx <= lWidth){
																rclDirty.left = (lWidth - stZeichengrosse.cx) / 2;
																rclDirty.right = rclDirty.left + stZeichengrosse.cx;
																vasInhalt->Delete(--ulZeichenPos, 1);
																GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
																stZeichengrosse.cx = (lWidth - stZeichengrosse.cx) / 2;
																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse_1);
																ptlCaret.x = stZeichengrosse_1.cx + stZeichengrosse.cx;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															else if(!lTextPos){
																rclDirty.right = lWidth;
																vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
																GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																rclScroll.right = rclDirty.left = rclDirty.right - stZeichengrosse.cx;
																vasInhalt->Delete(--ulZeichenPos, 1);

																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																ptlCaret.x = stZeichengrosse.cx;
																rclScroll.left = ptlCaret.x;
																ptlScrollOffset.x = rclDirty.left - rclDirty.right;
															}
															else if(stZeichengrosse.cx - lTextPos > lWidth){
																rclDirty.right = lWidth;
																vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
																GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																rclScroll.right = rclDirty.left = rclDirty.right - stZeichengrosse.cx;

																vasInhalt->Delete(--ulZeichenPos, 1);

																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																ptlCaret.x = stZeichengrosse.cx - lTextPos;

																rclScroll.left = ptlCaret.x;
																ptlScrollOffset.x = rclDirty.left - rclDirty.right;
															}
															else{
																rclDirty.left = 0;
																vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
																GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																rclScroll.left = rclDirty.right = stZeichengrosse.cx;

																vasInhalt->Delete(--ulZeichenPos, 1);

																GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
																lTextPos = stZeichengrosse.cx - lWidth;

																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																ptlCaret.x = stZeichengrosse.cx - lTextPos;
																ptlScrollOffset.x = rclDirty.right;
																if(lTextPos < 0){ lTextPos = 0; rclDirty.right = lWidth; }

																rclScroll.right = ptlCaret.x;
															}
															dxgiPresent.pScrollRect = &rclScroll;
															dxgiPresent.pScrollOffset = &ptlScrollOffset;
															OnRender(true);
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															dxgiPresent.pScrollRect = nullptr;
															dxgiPresent.pScrollOffset = nullptr;
														}
													}
												}
												else{
													if(cSelect){
														if(cSelect < 0) ulZeichenPos = ulSelectPos;
														DeSelect();
														GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
														ptlCaret.x = stZeichengrosse.cx;	
													}

													if(ZeichenMaske_Loschen()){
														vasInhalt->Delete(--ulZeichenPos, 1);
														(ucZeichenVorgabe & ZV_SICHTBAR ? vasInhalt->Insert("_", ulZeichenPos) : vasInhalt->Insert(" ", ulZeichenPos));
														rclDirty.right = 1;
													}
													else rclDirty.right = 0;

													if(ucSchriftausrichtung & TXA_LINKS) ptlCaret.x = 0;	
													else if(ucSchriftausrichtung & TXA_RECHTS){
														GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
														ptlCaret.x = lWidth - stZeichengrosse.cx;
													}
													else{
														GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
														ptlCaret.x = (lWidth - stZeichengrosse.cx) / 2;
													}
													GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
													ptlCaret.x += stZeichengrosse.cx;

													if(rclDirty.right){ 
														rclDirty.left = ptlCaret.x; rclDirty.right = lWidth;
														OnRender(true);
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													}
												}
											} 
											ThreadSafe_End();
											PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), WM_CHAR);
											break;
		default        :  ThreadSafe_Begin();
											if(ZeichenVorgabe(wParam) && ulZeichenPos < ulZeichen_max){
												if(!vasZeichenMaske->Length()){
													if(cSelect) Select_Loschen();
													GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
													if(ucSchriftausrichtung & TXA_LINKS){
														if(stZeichengrosse.cx - lTextPos < lWidth){
															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
															rclDirty.left = stZeichengrosse.cx - lTextPos;
															GetTextPoint((LPSTR)&wParam, 1, stZeichengrosse);
															rclDirty.right = ptlCaret.x = rclDirty.left + stZeichengrosse.cx;
															if(rclDirty.right < lWidth){
																if(ulZeichenPos == vasInhalt->Length()){
																	*vasInhalt += (char*)&wParam;
																	ulZeichenPos++;

																	OnRender(true);
																	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																}
																else{
																	vasInhalt->Insert((char*)&wParam, ulZeichenPos);
																	GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
																	if(stZeichengrosse.cx < lWidth){
																		rclScroll.left = rclDirty.right;
																		ptlScrollOffset.x = rclDirty.right - rclDirty.left;
																		rclScroll.right = stZeichengrosse.cx - lTextPos;
																		ulZeichenPos++;

																		dxgiPresent.pScrollRect = &rclScroll;
																		dxgiPresent.pScrollOffset = &ptlScrollOffset;
																		OnRender(true);
																		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																		dxgiPresent.pScrollRect = nullptr;
																		dxgiPresent.pScrollOffset = nullptr;
																	}
																	else if(ucZeichenVorgabe & ZV_UBERLANGE){
																		ptlScrollOffset.x = rclDirty.left - rclDirty.right;
																		rclDirty.right = ptlCaret.x = rclDirty.left;
																		rclScroll.right = rclDirty.left += ptlScrollOffset.x;
																		rclScroll.left = 0;
																		lTextPos -= ptlScrollOffset.x;
																		ulZeichenPos++;

																		dxgiPresent.pScrollRect = &rclScroll;
																		dxgiPresent.pScrollOffset = &ptlScrollOffset;
																		OnRender(true);
																		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																		dxgiPresent.pScrollRect = nullptr;
																		dxgiPresent.pScrollOffset = nullptr;
																	}
																	else{
																		vasInhalt->Delete(ulZeichenPos, 1);
																		ptlCaret.x = rclDirty.left;
																	}
																}
															}
															else if(ucZeichenVorgabe & ZV_UBERLANGE){
																*vasInhalt += (char*)&wParam;
																ptlScrollOffset.x = lWidth - rclDirty.right;
																rclScroll.left = 0;
																rclDirty.right = lWidth;
																rclDirty.left = rclScroll.right = rclDirty.right - stZeichengrosse.cx;
																rclDirty.left -= CARET_PIXEL * 2;
																GetTextPoint(vasInhalt->c_Str(), ++ulZeichenPos, stZeichengrosse);
																lTextPos = stZeichengrosse.cx - lWidth;
																ptlCaret.x = lWidth - CARET_PIXEL;

																dxgiPresent.pScrollRect = &rclScroll;
																dxgiPresent.pScrollOffset = &ptlScrollOffset;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																dxgiPresent.pScrollRect = nullptr;
																dxgiPresent.pScrollOffset = nullptr;
															}
														}
														else if(ucZeichenVorgabe & ZV_UBERLANGE){
															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
															rclDirty.left = stZeichengrosse.cx - lTextPos;
															GetTextPoint((LPSTR)&wParam, 1, stZeichengrosse);
															rclDirty.right = rclDirty.left + stZeichengrosse.cx;
															if(rclDirty.right < lWidth){
																vasInhalt->Insert((char*)&wParam, ulZeichenPos);

																rclScroll.left = rclDirty.right;
																ptlScrollOffset.x = stZeichengrosse.cx;
																rclScroll.right = lWidth;
																ptlCaret.x = rclDirty.right;

																dxgiPresent.pScrollRect = &rclScroll;
																dxgiPresent.pScrollOffset = &ptlScrollOffset;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																dxgiPresent.pScrollRect = nullptr;
																dxgiPresent.pScrollOffset = nullptr;
															}
															else{
																ulZeichenPos == vasInhalt->Length() ? *vasInhalt += (char*)&wParam 
																	: vasInhalt->Insert((char*)&wParam, ulZeichenPos);

																rclScroll.left = 0;
																rclDirty.left = lWidth - stZeichengrosse.cx;
																rclDirty.left -= CARET_PIXEL * 2;
																rclScroll.right = rclDirty.left;
																ptlScrollOffset.x = lWidth - rclDirty.right - CARET_PIXEL;
																lTextPos -= ptlScrollOffset.x;
																rclDirty.right = lWidth;
																ptlCaret.x = lWidth - CARET_PIXEL;

																dxgiPresent.pScrollRect = &rclScroll;
																dxgiPresent.pScrollOffset = &ptlScrollOffset;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																dxgiPresent.pScrollRect = nullptr;
																dxgiPresent.pScrollOffset = nullptr;

															}
															ulZeichenPos++;
														}
													}
													else if(ucSchriftausrichtung & TXA_RECHTS){
														vasInhalt->Insert((char*)&wParam, ulZeichenPos);
														GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse_1);
														if(stZeichengrosse_1.cx < lWidth){
															if(!ulZeichenPos){
																GetTextPoint((LPSTR)&wParam, 1, stZeichengrosse);
																if(ptlCaret.x < stZeichengrosse.cx){
																	rclDirty.left = 0;
																	rclDirty.right = rclScroll.left = stZeichengrosse.cx;
																	rclDirty.right += CARET_PIXEL;
																	rclScroll.right = lWidth;
																	ptlScrollOffset.x = stZeichengrosse.cx - ptlCaret.x;
																	ptlCaret.x = 0;

																	dxgiPresent.pScrollRect = &rclScroll;
																	dxgiPresent.pScrollOffset = &ptlScrollOffset;
																	OnRender(true);
																	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																	dxgiPresent.pScrollRect = nullptr;
																	dxgiPresent.pScrollOffset = nullptr;

																}
																else{
																	rclDirty.right = ptlCaret.x;
																	rclDirty.left = ptlCaret.x - stZeichengrosse.cx;
																	ulZeichenPos++;

																	OnRender(true);
																	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																}
															}
															else{
																rclScroll.left = lWidth - stZeichengrosse.cx;
																GetTextPoint((LPSTR)&wParam, 1, stZeichengrosse);
																rclDirty.right = ptlCaret.x;
																rclDirty.left = ptlCaret.x - stZeichengrosse.cx - CARET_PIXEL;
																rclScroll.right = rclDirty.left;
																rclScroll.left -= stZeichengrosse.cx;
																if(rclScroll.left < 0){
																	lTextPos -= rclScroll.left;
																	rclScroll.left = 0;
																}
																ptlScrollOffset.x = stZeichengrosse.cx * -1;
																ulZeichenPos++;

																dxgiPresent.pScrollRect = &rclScroll;
																dxgiPresent.pScrollOffset = &ptlScrollOffset;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																dxgiPresent.pScrollRect = nullptr;
																dxgiPresent.pScrollOffset = nullptr;

															}
														}
														else if(ucZeichenVorgabe & ZV_UBERLANGE){
															if(!ptlCaret.x){
																GetTextPoint((LPSTR)&wParam, 1, stZeichengrosse);
																rclDirty.left = 0;
																rclScroll.right = lWidth;
																rclDirty.right = rclScroll.left = stZeichengrosse.cx;
																rclDirty.right += CARET_PIXEL;
																ptlScrollOffset.x = stZeichengrosse.cx;

																dxgiPresent.pScrollRect = &rclScroll;
																dxgiPresent.pScrollOffset = &ptlScrollOffset;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																dxgiPresent.pScrollRect = nullptr;
																dxgiPresent.pScrollOffset = nullptr;
															}
															else{
																rclScroll.left = 0;
																GetTextPoint((LPSTR)&wParam, 1, stZeichengrosse);
																rclDirty.right = ptlCaret.x;
																rclDirty.left = ptlCaret.x - stZeichengrosse.cx - CARET_PIXEL;
																rclScroll.right = ptlCaret.x;
																ptlScrollOffset.x = stZeichengrosse.cx * -1;
																ulZeichenPos++;
																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																lTextPos = stZeichengrosse.cx - ptlCaret.x;

																dxgiPresent.pScrollRect = &rclScroll;
																dxgiPresent.pScrollOffset = &ptlScrollOffset;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																dxgiPresent.pScrollRect = nullptr;
																dxgiPresent.pScrollOffset = nullptr;
															}
														}
														else vasInhalt->Delete(ulZeichenPos, 1);
													}
													else{ // TXA_MITTE
														vasInhalt->Insert((char*)&wParam, ulZeichenPos);
														GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
														rclDirty.left = (lWidth - stZeichengrosse.cx) / 2;
														if(stZeichengrosse.cx < lWidth){
															if(rclDirty.left < 0){
																lTextPos = rclDirty.left * -1;
																rclDirty.left = 0;
																rclDirty.right = lWidth;
															}
															else rclDirty.right = rclDirty.left + stZeichengrosse.cx;

															GetTextPoint(vasInhalt->c_Str(), ++ulZeichenPos, stZeichengrosse);
															ptlCaret.x = rclDirty.left + stZeichengrosse.cx;

															OnRender(true);
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														}
														else if(ucZeichenVorgabe & ZV_UBERLANGE){
															rclDirty.left *= -1;
															if(lTextPos < rclDirty.left) lTextPos = rclDirty.left;
															rclDirty.right = lWidth;

															GetTextPoint(vasInhalt->c_Str(), ++ulZeichenPos, stZeichengrosse);
															ptlCaret.x = stZeichengrosse.cx - lTextPos;

															if(ptlCaret.x < lWidth){
																rclDirty.left = 0;

																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															else{
																GetTextPoint((LPSTR)&wParam, 1, stZeichengrosse);
																ptlScrollOffset.x = stZeichengrosse.cx * -1;
																if(lTextPos == rclDirty.left) lTextPos += lWidth - ptlCaret.x + stZeichengrosse.cx;
																else{
																	GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																	lTextPos = stZeichengrosse.cx - lWidth;
																}

																rclScroll.left = 0;
																rclScroll.right = rclDirty.left = lWidth + ptlScrollOffset.x;
																rclDirty.left -= CARET_PIXEL * 2;
																rclDirty.right = lWidth;
																ptlCaret.x = lWidth - CARET_PIXEL;

																dxgiPresent.pScrollRect = &rclScroll;
																dxgiPresent.pScrollOffset = &ptlScrollOffset;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																dxgiPresent.pScrollRect = nullptr;
																dxgiPresent.pScrollOffset = nullptr;
															}
														}
														else vasInhalt->Delete(ulZeichenPos, 1);
													}
												}
												else{
													if(cSelect){
														if(cSelect > 0){
															ulZeichenPos = ulSelectPos;
															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
															ptlCaret.x = stZeichengrosse.cx;
														}
														DeSelect();
													}
													else{
														if(ZeichenMaske_Einfugen(wParam)){
															vasInhalt->Delete(ulZeichenPos, 1); vasInhalt->Insert((char*)&wParam, ulZeichenPos++);
															if(ucSchriftausrichtung & TXA_LINKS) ptlCaret.x = 0;
															else if(ucSchriftausrichtung & TXA_RECHTS){
																GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
																ptlCaret.x = lWidth - stZeichengrosse.cx;
															}
															else{ // TXA_MITTE
																GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
																ptlCaret.x = (lWidth - stZeichengrosse.cx) / 2;
															}

															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
															ptlCaret.x += stZeichengrosse.cx;
															rclDirty.left = 0; rclDirty.right = ptlCaret.x;
															OnRender(true);
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														}
													}
												}
											}
											ThreadSafe_End();
											PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), WM_CHAR);
											break;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COEditLine::WM_Command(WPARAM wParam)
{
	VMBLOCK vbZeichen; HGLOBAL hGlobal; char* pcAblage; ULONG ulZeichen; SIZE szZeichen_Inhalt, szZeichen_Clipboard; RECT rcl2Dirty[2];
	switch(LOWORD(wParam)){
		case IDM_KOPIEREN			: ThreadSafe_Begin();
														OpenClipboard(hWndElement); EmptyClipboard();
														if(cSelect > 0) ulZeichen = vasInhalt->SubString(vbZeichen, ulSelectPos + 1, ulZeichenPos);
														else ulZeichen = vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulSelectPos);
														hGlobal = GlobalAlloc(GMEM_MOVEABLE, ulZeichen + 1);
														pcAblage = (char*)GlobalLock(hGlobal);
														MemCopy(pcAblage, vbZeichen, ulZeichen); VMFrei(vbZeichen);
														pcAblage[ulZeichen] = 0;
														GlobalUnlock(hGlobal);
														SetClipboardData(CF_TEXT, hGlobal); CloseClipboard();
														ThreadSafe_End();
														return false;
		case IDM_AUSSCHNEIDEN : ThreadSafe_Begin();
														if(vasZeichenMaske->Length() || !ucZeichenVorgabe){ ThreadSafe_End(); return false; }
														OpenClipboard(hWndElement); EmptyClipboard();
														DeSelect();
														GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), szZeichen_Inhalt);
														if(ulSelectPos <= ulZeichenPos){
															ulZeichen = vasInhalt->SubString(vbZeichen, ulSelectPos + 1, ulZeichenPos);
															vasInhalt->Delete(ulSelectPos, ulZeichen);
															ulZeichenPos = ulSelectPos;
														}
														else{
															ulZeichen = vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulSelectPos);
															vasInhalt->Delete(ulZeichenPos, ulZeichen);
														}

														hGlobal = GlobalAlloc(GMEM_MOVEABLE, ulZeichen + 1);
														pcAblage = (char*)GlobalLock(hGlobal);
														MemCopy(pcAblage, vbZeichen, ulZeichen); VMFrei(vbZeichen);
														pcAblage[ulZeichen] = 0;
														GlobalUnlock(hGlobal);
														SetClipboardData(CF_TEXT, hGlobal); CloseClipboard();
														GetTextPoint(pcAblage, ulZeichen, szZeichen_Clipboard);

														if(ucSchriftausrichtung & TXA_LINKS){
															if(lTextPos){
																if(lTextPos >= szZeichen_Clipboard.cx){
																	ptlScrollOffset.x = szZeichen_Clipboard.cx;
																	rclScroll.left = rclDirty.right = szZeichen_Clipboard.cx;
																	rclDirty.left = 0;
																	rclScroll.right = rclSelect.right;

																	lTextPos -= szZeichen_Clipboard.cx;
																	ptlCaret.x = rclSelect.right - CARET_PIXEL;
																}
																else{
																	lTextPos = 0;
																	rclDirty.left = 0;
																	rclDirty.right = lWidth;
																	GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, szZeichen_Inhalt);
																	ptlCaret.x = szZeichen_Inhalt.cx;
																}
															}
															else{
																ptlScrollOffset.x = szZeichen_Clipboard.cx * -1;
																rclScroll.left = ptlCaret.x;
																if(szZeichen_Inhalt.cx > lWidth){
																	rclDirty.left = rclScroll.right = lWidth - szZeichen_Clipboard.cx;
																	rclDirty.right = lWidth;
																}
																else{
																	rclDirty.left = rclScroll.right = szZeichen_Inhalt.cx - szZeichen_Clipboard.cx;
																	rclDirty.right = szZeichen_Inhalt.cx;
																}
															}
															dxgiPresent.pScrollRect = &rclScroll;
															dxgiPresent.pScrollOffset = &ptlScrollOffset;
															OnRender(true);
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															dxgiPresent.pScrollRect = nullptr;
															dxgiPresent.pScrollOffset = nullptr;
														}
														else if(ucSchriftausrichtung & TXA_RECHTS){
															if(szZeichen_Inhalt.cx - lTextPos > lWidth){
																if(szZeichen_Inhalt.cx - lWidth - lTextPos >= szZeichen_Clipboard.cx){
																	rclScroll.left = ptlCaret.x = rclSelect.left;
																	rclDirty.right = lWidth;
																	rclDirty.left = rclScroll.right = lWidth - szZeichen_Clipboard.cx;
																	ptlScrollOffset.x = szZeichen_Clipboard.cx * -1;

																	dxgiPresent.pScrollRect = &rclScroll;
																	dxgiPresent.pScrollOffset = &ptlScrollOffset;
																	OnRender(true);
																	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																	dxgiPresent.pScrollRect = nullptr;
																	dxgiPresent.pScrollOffset = nullptr;
																}
																else{
																	lTextPos += (szZeichen_Inhalt.cx - lWidth - lTextPos) - szZeichen_Clipboard.cx;
																	GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, szZeichen_Inhalt);
																	ptlCaret.x = szZeichen_Inhalt.cx - lTextPos;
																	if(lTextPos < 0) lTextPos = 0;
																	rclDirty.right = lWidth;
																	rclDirty.left = 0;

																	OnRender(true);
																	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																}
															}
															else{
																ptlCaret.x = rclScroll.right = rclSelect.right;
																ptlCaret.x = rclSelect.right - CARET_PIXEL;
																rclDirty.left = 0;
																rclDirty.right = rclScroll.left = szZeichen_Clipboard.cx;
																ptlScrollOffset.x = szZeichen_Clipboard.cx;
																
																lTextPos -= szZeichen_Clipboard.cx;
																if(lTextPos < 0) lTextPos = 0;

																dxgiPresent.pScrollRect = &rclScroll;
																dxgiPresent.pScrollOffset = &ptlScrollOffset;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																dxgiPresent.pScrollRect = nullptr;
																dxgiPresent.pScrollOffset = nullptr;
															}
														}
														else{ // TXA_MITTE
															lTextPos = ((szZeichen_Inhalt.cx - szZeichen_Clipboard.cx) - lWidth) / 2;
															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, szZeichen_Inhalt);
															ptlCaret.x = szZeichen_Inhalt.cx - lTextPos;
															if(lTextPos < 0) lTextPos = 0;
															rclDirty.left = 0;
															rclDirty.right = lWidth;

															OnRender(true);
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														}
														ThreadSafe_End();
														return false;
		case IDM_EINFUGEN	:			ThreadSafe_Begin();
														if(!IsClipboardFormatAvailable(CF_TEXT) || !ucZeichenVorgabe){ ThreadSafe_End(); return false; }
														if(vasZeichenMaske->Length()){ ThreadSafe_End(); return false; }
														if(cSelect) Select_Loschen();
														GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), szZeichen_Inhalt);

														OpenClipboard(hWndElement);
														hGlobal = GetClipboardData(CF_TEXT);
														pcAblage = (char*)GlobalLock(hGlobal);
														ulZeichen = StrLength(pcAblage);
														GlobalUnlock(hGlobal);
														CloseClipboard();
														GetTextPoint(pcAblage, ulZeichen, szZeichen_Clipboard);

														szZeichen_Inhalt.cx -= lTextPos;
														if(ucSchriftausrichtung & TXA_LINKS){
															if(szZeichen_Inhalt.cx + szZeichen_Clipboard.cx < lWidth){
																ulZeichenPos == vasInhalt->Length() ? *vasInhalt += pcAblage : vasInhalt->Insert(pcAblage, ulZeichenPos);
																ulZeichenPos += ulZeichen;
																ptlCaret.x += szZeichen_Clipboard.cx;

																rclDirty.left = 0; rclDirty.right = lWidth;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															else if(ucZeichenVorgabe & ZV_UBERLANGE){
																ulZeichenPos == vasInhalt->Length() ? *vasInhalt += pcAblage : vasInhalt->Insert(pcAblage, ulZeichenPos);
																ulZeichenPos += ulZeichen;
																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, szZeichen_Inhalt);
																if(szZeichen_Inhalt.cx - lTextPos > lWidth) lTextPos = szZeichen_Inhalt.cx - lWidth;

																szZeichen_Inhalt.cx - lTextPos < lWidth ? ptlCaret.x = szZeichen_Inhalt.cx - lTextPos
																	: ptlCaret.x = lWidth - CARET_PIXEL;

																rclDirty.left = 0; rclDirty.right = lWidth;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															// tue nix
														}
														else if(ucSchriftausrichtung & TXA_RECHTS){
															if(szZeichen_Inhalt.cx + szZeichen_Clipboard.cx < lWidth){
																ulZeichenPos == vasInhalt->Length() ? *vasInhalt += pcAblage : vasInhalt->Insert(pcAblage, ulZeichenPos);
																ulZeichenPos += ulZeichen;

																rclDirty.left = 0; rclDirty.right = lWidth;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															else if(ucZeichenVorgabe & ZV_UBERLANGE){
																if(ulZeichenPos == vasInhalt->Length()){
																	*vasInhalt += pcAblage;
																	ptlCaret.x = lWidth - CARET_PIXEL;
																}
																else vasInhalt->Insert(pcAblage, ulZeichenPos);
																ulZeichenPos += ulZeichen;
																lTextPos += szZeichen_Clipboard.cx;

																rclDirty.left = 0; rclDirty.right = lWidth;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															// tue nix
														}
														else{
															if(szZeichen_Inhalt.cx + szZeichen_Clipboard.cx < lWidth){
																ulZeichenPos == vasInhalt->Length() ? *vasInhalt += pcAblage : vasInhalt->Insert(pcAblage, ulZeichenPos);
																ulZeichenPos += ulZeichen;
																ptlCaret.x += szZeichen_Clipboard.cx / 2;

																rclDirty.left = 0; rclDirty.right = lWidth;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															else if(ucZeichenVorgabe & ZV_UBERLANGE){
																if(ulZeichenPos == vasInhalt->Length()){
																	*vasInhalt += pcAblage;
																	lTextPos = (szZeichen_Inhalt.cx + szZeichen_Clipboard.cx) - lWidth;
																	ptlCaret.x = lWidth - CARET_PIXEL;
																}
																else{
																	vasInhalt->Insert(pcAblage, ulZeichenPos);
																	lTextPos = ((szZeichen_Inhalt.cx + szZeichen_Clipboard.cx) - lWidth) / 2;
																	ptlCaret.x += szZeichen_Clipboard.cx / 2;
																}
																ulZeichenPos += ulZeichen;

																rclDirty.left = 0; rclDirty.right = lWidth;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															// tue nix
														}
														ThreadSafe_End();
														return false;
		default               : return true;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_ContexMenu(LPARAM lParam)
{
	ThreadSafe_Begin();
	if(vasZeichenMaske->Length()){
		EnableMenuItem(hMenu, IDM_AUSSCHNEIDEN, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_EINFUGEN, MF_BYCOMMAND | MF_GRAYED);
	}
	else{
		EnableMenuItem(hMenu, IDM_AUSSCHNEIDEN, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(hMenu, IDM_EINFUGEN, MF_BYCOMMAND | MF_ENABLED);
	}

	POINT ptPosition;
	ptPosition.x = LOWORD(lParam); ptPosition.y = HIWORD(lParam);
	if(ptPosition.x == USHRT_MAX && ptPosition.y == USHRT_MAX) ClientToScreen(GetParent(hWndElement), &Position(ptPosition));
	TrackPopupMenuEx(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, ptPosition.x, ptPosition.y, hWndElement, nullptr);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_MouseMove(WPARAM wParam, LPARAM lParam)
{
	if(hWndElement == GetFocus() && wParam == MK_LBUTTON){
		ThreadSafe_Begin();
		if(GET_X_LPARAM(lParam) < ptlCaret.x - lZeichen_mittel) SendMessage(hWndElement, WM_KEYDOWN, VK_LEFT, NULL);
		else if(GET_X_LPARAM(lParam) > ptlCaret.x + lZeichen_mittel) SendMessage(hWndElement, WM_KEYDOWN, VK_RIGHT, NULL);
		ThreadSafe_End();
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_LButtonDown(WPARAM wParam, LPARAM lParam)
{
	SetCapture(hWndElement);
	ThreadSafe_Begin();
	if(hWndElement == GetFocus()) DeleteCaretPos();
	else SetFocus(hWndElement);
	if(cSelect) DeSelect();

	ulZeichenPos = 0;
	if(vasInhalt->Length()){ SIZE szZeichen_Inhalt;
		if(lTextPos || ucSchriftausrichtung & TXA_LINKS){
			do{ GetTextPoint(vasInhalt->c_Str(), ++ulZeichenPos, szZeichen_Inhalt); }
			while(szZeichen_Inhalt.cx - lTextPos < LOWORD(lParam) && ulZeichenPos < vasInhalt->Length());
			ptlCaret.x = szZeichen_Inhalt.cx - lTextPos;

			if(vasZeichenMaske->Length()){
				if(ZeichenMaske_FestRechts()){
					GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, szZeichen_Inhalt);
					ptlCaret.x = szZeichen_Inhalt.cx;
				}
			}
		}
		else{
			GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), szZeichen_Inhalt);
			if(szZeichen_Inhalt.cx > lWidth){
				do{ GetTextPoint(vasInhalt->c_Str(), ++ulZeichenPos, szZeichen_Inhalt); }
				while(szZeichen_Inhalt.cx < LOWORD(lParam) && ulZeichenPos < vasInhalt->Length());
				ptlCaret.x = szZeichen_Inhalt.cx;
			}
			else if(ucSchriftausrichtung & TXA_RECHTS) ptlCaret.x = lWidth - szZeichen_Inhalt.cx;
			else ptlCaret.x = (lWidth - szZeichen_Inhalt.cx) / 2;

			if(LOWORD(lParam) > ptlCaret.x){
				do{ GetTextPoint(vasInhalt->c_Str(), ++ulZeichenPos, szZeichen_Inhalt);}
				while(szZeichen_Inhalt.cx + ptlCaret.x < LOWORD(lParam) && ulZeichenPos < vasInhalt->Length());
			 
				if(vasZeichenMaske->Length()){
					if(ZeichenMaske_FestRechts()) GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, szZeichen_Inhalt);
				}
				ptlCaret.x += szZeichen_Inhalt.cx;
			} 
		}
		if(ptlCaret.x == lWidth) ptlCaret.x -= CARET_PIXEL;
	}

	if(pfnWM_LButtonDown) pfnWM_LButtonDown(this, wParam, lParam);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_LButtonUp(WPARAM wParam, LPARAM lParam)
{
	ReleaseCapture();
	ThreadSafe_Begin();
	if(pfnWM_LButtonUp) pfnWM_LButtonUp(this, wParam, lParam);
	else PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), WM_LBUTTONUP);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_LButtonDBClick(WPARAM wParam, LPARAM lParam)
{
	ThreadSafe_Begin();
	SelectAlles();
	if(pfnWM_LButtonDBClick) pfnWM_LButtonDBClick(this, wParam, lParam);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SetzVerfugbar(bool bVerfugbar)
{
	if(bVerfugbar){ 
		if(!IsWindowVisible(hWndElement)) ShowWindow(hWndElement, SW_SHOW);
		if(!IsWindowEnabled(hWndElement)) EnableWindow(hWndElement, bVerfugbar);
		else if(hWndElement == GetFocus()) SendMessage(hWndElement, WM_SETFOCUS, NULL, NULL);
	}
	else{ if(hWndElement == GetFocus()) SetFocus(GetParent(hWndElement)); EnableWindow(hWndElement, bVerfugbar); }
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COEditLine::ZeichenVorgabe(WPARAM wParam)
{
	for(BYTE ucBit = 0; ucBit < 4; ucBit++){
		switch(ucBit){
			case 0   : if(ucZeichenVorgabe & (1 << ucBit)){
										if(wParam >= 0x41 && wParam <= 0x5a || wParam >= 0x61 && wParam <= 0x7a || wParam == 0x20) return true;
									} break;
			case 1   : if(ucZeichenVorgabe & (1 << ucBit)){ 
										if(wParam >= 0x30 && wParam <= 0x39) return true;
									} break;
			case 2   : if(ucZeichenVorgabe & (1 << ucBit)){
										if(wParam >= 0x80 && wParam <= 0xff) return true;
									} break;
			case 3   : if(ucZeichenVorgabe & (1 << ucBit)){ 
										if(wParam >= 0x20 && wParam <= 0x2f || wParam >= 0x3a && wParam <= 0x40 || 
											wParam >= 0x5b && wParam <= 0x60 || wParam >= 0x7b && wParam <= 0x7f) return true;
									} break;
		}
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COEditLine::ZeichenMaske_Einfugen(WPARAM wParam)
{
	VMBLOCK vbZeichen_Maske; ULONG ulZeichen; BYTE ucFesteZeichen = 1;

	for(ulZeichen = 1; ulZeichen <= ulZeichenPos + ucFesteZeichen; ulZeichen++){
		vasZeichenMaske->SubString(vbZeichen_Maske, ulZeichen, ulZeichen);
		if(*(PBYTE)vbZeichen_Maske == 0x27){ VMFrei(vbZeichen_Maske);
			if(ulZeichen == ulZeichenPos + ucFesteZeichen){
				ucFesteZeichen = (BYTE)vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
				vasZeichenMaske->SubString(vbZeichen_Maske, ++ucFesteZeichen, ucFesteZeichen);
				if(!ZeichenMaske_Einfugen_Prufen(wParam, vbZeichen_Maske)) return false;
				ulZeichenPos += ucFesteZeichen - ulZeichen - 2;
				return true;
			}
			ucFesteZeichen += 2;
			ulZeichen = vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
		}
		else VMFrei(vbZeichen_Maske);
	}
	vasZeichenMaske->SubString(vbZeichen_Maske, --ulZeichen, ulZeichen);

	return ZeichenMaske_Einfugen_Prufen(wParam, vbZeichen_Maske);
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COEditLine::ZeichenMaske_Einfugen_Prufen(WPARAM wParam, VMBLOCK vbZeichen_Maske)
{
	switch(*(PBYTE)vbZeichen_Maske){
		case 0x41 : if(wParam >= 0x41 && wParam <= 0x5a){ VMFrei(vbZeichen_Maske); return true; }
								break;
		case 0x61 : if(wParam >= 0x61 && wParam <= 0x7a){ VMFrei(vbZeichen_Maske); return true; }
								break;
		case 0x42 : if(wParam >= 0x20 && wParam <= 0x2f || wParam >= 0x3a && wParam <= 0x7d){	VMFrei(vbZeichen_Maske); return true; }
								break;
		case 0x62 : if(wParam >= 0x20 && wParam <= 0x2f || wParam >= 0x3a && wParam <= 0x40 ||
										wParam >= 0x5b && wParam <= 0x60 || wParam >= 0x7b && wParam <= 0x7e){ VMFrei(vbZeichen_Maske); return true; }
								break;
		case 0x5a : VMFrei(vbZeichen_Maske); return true;
		case 0x39 : if(wParam >= 0x30 && wParam <= 0x39){ VMFrei(vbZeichen_Maske); return true; }
								break;
		case 0x38 : if(wParam >= 0x30 && wParam <= 0x39  || wParam >= 0x2b && wParam <= 0x2e){ VMFrei(vbZeichen_Maske); return true; }
								break;
		case 0x58	: if(wParam >= 0x30 && wParam <= 0x39 || wParam >= 0x41 && wParam <= 0x46 ||
								wParam >= 0x61 && wParam <= 0x66){ VMFrei(vbZeichen_Maske); return true; }
								break;
	}
	VMFrei(vbZeichen_Maske);
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COEditLine::ZeichenMaske_FestRechts(void)
{
	VMBLOCK vbZeichen_Maske; ULONG ulZeichen; BYTE ucFesteZeichen = 1;
	for(ulZeichen = 1; ulZeichen < ulZeichenPos + ucFesteZeichen; ulZeichen++){
		vasZeichenMaske->SubString(vbZeichen_Maske, ulZeichen, ulZeichen);
		if(*(PBYTE)vbZeichen_Maske == 0x27){
			VMFrei(vbZeichen_Maske);
			ucFesteZeichen += 2;
			ulZeichen = vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
		}
		else VMFrei(vbZeichen_Maske);
	}
	if(ulZeichenPos < ulZeichen - ucFesteZeichen){ ulZeichenPos = ulZeichen - ucFesteZeichen; return true; }
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COEditLine::ZeichenMaske_FestLinks(void)
{
	VMBLOCK vbZeichen_Maske; BYTE ucFesteZeichen = 1;

	for(ULONG ulZeichen = 1; ulZeichen <= ulZeichenPos + ucFesteZeichen; ulZeichen++){
		vasZeichenMaske->SubString(vbZeichen_Maske, ulZeichen, ulZeichen);
		if(*(PBYTE)vbZeichen_Maske == 0x27){ VMFrei(vbZeichen_Maske);
			ucFesteZeichen += 2;
			ULONG ulSprungZeichen = vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
			if(ulSprungZeichen == ulZeichenPos + ucFesteZeichen){ ulZeichenPos -= ulSprungZeichen - ulZeichen - 2; return true; }
			ulZeichen = ulSprungZeichen;
		}
		else VMFrei(vbZeichen_Maske);
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COEditLine::ZeichenMaske_Loschen(void)
{
	VMBLOCK vbZeichen_Maske; BYTE ucFesteZeichen = 1;

	for(ULONG ulZeichen = 1; ulZeichen <= ulZeichenPos + ucFesteZeichen; ulZeichen++){
		vasZeichenMaske->SubString(vbZeichen_Maske, ulZeichen, ulZeichen);
		if(*(PBYTE)vbZeichen_Maske == 0x27){ VMFrei(vbZeichen_Maske);
			ucFesteZeichen += 2;
			ULONG ulSprungZeichen = vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
			if(ulSprungZeichen == ulZeichenPos + ucFesteZeichen - 1){ ulZeichenPos -= --ulSprungZeichen - ulZeichen; return false; }
			ulZeichen = ulSprungZeichen;
		}
		else VMFrei(vbZeichen_Maske);
	}
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::Text(char* pcText)
{
	ThreadSafe_Begin();
	*vasInhalt = NULL; ulZeichenPos = 0;
	if(pcText){
		if(vasZeichenMaske->Length() && StrLength(pcText)){ VMBLOCK vbZeichen_Maske, vbZeichen_Text; BYTE ucFesteZeichen = 0, ucZeichen; COStringA asText = pcText;
			for(ULONG ulZeichen = 1; ulZeichen <= vasZeichenMaske->Length(); ulZeichen++){
				vasZeichenMaske->SubString(vbZeichen_Maske, ulZeichen, ulZeichen);
				if(asText.Length() >= ulZeichen - ucFesteZeichen){
					asText.SubString(vbZeichen_Text, ulZeichen - ucFesteZeichen, ulZeichen - ucFesteZeichen);
					switch(*(PBYTE)vbZeichen_Maske){
						case 0x41	: if(*(PBYTE)vbZeichen_Text >= 0x41 && *(PBYTE)vbZeichen_Text <= 0x5a) *vasInhalt += vbZeichen_Text;
												else if(ucZeichenVorgabe & 1 << 5) *vasInhalt += "_";
												else *vasInhalt += " ";
												break;
						case 0x61	: if(*(PBYTE)vbZeichen_Text >= 0x61 && *(PBYTE)vbZeichen_Text <= 0x7a) *vasInhalt += vbZeichen_Text;
												else if(ucZeichenVorgabe & 1 << 5) *vasInhalt += "_";
												else *vasInhalt += " ";
												break;
						case 0x42	: if(*(PBYTE)vbZeichen_Text >= 0x20 && *(PBYTE)vbZeichen_Text <= 0x2f || *(PBYTE)vbZeichen_Text >= 0x3a && *(PBYTE)vbZeichen_Text <= 0x7d)
												*vasInhalt += vbZeichen_Text;
												else if(ucZeichenVorgabe & 1 << 5) *vasInhalt += "_";
												else *vasInhalt += " ";
												break;
						case 0x62	: if(*(PBYTE)vbZeichen_Text >= 0x20 && *(PBYTE)vbZeichen_Text <= 0x2f || *(PBYTE)vbZeichen_Text >= 0x3a && *(PBYTE)vbZeichen_Text <= 0x40 ||
														*(PBYTE)vbZeichen_Text >= 0x5b && *(PBYTE)vbZeichen_Text <= 0x60 || *(PBYTE)vbZeichen_Text >= 0x7b && *(PBYTE)vbZeichen_Text <= 0x7e)
												*vasInhalt += vbZeichen_Text;
												else if(ucZeichenVorgabe & 1 << 5) *vasInhalt += "_";
												else *vasInhalt += " ";
												break;
						case 0x5a	: *vasInhalt += vbZeichen_Text;
												break;
						case 0x39	: if(*(PBYTE)vbZeichen_Text >= 0x30 && *(PBYTE)vbZeichen_Text <= 0x39) *vasInhalt += vbZeichen_Text;
												else if(ucZeichenVorgabe & 1 << 5) *vasInhalt += "_";
												else *vasInhalt += " ";
												break;
						case 0x38	: if(*(PBYTE)vbZeichen_Text >= 0x30 && *(PBYTE)vbZeichen_Text <= 0x39 || *(PBYTE)vbZeichen_Text >= 0x2b && *(PBYTE)vbZeichen_Text <= 0x2e) *vasInhalt += vbZeichen_Text;
												else if(ucZeichenVorgabe & 1 << 5) *vasInhalt += "_";
												else *vasInhalt += " ";
												break;
						case 0x27	: ucZeichen = (BYTE)(vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length()) - ulZeichen++);
												vasZeichenMaske->SubString(vbZeichen_Maske, ulZeichen, ulZeichen + ucZeichen - 2);
												*vasInhalt += vbZeichen_Maske;
												ucFesteZeichen += 2;
												ulZeichen += ucZeichen - 1;
												break;
						case 0x58	: if(*(PBYTE)vbZeichen_Text >= 0x30 && *(PBYTE)vbZeichen_Text <= 0x39 || *(PBYTE)vbZeichen_Text >= 0x41 && *(PBYTE)vbZeichen_Text <= 0x46 ||
														*(PBYTE)vbZeichen_Text >= 0x61 && *(PBYTE)vbZeichen_Text <= 0x66)	*vasInhalt += vbZeichen_Text;
												else if(ucZeichenVorgabe & 1 << 5) *vasInhalt += "_";
												else *vasInhalt += " ";
												break;
					}
					VMFrei(vbZeichen_Text);
				}
				else if(ucZeichenVorgabe & 1 << 5) *vasInhalt += "_";
				else *vasInhalt += " ";
				VMFrei(vbZeichen_Maske); 
			}
		}		
		else *vasInhalt = pcText;
	}
	//UpdateFenster(nullptr, true, false);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::DeSelect(void)
{
	cSelect = 0;
	rclDirty = rclSelect;
	if(rclDirty.left < 0) rclDirty.left = 0;
	if(rclDirty.right > lWidth) rclDirty.right = lWidth;
	OnRender(true);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	SetEvent(heCaret);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::Select_Loschen()
{
	SIZE stZeichengrosse;

	DeSelect();
	GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
	if(ulSelectPos <= ulZeichenPos){ vasInhalt->Delete(ulSelectPos, ulZeichenPos - ulSelectPos); ulZeichenPos = ulSelectPos; }
	else vasInhalt->Delete(ulZeichenPos, ulSelectPos - ulZeichenPos);

	if(ucSchriftausrichtung & TXA_LINKS){
		stZeichengrosse.cx - lTextPos < lWidth ? rclDirty.right = stZeichengrosse.cx - lTextPos : rclDirty.right = lWidth;
		ptlScrollOffset.x = rclSelect.left - rclSelect.right;
		rclDirty.left = rclDirty.right + ptlScrollOffset.x;
		rclScroll.right = rclDirty.left;
		rclScroll.left = rclSelect.left;

		dxgiPresent.pScrollRect = &rclScroll;
		dxgiPresent.pScrollOffset = &ptlScrollOffset;
		OnRender(true);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		dxgiPresent.pScrollRect = nullptr;
		dxgiPresent.pScrollOffset = nullptr;

	}
	else if(ucSchriftausrichtung & TXA_RECHTS){
		ptlScrollOffset.x = rclSelect.right - rclSelect.left;
		if(!lTextPos) rclDirty.left = lWidth - stZeichengrosse.cx;
		else{	rclDirty.left = 0; lTextPos -= ptlScrollOffset.x;	}
		rclDirty.right = rclDirty.left + ptlScrollOffset.x;
		rclScroll.left = rclDirty.right;
		rclScroll.right = rclSelect.right;
		ptlCaret.x = rclSelect.right;

		dxgiPresent.pScrollRect = &rclScroll;
		dxgiPresent.pScrollOffset = &ptlScrollOffset;
		OnRender(true);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		dxgiPresent.pScrollRect = nullptr;
		dxgiPresent.pScrollOffset = nullptr;

		if(lTextPos < 0) lTextPos = 0;
	}
	else{ // TXA_MITTE
		if(stZeichengrosse.cx < lWidth){
			rclDirty.left = (lWidth - stZeichengrosse.cx) / 2;
			rclDirty.right = lWidth - rclDirty.left;

			GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
			ptlCaret.x = (lWidth - stZeichengrosse.cx) / 2;

			GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
			ptlCaret.x += stZeichengrosse.cx;
		}
		else{	rclDirty.left = 0; rclDirty.right = lWidth; }

		OnRender(true);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SetSelectTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																																 _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfSelectText = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SetSelectTextColor(_In_ D2D1_COLOR_F& crfSelectTextA)
{
	ThreadSafe_Begin();
	crfSelectText = crfSelectTextA;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SetSelectBackgroundColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																																			 _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfSelectBack = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifSelectBackColor) ifSelectBackColor->SetColor(crfSelectBack);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SetSelectBackgroundColor(_In_ D2D1_COLOR_F& crfSelectBackA)
{
	ThreadSafe_Begin();
	crfSelectBack = crfSelectBackA;
	if(ifSelectBackColor) ifSelectBackColor->SetColor(crfSelectBack);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SetCaretColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																														_In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSafe_Begin();
	crfCaret = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifCaretColor) ifCaretColor->SetColor(crfCaret);

	rclDirty.left = ptlCaret.x; rclDirty.right = ptlCaret.x + CARET_PIXEL;
	OnRender(true);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SetCaretColor(_In_ D2D1_COLOR_F& crfCaretA)
{
	ThreadSafe_Begin();
	crfCaret = crfCaretA;
	if(ifCaretColor) ifCaretColor->SetColor(crfCaret);

	rclDirty.left = ptlCaret.x; rclDirty.right = ptlCaret.x + CARET_PIXEL;
	OnRender(true);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SetzZeichen_Max(unsigned long ulZeichen)
{
	ThreadSafe_Begin();
	ulZeichen_max = ulZeichen;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
unsigned long __vectorcall RePag::DirectX::COEditLine::Zeichen_Max(void)
{
	ThreadSafe_Begin();
	ULONG ulMaxZeichen = ulZeichen_max;
	ThreadSafe_End();
	return ulMaxZeichen;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::Zeichenvorgabe(unsigned char ucZeichenVorgabeA)
{
	ThreadSafe_Begin();
	ucZeichenVorgabe = ucZeichenVorgabeA;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::Zeichenmaske(const char* pcZeichenmaske)
{
	ThreadSafe_Begin();
	if(!pcZeichenmaske || !StrCompare(pcZeichenmaske, 1, "", 1)){ *vasZeichenMaske = NULL; ulZeichen_max = 0x7fffffff; }
	else if(*vasZeichenMaske != pcZeichenmaske){ VMBLOCK vbZeichen; ULONG ulZeichenLange;
		*vasZeichenMaske = pcZeichenmaske; *vasInhalt = NULL;

		for(ULONG ulZeichen = 1; ulZeichen <= vasZeichenMaske->Length(); ulZeichen++){
			vasZeichenMaske->SubString(vbZeichen, ulZeichen, ulZeichen);
			if(*(PBYTE)vbZeichen == 0x27){ VMFrei(vbZeichen); ulZeichen++;
				ulZeichenLange = vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
				vasZeichenMaske->SubString(vbZeichen, ulZeichen, ulZeichenLange - 1);
				*vasInhalt += vbZeichen;
				ulZeichen += ulZeichenLange - ulZeichen;
			}
			else if(ucZeichenVorgabe & 1 << 5) *vasInhalt += "_";
			else *vasInhalt += " ";
			VMFrei(vbZeichen);
		}
		ulZeichen_max = vasInhalt->Length();
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::DirectX::COEditLine::Zeichenmaske(COStringA* pasZeichenmaske)
{
	ThreadSafe_Begin();
	*pasZeichenmaske = *vasZeichenMaske;
	ThreadSafe_End();
	return pasZeichenmaske;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SelectAlles(void)
{
	if(hWndElement){
		SIZE stZeichengrosse;
		ThreadSafe_Begin();
		cSelect = 1; ulSelectPos = 0; ulZeichenPos = vasInhalt->Length();

		GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
		ResetEvent(heCaret);

		if(ucSchriftausrichtung & TXA_LINKS){ rclSelect.right = stZeichengrosse.cx; rclSelect.left = 0; ptlCaret.x = stZeichengrosse.cx; }
		else if(ucSchriftausrichtung & TXA_RECHTS){ rclSelect.right = lWidth; ptlCaret.x = lWidth;
			(stZeichengrosse.cx > lWidth ? rclSelect.left = 0 : rclSelect.left = lWidth - stZeichengrosse.cx);
		}
		else{
			if(stZeichengrosse.cx > lWidth){ rclSelect.left = 0; rclSelect.right = lWidth;}
			else{ rclSelect.left = (lWidth - stZeichengrosse.cx) / 2; rclSelect.right = rclSelect.left + stZeichengrosse.cx; }
		}

		rclDirty.left = rclSelect.left; rclDirty.right = rclSelect.right;
		OnRender(true);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		ThreadSafe_End();
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SelectEntfernen(void)
{
	ThreadSafe_Begin();
	if(cSelect) DeSelect();
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::DeleteCaretPos(void)
{
	rclDirty.left = ptlCaret.x;	rclDirty.right = rclDirty.left + CARET_PIXEL;
	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::GetTextPoint(_In_ char* pcText, _In_ unsigned long ulTextLength, _Out_ SIZE& stTextPoint)
{
	IDWriteTextLayout* ifTextLayout; DWRITE_TEXT_METRICS stTextMetrics;
	size_t szBytes_Text; WCHAR wcInhalt[255]; mbstowcs_s(&szBytes_Text, wcInhalt, pcText, ulTextLength);
	pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wcInhalt, szBytes_Text, ifText, (float)lWidth, (float)lHeight, &ifTextLayout);
	ifTextLayout->GetMetrics(&stTextMetrics);
	SafeRelease(&ifTextLayout);

	stTextPoint.cx = (long)stTextMetrics.width; 
	stTextMetrics.width -= (float)stTextPoint.cx;
	if(stTextMetrics.width >= 0.5f) stTextPoint.cx += 1;
	
	stTextPoint.cy = (long)stTextMetrics.height;
	stTextMetrics.height -= (float)stTextPoint.cy;
	if(stTextMetrics.height >= 0.5f) stTextPoint.cy += 1;

}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::ScrollRight(SIZE& stZeichengrosse)
{
	ptlScrollOffset.x = lWidth - stZeichengrosse.cx + lTextPos;
	lTextPos = stZeichengrosse.cx - lWidth;
	rclScroll.left = 0; rclScroll.right = lWidth + ptlScrollOffset.x;
	rclDirty.right = lWidth; rclDirty.left = rclScroll.right - CARET_PIXEL * 2;
	dxgiPresent.pScrollRect = &rclScroll;
	dxgiPresent.pScrollOffset = &ptlScrollOffset;
	OnRender(true);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	dxgiPresent.pScrollRect = nullptr;
	dxgiPresent.pScrollOffset = nullptr;

	if(cSelect){
		rclSelect.left += ptlScrollOffset.x;
		rclDirty.left = rclSelect.right + ptlScrollOffset.x;
	}
	else{
		if(ptlCaret.x + CARET_PIXEL == lWidth) rclSelect.left = rclScroll.right;
		else rclSelect.left = ptlCaret.x - CARET_PIXEL - lTextPos;
	}
	ptlCaret.x = lWidth - CARET_PIXEL;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::ScrollLeft(SIZE& stZeichengrosse)
{
	ptlScrollOffset.x = lTextPos - stZeichengrosse.cx;
	lTextPos = stZeichengrosse.cx;
	rclScroll.right = lWidth; rclScroll.left = ptlScrollOffset.x;
	rclDirty.left = 0; rclDirty.right = rclScroll.left + CARET_PIXEL * 2;
	dxgiPresent.pScrollRect = &rclScroll;
	dxgiPresent.pScrollOffset = &ptlScrollOffset;
	OnRender(true);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	dxgiPresent.pScrollRect = nullptr;
	dxgiPresent.pScrollOffset = nullptr;

	if(cSelect){
		if(!ptlCaret.x)
			rclSelect.right += ptlScrollOffset.x;
		else{
			rclDirty.right = ptlCaret.x + ptlScrollOffset.x;
			rclSelect.right += ptlScrollOffset.x;
		}
	}
	else{
		if(!ptlCaret.x)	rclSelect.right = rclDirty.right;
		else rclSelect.right = ptlCaret.x + CARET_PIXEL;
	}
	ptlCaret.x = 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------