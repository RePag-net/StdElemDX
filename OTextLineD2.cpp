/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OTextLineD2.cpp
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
#include "OTextLineD2.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COTextLine* __vectorcall RePag::DirectX::COTextLineV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																		 _In_ STDeviceResources* pstDeviceResourcesA)
{
	COTextLine* vTextZeile = (COTextLine*)VMBlock(VMDialog(), sizeof(COTextLine));
	vTextZeile->COTextLineV(VMDialog(), pcWindowName, uiIDElement, pstDeviceResourcesA);
	return vTextZeile;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COTextLine* __vectorcall RePag::DirectX::COTextLineV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																		 _In_ STDeviceResources* pstDeviceResourcesA)
{
	COTextLine* vTextZeile = (COTextLine*)VMBlock(vmMemory, sizeof(COTextLine));
	vTextZeile->COTextLineV(vmMemory, pcWindowName, uiIDElement, pstDeviceResourcesA);
	return vTextZeile;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_TextLine(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	COTextLine* pTextZeile;
	switch(uiMessage){
		case WM_CREATE			: ((COTextLine*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
													((COTextLine*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
													return NULL;
		case WM_SIZE				:	pTextZeile = (COTextLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pTextZeile) pTextZeile->WM_Size(lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_SETFOCUS		: ((COTextLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_SetFocus();
													return NULL;
		case WM_KEYDOWN			:	((COTextLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KeyDown(wParam, lParam);
													return NULL;
		case WM_COMMAND			: ((COTextLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Command(wParam);
													return NULL;
		case WM_LBUTTONDOWN	: ((COTextLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDown(wParam, lParam);
													return NULL;
		case WM_CONTEXTMENU: ((COTextLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_ContexMenu(lParam);
													return NULL;
		case WM_MOUSEMOVE		: ((COTextLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseMove(wParam, lParam);
													return NULL;
		case WM_NCDESTROY		:	pTextZeile = (COTextLine*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pTextZeile->htEffect_Timer) DeleteTimerQueueTimer(TimerQueue(), pTextZeile->htEffect_Timer, INVALID_HANDLE_VALUE);
													VMFreiV(pTextZeile);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::COTextLineV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName,
																											_In_ unsigned int uiIDElementA,	_In_ STDeviceResources* pstDeviceResourcesA)
{
	COSelectV(vmMemory, pcClassName, pcWindowName, uiIDElementA, pstDeviceResourcesA);

	ulSelectPos = 0;
	fTextPos = 0.0f;
	rcfSelect = D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::COTextLineV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																											_In_ STDeviceResources* pstDeviceResourcesA)
{
	COTextLineV(vmMemory, pcRePag_TextLine, pcWindowName, uiIDElementA, pstDeviceResourcesA);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COTextLine::COFreiV(void)
{
	SafeRelease(&ifTextColor); SafeRelease(&ifSelectBackColor);
	return ((COSelect*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::OnRender(_In_ bool bCaret)
{
	IDWriteTextLayout* ifTextLayout; float fTextWidth; size_t szBytes_Text; WCHAR wc255Content[255]; D2D1::Matrix3x2F tfPrevTransform;

	WaitForSingleObjectEx(heRender, INFINITE, false);
	ifTextColor->SetColor(crfText);
	D2D1_RECT_F rcfText = D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);
	if(mbstowcs_s(&szBytes_Text, wc255Content, 255, vasContent->c_Str(), vasContent->Length())) goto Error;
	if(pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wc255Content, (UINT32)szBytes_Text, ifText, (float)lWidth, (float)lHeight, &ifTextLayout)) goto Error;
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
	ifD2D1Context6->DrawText(wc255Content, (UINT32)szBytes_Text, ifText, rcfText, ifTextColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);

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
		mbstowcs_s(&szBytes_Text, wc255Content, 255, vbCharacter, ulZeichen); VMFrei(vbCharacter);
		ifD2D1Context6->DrawText(wc255Content, (UINT32)szBytes_Text, ifText, rcfSelect_1, ifTextColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);
	}

	ifD2D1Context6->SetTransform(tfPrevTransform);
	ifD2D1Context6->EndDraw();

Error:
	SetEvent(heRender);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::OnPaint(void)
{
	ThreadSafe_Begin();	
	OnRender(false);
	rclDirty.left = 0; rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::WM_Create(void)
{
	CharacterMetric();
	ifD2D1Context6->CreateSolidColorBrush(crfText, &ifTextColor);
	ifD2D1Context6->CreateSolidColorBrush(crfSelectBack, &ifSelectBackColor);

	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::WM_Size(_In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	if(lHeight != HIWORD(lParam) || lWidth != LOWORD(lParam)){
		lHeight = HIWORD(lParam); lWidth = LOWORD(lParam);
		CreateWindowSizeDependentResources();
		OnRender(false);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::WM_SetFocus(void)
{
	ThreadSafe_Begin();
	IDWriteTextLayout* ifTextLayout; size_t szBytes_Text; WCHAR wc255Content[255]; D2D1_POINT_2F ptfText = {0}; float fTextWidth;

	mbstowcs_s(&szBytes_Text, wc255Content, 255, vasContent->c_Str(), vasContent->Length());
	pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wc255Content, (UINT32)szBytes_Text, ifText, (float)lWidth, (float)lHeight, &ifTextLayout);

	TextAlignment(ifTextLayout, fTextWidth, ptfText);
	SafeRelease(&ifTextLayout);

	rcfSelect.top = ptfCaret.y = ptfText.y;
	rclDirty.top = FloatToLong(ptfText.y);
	rcfSelect.bottom = (float)lHeight - ptfCaret.y;
	rclDirty.bottom = FloatToLong(rcfSelect.bottom);
  rclDirty.right = FloatToLong(fTextWidth);

	ptfCaret.x = ptfText.x + fTextWidth;
	if(!cSelect) ulCharacterPos = vasContent->Length();
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::WM_KeyDown(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	D2D_SIZE_F szfTextPoint, szfTextPoint_1;

	switch(wParam){
		case VK_LEFT	: ThreadSafe_Begin();
										if(!ulCharacterPos){ ThreadSafe_End(); break; }
										if(!bDoNotCopy){
											if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
												if(!cSelect) rclDirty.right = FloatToLong(ptfCaret.x) + ucCaretStrength;
												GetTextPoint(vasContent->c_Str(), --ulCharacterPos, szfTextPoint);
												if(ucTextAlignment & TXA_LEFT) ptfCaret.x = szfTextPoint.width;
												else{
													GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint_1);
													if(ucTextAlignment & TXA_RIGHT)	ptfCaret.x = (float)lWidth - szfTextPoint_1.width + szfTextPoint.width;
													else ptfCaret.x = ((float)lWidth - szfTextPoint_1.width) / 2.0f + szfTextPoint.width;
												}
												SelectText_Left(false);
											}
											else if(cSelect) DeSelect();
											OnRender(false);
											ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
										}
										ThreadSafe_End();
										break;
		case VK_RIGHT	: ThreadSafe_Begin();
										if(ulCharacterPos >= vasContent->Length()){ ThreadSafe_End(); break; }
										if(!bDoNotCopy){
											if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
												if(!cSelect) rclDirty.left = FloatToLong(ptfCaret.x);
												GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint);
												if(ucTextAlignment & TXA_LEFT){
													rclDirty.left ? rclDirty.left -= ucCaretStrength : rclDirty.left = 0;
													ptfCaret.x = szfTextPoint.width;
												}
												else{
													GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint_1);
													if(ucTextAlignment & TXA_RIGHT) ptfCaret.x = (float)lWidth - szfTextPoint_1.width + szfTextPoint.width;
													else ptfCaret.x = ((float)lWidth - szfTextPoint_1.width) / 2 + szfTextPoint.width;
												}
												SelectText_Right(false);
											}
											else if(cSelect) DeSelect();
											OnRender(false);
											ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
										}
										ThreadSafe_End();
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::WM_Command(_In_ WPARAM wParam)
{
	VMBLOCK vbCharacter = nullptr; HGLOBAL hGlobal; char* pcAblage; ULONG ulZeichen;
	if(LOWORD(wParam) == IDM_COPY){
		ThreadSafe_Begin();
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
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::WM_ContexMenu(_In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	EnableMenuItem(hMenu, IDM_CUT, MF_GRAYED);
	EnableMenuItem(hMenu, IDM_PASTE, MF_GRAYED);

	POINT ptPosition;
	ptPosition.x = GET_X_LPARAM(lParam); ptPosition.y = GET_Y_LPARAM(lParam);
	if(ptPosition.x == USHRT_MAX && ptPosition.y == USHRT_MAX) ClientToScreen(GetParent(hWndElement), &Position(ptPosition));
	TrackPopupMenuEx(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, ptPosition.x, ptPosition.y, hWndElement, nullptr);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::WM_MouseMove(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	if(hWndElement == GetFocus() && wParam == MK_LBUTTON){
		ThreadSafe_Begin();
		int iTest = (int)GET_X_LPARAM(lParam);
		if((float)GET_X_LPARAM(lParam) < ptfCaret.x - szfCharacter.width)	SendMessage(hWndElement, WM_KEYDOWN, VK_LEFT, NULL);
		else if((float)GET_X_LPARAM(lParam) > ptfCaret.x + szfCharacter.width) SendMessage(hWndElement, WM_KEYDOWN, VK_RIGHT, NULL);
		ThreadSafe_End();
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::WM_LButtonDown(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	if(hWndElement != GetFocus()) SetFocus(hWndElement);
	if(cSelect) DeSelect();

	ulCharacterPos = 0;
	if(vasContent->Length()){
		D2D_SIZE_F szfTextPoint;
		if(fTextPos || ucTextAlignment & TXA_LEFT){
			do{ GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint); }
			while(szfTextPoint.width - fTextPos < (float)GET_X_LPARAM(lParam) && ulCharacterPos < vasContent->Length());
			if(ulCharacterPos == 1 && szfTextPoint.width > (float)GET_X_LPARAM(lParam)){ ulCharacterPos = 0; ptfCaret.x = 0.0f; }
			else ptfCaret.x = szfTextPoint.width - fTextPos;
		}
		else{
			GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
			if(szfTextPoint.width > (float)lWidth){
				do{ GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint); }
				while(szfTextPoint.width < (float)GET_X_LPARAM(lParam) && ulCharacterPos < vasContent->Length());
				ptfCaret.x = szfTextPoint.width;
			}
			else if(ucTextAlignment & TXA_RIGHT) ptfCaret.x = (float)lWidth - szfTextPoint.width;
			else ptfCaret.x = ((float)lWidth - szfTextPoint.width) / 2.0f;

			if(GET_X_LPARAM(lParam) > (short)ptfCaret.x){
				do{ GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint); }
				while(szfTextPoint.width + ptfCaret.x < (float)GET_X_LPARAM(lParam) && ulCharacterPos < vasContent->Length());
				ptfCaret.x += szfTextPoint.width;
			}
		}
	}
	if(ptfCaret.x == (float)lWidth) ptfCaret.x -= (float)ucCaretStrength;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::CharacterMetric(void)
{
	IDWriteTextLayout* ifTextLayout; DWRITE_TEXT_METRICS stTextMetrics;
	pstDeviceResources->ifdwriteFactory7->CreateTextLayout(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
																												 63, ifText, 5000.0f, (float)lHeight, &ifTextLayout);
	ifTextLayout->GetMetrics(&stTextMetrics);
	szfCharacter.height = stTextMetrics.height;
	szfCharacter.width = stTextMetrics.width / 62.0f;
	fTextLine_maxwidth = stTextMetrics.width / 62.0f * 255.0f;
	SafeRelease(&ifTextLayout);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::Text(_In_z_ char* pcText)
{
	ThreadSafe_Begin();
	*vasContent = pcText;
	if(hWndElement){
    rclDirty.left = rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
		OnRender(false);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::DirectX::COTextLine::Content(_Out_ COStringA* vasContentA)
{
	ThreadSafe_Begin();
	*vasContentA = *vasContent;
	ThreadSafe_End();
	return vasContentA;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::SelectText_Left(_In_ bool bScroll)
{
	D2D_SIZE_F szfTextPoint; VMBLOCK vbCharacter = nullptr; ULONG ulCharacter; float fTemp, fWidth = (float)lWidth;
	if(cSelect < 0){ 
		rcfSelect.left = ptfCaret.x; rclDirty.left = FloatToLong(ptfCaret.x);
		ulCharacter = vasContent->SubString(vbCharacter, ulCharacterPos + 1, ulSelectPos);
		GetTextPoint(vbCharacter, ulCharacter, szfTextPoint); VMFrei(vbCharacter);
		fTemp = rcfSelect.left + szfTextPoint.width;
		if(fTemp > rcfSelect.right) rcfSelect.right = fTemp;
		if(rcfSelect.right > fWidth){
      fTemp = rcfSelect.right - fWidth;
			rcfSelect.right = fWidth;
			rcfSelect.left = ptfCaret.x - fTemp; rclDirty.left = FloatToLong(ptfCaret.x - fTemp);
		}
		if(!bScroll) rclDirty.right = FloatToLong(rcfSelect.right);
	}
	else if(cSelect > 0){
		if(ulSelectPos == ulCharacterPos){ cSelect = 0; SetEvent(heCaret); rclDirty.left = FloatToLong(rcfSelect.left); }
		else{
			rcfSelect.right = ptfCaret.x;
			rclDirty.right += ucCaretStrength;
			if(rclDirty.right > lWidth) rclDirty.right = lWidth;
			rclDirty.left = FloatToLong(ptfCaret.x);
		}
	}
	else{
		cSelect = -1;
		ulSelectPos = ulCharacterPos + 1;
		rcfSelect.left = ptfCaret.x;
		rclDirty.left = FloatToLong(ptfCaret.x);
		if(bScroll){
			ulCharacter = vasContent->SubString(vbCharacter, ulCharacterPos + 1, ulSelectPos);
			GetTextPoint(vbCharacter, ulCharacter, szfTextPoint); VMFrei(vbCharacter);
			rcfSelect.right = szfTextPoint.width;
		}
		else rcfSelect.right = (float)rclDirty.right;
		ResetEvent(heCaret);
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::SelectText_Right(_In_ bool bScroll)
{
	D2D_SIZE_F szfTextPoint; VMBLOCK vbCharacter = nullptr; ULONG ulCharacter;
	if(cSelect > 0){
		rcfSelect.right = ptfCaret.x + (float)ucCaretStrength;
    if(rcfSelect.right > (float)lWidth) rcfSelect.right = (float)lWidth;
		rclDirty.right = FloatToLong(rcfSelect.right); 
	}
	else if(cSelect < 0){
		if(ulSelectPos > ulCharacterPos) rcfSelect.left = ptfCaret.x;
		else{	cSelect = 0; SetEvent(heCaret);	rclDirty.right = FloatToLong(rcfSelect.right); }
	}
	else{
		cSelect = 1;
		ulSelectPos = ulCharacterPos - 1;
		rcfSelect.right = ptfCaret.x;
		rclDirty.right = FloatToLong(rcfSelect.right);
		if(bScroll){
			ulCharacter = vasContent->SubString(vbCharacter, ulSelectPos + 1, ulCharacterPos);
			GetTextPoint(vbCharacter, ulCharacter, szfTextPoint); VMFrei(vbCharacter);
			rcfSelect.left = (float)lWidth - szfTextPoint.width;
		}
		else rcfSelect.left = (float)rclDirty.left;
		ResetEvent(heCaret);
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextLine::DeSelect(void)
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
void __vectorcall RePag::DirectX::COTextLine::DeleteCaretPos(void)
{
	rclDirty.left = FloatToLong(ptfCaret.x);	rclDirty.right = rclDirty.left + ucCaretStrength;
	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//---------------------------------------------------------------------------------------------------------------------------------------
