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
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COEditLine* __vectorcall RePag::DirectX::COEditLineV(_In_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																														 _In_ STDeviceResources* pstDeviceResources)
{
	COEditLine* vEditLine = (COEditLine*)VMBlock(VMDialog(), sizeof(COEditLine));
	vEditLine->COEditLineV(VMDialog(), pcWindowName, uiIDElement, pstDeviceResources);
	return vEditLine;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COEditLine* __vectorcall RePag::DirectX::COEditLineV(_In_ const VMEMORY vmMemory, _In_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																														 _In_ STDeviceResources* pstDeviceResources)
{
	COEditLine* vEditLine = (COEditLine*)VMBlock(vmMemory, sizeof(COEditLine));
	vEditLine->COEditLineV(vmMemory, pcWindowName, uiIDElement, pstDeviceResources);
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
														if(pEditLine->htEffect_Timer) DeleteTimerQueueTimer(TimerQueue(), pEditLine->htEffect_Timer, INVALID_HANDLE_VALUE);
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
	_EditLine->rclDirty.left = _EditLine->FloatToLong(_EditLine->ptfCaret.x);
	_EditLine->rclDirty.right = _EditLine->rclDirty.left + _EditLine->ucCaretStrength;

	_EditLine->OnRender(bCaret);
	_EditLine->ifDXGISwapChain4->Present1(0, NULL, &_EditLine->dxgiPresent);
	_EditLine->ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::COEditLineV(_In_ const VMEMORY vmMemory, _In_ const char* pcClassName, _In_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																											_In_ STDeviceResources* pstDeviceResourcesA)
{
	 COTextLineV(vmMemory, pcClassName, pcWindowName, uiIDElementA, pstDeviceResourcesA);

	 pfnWM_Char_Return = nullptr;
	 pfnWM_Char_Escape = nullptr;
	 pfnWM_KillFocus = nullptr;
	 pfnWM_Command = nullptr;
	 pfnWM_LButtonDown = nullptr;
	 pfnWM_LButtonUp = nullptr;
	 pfnWM_LButtonDBClick = nullptr;
	 ptfCaret = {0};
	 ucCaretStrength = 1;
	 cSelect = 0;
	 rcfSelect = {0};
	 fTextPos = 0.0f;
	 ulCharacterPos = 0;
	 ulSelectPos = 0;
	 ulZeichen_max = 0x7fffffff;
	 ucZeichenVorgabe = ZV_ALLE; 
	 crfSelectText = D2D1::ColorF(RGB(0, 0, 0), 1.0f);
	 crfSelectBack = D2D1::ColorF(RGB(215, 215, 0), 1.0f);
	 crfCaret = D2D1::ColorF(RGB(255, 255, 255), 1.0f);
	 htCaret = nullptr;
	 heCaret = CreateEvent(nullptr, true, true, nullptr);

	 vasZeichenMaske = COStringAV(vmMemory);

	 hMenu = CreatePopupMenu();
	 AppendMenu(hMenu, MF_STRING, IDM_AUSSCHNEIDEN, "Ausschneiden		Strg+X");
	 AppendMenu(hMenu, MF_STRING, IDM_KOPIEREN, "Kopieren		Strg+C");
	 AppendMenu(hMenu, MF_STRING, IDM_EINFUGEN, "Einfügen		Strg+V");
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::COEditLineV(_In_ const VMEMORY vmMemory, _In_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																											_In_ STDeviceResources* pstDeviceResourcesA)
{
	COEditLineV(vmMemory, pcRePag_EditLine, pcWindowName, uiIDElementA, pstDeviceResourcesA);
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
	mbstowcs_s(&szBytes_Text, wcInhalt, vasContent->c_Str(), vasContent->Length());
	pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wcInhalt, (UINT32)szBytes_Text, ifText, (float)lWidth, (float)lHeight, &ifTextLayout);
	TextAlignment(ifTextLayout, fTextWidth, rcfText);
	SafeRelease(&ifTextLayout);

	if(ucTextAlignment & TXA_RIGHT && !fTextPos && rcfText.left >= 0){}
	else if(ucTextAlignment & TXA_CENTERHORIZONTAL && rcfText.left >= 0){}
	else rcfText.left = 0;
  rcfText.right = fTextPos + (float)lWidth;

	ifD2D1Context6->BeginDraw();
	ifD2D1Context6->Clear(crfBackground);

	ifD2D1Context6->SetTransform(D2D1::Matrix3x2F::Translation(-fTextPos, 0.0f));
	ifD2D1Context6->DrawText(wcInhalt, (UINT32)szBytes_Text, ifText, rcfText, ifTextColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);
		
	if(!cSelect){
		if(bCaret){	D2D1_POINT_2F ptfTop, ptfBottom;
			ptfTop.x = ptfCaret.x + fTextPos;
			ptfTop.y = ptfCaret.y + szfCharacter.height;
			ptfBottom.x = ptfCaret.x + fTextPos;
			ptfBottom.y = ptfCaret.y;
			ifD2D1Context6->DrawLine(ptfTop, ptfBottom, ifCaretColor, (float)ucCaretStrength, nullptr);
		}
	}
	else{	VMBLOCK vbCharacter; ULONG ulZeichen; D2D1_RECT_F rcfSelect_1;
		if(cSelect > 0)	ulZeichen = vasContent->SubString(vbCharacter, ulSelectPos + 1, ulCharacterPos);
		else ulZeichen = vasContent->SubString(vbCharacter, ulCharacterPos + 1, ulSelectPos);

		rcfSelect_1.top = rcfSelect.top; rcfSelect_1.bottom = rcfSelect.bottom;
		rcfSelect_1.left = rcfSelect.left + fTextPos; rcfSelect_1.right = rcfSelect.right + fTextPos;

		ifD2D1Context6->FillRectangle(&rcfSelect_1, ifSelectBackColor);

		ifTextColor->SetColor(crfSelectText);
		mbstowcs_s(&szBytes_Text, wcInhalt, vbCharacter, ulZeichen); VMFrei(vbCharacter);
		ifD2D1Context6->DrawText(wcInhalt, (UINT32)szBytes_Text, ifText, rcfSelect_1, ifTextColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);
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

	ifD2D1Context6->CreateSolidColorBrush(crfText, &ifTextColor);
	ifD2D1Context6->CreateSolidColorBrush(crfSelectBack, &ifSelectBackColor);
	ifD2D1Context6->CreateSolidColorBrush(crfCaret, &ifCaretColor);

	ifText->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	D2D_SIZE_F szfTextPoint;
	if(ucTextAlignment & TXA_RIGHT){
		GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
		if(szfTextPoint.width > (float)lWidth) fTextPos = szfTextPoint.width - (float)lWidth;
	}
	else if(ucTextAlignment & TXA_CENTERHORIZONTAL){
		GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
		if(szfTextPoint.width > (float)lWidth){
			fTextPos = szfTextPoint.width - (float)lWidth;
			fTextPos /= 2;
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

	mbstowcs_s(&szBytes_Text, wcInhalt, vasContent->c_Str(), vasContent->Length());
	pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wcInhalt, (UINT32)szBytes_Text, ifText, (float)lWidth, (float)lHeight, &ifTextLayout);

	TextAlignment(ifTextLayout, fTextWidth, ptfText);
	SafeRelease(&ifTextLayout);

	rcfSelect.top = ptfCaret.y = ptfText.y;
  rclDirty.top = FloatToLong(ptfText.y);
	rcfSelect.bottom = (float)lHeight - ptfCaret.y;
	rclDirty.bottom = FloatToLong(rcfSelect.bottom);

	if(vasZeichenMaske->Length()){
		ptfCaret.x = ptfText.x;
		if(!cSelect) ulCharacterPos = 0;
		EnableMenuItem(hMenu, IDM_AUSSCHNEIDEN, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_EINFUGEN, MF_BYCOMMAND | MF_GRAYED);
	}
	else{	ptfCaret.x = ptfText.x + fTextWidth;
		if(!cSelect) ulCharacterPos = vasContent->Length();
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
	
	rclDirty.left = FloatToLong(ptfCaret.x); rclDirty.right = FloatToLong(ptfCaret.x) + ucCaretStrength;
	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);

	if(pfnWM_KillFocus) pfnWM_KillFocus(this);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::WM_KeyDown(WPARAM wParam, LPARAM lParam)
{
  D2D_SIZE_F szfTextPoint, szfTextPoint_1; float fScrollOffset; VMBLOCK vbCharacter;
	switch(wParam){
		case VK_HOME    : ThreadSafe_Begin();
											DeleteCaretPos();
											if(cSelect) DeSelect();
											ulCharacterPos = 0;
											if(fTextPos){
												fTextPos = 0.0f; ptfCaret.x = 0.0f;
												rclDirty.left = 0; rclDirty.right = lWidth;
												OnRender(true);
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											}
											else if(ucTextAlignment & TXA_LEFT) ptfCaret.x = 0.0f; 
											else{ 
												GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
												if(szfTextPoint.width > (float)lWidth) ptfCaret.x = 0.0f;
												else if(ucTextAlignment & TXA_RIGHT) ptfCaret.x = (float)lWidth - szfTextPoint.width;
												else ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f;
											} 
											ThreadSafe_End();
											break;
		case VK_END     : ThreadSafe_Begin();
											DeleteCaretPos();
											if(cSelect) DeSelect();
											ulCharacterPos = vasContent->Length();
											GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
											if(szfTextPoint.width > (float)lWidth){
												fTextPos = szfTextPoint.width - (float)lWidth; ptfCaret.x = (float)(lWidth - ucCaretStrength);
												rclDirty.left = 0; rclDirty.right = lWidth;
												OnRender(true);
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											}
											else if(ucTextAlignment & TXA_LEFT) ptfCaret.x = szfTextPoint.width;
											else if(ucTextAlignment & TXA_RIGHT) ptfCaret.x = (float)(lWidth - ucCaretStrength);
											else ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f + szfTextPoint.width;
											ThreadSafe_End();
											break;
		case VK_LEFT		: ThreadSafe_Begin();											
											if(ulCharacterPos){
												if(cSelect) DeleteCaretPos();
												rclDirty.right = FloatToLong(ptfCaret.x);
												GetTextPoint(vasContent->c_Str(), --ulCharacterPos, szfTextPoint);
												if(szfTextPoint.width - fTextPos < 0.0f){
													fScrollOffset = fTextPos - szfTextPoint.width;
													fTextPos = szfTextPoint.width;
													rclDirty.left = 0; rclDirty.right = lWidth;
													OnRender(true);
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);

													if(cSelect){
														rcfSelect.right += fScrollOffset;
														rclDirty.right = FloatToLong(rcfSelect.right);
													}
													else rcfSelect.right = szfCharacter.width;
													ptfCaret.x = 0.0f;
												}
												else if(ucTextAlignment & TXA_LEFT){
													if(vasZeichenMaske->Length()){
														if(ZeichenMaske_FestLinks()) GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
													}
													ptfCaret.x = szfTextPoint.width - fTextPos;
													rclDirty.left = FloatToLong(ptfCaret.x);
													rclDirty.right += ucCaretStrength;
												}
												else{
													if(vasZeichenMaske->Length()){
														if(ZeichenMaske_FestLinks()){
															GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
															ucTextAlignment & TXA_RIGHT ? ptfCaret.x = (float)lWidth - szfTextPoint.width
																: ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f;
															GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
															ptfCaret.x += szfTextPoint.width;
														}
														else{
															vasContent->SubString(vbCharacter, ulCharacterPos + 1, ulCharacterPos + 1);
															GetTextPoint(vbCharacter, 1, szfTextPoint); VMFrei(vbCharacter);
															ptfCaret.x -= szfTextPoint.width;
														}
													}
													else{
														szfTextPoint_1 = szfTextPoint;
														GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
														if(szfTextPoint.width < (float)lWidth){
															if(ucTextAlignment & TXA_RIGHT)	ptfCaret.x = (float)lWidth - szfTextPoint.width + szfTextPoint_1.width;
															else ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f + szfTextPoint_1.width;
														}
														else ptfCaret.x = szfTextPoint_1.width - fTextPos;
														rclDirty.left = FloatToLong(ptfCaret.x);
														rclDirty.right += ucCaretStrength;
													}
												}

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
													if(cSelect < 0){ rcfSelect.left = ptfCaret.x;	rclDirty.left = FloatToLong(ptfCaret.x); }
													else if(cSelect > 0){
														if(ulSelectPos < ulCharacterPos){
															rcfSelect.right = ptfCaret.x;
															rclDirty.right += ucCaretStrength;
															rclDirty.left = FloatToLong(ptfCaret.x);
														}
														else{ cSelect = 0; SetEvent(heCaret);
															rclDirty.left = FloatToLong(ptfCaret.x);	}
													}
													else{
														cSelect = -1;
														ulSelectPos = ulCharacterPos + 1;
														if(!ptfCaret.x) rclDirty.right = FloatToLong(rcfSelect.right);
														else{
															rcfSelect.left = ptfCaret.x;
															rclDirty.left = FloatToLong(ptfCaret.x);
															rcfSelect.right = (float)(rclDirty.right - ucCaretStrength);
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
											if(ulCharacterPos < vasContent->Length()){
												if(!cSelect) DeleteCaretPos();
												rclDirty.left = FloatToLong(ptfCaret.x);
												GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint);
												if(szfTextPoint.width - fTextPos > (float)lWidth){
													fScrollOffset = (float)lWidth - szfTextPoint.width + fTextPos;
													fTextPos = szfTextPoint.width - (float)lWidth;
													rclDirty.left = 0; rclDirty.right = lWidth;
													OnRender(true);
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													if(cSelect){
														rcfSelect.left += fScrollOffset;
														rclDirty.left = FloatToLong(rcfSelect.left);
													}
													else rcfSelect.left = (float)lWidth - szfCharacter.width - ucCaretStrength;
													ptfCaret.x = (float)(lWidth - ucCaretStrength);
												}
												else if(szfTextPoint.width - fTextPos == (float)lWidth)	ptfCaret.x = (float)(lWidth - ucCaretStrength);
												else if(ucTextAlignment & TXA_LEFT){
													if(vasZeichenMaske->Length()){
														if(ZeichenMaske_FestRechts()) GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
													}
													rclDirty.left -= ucCaretStrength;
													ptfCaret.x = szfTextPoint.width - fTextPos;
													rclDirty.right = FloatToLong(ptfCaret.x);
												}
												else{
 													if(vasZeichenMaske->Length()){
														if(ZeichenMaske_FestRechts()){
															GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
															ucTextAlignment & TXA_RIGHT ? ptfCaret.x = (float)lWidth - szfTextPoint.width
																: ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f;
															GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
															ptfCaret.x += szfTextPoint.width;
														}
	 													else{
															vasContent->SubString(vbCharacter, ulCharacterPos, ulCharacterPos);
															GetTextPoint(vbCharacter, 1, szfTextPoint); VMFrei(vbCharacter);
															ptfCaret.x = (float)lWidth - szfTextPoint.width;
														}
													}
													else{
														szfTextPoint_1 = szfTextPoint;
														GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
														if(szfTextPoint.width < (float)lWidth){
															if(ucTextAlignment & TXA_RIGHT)	ptfCaret.x = (float)lWidth - szfTextPoint.width + szfTextPoint_1.width;
															else ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2 + szfTextPoint_1.width;
															if(ptfCaret.x == (float)lWidth) ptfCaret.x -= (float)ucCaretStrength;
														}
														else ptfCaret.x = szfTextPoint_1.width - fTextPos;
														rclDirty.left -= ucCaretStrength;
														rclDirty.right = FloatToLong(ptfCaret.x);
													}
												}

												if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){ 
													if(cSelect > 0){
														rcfSelect.right = ptfCaret.x;// +(float)ucCaretStrength;
														rclDirty.right = FloatToLong(rcfSelect.right);
													}
													else if(cSelect < 0){
														if(ulSelectPos > ulCharacterPos){
															rcfSelect.left = ptfCaret.x;
															//if(ptfCaret.x > (float)rclDirty.right){
															//	rclDirty.left = rclDirty.right;
															//	rclDirty.right = FloatToLong(ptfCaret.x);
															//}
														}
														else{ cSelect = 0; SetEvent(heCaret); }
													}
													else{ 
														cSelect = 1;
														ulSelectPos = ulCharacterPos - 1;
														//if(szfTextPoint.width - fTextPos == (float)lWidth){
														//	rclDirty.left = FloatToLong(rcfSelect.left);
														//	rcfSelect.right = (float)lWidth;
														//}
														//else{
															rcfSelect.left = (float)rclDirty.left;
															rcfSelect.right = ptfCaret.x - (float)ucCaretStrength;
															rclDirty.right = FloatToLong(rcfSelect.right);
														//}
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
												else if(ulCharacterPos < vasContent->Length()){
													GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
													if(ucTextAlignment & TXA_LEFT){
														if(!fTextPos){
															vasContent->Delete(ulCharacterPos, 1);
															rclDirty.right = lWidth;
                              rclDirty.left = FloatToLong(ptfCaret.x);
														}
														else{
															vasContent->Delete(--ulCharacterPos, 1);
															GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
                              fTextPos = szfTextPoint.width - ptfCaret.x;
                              if(fTextPos < 0.0f){
																ptfCaret.x += fTextPos;
																fTextPos = 0.0f;
																rclDirty.right = lWidth;
															}
															else rclDirty.right = FloatToLong(ptfCaret.x);
															rclDirty.left = 0;
														}
														OnRender(true);
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													}
													else if(ucTextAlignment & TXA_RIGHT){
														if(szfTextPoint.width - fTextPos <= (float)lWidth){
															if(!ulCharacterPos){
																vasContent->Delete(ulCharacterPos, 1);
																rclDirty.left = FloatToLong(ptfCaret.x) - ucCaretStrength;
																GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
																rclDirty.right = lWidth - FloatToLong(szfTextPoint.width);
																ptfCaret.x = (float)rclDirty.right;
																if(ptfCaret.x >= (float)lWidth) ptfCaret.x -= ucCaretStrength;
															}
															else{
																vasContent->Delete(--ulCharacterPos, 1);
																if(fTextPos <= 0) fTextPos = 0;
																else{
																	GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
																	fTextPos = szfTextPoint.width - ptfCaret.x;
																}
																rclDirty.right = FloatToLong(ptfCaret.x);
																rclDirty.left = 0;
															}
														}
														else{
															vasContent->Delete(ulCharacterPos, 1);
															vasContent->SubString(vbCharacter, ulCharacterPos + 1, ulCharacterPos + 1);
															GetTextPoint(vbCharacter, 1, szfTextPoint_1); VMFrei(vbCharacter);
															if(szfTextPoint.width - fTextPos - (float)lWidth < szfTextPoint_1.width){
																GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
																fTextPos = szfTextPoint.width - (float)lWidth;
																GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
																ptfCaret.x = szfTextPoint.width - fTextPos;
																rclDirty.left = 0;
															}
															else rclDirty.left = FloatToLong(ptfCaret.x);
															rclDirty.right = lWidth;
														}
														OnRender(true);
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													}
													else{ // TXA_Mitte
														if(szfTextPoint.width < (float)lWidth){
															rclDirty.left = FloatToLong(((float)lWidth - szfTextPoint.width) / 2.0f);
															rclDirty.right = rclDirty.left + FloatToLong(szfTextPoint.width);
															vasContent->Delete(ulCharacterPos, 1);
															GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
															ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f;
															GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
															ptfCaret.x += szfTextPoint.width;
                            }
														else{
															rclDirty.left = 0; rclDirty.right = lWidth;
															vasContent->Delete(ulCharacterPos, 1);

															vasContent->SubString(vbCharacter, ulCharacterPos + 1, ulCharacterPos + 1);
															GetTextPoint(vbCharacter, 1, szfTextPoint_1); VMFrei(vbCharacter);
															if(szfTextPoint.width - szfTextPoint_1.width < (float)lWidth){
																GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
																fTextPos = 0;
																ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f;
																GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
																ptfCaret.x += szfTextPoint.width;
															}
															else{
																GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
																fTextPos = (szfTextPoint.width - (float)lWidth) / 2.0f;
																GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
																ptfCaret.x = szfTextPoint.width - fTextPos;
															}
														}
														OnRender(true);
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
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
  D2D_SIZE_F szfTextPoint, szfTextPoint_1; VMBLOCK vbCharacter;
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
											if(ulCharacterPos || !ulCharacterPos && cSelect){ 
												if(!vasZeichenMaske->Length()){
													if(cSelect) Select_Loschen();
													else{
														GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
														if(ucTextAlignment & TXA_LEFT){ 
															vasContent->Delete(--ulCharacterPos, 1);
															if(!fTextPos){
																rclDirty.right = lWidth;
																GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
                                ptfCaret.x = szfTextPoint.width;
																rclDirty.left = FloatToLong(ptfCaret.x);
															}
															else{
																rclDirty.left = 0;
																rclDirty.right = FloatToLong(ptfCaret.x);
																GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
																fTextPos = szfTextPoint.width - ptfCaret.x;
																if(fTextPos < 0.0f){
																	rclDirty.right = lWidth;
                                  ptfCaret.x += fTextPos; 
																	fTextPos = 0.0f;
																}
															}
															OnRender(true);
                              ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														}
														else if(ucTextAlignment & TXA_RIGHT){ 
															if(!fTextPos){
																rclDirty.right = FloatToLong(ptfCaret.x);
																rclDirty.left = lWidth - FloatToLong(szfTextPoint.width);
																vasContent->Delete(--ulCharacterPos, 1);
															}
															else{
																vasContent->SubString(vbCharacter, ulCharacterPos, ulCharacterPos);
																GetTextPoint(vbCharacter, 1, szfTextPoint_1); VMFrei(vbCharacter);
																vasContent->Delete(--ulCharacterPos, 1);
																if(szfTextPoint.width - fTextPos - (float)lWidth <= 0.0f){
																	rclDirty.left = 0;
																	rclDirty.right = FloatToLong(ptfCaret.x);
																	GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
																	fTextPos = szfTextPoint.width - (float)lWidth;
																}
																else{
																	if(szfTextPoint.width - fTextPos - (float)lWidth > szfTextPoint_1.width){
																		rclDirty.right = lWidth;
																		GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
																		ptfCaret.x = szfTextPoint.width - fTextPos;
																		rclDirty.left = FloatToLong(ptfCaret.x);
																	}
																	else{
																		rclDirty.left = 0;
																		rclDirty.right = lWidth;
																		GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
																		fTextPos = szfTextPoint.width - (float)lWidth;
																		GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
																		ptfCaret.x = szfTextPoint.width - fTextPos;

																	}
																}
															}
															OnRender(true);
                              ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);	
														}
														else{ // TXA_Mitte
															if(szfTextPoint.width <= (float)lWidth){
																rclDirty.left = FloatToLong(((float)lWidth - szfTextPoint.width) / 2.0f);
																rclDirty.right = rclDirty.left + FloatToLong(szfTextPoint.width);
																vasContent->Delete(--ulCharacterPos, 1);
																GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
																ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f;
																GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
																ptfCaret.x += szfTextPoint.width;
															}
															else{
																vasContent->SubString(vbCharacter, ulCharacterPos, ulCharacterPos);
																GetTextPoint(vbCharacter, 1, szfTextPoint_1); VMFrei(vbCharacter);
																vasContent->Delete(--ulCharacterPos, 1);
																rclDirty.left = 0; rclDirty.right = lWidth;
																if(szfTextPoint.width - szfTextPoint_1.width < (float)lWidth){
																	fTextPos = 0;
																	GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
																	ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f;
																	GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
																	ptfCaret.x += szfTextPoint.width;
																}
																else{
																	GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
																	fTextPos = (szfTextPoint.width - (float)lWidth) / 2.0f;
																	GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
																	ptfCaret.x = szfTextPoint.width - fTextPos;
																}
                              }
															OnRender(true);
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														}
													}
												}
												else{
													if(cSelect){
														if(cSelect < 0) ulCharacterPos = ulSelectPos;
														DeSelect();
														GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
														ptfCaret.x = szfTextPoint.width;
													}

													if(ZeichenMaske_Loschen()){
														vasContent->Delete(--ulCharacterPos, 1);
														(ucZeichenVorgabe & ZV_SICHTBAR ? vasContent->Insert("_", ulCharacterPos) : vasContent->Insert(" ", ulCharacterPos));
														rclDirty.right = 1;
													}
													else rclDirty.right = 0;

													if(ucTextAlignment & TXA_LEFT) ptfCaret.x = 0.0f;	
													else if(ucTextAlignment & TXA_RIGHT){
														GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
														ptfCaret.x = (float)lWidth - szfTextPoint.width;
													}
													else{
														GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
														ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f;
													}
													GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
													ptfCaret.x += szfTextPoint.width;

													if(rclDirty.right){ 
														rclDirty.left = FloatToLong(ptfCaret.x); rclDirty.right = lWidth;
														OnRender(true);
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													}
												}
											} 
											ThreadSafe_End();
											PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), WM_CHAR);
											break;
		default        :  ThreadSafe_Begin();
											if(ZeichenVorgabe(wParam) && ulCharacterPos < ulZeichen_max){
												if(!vasZeichenMaske->Length()){
													if(cSelect) Select_Loschen();
													GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
													if(ucTextAlignment & TXA_LEFT){
														if(szfTextPoint.width < (float)lWidth){
															vasContent->SubString(vbCharacter, ulCharacterPos, ulCharacterPos);
															GetTextPoint(vbCharacter, 1, szfTextPoint_1); VMFrei(vbCharacter);
															if((float)lWidth - ptfCaret.x > szfTextPoint_1.width){
																rclDirty.left = FloatToLong(ptfCaret.x) - ucCaretStrength;
																ulCharacterPos == vasContent->Length() ? *vasContent += (char*)&wParam
																																			 : vasContent->Insert((char*)&wParam, ulCharacterPos);
																GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
																szfTextPoint.width > (float)lWidth ? rclDirty.right = lWidth : rclDirty.right = FloatToLong(szfTextPoint.width);
																GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint);
																ptfCaret.x = szfTextPoint.width;
															}
															else{
																rclDirty.left = 0; rclDirty.right = lWidth;
																ulCharacterPos == vasContent->Length() ? *vasContent += (char*)&wParam
																																			 : vasContent->Insert((char*)&wParam, ulCharacterPos);
																GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint);
																fTextPos = szfTextPoint.width - (float)lWidth;
																ptfCaret.x = (float)(lWidth - ucCaretStrength);
															}
														}
														else if(ucZeichenVorgabe & ZV_UBERLANGE){
															vasContent->SubString(vbCharacter, ulCharacterPos, ulCharacterPos);
															GetTextPoint(vbCharacter, 1, szfTextPoint_1); VMFrei(vbCharacter);
															if((float)lWidth - ptfCaret.x > szfTextPoint_1.width){
																rclDirty.left = FloatToLong(ptfCaret.x - ucCaretStrength);
																rclDirty.right = lWidth;
																ulCharacterPos == vasContent->Length() ? *vasContent += (char*)&wParam
																																			 : vasContent->Insert((char*)&wParam, ulCharacterPos);
																GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint);
																ptfCaret.x = szfTextPoint.width - fTextPos;
															}
															else{
																rclDirty.left = 0; rclDirty.right = lWidth;
																ulCharacterPos == vasContent->Length() ? *vasContent += (char*)&wParam
																																			 : vasContent->Insert((char*)&wParam, ulCharacterPos);
																GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint);
																fTextPos = szfTextPoint.width - (float)lWidth;
																ptfCaret.x = (float)(lWidth - ucCaretStrength);
															}
														}
														OnRender(true);
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													}
													else if(ucTextAlignment & TXA_RIGHT){
														if(szfTextPoint.width < (float)lWidth){
															vasContent->SubString(vbCharacter, ulCharacterPos, ulCharacterPos);
															GetTextPoint(vbCharacter, 1, szfTextPoint_1); VMFrei(vbCharacter);
															if((float)lWidth - szfTextPoint.width > szfTextPoint_1.width){
																rclDirty.right = FloatToLong(ptfCaret.x);
																ulCharacterPos == vasContent->Length() ? *vasContent += (char*)&wParam
																																			 : vasContent->Insert((char*)&wParam, ulCharacterPos);
																GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
																rclDirty.left = lWidth - FloatToLong(szfTextPoint.width);
																ulCharacterPos++;
															}
															else{
																rclDirty.right = FloatToLong(ptfCaret.x);
																ulCharacterPos == vasContent->Length() ? *vasContent += (char*)&wParam
																																			 : vasContent->Insert((char*)&wParam, ulCharacterPos);
																rclDirty.left = 0;
																GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint);
																fTextPos = szfTextPoint.width - ptfCaret.x;
															}
														}
														else if(ucZeichenVorgabe & ZV_UBERLANGE){
															vasContent->SubString(vbCharacter, ulCharacterPos, ulCharacterPos);
															GetTextPoint(vbCharacter, 1, szfTextPoint_1); VMFrei(vbCharacter);
															rclDirty.right = FloatToLong(ptfCaret.x);
															rclDirty.left = 0;
															ulCharacterPos == vasContent->Length() ? *vasContent += (char*)&wParam
																																			: vasContent->Insert((char*)&wParam, ulCharacterPos);
															GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint);
															fTextPos = szfTextPoint.width - ptfCaret.x;
														}
														OnRender(true);
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													}
													else{ // TXA_MITTE
														ulCharacterPos == vasContent->Length() ? *vasContent += (char*)&wParam
																																	 : vasContent->Insert((char*)&wParam, ulCharacterPos);
														GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
														if(szfTextPoint.width < (float)lWidth){
															rclDirty.left = (lWidth - FloatToLong(szfTextPoint.width)) / 2;
															rclDirty.right = rclDirty.left + FloatToLong(szfTextPoint.width);
															GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint);
															ptfCaret.x = (float)rclDirty.left + szfTextPoint.width;
															OnRender(true);
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														}
														else if(ucZeichenVorgabe & ZV_UBERLANGE){
															rclDirty.left = 0; rclDirty.right = lWidth;
															fTextPos = (szfTextPoint.width - (float)lWidth) / 2.0f;
															GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint);
															ptfCaret.x = szfTextPoint.width - fTextPos;
															if(ptfCaret.x >= (float)lWidth){
																ptfCaret.x = (float)(lWidth - ucCaretStrength);
																fTextPos = szfTextPoint.width - (float)lWidth;
															}
															OnRender(true);
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														}
														else vasContent->Delete(ulCharacterPos, 1);
													}
												}
												else{
													if(cSelect){
														if(cSelect > 0){
															ulCharacterPos = ulSelectPos;
															GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
															ptfCaret.x = szfTextPoint.width;
														}
														DeSelect();
													}
													else{
														if(ZeichenMaske_Einfugen(wParam)){
															vasContent->Delete(ulCharacterPos, 1); vasContent->Insert((char*)&wParam, ulCharacterPos++);
															if(ucTextAlignment & TXA_LEFT) ptfCaret.x = 0.0f;
															else if(ucTextAlignment & TXA_RIGHT){
																GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
																ptfCaret.x = (float)lWidth - szfTextPoint.width;
															}
															else{ // TXA_MITTE
																GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
																ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f;
															}

															GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
															ptfCaret.x += szfTextPoint.width;
															rclDirty.left = 0; rclDirty.right = FloatToLong(ptfCaret.x);
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
	VMBLOCK vbCharacter; HGLOBAL hGlobal; char* pcAblage; ULONG ulZeichen; D2D_SIZE_F szfTextPoint_Inhalt, szfTextPoint_Clipboard;
	switch(LOWORD(wParam)){
		case IDM_KOPIEREN			: ThreadSafe_Begin();
														OpenClipboard(hWndElement); EmptyClipboard();
														if(cSelect > 0) ulZeichen = vasContent->SubString(vbCharacter, ulSelectPos + 1, ulCharacterPos);
														else ulZeichen = vasContent->SubString(vbCharacter, ulCharacterPos + 1, ulSelectPos);
														hGlobal = GlobalAlloc(GMEM_MOVEABLE, ulZeichen + 1);
														pcAblage = (char*)GlobalLock(hGlobal);
														MemCopy(pcAblage, vbCharacter, ulZeichen); VMFrei(vbCharacter);
														pcAblage[ulZeichen] = 0;
														GlobalUnlock(hGlobal);
														SetClipboardData(CF_TEXT, hGlobal); CloseClipboard();
														ThreadSafe_End();
														return false;
		case IDM_AUSSCHNEIDEN : ThreadSafe_Begin();
														if(vasZeichenMaske->Length() || !ucZeichenVorgabe){ ThreadSafe_End(); return false; }
														OpenClipboard(hWndElement); EmptyClipboard();
														DeSelect();
														GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint_Inhalt);
														if(ulSelectPos <= ulCharacterPos){
															ulZeichen = vasContent->SubString(vbCharacter, ulSelectPos + 1, ulCharacterPos);
															vasContent->Delete(ulSelectPos, ulZeichen);
															ulCharacterPos = ulSelectPos;
														}
														else{
															ulZeichen = vasContent->SubString(vbCharacter, ulCharacterPos + 1, ulSelectPos);
															vasContent->Delete(ulCharacterPos, ulZeichen);
														}

														hGlobal = GlobalAlloc(GMEM_MOVEABLE, ulZeichen + 1);
														pcAblage = (char*)GlobalLock(hGlobal);
														MemCopy(pcAblage, vbCharacter, ulZeichen); VMFrei(vbCharacter);
														pcAblage[ulZeichen] = 0;
														GlobalUnlock(hGlobal);
														SetClipboardData(CF_TEXT, hGlobal); CloseClipboard();
														GetTextPoint(pcAblage, ulZeichen, szfTextPoint_Clipboard);						
														if(ucTextAlignment & TXA_LEFT){
															if(!fTextPos){
																szfTextPoint_Inhalt.width <= (float)lWidth ? rclDirty.right = FloatToLong(szfTextPoint_Inhalt.width)
																																					 : rclDirty.right = lWidth;
																GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint_Inhalt);
																ptfCaret.x = szfTextPoint_Inhalt.width;
																rclDirty.left = FloatToLong(szfTextPoint_Inhalt.width);
															}
															else{
																rclDirty.left = 0;
																if(fTextPos > szfTextPoint_Clipboard.width){
																	GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint_Inhalt);
																	ptfCaret.x = szfTextPoint_Inhalt.width + szfTextPoint_Clipboard.width - fTextPos;
																	rclDirty.right = FloatToLong(ptfCaret.x);																	
																	fTextPos -= szfTextPoint_Clipboard.width;
																}
																else{
																	GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint_Inhalt);
																	ptfCaret.x = szfTextPoint_Inhalt.width;
																	rclDirty.right = lWidth;
																	fTextPos = 0.0f;
																}
															}
															OnRender(true);
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														}
														else if(ucTextAlignment & TXA_RIGHT){														
															if(szfTextPoint_Inhalt.width - fTextPos < (float)lWidth){
																rclDirty.right = FloatToLong(rcfSelect.right);
																ptfCaret.x = rcfSelect.right;
																rclDirty.left = lWidth - FloatToLong(szfTextPoint_Inhalt.width);
															}
															else{
																if(szfTextPoint_Inhalt.width - fTextPos == (float)lWidth){
																	rclDirty.right = FloatToLong(rcfSelect.right);
																	ptfCaret.x = rcfSelect.right;
																	rclDirty.left = 0;
																	fTextPos -= szfTextPoint_Clipboard.width;
																	if(fTextPos < 0.0f) fTextPos = 0.0f;
																}
																else{
																	if(szfTextPoint_Inhalt.width - fTextPos - (float)lWidth < szfTextPoint_Clipboard.width){
																		rclDirty.left = 0; rclDirty.right = lWidth;
																		ptfCaret.x += szfTextPoint_Clipboard.width - (szfTextPoint_Inhalt.width - fTextPos - (float)lWidth);
																		fTextPos -= szfTextPoint_Clipboard.width - (szfTextPoint_Inhalt.width - fTextPos - (float)lWidth);
																	}
																	else{
																		rclDirty.right = lWidth;
																		rclDirty.left = FloatToLong(rcfSelect.left);
																		ptfCaret.x = rcfSelect.left;
																	}
																}
															}
															OnRender(true);
															ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														}
														else{ // TXA_MITTE
															fTextPos = ((szfTextPoint_Inhalt.width - szfTextPoint_Clipboard.width) - (float)lWidth) / 2.0f;
															GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint_Inhalt);
															ptfCaret.x = szfTextPoint_Inhalt.width - fTextPos;
															if(fTextPos < 0.0f) fTextPos = 0.0f;
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
														GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint_Inhalt);

														OpenClipboard(hWndElement);
														hGlobal = GetClipboardData(CF_TEXT);
														pcAblage = (char*)GlobalLock(hGlobal);
														ulZeichen = StrLength(pcAblage);
														GlobalUnlock(hGlobal);
														CloseClipboard();
														GetTextPoint(pcAblage, ulZeichen, szfTextPoint_Clipboard);

														szfTextPoint_Inhalt.width -= fTextPos;
														if(ucTextAlignment & TXA_LEFT){
															if(szfTextPoint_Inhalt.width + szfTextPoint_Clipboard.width < (float)lWidth){
																ulCharacterPos == vasContent->Length() ? *vasContent += pcAblage : vasContent->Insert(pcAblage, ulCharacterPos);
																ulCharacterPos += ulZeichen;
																rclDirty.left = FloatToLong(ptfCaret.x);
																rclDirty.right = FloatToLong(szfTextPoint_Inhalt.width + szfTextPoint_Clipboard.width);
																ptfCaret.x += szfTextPoint_Clipboard.width;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															else if(ucZeichenVorgabe & ZV_UBERLANGE){
																ulCharacterPos == vasContent->Length() ? *vasContent += pcAblage : vasContent->Insert(pcAblage, ulCharacterPos);
																ulCharacterPos += ulZeichen;
																GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint_Inhalt);
																if(szfTextPoint_Inhalt.width - fTextPos > (float)lWidth){
																	rclDirty.left = 0;
																	fTextPos = szfTextPoint_Inhalt.width - (float)lWidth;
																}
																else rclDirty.left = FloatToLong(ptfCaret.x);
																szfTextPoint_Inhalt.width - fTextPos < (float)lWidth ? ptfCaret.x = szfTextPoint_Inhalt.width - fTextPos
																																										 : ptfCaret.x = (float)(lWidth - ucCaretStrength);
																rclDirty.right = lWidth;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															// tue nix
														}
														else if(ucTextAlignment & TXA_RIGHT){
															if(szfTextPoint_Inhalt.width + szfTextPoint_Clipboard.width < (float)lWidth){
																ulCharacterPos == vasContent->Length() ? *vasContent += pcAblage : vasContent->Insert(pcAblage, ulCharacterPos);
																ulCharacterPos += ulZeichen;
																rclDirty.left = FloatToLong((float)lWidth - szfTextPoint_Inhalt.width - szfTextPoint_Clipboard.width);
																rclDirty.right = FloatToLong(ptfCaret.x);
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															else if(ucZeichenVorgabe & ZV_UBERLANGE){
																if(ulCharacterPos == vasContent->Length()){
																	*vasContent += pcAblage;
																	ptfCaret.x = (float)(lWidth - ucCaretStrength);
																}
																else vasContent->Insert(pcAblage, ulCharacterPos);
																ulCharacterPos += ulZeichen;
																fTextPos += szfTextPoint_Clipboard.width;
																rclDirty.left = 0; rclDirty.right = FloatToLong(ptfCaret.x);
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															// tue nix
														}
														else{ // TXA_MITTE
															if(szfTextPoint_Inhalt.width + szfTextPoint_Clipboard.width < (float)lWidth){
																ulCharacterPos == vasContent->Length() ? *vasContent += pcAblage : vasContent->Insert(pcAblage, ulCharacterPos);
																ulCharacterPos += ulZeichen;
																ptfCaret.x += szfTextPoint_Clipboard.width / 2.0f;
																rclDirty.left = 0; rclDirty.right = lWidth;
																OnRender(true);
																ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
															}
															else if(ucZeichenVorgabe & ZV_UBERLANGE){
																if(ulCharacterPos == vasContent->Length()){
																	*vasContent += pcAblage;
																	fTextPos = (szfTextPoint_Inhalt.width + szfTextPoint_Clipboard.width) - (float)lWidth;
																	ptfCaret.x = (float)(lWidth - ucCaretStrength);
																}
																else{
																	vasContent->Insert(pcAblage, ulCharacterPos);
																	fTextPos = ((szfTextPoint_Inhalt.width + szfTextPoint_Clipboard.width) - (float)lWidth) / 2.0f;
																	ptfCaret.x += szfTextPoint_Clipboard.width / 2.0f;
																}
																ulCharacterPos += ulZeichen;
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
		if(GET_X_LPARAM(lParam) < FloatToLong(ptfCaret.x - szfCharacter.width)) SendMessage(hWndElement, WM_KEYDOWN, VK_LEFT, NULL);
		else if(GET_X_LPARAM(lParam) > FloatToLong(ptfCaret.x + szfCharacter.width)) SendMessage(hWndElement, WM_KEYDOWN, VK_RIGHT, NULL);
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

	ulCharacterPos = 0;
	if(vasContent->Length()){	D2D_SIZE_F szfTextPoint;
		if(fTextPos || ucTextAlignment & TXA_LEFT){
			do{ GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint); }
			while(szfTextPoint.width - fTextPos < (float)LOWORD(lParam) && ulCharacterPos < vasContent->Length());
			ptfCaret.x = szfTextPoint.width - fTextPos;

			if(vasZeichenMaske->Length()){
				if(ZeichenMaske_FestRechts()){
					GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
					ptfCaret.x = szfTextPoint.width;
				}
			}
		}
		else{
			GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
			if(szfTextPoint.width > (float)lWidth){
				do{ GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint); }
				while(szfTextPoint.width < (float)LOWORD(lParam) && ulCharacterPos < vasContent->Length());
				ptfCaret.x = szfTextPoint.width;
			}
			else if(ucTextAlignment & TXA_RIGHT) ptfCaret.x = (float)lWidth - szfTextPoint.width;
			else ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f;

			if(LOWORD(lParam) > (short)ptfCaret.x){
				do{ GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint);}
				while(szfTextPoint.width + ptfCaret.x < (float)LOWORD(lParam) && ulCharacterPos < vasContent->Length());
			 
				if(vasZeichenMaske->Length()){
					if(ZeichenMaske_FestRechts()) GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
				}
				ptfCaret.x += szfTextPoint.width;
			} 
		}
		if(ptfCaret.x == (float)lWidth) ptfCaret.x -= (float)ucCaretStrength;
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
	VMBLOCK vbCharacter_Maske; ULONG ulZeichen; BYTE ucFesteZeichen = 1;

	for(ulZeichen = 1; ulZeichen <= ulCharacterPos + ucFesteZeichen; ulZeichen++){
		vasZeichenMaske->SubString(vbCharacter_Maske, ulZeichen, ulZeichen);
		if(*(PBYTE)vbCharacter_Maske == 0x27){ VMFrei(vbCharacter_Maske);
			if(ulZeichen == ulCharacterPos + ucFesteZeichen){
				ucFesteZeichen = (BYTE)vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
				vasZeichenMaske->SubString(vbCharacter_Maske, ++ucFesteZeichen, ucFesteZeichen);
				if(!ZeichenMaske_Einfugen_Prufen(wParam, vbCharacter_Maske)) return false;
				ulCharacterPos += ucFesteZeichen - ulZeichen - 2;
				return true;
			}
			ucFesteZeichen += 2;
			ulZeichen = vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
		}
		else VMFrei(vbCharacter_Maske);
	}
	vasZeichenMaske->SubString(vbCharacter_Maske, --ulZeichen, ulZeichen);

	return ZeichenMaske_Einfugen_Prufen(wParam, vbCharacter_Maske);
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COEditLine::ZeichenMaske_Einfugen_Prufen(WPARAM wParam, VMBLOCK vbCharacter_Maske)
{
	switch(*(PBYTE)vbCharacter_Maske){
		case 0x41 : if(wParam >= 0x41 && wParam <= 0x5a){ VMFrei(vbCharacter_Maske); return true; }
								break;
		case 0x61 : if(wParam >= 0x61 && wParam <= 0x7a){ VMFrei(vbCharacter_Maske); return true; }
								break;
		case 0x42 : if(wParam >= 0x20 && wParam <= 0x2f || wParam >= 0x3a && wParam <= 0x7d){	VMFrei(vbCharacter_Maske); return true; }
								break;
		case 0x62 : if(wParam >= 0x20 && wParam <= 0x2f || wParam >= 0x3a && wParam <= 0x40 ||
										wParam >= 0x5b && wParam <= 0x60 || wParam >= 0x7b && wParam <= 0x7e){ VMFrei(vbCharacter_Maske); return true; }
								break;
		case 0x5a : VMFrei(vbCharacter_Maske); return true;
		case 0x39 : if(wParam >= 0x30 && wParam <= 0x39){ VMFrei(vbCharacter_Maske); return true; }
								break;
		case 0x38 : if(wParam >= 0x30 && wParam <= 0x39  || wParam >= 0x2b && wParam <= 0x2e){ VMFrei(vbCharacter_Maske); return true; }
								break;
		case 0x58	: if(wParam >= 0x30 && wParam <= 0x39 || wParam >= 0x41 && wParam <= 0x46 ||
								wParam >= 0x61 && wParam <= 0x66){ VMFrei(vbCharacter_Maske); return true; }
								break;
	}
	VMFrei(vbCharacter_Maske);
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COEditLine::ZeichenMaske_FestRechts(void)
{
	VMBLOCK vbCharacter_Maske; ULONG ulZeichen; BYTE ucFesteZeichen = 1;
	for(ulZeichen = 1; ulZeichen < ulCharacterPos + ucFesteZeichen; ulZeichen++){
		vasZeichenMaske->SubString(vbCharacter_Maske, ulZeichen, ulZeichen);
		if(*(PBYTE)vbCharacter_Maske == 0x27){
			VMFrei(vbCharacter_Maske);
			ucFesteZeichen += 2;
			ulZeichen = vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
		}
		else VMFrei(vbCharacter_Maske);
	}
	if(ulCharacterPos < ulZeichen - ucFesteZeichen){ ulCharacterPos = ulZeichen - ucFesteZeichen; return true; }
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COEditLine::ZeichenMaske_FestLinks(void)
{
	VMBLOCK vbCharacter_Maske; BYTE ucFesteZeichen = 1;

	for(ULONG ulZeichen = 1; ulZeichen <= ulCharacterPos + ucFesteZeichen; ulZeichen++){
		vasZeichenMaske->SubString(vbCharacter_Maske, ulZeichen, ulZeichen);
		if(*(PBYTE)vbCharacter_Maske == 0x27){ VMFrei(vbCharacter_Maske);
			ucFesteZeichen += 2;
			ULONG ulSprungZeichen = vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
			if(ulSprungZeichen == ulCharacterPos + ucFesteZeichen){ ulCharacterPos -= ulSprungZeichen - ulZeichen - 2; return true; }
			ulZeichen = ulSprungZeichen;
		}
		else VMFrei(vbCharacter_Maske);
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COEditLine::ZeichenMaske_Loschen(void)
{
	VMBLOCK vbCharacter_Maske; BYTE ucFesteZeichen = 1;

	for(ULONG ulZeichen = 1; ulZeichen <= ulCharacterPos + ucFesteZeichen; ulZeichen++){
		vasZeichenMaske->SubString(vbCharacter_Maske, ulZeichen, ulZeichen);
		if(*(PBYTE)vbCharacter_Maske == 0x27){ VMFrei(vbCharacter_Maske);
			ucFesteZeichen += 2;
			ULONG ulSprungZeichen = vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
			if(ulSprungZeichen == ulCharacterPos + ucFesteZeichen - 1){ ulCharacterPos -= --ulSprungZeichen - ulZeichen; return false; }
			ulZeichen = ulSprungZeichen;
		}
		else VMFrei(vbCharacter_Maske);
	}
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::Text(char* pcText)
{
	ThreadSafe_Begin();
	*vasContent = NULL; ulCharacterPos = 0;
	if(pcText){
		if(vasZeichenMaske->Length() && StrLength(pcText)){ VMBLOCK vbCharacter_Maske, vbCharacter_Text; BYTE ucFesteZeichen = 0, ucZeichen; COStringA asText = pcText;
			for(ULONG ulZeichen = 1; ulZeichen <= vasZeichenMaske->Length(); ulZeichen++){
				vasZeichenMaske->SubString(vbCharacter_Maske, ulZeichen, ulZeichen);
				if(asText.Length() >= ulZeichen - ucFesteZeichen){
					asText.SubString(vbCharacter_Text, ulZeichen - ucFesteZeichen, ulZeichen - ucFesteZeichen);
					switch(*(PBYTE)vbCharacter_Maske){
						case 0x41	: if(*(PBYTE)vbCharacter_Text >= 0x41 && *(PBYTE)vbCharacter_Text <= 0x5a) *vasContent += vbCharacter_Text;
												else if(ucZeichenVorgabe & 1 << 5) *vasContent += "_";
												else *vasContent += " ";
												break;
						case 0x61	: if(*(PBYTE)vbCharacter_Text >= 0x61 && *(PBYTE)vbCharacter_Text <= 0x7a) *vasContent += vbCharacter_Text;
												else if(ucZeichenVorgabe & 1 << 5) *vasContent += "_";
												else *vasContent += " ";
												break;
						case 0x42	: if(*(PBYTE)vbCharacter_Text >= 0x20 && *(PBYTE)vbCharacter_Text <= 0x2f || *(PBYTE)vbCharacter_Text >= 0x3a && *(PBYTE)vbCharacter_Text <= 0x7d)
												*vasContent += vbCharacter_Text;
												else if(ucZeichenVorgabe & 1 << 5) *vasContent += "_";
												else *vasContent += " ";
												break;
						case 0x62	: if(*(PBYTE)vbCharacter_Text >= 0x20 && *(PBYTE)vbCharacter_Text <= 0x2f || *(PBYTE)vbCharacter_Text >= 0x3a && *(PBYTE)vbCharacter_Text <= 0x40 ||
														*(PBYTE)vbCharacter_Text >= 0x5b && *(PBYTE)vbCharacter_Text <= 0x60 || *(PBYTE)vbCharacter_Text >= 0x7b && *(PBYTE)vbCharacter_Text <= 0x7e)
												*vasContent += vbCharacter_Text;
												else if(ucZeichenVorgabe & 1 << 5) *vasContent += "_";
												else *vasContent += " ";
												break;
						case 0x5a	: *vasContent += vbCharacter_Text;
												break;
						case 0x39	: if(*(PBYTE)vbCharacter_Text >= 0x30 && *(PBYTE)vbCharacter_Text <= 0x39) *vasContent += vbCharacter_Text;
												else if(ucZeichenVorgabe & 1 << 5) *vasContent += "_";
												else *vasContent += " ";
												break;
						case 0x38	: if(*(PBYTE)vbCharacter_Text >= 0x30 && *(PBYTE)vbCharacter_Text <= 0x39 || *(PBYTE)vbCharacter_Text >= 0x2b && *(PBYTE)vbCharacter_Text <= 0x2e) *vasContent += vbCharacter_Text;
												else if(ucZeichenVorgabe & 1 << 5) *vasContent += "_";
												else *vasContent += " ";
												break;
						case 0x27	: ucZeichen = (BYTE)(vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length()) - ulZeichen++);
												vasZeichenMaske->SubString(vbCharacter_Maske, ulZeichen, ulZeichen + ucZeichen - 2);
												*vasContent += vbCharacter_Maske;
												ucFesteZeichen += 2;
												ulZeichen += ucZeichen - 1;
												break;
						case 0x58	: if(*(PBYTE)vbCharacter_Text >= 0x30 && *(PBYTE)vbCharacter_Text <= 0x39 || *(PBYTE)vbCharacter_Text >= 0x41 && *(PBYTE)vbCharacter_Text <= 0x46 ||
														*(PBYTE)vbCharacter_Text >= 0x61 && *(PBYTE)vbCharacter_Text <= 0x66)	*vasContent += vbCharacter_Text;
												else if(ucZeichenVorgabe & 1 << 5) *vasContent += "_";
												else *vasContent += " ";
												break;
					}
					VMFrei(vbCharacter_Text);
				}
				else if(ucZeichenVorgabe & 1 << 5) *vasContent += "_";
				else *vasContent += " ";
				VMFrei(vbCharacter_Maske); 
			}
		}		
		else *vasContent = pcText;
	}
	//UpdateFenster(nullptr, true, false);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::DeSelect(void)
{
	cSelect = 0;
	rclDirty.left = FloatToLong(rcfSelect.left); rclDirty.right = FloatToLong(rcfSelect.right);
	if(rclDirty.left < 0) rclDirty.left = 0;
	if(rclDirty.right > lWidth) rclDirty.right = lWidth;
	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	SetEvent(heCaret);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::Select_Loschen(void)
{
	D2D_SIZE_F szfTextPoint;

	DeSelect();
	GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
	if(ulSelectPos <= ulCharacterPos){ vasContent->Delete(ulSelectPos, ulCharacterPos - ulSelectPos); ulCharacterPos = ulSelectPos; }
	else vasContent->Delete(ulCharacterPos, ulSelectPos - ulCharacterPos);

	if(ucTextAlignment & TXA_LEFT){
		szfTextPoint.width - fTextPos < (float)lWidth ? rclDirty.right = FloatToLong(szfTextPoint.width - fTextPos) : rclDirty.right = lWidth;
		rclDirty.left = FloatToLong(rcfSelect.left);
		ptfCaret.x = rcfSelect.left;
		OnRender(true);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	else if(ucTextAlignment & TXA_RIGHT){
		if(!fTextPos) rclDirty.left = lWidth - FloatToLong(szfTextPoint.width);
		else{	rclDirty.left = 0; fTextPos -= rcfSelect.right - rcfSelect.left - (float)ucCaretStrength; }
		rclDirty.right = FloatToLong(rcfSelect.right);
		ptfCaret.x = rcfSelect.right;
		OnRender(true);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		if(fTextPos < 0.0f) fTextPos = 0.0f;
	}
	else{ // TXA_MITTE
		if(szfTextPoint.width < (float)lWidth){
			rclDirty.left = (lWidth - FloatToLong(szfTextPoint.width)) / 2;
			rclDirty.right = lWidth - rclDirty.left;
			GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
			ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f;
			GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
			ptfCaret.x += szfTextPoint.width;
		}
		else{	
			rclDirty.left = 0; rclDirty.right = lWidth;
			fTextPos -= (rcfSelect.right - rcfSelect.left) / 2.0f;
			if(fTextPos < 0.0f){ fTextPos = 0.0f;
				GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
				ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f;
				GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
				ptfCaret.x += szfTextPoint.width;
			}
			else{
				GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
				ptfCaret.x = szfTextPoint.width - fTextPos;
			}
		}
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

	rclDirty.left = FloatToLong(ptfCaret.x); rclDirty.right = FloatToLong(ptfCaret.x) + ucCaretStrength;
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

	rclDirty.left = FloatToLong(ptfCaret.x); rclDirty.right = FloatToLong(ptfCaret.x) + ucCaretStrength;
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
	else if(*vasZeichenMaske != pcZeichenmaske){ VMBLOCK vbCharacter; ULONG ulZeichenLange;
		*vasZeichenMaske = pcZeichenmaske; *vasContent = NULL;

		for(ULONG ulZeichen = 1; ulZeichen <= vasZeichenMaske->Length(); ulZeichen++){
			vasZeichenMaske->SubString(vbCharacter, ulZeichen, ulZeichen);
			if(*(PBYTE)vbCharacter == 0x27){ VMFrei(vbCharacter); ulZeichen++;
				ulZeichenLange = vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
				vasZeichenMaske->SubString(vbCharacter, ulZeichen, ulZeichenLange - 1);
				*vasContent += vbCharacter;
				ulZeichen += ulZeichenLange - ulZeichen;
			}
			else if(ucZeichenVorgabe & 1 << 5) *vasContent += "_";
			else *vasContent += " ";
			VMFrei(vbCharacter);
		}
		ulZeichen_max = vasContent->Length();
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
		D2D_SIZE_F szfTextPoint;
		ThreadSafe_Begin();
		cSelect = 1; ulSelectPos = 0; ulCharacterPos = vasContent->Length();

		GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
		ResetEvent(heCaret);

		if(ucTextAlignment & TXA_LEFT){ rcfSelect.right = szfTextPoint.width; rcfSelect.left = 0.0f; ptfCaret.x = szfTextPoint.width; }
		else if(ucTextAlignment & TXA_RIGHT){ rcfSelect.right = (float)lWidth; ptfCaret.x = (float)lWidth;
			(szfTextPoint.width > (float)lWidth ? rcfSelect.left = 0.0f : rcfSelect.left = (float)lWidth - szfTextPoint.width);
		}
		else{
			if(szfTextPoint.width > (float)lWidth){ rcfSelect.left = 0.0f; rcfSelect.right = (float)lWidth; }
			else{ rcfSelect.left = ((float)lWidth - szfTextPoint.width) / 2.0f; rcfSelect.right = rcfSelect.left + szfTextPoint.width; }
		}

		rclDirty.left = FloatToLong(rcfSelect.left); rclDirty.right = FloatToLong(rcfSelect.right);
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
	rclDirty.left = FloatToLong(ptfCaret.x);	rclDirty.right = rclDirty.left + ucCaretStrength;
	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//---------------------------------------------------------------------------------------------------------------------------------------
inline long __vectorcall RePag::DirectX::COEditLine::FloatToLong(_In_ float fNumber)
{
	long lZahl = (long)fNumber;
	fNumber -= (float)lZahl;
	if(fNumber >= 0.5f) lZahl++;
	return lZahl;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::GetTextPoint(_In_ char* pcText, _In_ unsigned long ulTextLength, _Out_ D2D_SIZE_F& szfTextPoint)
{
	IDWriteTextLayout* ifTextLayout; DWRITE_TEXT_METRICS stTextMetrics;
	size_t szBytes_Text; WCHAR wcInhalt[255]; mbstowcs_s(&szBytes_Text, wcInhalt, pcText, ulTextLength);
	pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wcInhalt, (UINT)szBytes_Text, ifText, fTextLine_maxwidth, (float)lHeight, &ifTextLayout);
	ifTextLayout->GetMetrics(&stTextMetrics);
	SafeRelease(&ifTextLayout);

	szfTextPoint.width = stTextMetrics.width;
	szfTextPoint.height = stTextMetrics.height;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COEditLine::CaretStrength(BYTE ucCaretStrengthA)
{
	ThreadSafe_Begin();
	ucCaretStrength = ucCaretStrengthA;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------