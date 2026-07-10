/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OPasswordD2.cpp
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
#include "OPasswordD2.h"

#define _Password ((RePag::DirectX::COPassword*)pvParam)
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COPassword* __vectorcall RePag::DirectX::COPasswordV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources)
{
	COPassword* vPassword = (COPassword*)VMBlock(VMDialog(), sizeof(COPassword));
	vPassword->COPasswordV(VMDialog(), pcWindowName, uiIDElement, pstDeviceResources);
	return vPassword;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COPassword* __vectorcall RePag::DirectX::COPasswordV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																		 _In_ STDeviceResources* pstDeviceResources)
{
	COPassword* vPassword = (COPassword*)VMBlock(vmMemory, sizeof(COPassword));
	vPassword->COPasswordV(vmMemory, pcWindowName, uiIDElement, pstDeviceResources);
	return vPassword;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_Password(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	COPassword* pPassword;
	switch(uiMessage){
		case WM_CREATE			: ((COPassword*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
													((COPassword*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
													return NULL;
		case WM_SIZE				: pPassword = (COPassword*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pPassword) pPassword->WM_Size_Element(hWnd, lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_SETFOCUS		: ((COPassword*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_SetFocus();
													return NULL;
		case WM_KILLFOCUS		: ((COPassword*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KillFocus();
													return NULL;
		case WM_CHAR				:	((COPassword*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Char(wParam);
													return NULL;
		case WM_CONTEXTMENU	: ((COPassword*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_ContexMenu(lParam);
													return NULL;
		case WM_LBUTTONDOWN	: ((COPassword*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDown(lParam);
													return NULL;
		case WM_NCDESTROY		: pPassword = (COPassword*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pPassword->htEffect_Timer) DeleteTimerQueueTimer(TimerQueue(), pPassword->htEffect_Timer, INVALID_HANDLE_VALUE);
													VMFreiV(pPassword);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void CALLBACK RePag::DirectX::Timer_Caret_Password(_In_ void* pvParam, _In_ bool bTimerOrWaitFired)
{
	WaitForSingleObject(_Password->heCaret, INFINITE);
	static bool bCaret = false;
	bCaret ? bCaret = false : bCaret = true;

	_Password->ThreadSafe_Begin();
	_Password->rclDirty.left = _Password->FloatToLong(_Password->ptfCaret.x);
	_Password->rclDirty.right = _Password->rclDirty.left + _Password->ucCaretStrength;

	_Password->OnRender(bCaret);
	_Password->ifDXGISwapChain4->Present1(0, NULL, &_Password->dxgiPresent);
	_Password->ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COPassword::COPasswordV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																													_In_ STDeviceResources* pstDeviceResources)
{
	COEditLineV(vmMemory, pcRePag_Password, pcWindowName, uiIDElementA, pstDeviceResources);

	vasPassword = COStringAV(vmMemory);

	ucTextAlignment = TXA_CENTERVERTICAL | TXA_CENTERHORIZONTAL;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COPassword::COFreiV(void)
{
	VMFreiV(vasPassword);
	return ((COEditLine*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COPassword::OnRender(_In_ bool bCaret)
{
	IDWriteTextLayout* ifTextLayout; float fTextWidth; size_t szBytes_Text; WCHAR wcInhalt[255]; D2D1::Matrix3x2F tfPrevTransform;

	WaitForSingleObjectEx(heRender, INFINITE, false);
	ifTextColor->SetColor(crfText);
	D2D1_RECT_F rcfText = D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);
	if(mbstowcs_s(&szBytes_Text, wcInhalt, 255, vasPassword->c_Str(), vasPassword->Length())) goto Error;
	if(pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wcInhalt, (UINT32)szBytes_Text, ifText, (float)lWidth, (float)lHeight, &ifTextLayout)) goto Error;
	TextAlignment(ifTextLayout, fTextWidth, rcfText);
	SafeRelease(&ifTextLayout);

	if(ucTextAlignment & TXA_RIGHT && !fTextPos && rcfText.left >= 0){}
	else if(ucTextAlignment & TXA_CENTERHORIZONTAL && rcfText.left >= 0){}
	else rcfText.left = 0;
	rcfText.right = fTextPos + (float)lWidth;

	ifD2D1Context6->BeginDraw();
	ifD2D1Context6->Clear(crfBackground);

	ifD2D1Context6->GetTransform(&tfPrevTransform);
	ifD2D1Context6->SetTransform(D2D1::Matrix3x2F::Translation(-fTextPos, 0.0f));
	ifD2D1Context6->DrawText(wcInhalt, (UINT32)szBytes_Text, ifText, rcfText, ifTextColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);

	if(!cSelect){
		if(bCaret){
			D2D1_POINT_2F ptfTop, ptfBottom;
			ptfTop.x = ptfCaret.x + fTextPos;
			ptfTop.y = ptfCaret.y + szfCharacter.height;
			ptfBottom.x = ptfCaret.x + fTextPos;
			ptfBottom.y = ptfCaret.y;
			ifD2D1Context6->DrawLine(ptfTop, ptfBottom, ifCaretColor, (float)ucCaretStrength, nullptr);
		}
	}
	else{
		VMBLOCK vbCharacter = nullptr; ULONG ulZeichen;
		if(cSelect > 0)	ulZeichen = vasContent->SubString(vbCharacter, ulSelectPos + 1, ulCharacterPos);
		else ulZeichen = vasContent->SubString(vbCharacter, ulCharacterPos + 1, ulSelectPos);

		D2D1_RECT_F rcfSelect_1 = D2D1::RectF(rcfSelect.left + fTextPos, rcfSelect.top, rcfSelect.right + fTextPos, rcfSelect.bottom);

		ifD2D1Context6->FillRectangle(&rcfSelect_1, ifSelectBackColor);

		ifTextColor->SetColor(crfSelectText);
		mbstowcs_s(&szBytes_Text, wcInhalt, 255, vbCharacter, ulZeichen); VMFrei(vbCharacter);
		ifD2D1Context6->DrawText(wcInhalt, (UINT32)szBytes_Text, ifText, rcfSelect_1, ifTextColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);
	}

	ifD2D1Context6->SetTransform(tfPrevTransform);
	ifD2D1Context6->EndDraw();

Error:
	SetEvent(heRender);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COPassword::WM_SetFocus(void)
{
	ThreadSafe_Begin();
	IDWriteTextLayout* ifTextLayout; size_t szBytes_Text; WCHAR wcInhalt[255]; D2D1_POINT_2F ptfText = {0}; float fTextWidth;

	mbstowcs_s(&szBytes_Text, wcInhalt, 255, vasPassword->c_Str(), vasPassword->Length());
	pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wcInhalt, (UINT32)szBytes_Text, ifText, (float)lWidth, (float)lHeight, &ifTextLayout);

	TextAlignment(ifTextLayout, fTextWidth, ptfText);
	SafeRelease(&ifTextLayout);

	rcfSelect.top = ptfCaret.y = ptfText.y;
	rclDirty.top = FloatToLong(ptfText.y);
	rcfSelect.bottom = (float)lHeight - ptfCaret.y;
	rclDirty.bottom = FloatToLong(rcfSelect.bottom);
	ptfCaret.x = ptfText.x + fTextWidth;

	EnableMenuItem(hMenu, IDM_COPY, MF_BYCOMMAND | MF_GRAYED);
	EnableMenuItem(hMenu, IDM_CUT, MF_BYCOMMAND | MF_GRAYED);
	EnableMenuItem(hMenu, IDM_PASTE, MF_BYCOMMAND | MF_GRAYED);

	ulCharacterPos = vasPassword->Length();

	if(!cSelect){
		if(!htCaret) CreateTimerQueueTimer(&htCaret, TimerQueue(), (WAITORTIMERCALLBACK)Timer_Caret_Password, this, 0, 500, 0);
	}
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COPassword::WM_Char(_In_ WPARAM wParam)
{
	D2D_SIZE_F szfTextPoint;
	switch(wParam){
		case VK_BACK		: ThreadSafe_Begin();
											if(ulCharacterPos){
												vasContent->Delete(--ulCharacterPos, 1); vasPassword->Delete(ulCharacterPos, 1);
												rclDirty.right = FloatToLong(ptfCaret.x) + ucCaretStrength;
												if(ucTextAlignment & TXA_LEFT){
													GetTextPoint(vasPassword->c_Str(), ulCharacterPos, szfTextPoint);
													rclDirty.left = FloatToLong(szfTextPoint.width);
												}
												else if(ucTextAlignment & TXA_CENTERHORIZONTAL){
													GetTextPoint(vasPassword->c_Str(), vasPassword->Length(), szfTextPoint);
													ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f;
													GetTextPoint(vasPassword->c_Str(), ulCharacterPos, szfTextPoint);
													rclDirty.left += FloatToLong(szfTextPoint.width);
												}
												rclDirty.left = FloatToLong(ptfCaret.x);
												OnRender(true);
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											}
											ThreadSafe_End();
											break;
		case VK_RETURN	: ThreadSafe_Begin();
											if(pfnWM_Char_Return) pfnWM_Char_Return(this);
											ThreadSafe_End();
											break;
		case VK_ESCAPE	: ThreadSafe_Begin();
											if(pfnWM_Char_Escape) pfnWM_Char_Escape(this);
											ThreadSafe_End();
											break;
		default					: ThreadSafe_Begin();
											if(CharacterCheck(wParam)){
												if(ulCharacterPos < ulCharacter_max){
													if(ulCharacterPos == vasContent->Length()){ *vasContent += (char*)&wParam; *vasPassword += "*"; }
													else{ vasContent->Insert((char*)&wParam, ulCharacterPos); vasPassword->Insert("*", ulCharacterPos); }
													ulCharacterPos++;
													if(ucTextAlignment & TXA_LEFT){
														GetTextPoint(vasPassword->c_Str(), ulCharacterPos, szfTextPoint);
														rclDirty.left = FloatToLong(ptfCaret.x) - ucCaretStrength;
														if(rclDirty.left < 0) rclDirty.left = 0;
														ptfCaret.x = szfTextPoint.width;
													}
													else if(ucTextAlignment & TXA_CENTERHORIZONTAL){
														GetTextPoint(vasPassword->c_Str(), vasPassword->Length(), szfTextPoint);
														ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f;
														rclDirty.left = FloatToLong(ptfCaret.x);
														GetTextPoint(vasPassword->c_Str(), ulCharacterPos, szfTextPoint);
														ptfCaret.x += szfTextPoint.width;
													}
                          rclDirty.right = FloatToLong(ptfCaret.x) + ucCaretStrength;
													OnRender(true);
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
											}
											ThreadSafe_End();
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COPassword::WM_LButtonDown(_In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	if(hWndElement != GetFocus()) SetFocus(hWndElement);

	D2D_SIZE_F szfTextPoint;
	ulCharacterPos = 0;
	if(vasPassword->Length()){
		if(fTextPos || ucTextAlignment & TXA_LEFT){
			do{ GetTextPoint(vasPassword->c_Str(), ++ulCharacterPos, szfTextPoint); }
			while(szfTextPoint.width - fTextPos < LOWORD(lParam) && ulCharacterPos < vasPassword->Length());
			ptfCaret.x = szfTextPoint.width - fTextPos;
		}
		else if(ucTextAlignment & TXA_CENTERHORIZONTAL){
			GetTextPoint(vasPassword->c_Str(), vasPassword->Length(), szfTextPoint);
			ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f;

			if(LOWORD(lParam) > ptfCaret.x){
				do{ GetTextPoint(vasPassword->c_Str(), ++ulCharacterPos, szfTextPoint); }
				while(szfTextPoint.width + ptfCaret.x < LOWORD(lParam) && ulCharacterPos < vasPassword->Length());
				ptfCaret.x += szfTextPoint.width;
			}
		}
	}
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COPassword::Text(_In_z_ const char* pcText)
{
	ThreadSafe_Begin();
	*vasContent = pcText; *vasPassword = NULL;
	for(BYTE ucCharacter = 0; ucCharacter < vasContent->Length(); ucCharacter++) *vasPassword += "*";
  rclDirty.left = 0; rclDirty.right = lWidth;	
	OnRender(true);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
