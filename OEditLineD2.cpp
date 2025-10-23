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
															pEditLine->ThreadSicher_Anfang();
															if(!pEditLine->pfnWM_Command(pEditLine, wParam)){ pEditLine->ThreadSicher_Ende(); return NULL; }
															pEditLine->ThreadSicher_Ende();
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
		//case WM_PAINT         : ((COEditLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Paint();
		//												return NULL;
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

	_EditLine->ThreadSicher_Anfang();
	_EditLine->rclDirty.left = _EditLine->ptlCaret.x;
	_EditLine->rclDirty.right = _EditLine->rclDirty.left + CARET_PIXEL;

	_EditLine->OnRender(bCaret);
	_EditLine->ifDXGISwapChain4->Present1(0, NULL, &_EditLine->dxgiPresent);
	_EditLine->ThreadSicher_Ende();
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
	 //stSelectTextColor.r = stSelectTextColor.g = stSelectTextColor.b = 0.0f; stSelectTextColor.a = 1.0f;
	 //stSelectBackColor.r = 0; stSelectBackColor.g = 0.85f; stSelectBackColor.b = 0.85f; stSelectBackColor.a = 1.0f;
	 //stCaretColor.r = stCaretColor.g = stCaretColor.b = stCaretColor.a = 1.0f;
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
	pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wcInhalt, szBytes_Text, ifText, (float)lBreite, (float)lHohe, &ifTextLayout);
	TextAlignment(ifTextLayout, fTextWidth, rcfText);
	SafeRelease(&ifTextLayout);

	if(lTextPos || (long)fTextWidth > lBreite) rcfText.left = static_cast<float>(lTextPos * -1);

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
	ThreadSicher_Anfang();
	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_Create(void)
{
	CharacterMetric();
	//rclSelect.bottom = lHohe;
	ifD2D1Context6->CreateSolidColorBrush(crfText, &ifTextColor);
	ifD2D1Context6->CreateSolidColorBrush(crfSelectBack, &ifSelectBackColor);
	ifD2D1Context6->CreateSolidColorBrush(crfCaret, &ifCaretColor);

	ifText->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	SIZE szZeichen_Inhalt;
	if(ucSchriftausrichtung & TXA_RECHTS){
		GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), szZeichen_Inhalt);
		if(szZeichen_Inhalt.cx > lBreite) lTextPos = szZeichen_Inhalt.cx - lBreite;
	}
	else if(ucSchriftausrichtung & TXA_MITTEHORIZONTAL){
		GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), szZeichen_Inhalt);
		if(szZeichen_Inhalt.cx > lBreite){
			lTextPos = szZeichen_Inhalt.cx - lBreite;
			lTextPos /= 2;
		}
	}

	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_SetFocus(void)
{
	ThreadSicher_Anfang();
	IDWriteTextLayout* ifTextLayout; size_t szBytes_Text; WCHAR wcInhalt[255]; D2D1_POINT_2F ptfText = {0}; float fTextWidth;

	mbstowcs_s(&szBytes_Text, wcInhalt, vasInhalt->c_Str(), vasInhalt->Length());
	pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wcInhalt, szBytes_Text, ifText, (float)lBreite, (float)lHohe, &ifTextLayout);

	TextAlignment(ifTextLayout, fTextWidth, ptfText);
	SafeRelease(&ifTextLayout);

	ptlCaret.y = (long)ptfText.y;
	ptfText.y -= (float)ptlCaret.y;
	if(ptfText.y >= 0.5f) ptlCaret.y += 1;

	rclScroll.top = rclDirty.top = lHohe - lZeichen_hohe - ptlCaret.y;
	rclScroll.bottom = rclDirty.bottom = lHohe - ptlCaret.y;
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
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_KillFocus(void)
{
	ThreadSicher_Anfang();
	DeleteTimerQueueTimer(TimerQueue(), htCaret, NULL); htCaret = nullptr;
	
	rclDirty.left = ptlCaret.x; rclDirty.right = ptlCaret.x + CARET_PIXEL;
	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);

	if(pfnWM_KillFocus) pfnWM_KillFocus(this);
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_KeyDown(WPARAM wParam, LPARAM lParam)
{
	SIZE stZeichengrosse, stZeichenPos_Caret; VMBLOCK vbZeichen; RECT rcl2Dirty[2];
	switch(wParam){
		case VK_HOME    : ThreadSicher_Anfang();
											DeleteCaretPos();
											if(cSelect) DeSelect();
											ulZeichenPos = 0;
											if(lTextPos){
												lTextPos = 0; ptlCaret.x = 0;
												rclDirty.left = 0; rclDirty.right = lBreite;
												OnRender(true);
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											}
											else if(ucSchriftausrichtung & TXA_LINKS) ptlCaret.x = 0; 
											else{ 
												GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
												if(stZeichengrosse.cx > lBreite) ptlCaret.x = 0;
												else if(ucSchriftausrichtung & TXA_RECHTS) ptlCaret.x = lBreite - stZeichengrosse.cx;
												else ptlCaret.x = (lBreite - stZeichengrosse.cx) / 2;
											} 
											ThreadSicher_Ende();
											break;
		case VK_END     : ThreadSicher_Anfang();
											DeleteCaretPos();
											if(cSelect) DeSelect();
											ulZeichenPos = vasInhalt->Length();
											GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
											if(stZeichengrosse.cx > lBreite){
												lTextPos = stZeichengrosse.cx - lBreite; ptlCaret.x = lBreite - CARET_PIXEL;
												rclDirty.left = 0; rclDirty.right = lBreite;
												OnRender(true);
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											}
											else if(ucSchriftausrichtung & TXA_LINKS) ptlCaret.x = stZeichengrosse.cx;
											else if(ucSchriftausrichtung & TXA_RECHTS) ptlCaret.x = lBreite - CARET_PIXEL;
											else ptlCaret.x = (lBreite - stZeichengrosse.cx) / 2 + stZeichengrosse.cx;
											ThreadSicher_Ende();
											break;
		case VK_LEFT		: ThreadSicher_Anfang();											
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
															ucSchriftausrichtung & TXA_RECHTS ? ptlCaret.x = lBreite - stZeichengrosse.cx
																: ptlCaret.x = (lBreite - stZeichengrosse.cx) / 2;
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
														if(stZeichengrosse.cx < lBreite){
															if(ucSchriftausrichtung & TXA_RECHTS)	ptlCaret.x = lBreite - stZeichengrosse.cx + stZeichenPos_Caret.cx;
															else ptlCaret.x = (lBreite - stZeichengrosse.cx) / 2 + stZeichenPos_Caret.cx;
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
											ThreadSicher_Ende();
											break;
		case VK_RIGHT		: ThreadSicher_Anfang();
											if(ulZeichenPos < vasInhalt->Length()){
												if(!cSelect) DeleteCaretPos();
												rclDirty.left = ptlCaret.x;
												GetTextPoint(vasInhalt->c_Str(), ++ulZeichenPos, stZeichengrosse);
												if(stZeichengrosse.cx - lTextPos > lBreite)	ScrollRight(stZeichengrosse);
												else if(stZeichengrosse.cx - lTextPos == lBreite)	ptlCaret.x = lBreite - CARET_PIXEL;
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
															ucSchriftausrichtung & TXA_RECHTS ? ptlCaret.x = lBreite - stZeichengrosse.cx
																: ptlCaret.x = (lBreite - stZeichengrosse.cx) / 2;
															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
															ptlCaret.x += stZeichengrosse.cx;
														}
	 													else{
															vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
															GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
															ptlCaret.x = lBreite - stZeichengrosse.cx;
														}
													}
													else{
														stZeichenPos_Caret = stZeichengrosse;
														GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
														if(stZeichengrosse.cx < lBreite){
															if(ucSchriftausrichtung & TXA_RECHTS)	ptlCaret.x = lBreite - stZeichengrosse.cx + stZeichenPos_Caret.cx;
															else ptlCaret.x = (lBreite - stZeichengrosse.cx) / 2 + stZeichenPos_Caret.cx;
															if(ptlCaret.x == lBreite) ptlCaret.x -= CARET_PIXEL;
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
														if(stZeichengrosse.cx - lTextPos == lBreite){
															rclDirty.left = rclSelect.left;
															rclSelect.right = lBreite;
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
											ThreadSicher_Ende();
											break;
		case VK_DELETE	: ThreadSicher_Anfang();
											if(!ucZeichenVorgabe){ ThreadSicher_Ende(); break; }
											if(!vasZeichenMaske->Length()){	
												if(cSelect) Select_Loschen();
												else if(ulZeichenPos < vasInhalt->Length()){
													GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
													if(ucSchriftausrichtung & TXA_LINKS){
														if(!lTextPos){
															if(stZeichengrosse.cx < lBreite) rclDirty.right = stZeichengrosse.cx;
															else rclDirty.right = lBreite;
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
															if(stZeichengrosse.cx - lTextPos > lBreite){
																rclDirty.right = lBreite;
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
																lTextPos = stZeichengrosse.cx - lBreite;

																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																ptlCaret.x = stZeichengrosse.cx - lTextPos;
																if(lTextPos < 0){	lTextPos = 0;	rclDirty.right = lBreite; }

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
														if(stZeichengrosse.cx <= lBreite){
															rclDirty.left = lBreite - stZeichengrosse.cx;
															if(rclDirty.left == ptlCaret.x){
																rclDirty.left = lBreite - stZeichengrosse.cx - CARET_PIXEL;
																vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
																GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																rclDirty.right = rclDirty.left + stZeichengrosse.cx;
																vasInhalt->Delete(ulZeichenPos, 1);
																GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
																ptlCaret.x = lBreite - stZeichengrosse.cx;
																if(ptlCaret.x >= lBreite) ptlCaret.x = lBreite - CARET_PIXEL;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															else{
																vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
																GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																rclScroll.left = rclDirty.right = rclDirty.left + stZeichengrosse.cx;
																if(rclDirty.left < stZeichengrosse.cx) rclDirty.left = 0;
																rclScroll.right = ptlCaret.x + stZeichengrosse.cx;
																if(rclScroll.right > lBreite) rclScroll.right = lBreite;
																ptlScrollOffset.x = stZeichengrosse.cx;
																dxgiPresent.pScrollRect = &rclScroll;
																dxgiPresent.pScrollOffset = &ptlScrollOffset;
																vasInhalt->Delete(ulZeichenPos, 1);
																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																ptlCaret.x = rclDirty.right + stZeichengrosse.cx;
																if(ptlCaret.x >= lBreite) ptlCaret.x = lBreite - CARET_PIXEL;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																dxgiPresent.pScrollRect = nullptr;
																dxgiPresent.pScrollOffset = nullptr;
															}
														}
														else if(!lTextPos){
															rclDirty.left = stZeichengrosse.cx - lBreite;
															vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
															GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
															if(rclDirty.left < stZeichengrosse.cx){
																rcl2Dirty[0].top = rcl2Dirty[1].top = rclDirty.top;
																rcl2Dirty[0].bottom = rcl2Dirty[1].bottom = rclDirty.bottom;
																rcl2Dirty[0].right = lBreite;
																rcl2Dirty[1].left = 0;
																ptlCaret.x = stZeichengrosse.cx - rclDirty.left;
																vasInhalt->Delete(ulZeichenPos, 1);
																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																ptlCaret.x += stZeichengrosse.cx;
																if(ptlCaret.x >= lBreite) ptlCaret.x = lBreite - CARET_PIXEL;
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
																rclDirty.right = lBreite;
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
															if(stZeichengrosse.cx - lTextPos > lBreite){
																vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
																GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																vasInhalt->Delete(ulZeichenPos, 1);
																rclDirty.right = lBreite;
																rclScroll.left = ptlCaret.x;
																ptlScrollOffset.x = stZeichengrosse.cx * -1;
																rclScroll.right = rclDirty.left = lBreite - stZeichengrosse.cx;
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
																lTextPos = stZeichengrosse.cx - lBreite;

																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																ptlCaret.x = stZeichengrosse.cx - lTextPos;
																if(ptlCaret.x >= lBreite)	ptlCaret.x = lBreite - CARET_PIXEL;
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
														if(stZeichengrosse.cx <= lBreite){
															rclDirty.left = (lBreite - stZeichengrosse.cx) / 2;
															rclDirty.right = rclDirty.left + stZeichengrosse.cx;
															vasInhalt->Delete(ulZeichenPos, 1);
															GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
															stZeichengrosse.cx = (lBreite - stZeichengrosse.cx) / 2;
															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichenPos_Caret);
															ptlCaret.x = stZeichenPos_Caret.cx + stZeichengrosse.cx;
															OnRender(true);
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														}
														else if(!lTextPos){
															rclDirty.right = lBreite;
															vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
															GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
															rclScroll.right = rclDirty.left = rclDirty.right - stZeichengrosse.cx;
															vasInhalt->Delete(ulZeichenPos, 1);

															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
															ptlCaret.x = stZeichengrosse.cx;
															rclScroll.left = ptlCaret.x;
															ptlScrollOffset.x = rclDirty.left - rclDirty.right;
														}
														else if(stZeichengrosse.cx - lTextPos > lBreite){
															rclDirty.right = lBreite;
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
															lTextPos = stZeichengrosse.cx - lBreite;

															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
															ptlCaret.x = stZeichengrosse.cx - lTextPos;
															ptlScrollOffset.x = rclDirty.right;
															if(lTextPos < 0){	lTextPos = 0; rclDirty.right = lBreite;	}

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
												ThreadSicher_Ende();
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
		case VK_TAB    : ThreadSicher_Anfang();
										 if(!ucZeichenVorgabe){ ThreadSicher_Ende(); break; }
										 rclDirty.right = 0;
										 do{ SendMessage(hWndElement, WM_CHAR, ' ', NULL); }
										 while(++rclDirty.right < 4);
										 ThreadSicher_Ende();
										 break;
		case VK_RETURN : ThreadSicher_Anfang();
										 if(pfnWM_Char_Return) pfnWM_Char_Return(this);
										 ThreadSicher_Ende();
										 break;
		case VK_ESCAPE : ThreadSicher_Anfang();
										 SetFocus(GetParent(hWndElement));
										 if(pfnWM_Char_Escape) pfnWM_Char_Escape(this);
										 ThreadSicher_Ende();
										 break;
		case VK_BACK   : ThreadSicher_Anfang();
											if(!ucZeichenVorgabe){ ThreadSicher_Ende(); break; }
											if(ulZeichenPos || !ulZeichenPos && cSelect){ 
												if(!vasZeichenMaske->Length()){
													if(cSelect) Select_Loschen();
													else{
														GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
														if(ucSchriftausrichtung & TXA_LINKS){ 
															if(!lTextPos){
																if(stZeichengrosse.cx < lBreite) rclDirty.right = stZeichengrosse.cx;
																else rclDirty.right = lBreite;
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
																if(stZeichengrosse.cx - lTextPos > lBreite){
																	rclDirty.right = lBreite;
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
																	lTextPos = stZeichengrosse.cx - lBreite;

																	GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																	ptlCaret.x = stZeichengrosse.cx - lTextPos;
																	if(lTextPos < 0){ lTextPos = 0;	rclDirty.right = lBreite; }

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
															if(stZeichengrosse.cx <= lBreite){
																rclDirty.left = lBreite - stZeichengrosse.cx;
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
																	if(rclScroll.right > lBreite) rclScroll.right = lBreite;
																	ptlScrollOffset.x = stZeichengrosse.cx;
																	dxgiPresent.pScrollRect = &rclScroll;
																	dxgiPresent.pScrollOffset = &ptlScrollOffset;
																	GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																	ptlCaret.x = rclDirty.right + stZeichengrosse.cx;
																	if(ptlCaret.x >= lBreite) ptlCaret.x = lBreite - CARET_PIXEL;
																	OnRender(true);
																	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																	dxgiPresent.pScrollRect = nullptr;
																	dxgiPresent.pScrollOffset = nullptr;
																}
															}
															else if(!lTextPos){
																rclDirty.left = stZeichengrosse.cx - lBreite;
																vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
																GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																if(rclDirty.left < stZeichengrosse.cx){
																	rcl2Dirty[0].top = rcl2Dirty[1].top = rclDirty.top;
																	rcl2Dirty[0].bottom = rcl2Dirty[1].bottom = rclDirty.bottom;
																	rcl2Dirty[0].right = lBreite;
																	rcl2Dirty[1].left = 0;
																	ptlCaret.x = stZeichengrosse.cx - rclDirty.left;
																	vasInhalt->Delete(--ulZeichenPos, 1);
																	GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																	ptlCaret.x += stZeichengrosse.cx;
																	if(ptlCaret.x >= lBreite) ptlCaret.x = lBreite - CARET_PIXEL;
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
																	rclDirty.right = lBreite;
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
																if(stZeichengrosse.cx - lTextPos > lBreite){
																	vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
																	GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																	vasInhalt->Delete(--ulZeichenPos, 1);
																	rclDirty.right = lBreite;
																	rclScroll.right = rclDirty.left = lBreite - stZeichengrosse.cx;
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
																	lTextPos = stZeichengrosse.cx - lBreite;

																	GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																	ptlCaret.x = stZeichengrosse.cx - lTextPos;
																	if(ptlCaret.x >= lBreite)	ptlCaret.x = lBreite - CARET_PIXEL;
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
															if(stZeichengrosse.cx <= lBreite){
																rclDirty.left = (lBreite - stZeichengrosse.cx) / 2;
																rclDirty.right = rclDirty.left + stZeichengrosse.cx;
																vasInhalt->Delete(--ulZeichenPos, 1);
																GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
																stZeichengrosse.cx = (lBreite - stZeichengrosse.cx) / 2;
																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse_1);
																ptlCaret.x = stZeichengrosse_1.cx + stZeichengrosse.cx;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															else if(!lTextPos){
																rclDirty.right = lBreite;
																vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
																GetTextPoint(vbZeichen, 1, stZeichengrosse); VMFrei(vbZeichen);
																rclScroll.right = rclDirty.left = rclDirty.right - stZeichengrosse.cx;
																vasInhalt->Delete(--ulZeichenPos, 1);

																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																ptlCaret.x = stZeichengrosse.cx;
																rclScroll.left = ptlCaret.x;
																ptlScrollOffset.x = rclDirty.left - rclDirty.right;
															}
															else if(stZeichengrosse.cx - lTextPos > lBreite){
																rclDirty.right = lBreite;
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
																lTextPos = stZeichengrosse.cx - lBreite;

																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																ptlCaret.x = stZeichengrosse.cx - lTextPos;
																ptlScrollOffset.x = rclDirty.right;
																if(lTextPos < 0){ lTextPos = 0; rclDirty.right = lBreite; }

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
														ptlCaret.x = lBreite - stZeichengrosse.cx;
													}
													else{
														GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
														ptlCaret.x = (lBreite - stZeichengrosse.cx) / 2;
													}
													GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
													ptlCaret.x += stZeichengrosse.cx;

													if(rclDirty.right){ 
														rclDirty.left = ptlCaret.x; rclDirty.right = lBreite;
														OnRender(true);
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													}
												}
											} 
											ThreadSicher_Ende();
											PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), WM_CHAR);
											break;
		default        :  ThreadSicher_Anfang();
											if(ZeichenVorgabe(wParam) && ulZeichenPos < ulZeichen_max){
												if(!vasZeichenMaske->Length()){
													if(cSelect) Select_Loschen();
													GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
													if(ucSchriftausrichtung & TXA_LINKS){
														if(stZeichengrosse.cx - lTextPos < lBreite){
															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
															rclDirty.left = stZeichengrosse.cx - lTextPos;
															GetTextPoint((LPSTR)&wParam, 1, stZeichengrosse);
															rclDirty.right = ptlCaret.x = rclDirty.left + stZeichengrosse.cx;
															if(rclDirty.right < lBreite){
																if(ulZeichenPos == vasInhalt->Length()){
																	*vasInhalt += (char*)&wParam;
																	ulZeichenPos++;

																	OnRender(true);
																	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																}
																else{
																	vasInhalt->Insert((char*)&wParam, ulZeichenPos);
																	GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
																	if(stZeichengrosse.cx < lBreite){
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
																ptlScrollOffset.x = lBreite - rclDirty.right;
																rclScroll.left = 0;
																rclDirty.right = lBreite;
																rclDirty.left = rclScroll.right = rclDirty.right - stZeichengrosse.cx;
																rclDirty.left -= CARET_PIXEL * 2;
																GetTextPoint(vasInhalt->c_Str(), ++ulZeichenPos, stZeichengrosse);
																lTextPos = stZeichengrosse.cx - lBreite;
																ptlCaret.x = lBreite - CARET_PIXEL;

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
															if(rclDirty.right < lBreite){
																vasInhalt->Insert((char*)&wParam, ulZeichenPos);

																rclScroll.left = rclDirty.right;
																ptlScrollOffset.x = stZeichengrosse.cx;
																rclScroll.right = lBreite;
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
																rclDirty.left = lBreite - stZeichengrosse.cx;
																rclDirty.left -= CARET_PIXEL * 2;
																rclScroll.right = rclDirty.left;
																ptlScrollOffset.x = lBreite - rclDirty.right - CARET_PIXEL;
																lTextPos -= ptlScrollOffset.x;
																rclDirty.right = lBreite;
																ptlCaret.x = lBreite - CARET_PIXEL;

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
														if(stZeichengrosse_1.cx < lBreite){
															if(!ulZeichenPos){
																GetTextPoint((LPSTR)&wParam, 1, stZeichengrosse);
																if(ptlCaret.x < stZeichengrosse.cx){
																	rclDirty.left = 0;
																	rclDirty.right = rclScroll.left = stZeichengrosse.cx;
																	rclDirty.right += CARET_PIXEL;
																	rclScroll.right = lBreite;
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
																rclScroll.left = lBreite - stZeichengrosse.cx;
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
																rclScroll.right = lBreite;
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
														rclDirty.left = (lBreite - stZeichengrosse.cx) / 2;
														if(stZeichengrosse.cx < lBreite){
															if(rclDirty.left < 0){
																lTextPos = rclDirty.left * -1;
																rclDirty.left = 0;
																rclDirty.right = lBreite;
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
															rclDirty.right = lBreite;

															GetTextPoint(vasInhalt->c_Str(), ++ulZeichenPos, stZeichengrosse);
															ptlCaret.x = stZeichengrosse.cx - lTextPos;

															if(ptlCaret.x < lBreite){
																rclDirty.left = 0;

																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															else{
																GetTextPoint((LPSTR)&wParam, 1, stZeichengrosse);
																ptlScrollOffset.x = stZeichengrosse.cx * -1;
																if(lTextPos == rclDirty.left) lTextPos += lBreite - ptlCaret.x + stZeichengrosse.cx;
																else{
																	GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
																	lTextPos = stZeichengrosse.cx - lBreite;
																}

																rclScroll.left = 0;
																rclScroll.right = rclDirty.left = lBreite + ptlScrollOffset.x;
																rclDirty.left -= CARET_PIXEL * 2;
																rclDirty.right = lBreite;
																ptlCaret.x = lBreite - CARET_PIXEL;

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
																ptlCaret.x = lBreite - stZeichengrosse.cx;
															}
															else{ // TXA_MITTE
																GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
																ptlCaret.x = (lBreite - stZeichengrosse.cx) / 2;
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
											ThreadSicher_Ende();
											PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), WM_CHAR);
											break;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COEditLine::WM_Command(WPARAM wParam)
{
	VMBLOCK vbZeichen; HGLOBAL hGlobal; char* pcAblage; ULONG ulZeichen; SIZE szZeichen_Inhalt, szZeichen_Clipboard; RECT rcl2Dirty[2];
	switch(LOWORD(wParam)){
		case IDM_KOPIEREN			: ThreadSicher_Anfang();
														OpenClipboard(hWndElement); EmptyClipboard();
														if(cSelect > 0) ulZeichen = vasInhalt->SubString(vbZeichen, ulSelectPos + 1, ulZeichenPos);
														else ulZeichen = vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulSelectPos);
														hGlobal = GlobalAlloc(GMEM_MOVEABLE, ulZeichen + 1);
														pcAblage = (char*)GlobalLock(hGlobal);
														MemCopy(pcAblage, vbZeichen, ulZeichen); VMFrei(vbZeichen);
														pcAblage[ulZeichen] = 0;
														GlobalUnlock(hGlobal);
														SetClipboardData(CF_TEXT, hGlobal); CloseClipboard();
														ThreadSicher_Ende();
														return false;
		case IDM_AUSSCHNEIDEN : ThreadSicher_Anfang();
														if(vasZeichenMaske->Length() || !ucZeichenVorgabe){ ThreadSicher_Ende(); return false; }
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
																	rclDirty.right = lBreite;
																	GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, szZeichen_Inhalt);
																	ptlCaret.x = szZeichen_Inhalt.cx;
																}
															}
															else{
																ptlScrollOffset.x = szZeichen_Clipboard.cx * -1;
																rclScroll.left = ptlCaret.x;
																if(szZeichen_Inhalt.cx > lBreite){
																	rclDirty.left = rclScroll.right = lBreite - szZeichen_Clipboard.cx;
																	rclDirty.right = lBreite;
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
															if(szZeichen_Inhalt.cx - lTextPos > lBreite){
																if(szZeichen_Inhalt.cx - lBreite - lTextPos >= szZeichen_Clipboard.cx){
																	rclScroll.left = ptlCaret.x = rclSelect.left;
																	rclDirty.right = lBreite;
																	rclDirty.left = rclScroll.right = lBreite - szZeichen_Clipboard.cx;
																	ptlScrollOffset.x = szZeichen_Clipboard.cx * -1;

																	dxgiPresent.pScrollRect = &rclScroll;
																	dxgiPresent.pScrollOffset = &ptlScrollOffset;
																	OnRender(true);
																	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
																	dxgiPresent.pScrollRect = nullptr;
																	dxgiPresent.pScrollOffset = nullptr;
																}
																else{
																	lTextPos += (szZeichen_Inhalt.cx - lBreite - lTextPos) - szZeichen_Clipboard.cx;
																	GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, szZeichen_Inhalt);
																	ptlCaret.x = szZeichen_Inhalt.cx - lTextPos;
																	if(lTextPos < 0) lTextPos = 0;
																	rclDirty.right = lBreite;
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
															lTextPos = ((szZeichen_Inhalt.cx - szZeichen_Clipboard.cx) - lBreite) / 2;
															GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, szZeichen_Inhalt);
															ptlCaret.x = szZeichen_Inhalt.cx - lTextPos;
															if(lTextPos < 0) lTextPos = 0;
															rclDirty.left = 0;
															rclDirty.right = lBreite;

															OnRender(true);
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														}
														ThreadSicher_Ende();
														return false;
		case IDM_EINFUGEN	:			ThreadSicher_Anfang();
														if(!IsClipboardFormatAvailable(CF_TEXT) || !ucZeichenVorgabe){ ThreadSicher_Ende(); return false; }
														if(vasZeichenMaske->Length()){ ThreadSicher_Ende(); return false; }
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
															if(szZeichen_Inhalt.cx + szZeichen_Clipboard.cx < lBreite){
																ulZeichenPos == vasInhalt->Length() ? *vasInhalt += pcAblage : vasInhalt->Insert(pcAblage, ulZeichenPos);
																ulZeichenPos += ulZeichen;
																ptlCaret.x += szZeichen_Clipboard.cx;

																rclDirty.left = 0; rclDirty.right = lBreite;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															else if(ucZeichenVorgabe & ZV_UBERLANGE){
																ulZeichenPos == vasInhalt->Length() ? *vasInhalt += pcAblage : vasInhalt->Insert(pcAblage, ulZeichenPos);
																ulZeichenPos += ulZeichen;
																GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, szZeichen_Inhalt);
																if(szZeichen_Inhalt.cx - lTextPos > lBreite) lTextPos = szZeichen_Inhalt.cx - lBreite;

																szZeichen_Inhalt.cx - lTextPos < lBreite ? ptlCaret.x = szZeichen_Inhalt.cx - lTextPos
																	: ptlCaret.x = lBreite - CARET_PIXEL;

																rclDirty.left = 0; rclDirty.right = lBreite;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															// tue nix
														}
														else if(ucSchriftausrichtung & TXA_RECHTS){
															if(szZeichen_Inhalt.cx + szZeichen_Clipboard.cx < lBreite){
																ulZeichenPos == vasInhalt->Length() ? *vasInhalt += pcAblage : vasInhalt->Insert(pcAblage, ulZeichenPos);
																ulZeichenPos += ulZeichen;

																rclDirty.left = 0; rclDirty.right = lBreite;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															else if(ucZeichenVorgabe & ZV_UBERLANGE){
																if(ulZeichenPos == vasInhalt->Length()){
																	*vasInhalt += pcAblage;
																	ptlCaret.x = lBreite - CARET_PIXEL;
																}
																else vasInhalt->Insert(pcAblage, ulZeichenPos);
																ulZeichenPos += ulZeichen;
																lTextPos += szZeichen_Clipboard.cx;

																rclDirty.left = 0; rclDirty.right = lBreite;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															// tue nix
														}
														else{
															if(szZeichen_Inhalt.cx + szZeichen_Clipboard.cx < lBreite){
																ulZeichenPos == vasInhalt->Length() ? *vasInhalt += pcAblage : vasInhalt->Insert(pcAblage, ulZeichenPos);
																ulZeichenPos += ulZeichen;
																ptlCaret.x += szZeichen_Clipboard.cx / 2;

																rclDirty.left = 0; rclDirty.right = lBreite;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															else if(ucZeichenVorgabe & ZV_UBERLANGE){
																if(ulZeichenPos == vasInhalt->Length()){
																	*vasInhalt += pcAblage;
																	lTextPos = (szZeichen_Inhalt.cx + szZeichen_Clipboard.cx) - lBreite;
																	ptlCaret.x = lBreite - CARET_PIXEL;
																}
																else{
																	vasInhalt->Insert(pcAblage, ulZeichenPos);
																	lTextPos = ((szZeichen_Inhalt.cx + szZeichen_Clipboard.cx) - lBreite) / 2;
																	ptlCaret.x += szZeichen_Clipboard.cx / 2;
																}
																ulZeichenPos += ulZeichen;

																rclDirty.left = 0; rclDirty.right = lBreite;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															// tue nix
														}
														ThreadSicher_Ende();
														return false;
		default               : return true;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_ContexMenu(LPARAM lParam)
{
	ThreadSicher_Anfang();
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
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_MouseMove(WPARAM wParam, LPARAM lParam)
{
	if(hWndElement == GetFocus() && wParam == MK_LBUTTON){
		ThreadSicher_Anfang();
		if(GET_X_LPARAM(lParam) < ptlCaret.x - lZeichen_mittel) SendMessage(hWndElement, WM_KEYDOWN, VK_LEFT, NULL);
		else if(GET_X_LPARAM(lParam) > ptlCaret.x + lZeichen_mittel) SendMessage(hWndElement, WM_KEYDOWN, VK_RIGHT, NULL);
		ThreadSicher_Ende();
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_LButtonDown(WPARAM wParam, LPARAM lParam)
{
	SetCapture(hWndElement);
	ThreadSicher_Anfang();
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
			if(szZeichen_Inhalt.cx > lBreite){
				do{ GetTextPoint(vasInhalt->c_Str(), ++ulZeichenPos, szZeichen_Inhalt); }
				while(szZeichen_Inhalt.cx < LOWORD(lParam) && ulZeichenPos < vasInhalt->Length());
				ptlCaret.x = szZeichen_Inhalt.cx;
			}
			else if(ucSchriftausrichtung & TXA_RECHTS) ptlCaret.x = lBreite - szZeichen_Inhalt.cx;
			else ptlCaret.x = (lBreite - szZeichen_Inhalt.cx) / 2;

			if(LOWORD(lParam) > ptlCaret.x){
				do{ GetTextPoint(vasInhalt->c_Str(), ++ulZeichenPos, szZeichen_Inhalt);}
				while(szZeichen_Inhalt.cx + ptlCaret.x < LOWORD(lParam) && ulZeichenPos < vasInhalt->Length());
			 
				if(vasZeichenMaske->Length()){
					if(ZeichenMaske_FestRechts()) GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, szZeichen_Inhalt);
				}
				ptlCaret.x += szZeichen_Inhalt.cx;
			} 
		}
		if(ptlCaret.x == lBreite) ptlCaret.x -= CARET_PIXEL;
	}

	if(pfnWM_LButtonDown) pfnWM_LButtonDown(this, wParam, lParam);
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_LButtonUp(WPARAM wParam, LPARAM lParam)
{
	ReleaseCapture();
	ThreadSicher_Anfang();
	if(pfnWM_LButtonUp) pfnWM_LButtonUp(this, wParam, lParam);
	else PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), WM_LBUTTONUP);
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_LButtonDBClick(WPARAM wParam, LPARAM lParam)
{
	ThreadSicher_Anfang();
	SelectAlles();
	if(pfnWM_LButtonDBClick) pfnWM_LButtonDBClick(this, wParam, lParam);
	ThreadSicher_Ende();
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
	ThreadSicher_Anfang();
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
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::DeSelect(void)
{
	cSelect = 0;
	rclDirty = rclSelect;
	if(rclDirty.left < 0) rclDirty.left = 0;
	if(rclDirty.right > lBreite) rclDirty.right = lBreite;
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
		stZeichengrosse.cx - lTextPos < lBreite ? rclDirty.right = stZeichengrosse.cx - lTextPos : rclDirty.right = lBreite;
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
		if(!lTextPos) rclDirty.left = lBreite - stZeichengrosse.cx;
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
		if(stZeichengrosse.cx < lBreite){
			rclDirty.left = (lBreite - stZeichengrosse.cx) / 2;
			rclDirty.right = lBreite - rclDirty.left;

			GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
			ptlCaret.x = (lBreite - stZeichengrosse.cx) / 2;

			GetTextPoint(vasInhalt->c_Str(), ulZeichenPos, stZeichengrosse);
			ptlCaret.x += stZeichengrosse.cx;
		}
		else{	rclDirty.left = 0; rclDirty.right = lBreite; }

		OnRender(true);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SetSelectTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																																 _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSicher_Anfang();
	crfSelectText = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SetSelectTextColor(_In_ D2D1_COLOR_F& crfSelectTextA)
{
	ThreadSicher_Anfang();
	crfSelectText = crfSelectTextA;
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SetSelectBackgroundColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																																			 _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSicher_Anfang();
	crfSelectBack = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifSelectBackColor) ifSelectBackColor->SetColor(crfSelectBack);
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SetSelectBackgroundColor(_In_ D2D1_COLOR_F& crfSelectBackA)
{
	ThreadSicher_Anfang();
	crfSelectBack = crfSelectBackA;
	if(ifSelectBackColor) ifSelectBackColor->SetColor(crfSelectBack);
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SetCaretColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen,
																														_In_ unsigned char ucBlue, _In_ unsigned char ucAlpha)
{
	ThreadSicher_Anfang();
	crfCaret = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), ucAlpha);
	if(ifCaretColor) ifCaretColor->SetColor(crfCaret);

	rclDirty.left = ptlCaret.x; rclDirty.right = ptlCaret.x + CARET_PIXEL;
	OnRender(true);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SetCaretColor(_In_ D2D1_COLOR_F& crfCaretA)
{
	ThreadSicher_Anfang();
	crfCaret = crfCaretA;
	if(ifCaretColor) ifCaretColor->SetColor(crfCaret);

	rclDirty.left = ptlCaret.x; rclDirty.right = ptlCaret.x + CARET_PIXEL;
	OnRender(true);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SetzZeichen_Max(unsigned long ulZeichen)
{
	ThreadSicher_Anfang();
	ulZeichen_max = ulZeichen;
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
unsigned long __vectorcall RePag::DirectX::COEditLine::Zeichen_Max(void)
{
	ThreadSicher_Anfang();
	ULONG ulMaxZeichen = ulZeichen_max;
	ThreadSicher_Ende();
	return ulMaxZeichen;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::Zeichenvorgabe(unsigned char ucZeichenVorgabeA)
{
	ThreadSicher_Anfang();
	ucZeichenVorgabe = ucZeichenVorgabeA;
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::Zeichenmaske(const char* pcZeichenmaske)
{
	ThreadSicher_Anfang();
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
	ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::DirectX::COEditLine::Zeichenmaske(COStringA* pasZeichenmaske)
{
	ThreadSicher_Anfang();
	*pasZeichenmaske = *vasZeichenMaske;
	ThreadSicher_Ende();
	return pasZeichenmaske;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SelectAlles(void)
{
	if(hWndElement){
		SIZE stZeichengrosse;
		ThreadSicher_Anfang();
		cSelect = 1; ulSelectPos = 0; ulZeichenPos = vasInhalt->Length();

		GetTextPoint(vasInhalt->c_Str(), vasInhalt->Length(), stZeichengrosse);
		ResetEvent(heCaret);

		if(ucSchriftausrichtung & TXA_LINKS){ rclSelect.right = stZeichengrosse.cx; rclSelect.left = 0; ptlCaret.x = stZeichengrosse.cx; }
		else if(ucSchriftausrichtung & TXA_RECHTS){ rclSelect.right = lBreite; ptlCaret.x = lBreite;
			(stZeichengrosse.cx > lBreite ? rclSelect.left = 0 : rclSelect.left = lBreite - stZeichengrosse.cx);
		}
		else{
			if(stZeichengrosse.cx > lBreite){ rclSelect.left = 0; rclSelect.right = lBreite;}
			else{ rclSelect.left = (lBreite - stZeichengrosse.cx) / 2; rclSelect.right = rclSelect.left + stZeichengrosse.cx; }
		}

		rclDirty.left = rclSelect.left; rclDirty.right = rclSelect.right;
		OnRender(true);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		ThreadSicher_Ende();
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::SelectEntfernen(void)
{
	ThreadSicher_Anfang();
	if(cSelect) DeSelect();
	ThreadSicher_Ende();
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
	pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wcInhalt, szBytes_Text, ifText, (float)lBreite, (float)lHohe, &ifTextLayout);
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
	ptlScrollOffset.x = lBreite - stZeichengrosse.cx + lTextPos;
	lTextPos = stZeichengrosse.cx - lBreite;
	rclScroll.left = 0; rclScroll.right = lBreite + ptlScrollOffset.x;
	rclDirty.right = lBreite; rclDirty.left = rclScroll.right - CARET_PIXEL * 2;
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
		if(ptlCaret.x + CARET_PIXEL == lBreite) rclSelect.left = rclScroll.right;
		else rclSelect.left = ptlCaret.x - CARET_PIXEL - lTextPos;
	}
	ptlCaret.x = lBreite - CARET_PIXEL;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::ScrollLeft(SIZE& stZeichengrosse)
{
	ptlScrollOffset.x = lTextPos - stZeichengrosse.cx;
	lTextPos = stZeichengrosse.cx;
	rclScroll.right = lBreite; rclScroll.left = ptlScrollOffset.x;
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