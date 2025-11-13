/******************************************************************************
MIT License

Copyright(c) 2025 René Pagel

Filename: OTextBoxD2.cpp
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
#include "OTextBoxD2.h"

#define _Line ((COStringA*)vliText->Element(pvIterator))
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COTextBox* __vectorcall RePag::DirectX::COTextBoxV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																														 _In_ STDeviceResources* pstDeviceResources)
{
	COTextBox* vTextBox = (COTextBox*)VMBlock(VMDialog(), sizeof(COTextBox));
	vTextBox->COTextBoxV(VMDialog(), pcWindowName, uiIDElement, pstDeviceResources);
	return vTextBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COTextBox* __vectorcall RePag::DirectX::COTextBoxV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																														 _In_ STDeviceResources* pstDeviceResources)
{
	COTextBox* vTextBox = (COTextBox*)VMBlock(vmMemory, sizeof(COTextBox));
	vTextBox->COTextBoxV(vmMemory, pcWindowName, uiIDElement, pstDeviceResources);
	return vTextBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_TextBox(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	COTextBox* pTextBox;
	switch(uiMessage){
		case WM_CREATE			: ((COTextBox*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
													((COTextBox*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
													return NULL;
		case WM_SIZE				:	pTextBox = (COTextBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pTextBox) pTextBox->WM_Size(lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_VSCROLL			: ((COTextBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_VScroll(wParam);
													return NULL;
		case WM_HSCROLL			: ((COTextBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_HScroll(wParam);
													return NULL;
		case WM_LBUTTONDOWN : ((COTextBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDown();
													return NULL;
		case WM_LBUTTONUP		: ((COTextBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonUp(wParam, lParam);
													return NULL;
		case WM_MOUSEWHEEL	: ((COTextBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseWheel(wParam, lParam);
													return NULL;
		case WM_NCDESTROY		: pTextBox = (COTextBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pTextBox->htEffect_Timer) DeleteTimerQueueTimer(TimerQueue(), pTextBox->htEffect_Timer, INVALID_HANDLE_VALUE);
													VMFreiV(pTextBox);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::COTextBoxV(_In_ VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName, 
																												_In_ unsigned int uiIDElementA,	_In_ STDeviceResources* pstDeviceResourcesA)
{
	// Note: three numbers uiIDElement !!!
	COEditLineV(vmMemory, pcClassName, pcWindowName, uiIDElementA, pstDeviceResourcesA);

	char pcWindowNameScrollbar[256];
	MemCopy(pcWindowNameScrollbar, "sbVertical_", 11);
	MemCopy(&pcWindowNameScrollbar[13], pcWindowName, StrLength(pcWindowName));
	pcWindowNameScrollbar[13 + StrLength(pcWindowName)] = 0;
	// Note: three numbers uiIDElement !!!
	sbVertical = COScrollBarV(vmMemory, pcWindowNameScrollbar, uiIDElementA + 1, pstDeviceResourcesA, false);

	MemCopy(pcWindowNameScrollbar, "sbHorizontal_", 13);
	MemCopy(&pcWindowNameScrollbar[11], pcWindowName, StrLength(pcWindowName));
	pcWindowNameScrollbar[11 + StrLength(pcWindowName)] = 0;
	// Note: three numbers uiIDElement !!!
	sbHorizontal = COScrollBarV(vmMemory, pcWindowNameScrollbar, uiIDElementA + 2, pstDeviceResourcesA, true);

	vliText = COListV(vmMemory, false);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::COTextBoxV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																												_In_ STDeviceResources* pstDeviceResourcesA)
{
	COTextBoxV(vmMemory, pcRePag_TextBox, pcWindowName, uiIDElementA, pstDeviceResourcesA);
}
//---------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COTextBox::COFreiV(void)
{
	VMFrei(vmMemory, sbVertical); VMFrei(vmMemory, sbHorizontal);
	vliText->DeleteList(true); VMFreiV(vliText);
	return ((COEditLine*)this)->COFreiV();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::OnRender(void)
{
	/*
	ThreadSicher_Anfang();

	PAINTSTRUCT stPaint;
	BeginPaint(hWndElement, &stPaint);
	WM_Paint_Hintergrund(stPaint);

	if(vliText->Number()){
		SCROLLINFO siZeile, siSpalte; siZeile.cbSize = siSpalte.cbSize = sizeof(SCROLLINFO); siZeile.fMask = siSpalte.fMask = SIF_ALL;
		GetScrollInfo(hWndElement, SB_HORZ, &siSpalte);
		GetScrollInfo(hWndElement, SB_VERT, &siZeile);

		long lZeile = 0;
		RECT rcZeichnen; rcZeichnen.top = 0; rcZeichnen.left = stPaint.rcPaint.left; rcZeichnen.right = stPaint.rcPaint.right;
		void* pvIterator = vliText->IteratorToBegin(); void* pvIterator_1 = nullptr;
		while(pvIterator && lZeile++ < (long)siZeile.nPos) vliText->NextElement(pvIterator);
		while(pvIterator && rcZeichnen.top < stPaint.rcPaint.top){ rcZeichnen.top += lZeichen_hohe; vliText->NextElement(pvIterator, pvIterator_1); }
		if(!pvIterator) pvIterator = pvIterator_1;
		if(rcZeichnen.top > stPaint.rcPaint.top){ pvIterator = pvIterator_1; rcZeichnen.top -= lZeichen_hohe; }
		rcZeichnen.bottom = rcZeichnen.top;

		SelectObject(stPaint.hdc, hFont);
		SetBkMode(stPaint.hdc, TRANSPARENT); SetTextColor(stPaint.hdc, RGB(stSchriftfarbe.ucRot, stSchriftfarbe.ucGrun, stSchriftfarbe.ucBlau));
		if(!cSelect){
			do{
				rcZeichnen.bottom += lZeichen_hohe;
				ExtTextOut(stPaint.hdc, siSpalte.nPos * lZeichen_mittel * -1, rcZeichnen.top, ETO_CLIPPED, &rcZeichnen, _Zeile->c_Str(), _Zeile->Length(), nullptr);

				vliText->NextElement(pvIterator);
				rcZeichnen.top += lZeichen_hohe;
			}
			while(pvIterator && rcZeichnen.top < stPaint.rcPaint.bottom);
		}
		else{
			do{
				rcZeichnen.bottom += lZeichen_hohe;
				if(rcSelect.top <= rcZeichnen.top && rcSelect.bottom >= rcZeichnen.bottom){
					SetBkMode(stPaint.hdc, OPAQUE);
					SetBkColor(stPaint.hdc, RGB(stSelectHintergrundfarbe.ucRot, stSelectHintergrundfarbe.ucGrun, stSelectHintergrundfarbe.ucBlau));
					SetTextColor(stPaint.hdc, RGB(stSelectSchriftfarbe.ucRot, stSelectSchriftfarbe.ucGrun, stSelectSchriftfarbe.ucBlau));

					if(rcSelect.left <= stPaint.rcPaint.left && rcSelect.right >= stPaint.rcPaint.right){
						ExtTextOut(stPaint.hdc, siSpalte.nPos * lZeichen_mittel * -1, rcZeichnen.top, ETO_CLIPPED, &rcZeichnen, _Zeile->c_Str(), _Zeile->Length(), nullptr);
						SetBkMode(stPaint.hdc, TRANSPARENT); SetTextColor(stPaint.hdc, RGB(stSchriftfarbe.ucRot, stSchriftfarbe.ucGrun, stSchriftfarbe.ucBlau));
					}
					else{
						if(cSelect == -1 || cSelect == 1){
							rcZeichnen.left = rcSelect.left; rcZeichnen.right = rcSelect.right;
						}
						else if(cSelect == -2 || cSelect == 2){
							if(rcSelect.top == rcZeichnen.top) rcZeichnen.left = rcSelect.left;
							else if(rcSelect.bottom == rcZeichnen.bottom) rcZeichnen.right = rcSelect.right;
						}
						ExtTextOut(stPaint.hdc, siSpalte.nPos * lZeichen_mittel * -1, rcZeichnen.top, ETO_CLIPPED, &rcZeichnen, _Zeile->c_Str(), _Zeile->Length(), nullptr);
						SetBkMode(stPaint.hdc, TRANSPARENT); SetTextColor(stPaint.hdc, RGB(stSchriftfarbe.ucRot, stSchriftfarbe.ucGrun, stSchriftfarbe.ucBlau));

						if(stPaint.rcPaint.left < rcSelect.left && rcSelect.top == rcZeichnen.top){
							rcZeichnen.right = rcSelect.left; rcZeichnen.left = stPaint.rcPaint.left;
							ExtTextOut(stPaint.hdc, siSpalte.nPos * lZeichen_mittel * -1, rcZeichnen.top, ETO_CLIPPED, &rcZeichnen, _Zeile->c_Str(), _Zeile->Length(), nullptr);
							rcZeichnen.left = 0; rcZeichnen.right = stPaint.rcPaint.right;
						}
						if(stPaint.rcPaint.right > rcSelect.right && rcSelect.bottom == rcZeichnen.bottom){
							rcZeichnen.right = stPaint.rcPaint.right; rcZeichnen.left = rcSelect.right;
							ExtTextOut(stPaint.hdc, siSpalte.nPos * lZeichen_mittel * -1, rcZeichnen.top, ETO_CLIPPED, &rcZeichnen, _Zeile->c_Str(), _Zeile->Length(), nullptr);
							rcZeichnen.left = stPaint.rcPaint.left;
						}
					}
				}
				else ExtTextOut(stPaint.hdc, siSpalte.nPos * lZeichen_mittel * -1, rcZeichnen.top, ETO_CLIPPED, &rcZeichnen, _Zeile->c_Str(), _Zeile->Length(), nullptr);

				vliText->NextElement(pvIterator);
				rcZeichnen.top += lZeichen_hohe;
			}
			while(pvIterator && rcZeichnen.top < stPaint.rcPaint.bottom);
		}
	}

	EndPaint(hWndElement, &stPaint);

	ThreadSicher_Ende();
	*/

	WaitForSingleObjectEx(heRender, INFINITE, false);
	ifTextColor->SetColor(crfText);

	ifD2D1Context6->BeginDraw();
	ifD2D1Context6->Clear(crfBackground);

	void* pvIterator = vliText->IteratorToBegin();
	if(pvIterator){
		float fLine = 0, fBottom; D2D1_RECT_F rcfText; size_t szBytes_Text; WCHAR wcInhalt[256];
		STScrollInfo siLine; siLine.ucMask = SBI_POS | SBI_PAGE | SBI_CHARACTER_HEIGHT;
		STScrollInfo siCharacter; siCharacter.ucMask = SBI_POS | SBI_MAX;
		sbVertical->GetScrollInfo(siLine); sbHorizontal->GetScrollInfo(siCharacter);
		while(pvIterator && fLine < siLine.fPos){	vliText->NextElement(pvIterator); fLine += siLine.szfCharacter.height; }
		fBottom = siLine.fPage;
		rcfText.left = siCharacter.fPos * -1.0f;
		rcfText.top = 0; rcfText.bottom = 0;  rcfText.right = siCharacter.fMax;

		if(!cSelect){
			do{
				rcfText.bottom += siLine.szfCharacter.height;
				mbstowcs_s(&szBytes_Text, wcInhalt, _Line->c_Str(), _Line->Length());
				ifD2D1Context6->DrawText(wcInhalt, (UINT)szBytes_Text, ifText, rcfText, ifTextColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);
				vliText->NextElement(pvIterator);
				rcfText.top += siLine.szfCharacter.height;
			}
			while(pvIterator && rcfText.top < fBottom);
		}
		else{


		}
	}

	ifD2D1Context6->EndDraw();
	SetEvent(heRender);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::WM_Create(void)
{
	CharacterMetric();
	ifD2D1Context6->CreateSolidColorBrush(crfText, &ifTextColor);
	ifD2D1Context6->CreateSolidColorBrush(crfSelectBack, &ifSelectBackColor);
	ifD2D1Context6->CreateSolidColorBrush(crfCaret, &ifCaretColor);

	sbVertical->CreateWindowGraphic(hWndElement, lHeight - 20, 20, lWidth - 20, 0);
	sbHorizontal->CreateWindowGraphic(hWndElement, 20, lWidth - 20, 0, lHeight - 20);

	STScrollInfo siScrollInfo;
	siScrollInfo.ucMask = SBI_ALL;
	siScrollInfo.fMax = siScrollInfo.fPos = 0;
	siScrollInfo.fPage = (float)lHeight - 20;
	siScrollInfo.szfCharacter = szfCharacter;
	sbVertical->SetVisible(false);
	sbVertical->SetScrollInfo(siScrollInfo);

	siScrollInfo.fPage = (float)lWidth - 20;
	sbHorizontal->SetVisible(false);
	sbHorizontal->SetScrollInfo(siScrollInfo);

	if(vasContent->Length()) CreateText();

	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::WM_Size(_In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	WM_Size_Element(hWndElement, lParam);

	STScrollInfo siScrollInfo; siScrollInfo.ucMask = SBI_PAGE;
	siScrollInfo.fPage = (float)lHeight;
	sbVertical->SetScrollInfo(siScrollInfo);
	siScrollInfo.fPage = (float)lWidth;
	sbHorizontal->SetScrollInfo(siScrollInfo);

	ChangeSizeVisibleScrollBars();

	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::WM_VScroll(_In_ WPARAM wParam)
{	
	/*
	SCROLLINFO stScrollVert; stScrollVert.cbSize = sizeof(SCROLLINFO); stScrollVert.fMask = SIF_ALL;
	ThreadSafe_Begin();
	GetScrollBar(hWndElement, SB_VERT, &stScrollVert);
	int iPos_alt = stScrollVert.nPos;

	switch(LOWORD(wParam)){
		case SB_LINEUP: if(stScrollVert.nPos) stScrollVert.nPos--;
			break;
		case SB_LINEDOWN: if(stScrollVert.nPos + (int)stScrollVert.nPage <= stScrollVert.nMax) stScrollVert.nPos++;
			break;
		case SB_PAGEUP: if(int(stScrollVert.nPos - stScrollVert.nPage) >= 0) stScrollVert.nPos -= stScrollVert.nPage;
									else stScrollVert.nPos = 0;
			break;
		case SB_PAGEDOWN: if(stScrollVert.nPos + stScrollVert.nPage < int(stScrollVert.nMax - stScrollVert.nPage)) stScrollVert.nPos += stScrollVert.nPage;
										else stScrollVert.nPos = stScrollVert.nMax - stScrollVert.nPage + 1;
			break;
		case SB_THUMBTRACK: stScrollVert.nPos = stScrollVert.nTrackPos; break;
		default: break;
	}

	if(stScrollVert.nPos + 1 == iPos_alt || stScrollVert.nPos - 1 == iPos_alt){
		RECT rcZeichnen;
		SetScrollBar(hWndElement, SB_VERT, &stScrollVert, true);

		if(lEdge_right < lWidestLine)  rcZeichnen.right = lEdge_right;
		else rcZeichnen.right = lWidestLine;

		rcZeichnen.left = 0;
		rcZeichnen.bottom = lEdge_bottom / lSign_height * lSign_height;
		if(stScrollVert.nPos > iPos_alt){
			rcZeichnen.top = lSign_height;
			ScrollWindow(hWndElement, 0, lSign_height * -1, &rcZeichnen, nullptr);

			rcZeichnen.top = rcZeichnen.bottom - lSign_height;
			rcZeichnen.bottom = lEdge_bottom;
			ptCaret.y -= lSign_height;
			if(cSelect){ rcSelect.top -= lSign_height; rcSelect.bottom -= lSign_height; }
		}
		else{
			rcZeichnen.top = 0;
			ScrollWindow(hWndElement, 0, lSign_height, &rcZeichnen, nullptr);

			rcZeichnen.bottom = rcZeichnen.top + lSign_height;
			ptCaret.y += lSign_height;
			if(cSelect){ rcSelect.top += lSign_height; rcSelect.bottom += lSign_height; }

			if(stScrollVert.nPos && lEdge_bottom >= (long)stScrollVert.nPage * lSign_height){
				stScrollVert.nPage++; SetScrollBar(hWndElement, SB_VERT, &stScrollVert, true);
			}
		}
		UpdateFenster(&rcZeichnen, true, false);
		if(GetFocus() == hWndElement) SetCaretPos(ptCaret.x, ptCaret.y);
	}
	else if(stScrollVert.nPos != iPos_alt){
		RECT rcZeichnen;
		SetScrollBar(hWndElement, SB_VERT, &stScrollVert, true);

		rcZeichnen.top = 0; rcZeichnen.left = 0; rcZeichnen.right = lEdge_right; rcZeichnen.bottom = lEdge_bottom;
		if(lEdge_bottom >= (long)stScrollVert.nPage * lSign_height) SetzScrollVert(stScrollVert);

		iPos_alt -= stScrollVert.nPos; iPos_alt *= lSign_height;
		ptCaret.y += iPos_alt;
		if(cSelect){ rcSelect.top += iPos_alt; rcSelect.bottom += iPos_alt; }

		UpdateFenster(&rcZeichnen, true, false);
		if(GetFocus() == hWndElement) SetCaretPos(ptCaret.x, ptCaret.y);
	}
	ThreadSafe_End();
	*/

	STScrollInfo siLine; siLine.ucMask = SBI_PAGE | SBI_CHARACTER_HEIGHT;
	STScrollInfo siCharacter; siCharacter.ucMask = SBI_PAGE;
	sbVertical->GetScrollInfo(siLine);
	sbHorizontal->GetScrollInfo(siCharacter);
	ThreadSafe_Begin();
	switch(LOWORD(wParam)){
		case SB_LINEUP		: rclScroll.left = 0; rclScroll.top = FloatToLong(siLine.szfCharacter.height, rclScroll.top);
												rclScroll.right = (long)siCharacter.fPage; rclScroll.bottom = (long)siLine.fPage;
												rclDirty.left = 0; rclDirty.top = 0;
												rclDirty.right = rclScroll.right; rclDirty.bottom = rclScroll.top;
												ptlScrollOffset.x = 0;
												ptlScrollOffset.y = rclScroll.top;

												dxgiPresent.pScrollRect = &rclScroll;
												dxgiPresent.pScrollOffset = &ptlScrollOffset;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												dxgiPresent.pScrollRect = nullptr;
												dxgiPresent.pScrollOffset = nullptr;
												break;
		case SB_LINEDOWN	: rclScroll.left = 0; rclScroll.top = 0; 
												rclScroll.right = (long)siCharacter.fPage; 
												rclScroll.bottom = FloatToLong(siLine.fPage - siLine.szfCharacter.height, rclScroll.bottom);
												rclDirty.left = 0; rclDirty.top = rclScroll.bottom;
												rclDirty.right = rclScroll.right;	rclDirty.bottom = (long)siLine.fPage;
												ptlScrollOffset.x = 0;
												ptlScrollOffset.y = (rclDirty.bottom - rclDirty.top) * -1;

												dxgiPresent.pScrollRect = &rclScroll;
												dxgiPresent.pScrollOffset = &ptlScrollOffset;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												dxgiPresent.pScrollRect = nullptr;
												dxgiPresent.pScrollOffset = nullptr;
												break;
		case SB_PAGEUP		: 
		case SB_PAGEDOWN	:	rclDirty.left = 0; rclDirty.top = 0;
												rclDirty.right = (long)siCharacter.fPage; rclDirty.bottom = (long)siLine.fPage;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												break;
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::WM_HScroll(_In_ WPARAM wParam)
{
	/*
	SCROLLINFO stScrollHorz; stScrollHorz.cbSize = sizeof(SCROLLINFO); stScrollHorz.fMask = SIF_ALL;
	ThreadSafe_Begin();
	GetScrollBar(hWndElement, SB_HORZ, &stScrollHorz);
	int iPos_alt = stScrollHorz.nPos;

	switch(LOWORD(wParam)){
		case SB_LINELEFT: if(stScrollHorz.nPos) stScrollHorz.nPos--;
			break;
		case SB_LINERIGHT: if(stScrollHorz.nMax >= (int)stScrollHorz.nPage + stScrollHorz.nPos) stScrollHorz.nPos++;
			break;
		case SB_PAGELEFT: stScrollHorz.nPos -= stScrollHorz.nPage;
			if(stScrollHorz.nPos < 0) stScrollHorz.nPos = 0;
			break;
		case SB_PAGERIGHT: if(stScrollHorz.nPos + stScrollHorz.nPage < int(stScrollHorz.nMax - stScrollHorz.nPage)) stScrollHorz.nPos += stScrollHorz.nPage;
										 else stScrollHorz.nPos = stScrollHorz.nMax - stScrollHorz.nPage + 1;
			break;
		case SB_THUMBTRACK: stScrollHorz.nPos = stScrollHorz.nTrackPos; break;
		default: break;
	}

	if(stScrollHorz.nPos + 1 == iPos_alt || stScrollHorz.nPos - 1 == iPos_alt){
		RECT rcZeichnen;
		SetScrollBar(hWndElement, SB_HORZ, &stScrollHorz, true);

		rcZeichnen.top = 0; rcZeichnen.bottom = lEdge_bottom;
		if(stScrollHorz.nPos > iPos_alt){
			rcZeichnen.left = lSign_middle;
			(lEdge_right < lWidestLine ? rcZeichnen.right = lEdge_right : rcZeichnen.right = lWidestLine);
			ScrollWindow(hWndElement, rcZeichnen.left * -1, 0, &rcZeichnen, nullptr);

			rcZeichnen.left = rcZeichnen.right - rcZeichnen.left; rcZeichnen.right = lEdge_right;
			ptCaret.x -= lSign_middle; lTextPos += lSign_middle;
			if(cSelect){ rcSelect.left -= lSign_middle; rcSelect.right -= lSign_middle; }
		}
		else{
			rcZeichnen.left = 0;
			(lEdge_right < lWidestLine ? rcZeichnen.right = lEdge_right - lSign_middle : rcZeichnen.right = lWidestLine);
			ScrollWindow(hWndElement, lSign_middle, 0, &rcZeichnen, nullptr);

			rcZeichnen.left = 0; rcZeichnen.right = lSign_middle;
			ptCaret.x += lSign_middle; lTextPos -= lSign_middle;
			if(cSelect){ rcSelect.left += lSign_middle; rcSelect.right += lSign_middle; }

			if(stScrollHorz.nPos && lEdge_right >= (long)stScrollHorz.nPage * lSign_middle){
				stScrollHorz.nPage++; SetScrollBar(hWndElement, SB_HORZ, &stScrollHorz, true);
			}
		}
		UpdateFenster(&rcZeichnen, true, false);
		if(GetFocus() == hWndElement) SetCaretPos(ptCaret.x, ptCaret.y);
	}
	else if(stScrollHorz.nPos != iPos_alt){
		RECT rcZeichnen;
		SetScrollBar(hWndElement, SB_HORZ, &stScrollHorz, true);

		rcZeichnen.top = 0; rcZeichnen.left = 0; rcZeichnen.right = lEdge_right; rcZeichnen.bottom = lEdge_bottom;
		if(lEdge_right >= (long)stScrollHorz.nPage * lSign_middle) SetzScrollHorz(stScrollHorz);

		iPos_alt -= stScrollHorz.nPos; iPos_alt *= lSign_middle;
		ptCaret.x += iPos_alt;
		lTextPos -= iPos_alt;
		if(cSelect){ rcSelect.left += iPos_alt; rcSelect.right += iPos_alt; }

		UpdateFenster(&rcZeichnen, true, false);
		if(GetFocus() == hWndElement) SetCaretPos(ptCaret.x, ptCaret.y);
	}
	ThreadSafe_End();
	*/

	STScrollInfo siLine; siLine.ucMask = SBI_PAGE;
	STScrollInfo siCharacter; siCharacter.ucMask = SBI_PAGE | SBI_CHARACTER_WIDTH;
	sbVertical->GetScrollInfo(siLine);
	sbHorizontal->GetScrollInfo(siCharacter);
	ThreadSafe_Begin();
	switch(LOWORD(wParam)){
		case SB_LINELEFT	: rclScroll.top = 0; rclScroll.left = FloatToLong(siCharacter.szfCharacter.width, rclScroll.left);
												rclScroll.right = (long)siCharacter.fPage; rclScroll.bottom = (long)siLine.fPage;
												rclDirty.left = 0; rclDirty.top = 0;
												rclDirty.right = rclScroll.left; rclDirty.bottom = rclScroll.bottom;
												ptlScrollOffset.y = 0;
												ptlScrollOffset.x = rclScroll.left;

												dxgiPresent.pScrollRect = &rclScroll;
												dxgiPresent.pScrollOffset = &ptlScrollOffset;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												dxgiPresent.pScrollRect = nullptr;
												dxgiPresent.pScrollOffset = nullptr;
												break;
		case SB_LINERIGHT	:	rclScroll.left = 0; rclScroll.top = 0;
												rclScroll.right = FloatToLong(siCharacter.fPage - siCharacter.szfCharacter.width, rclScroll.right);
												rclScroll.bottom = (long)siLine.fPage;
												rclDirty.left = rclScroll.right; rclDirty.top = 0;
												rclDirty.right = (long)siCharacter.fPage;	rclDirty.bottom = rclScroll.bottom;
												ptlScrollOffset.y = 0;
												ptlScrollOffset.x = (rclDirty.right - rclDirty.left) * - 1;

												dxgiPresent.pScrollRect = &rclScroll;
												dxgiPresent.pScrollOffset = &ptlScrollOffset;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												dxgiPresent.pScrollRect = nullptr;
												dxgiPresent.pScrollOffset = nullptr;
												break;
		case SB_PAGELEFT	:
		case SB_PAGERIGHT	:	rclDirty.left = 0; rclDirty.top = 0;
												rclDirty.right = (long)siCharacter.fPage; rclDirty.bottom = (long)siLine.fPage;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												break;
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::WM_LButtonDown(void)
{
	/*
	SetCapture(hWndElement);
	SetFocus(hWndElement);
	*/
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::WM_MouseWheel(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	/*
	ThreadSafe_Begin();
	RECT stClient; GetClientRect(hWndElement, &stClient);
	POINTS stPunkte = MAKEPOINTS(lParam);
	POINT stPunkt; stPunkt.x = stPunkte.x; stPunkt.y = stPunkte.y;
	ScreenToClient(hWndElement, &stPunkt);

	if(stPunkt.x > 0 && stPunkt.x < stClient.right && stPunkt.y > 0 && stPunkt.y < stClient.bottom){
		if(GET_WHEEL_DELTA_WPARAM(wParam) < 0) SendMessage(hWndElement, WM_VSCROLL, SB_LINEDOWN, NULL);
		else SendMessage(hWndElement, WM_VSCROLL, SB_LINEUP, NULL);
	}
	ThreadSafe_End();
	*/
}
//---------------------------------------------------------------------------------------------------------------------------------------
inline long& __vectorcall RePag::DirectX::COTextBox::FloatToLong(_In_ float fZahl, _Out_ long& lZahl)
{
	lZahl = (long)fZahl;
	fZahl -= (float)lZahl;
	if(fZahl >= 0.5f) lZahl++;
	return lZahl;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::GetTextPoint(_In_ char* pcText, _In_ unsigned long ulTextLength, _Out_ D2D_SIZE_F& szfTextPoint)
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
void __vectorcall RePag::DirectX::COTextBox::ChangeSizeVisibleScrollBars(void)
{
	STScrollInfo siLine; siLine.ucMask = SBI_MAX | SBI_PAGE; STScrollInfo siCharacter; siCharacter.ucMask = SBI_MAX | SBI_PAGE;	long lSize;
	sbVertical->GetScrollInfo(siLine);
	if(siLine.fMax > siLine.fPage){
		sbVertical->SetVisible(true);
		sbHorizontal->GetScrollInfo(siCharacter);
		if(siCharacter.fMax <= siCharacter.fPage){
			sbHorizontal->SetVisible(false);
			sbVertical->NewHeight(lHeight);
			siLine.fPage = (float)lHeight;
			sbVertical->SetScrollInfo(siLine);
		}
		else{
			sbHorizontal->SetVisible(true);
			siLine.fPage = (float)(lHeight - sbHorizontal->Height(lSize));
			sbVertical->SetScrollInfo(siLine);
			siCharacter.fPage = (float)(lWidth - sbVertical->Width(lSize));
			sbHorizontal->SetScrollInfo(siCharacter);
		}
	}
	else{
		sbVertical->SetVisible(false);
		sbHorizontal->GetScrollInfo(siCharacter);
		if(siCharacter.fMax <= siCharacter.fPage){
			sbHorizontal->SetVisible(false);
		}
		else{
			sbHorizontal->SetVisible(true);
			sbHorizontal->NewWidth(lWidth);
			siCharacter.fPage = (float)lWidth;
			sbHorizontal->SetScrollInfo(siCharacter);
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::CreateText(void)
{
	D2D_SIZE_F szfTextPoint; COStringA* vasLine; float fWidestLine = 0;
	cSelect = 0; 
	ULONG ulWidth = vasContent->Length(), ulSign = 0, ulSign_right = 1;
	do{
		ulSign_right++;
		if((*vasContent)[++ulSign] == 0x0A){
			vasLine = COStringAV(vmMemory);
			vasContent->SubString(vasLine, ulSign - ulSign_right + 2, ulSign);
			vliText->ToEnd(vasLine);
			ulSign_right = 0;
			GetTextPoint(vasLine->c_Str(), vasLine->Length(), szfTextPoint);
			if(fWidestLine < szfTextPoint.width) fWidestLine = szfTextPoint.width;
		}
	}
	while(ulSign < ulWidth);
	*vasContent = NULL;

	STScrollInfo siScrollInfo; siScrollInfo.ucMask = SBI_MAX;
	siScrollInfo.fMax = fWidestLine;
	sbHorizontal->SetScrollInfo(siScrollInfo);

	siScrollInfo.fMax = vliText->Number() * szfCharacter.height;
	sbVertical->SetScrollInfo(siScrollInfo);

	ChangeSizeVisibleScrollBars();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::Text(_In_ char* pcText)
{
	//ThreadSafe_Begin();
	//void* pvIterator = vliText->IteratorToBegin();
	//while(pvIterator){ VMFreiV((COStringA*)vliText->Element(pvIterator)); vliText->DeleteFirstElement(pvIterator, false); }

	//if(cSelect) DeSelect();

	//if(pcText && StrLength(pcText)){
	//	*vasContent = pcText;
	//	if(vasContent->Length() && (*vasContent)[vasContent->Length() - 1] != 0x0A) *vasContent += "\n";

	//	if(hWndElement){
	//		SIZE stZeichengrosse; COStringA* vasZeile;
	//		//HDC hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
	//		cSelect = 0; fWidestLine = 0;
	//		ULONG ulLange = vasContent->Length(), ulZeichen = 0, ulZeichen_Rechts = 1;
	//		do{
	//			ulZeichen_Rechts++;
	//			if((*vasContent)[++ulZeichen] == 0x0A){
	//				vasZeile = COStringAV(vmMemory);
	//				vasContent->SubString(vasZeile, ulZeichen - ulZeichen_Rechts + 2, ulZeichen);
	//				vliText->ToEnd(vasZeile);
	//				ulZeichen_Rechts = 0;
	//				//GetTextExtentPoint32(hdc, vasZeile->c_Str(), vasZeile->Length(), &stZeichengrosse);
	//				stZeichengrosse = {0};
	//				if(fWidestLine < stZeichengrosse.cx) fWidestLine = stZeichengrosse.cx;
	//			}
	//		}
	//		while(ulZeichen < ulLange);
	//		//ReleaseDC(hWndElement, hdc);
	//		*vasContent = NULL;

	//		STScrollInfo stScrollHorz, stScrollVert; //stScrollHorz.cbSize = stScrollVert.cbSize = sizeof(SCROLLINFO); stScrollHorz.fMask = stScrollVert.fMask = SIF_ALL;
	//		GetScrollBar(SB_HORZ, stScrollHorz);
	//		stScrollHorz.fPos = 0;
	//		//stScrollHorz.ulPage = lEdge_right / lSign_middle;
	//		//stScrollHorz.lMax = lWidestLine / lSign_middle;
	//		if(stScrollHorz.fPos + stScrollHorz.fPage >= stScrollHorz.fMax) stScrollHorz.fPage = stScrollHorz.fMax - stScrollHorz.fPos + 1;
	//		SetScrollBar(SB_HORZ, stScrollHorz);

	//		GetScrollBar(SB_VERT, stScrollVert);
	//		stScrollVert.fPos = 0;
	//		stScrollVert.fMax = vliText->Number();
	//		stScrollVert.fPage = lEdge_bottom / (ULONG)szfCharacter.height;
	//		if(stScrollVert.fPos + stScrollVert.fPage >= stScrollVert.fMax) stScrollVert.fPage = stScrollVert.fMax - stScrollVert.fPos + 1;
	//		else if(lEdge_bottom >= stScrollVert.fPage * (ULONG)szfCharacter.height) stScrollVert.fPage++;
	//		SetScrollBar(SB_VERT, stScrollVert);

	//	}
	//}
	//else if(fWidestLine){
	//	STScrollInfo siScrollInfo; //siScrollInfo.cbSize = sizeof(SCROLLINFO);
	//	fWidestLine = 0;
	//	SetScrollHorz(siScrollInfo);
	//	siScrollInfo.fPos = 0; siScrollInfo.fMax = 1;
	//	siScrollInfo.fPage = lEdge_bottom / (ULONG)szfCharacter.height;
	//	SetScrollBar(SB_VERT, siScrollInfo);
	//}
	////ptCaret.x = ptCaret.y = 0;
	////UpdateFenster(nullptr, true, false);
	//ThreadSafe_End();

	ThreadSafe_Begin();
	void* pvIterator = vliText->IteratorToBegin();
	while(pvIterator){ VMFreiV((COStringA*)vliText->Element(pvIterator)); vliText->DeleteFirstElement(pvIterator, false); }
	
	//if(cSelect) DeSelect();

	if(pcText && StrLength(pcText)){
		*vasContent = pcText;
		if(vasContent->Length() && (*vasContent)[vasContent->Length() - 1] != 0x0A) *vasContent += "\n";

		if(hWndElement){
		//	SIZE stZeichengrosse; COStringA* vasZeile;


		//	//HDC hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
		//	cSelect = 0; fWidestLine = 0;
		//	ULONG ulLange = vasContent->Length(), ulZeichen = 0, ulZeichen_Rechts = 1;
		//	do{
		//		ulZeichen_Rechts++;
		//		if((*vasContent)[++ulZeichen] == 0x0A){
		//			vasZeile = COStringAV(vmMemory);
		//			vasContent->SubString(vasZeile, ulZeichen - ulZeichen_Rechts + 2, ulZeichen);
		//			vliText->ToEnd(vasZeile);
		//			ulZeichen_Rechts = 0;
		//			//GetTextExtentPoint32(hdc, vasZeile->c_Str(), vasZeile->Length(), &stZeichengrosse);
		//			stZeichengrosse = {0};
		//			if(fWidestLine < stZeichengrosse.cx) fWidestLine = stZeichengrosse.cx;
		//		}
		//	}
		//	while(ulZeichen < ulLange);
		//	//ReleaseDC(hWndElement, hdc);
		//	*vasContent = NULL;

		//	STScrollInfo stScrollHorz, stScrollVert; //stScrollHorz.cbSize = stScrollVert.cbSize = sizeof(SCROLLINFO); stScrollHorz.fMask = stScrollVert.fMask = SIF_ALL;
		//	GetScrollBar(SB_HORZ, stScrollHorz);
		//	stScrollHorz.fPos = 0;
		//	//stScrollHorz.ulPage = lEdge_right / lSign_middle;
		//	//stScrollHorz.lMax = lWidestLine / lSign_middle;
		//	if(stScrollHorz.fPos + stScrollHorz.fPage >= stScrollHorz.fMax) stScrollHorz.fPage = stScrollHorz.fMax - stScrollHorz.fPos + 1;
		//	SetScrollBar(SB_HORZ, stScrollHorz);

		//	GetScrollBar(SB_VERT, stScrollVert);
		//	stScrollVert.fPos = 0;
		//	stScrollVert.fMax = vliText->Number();
		//	stScrollVert.fPage = lEdge_bottom / (ULONG)szfCharacter.height;
		//	if(stScrollVert.fPos + stScrollVert.fPage >= stScrollVert.fMax) stScrollVert.fPage = stScrollVert.fMax - stScrollVert.fPos + 1;
		//	else if(lEdge_bottom >= stScrollVert.fPage * (ULONG)szfCharacter.height) stScrollVert.fPage++;
		//	SetScrollBar(SB_VERT, stScrollVert);

		}
	}
	else{
		STScrollInfo siScrollInfo;
		siScrollInfo.ucMask = SBI_ALL;
		siScrollInfo.fMax = siScrollInfo.fPos = 0;
		siScrollInfo.fPage = (float)lHeight;
		siScrollInfo.szfCharacter = szfCharacter;
		sbVertical->SetScrollInfo(siScrollInfo);

		siScrollInfo.fPage = (float)lWidth;
		sbHorizontal->SetScrollInfo(siScrollInfo);
	}
	//ptCaret.x = ptCaret.y = 0;
	//UpdateFenster(nullptr, true, false);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::Text_NeueZeile(_In_ char* pcText)
{
	//if(!pcText) return;

	//if(!hWndElement){ *vasContent += pcText; *vasContent += "\n"; }
	//else{
	//	SIZE stZeichengrosse; //SCROLLINFO siScrollInfo; siScrollInfo.cbSize = sizeof(SCROLLINFO);
	//	ThreadSafe_Begin();
	//	COStringA* vasZeile = COStringAV(vmMemory, pcText); vliText->ToEnd(vasZeile);
	//	HDC hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
	//	GetTextExtentPoint32(hdc, vasZeile->c_Str(), vasZeile->Length(), &stZeichengrosse);
	//	if(lWidestLine < stZeichengrosse.cx){ lWidestLine = stZeichengrosse.cx; /*SetzScrollHorz(siScrollInfo);*/ }
	//	ReleaseDC(hWndElement, hdc);
	//	//SetzScrollVert(siScrollInfo);

	//	RECT rcZeichnen; rcZeichnen.left = 0; rcZeichnen.right = lEdge_right;
	//	//GetScrollBar(hWndElement, SB_VERT, &siScrollInfo);
	//	if(!siScrollInfo.nPos && siScrollInfo.nMax < (int)siScrollInfo.nPage){
	//		rcZeichnen.top = (siScrollInfo.nMax - 1) * lSign_height; rcZeichnen.bottom = rcZeichnen.top + lSign_height;
			//UpdateFenster(&rcZeichnen, true, false);
	//	}
	//	else{
	//		rcZeichnen.top = lEdge_bottom - lSign_height; rcZeichnen.bottom = lEdge_bottom;
	//		UpdateFenster(&rcZeichnen, true, false);
	//		while(siScrollInfo.nMax > siScrollInfo.nPos + (int)siScrollInfo.nPage){ SendMessage(hWndElement, WM_VSCROLL, SB_PAGEDOWN, NULL); GetScrollBar(hWndElement, SB_VERT, &siScrollInfo); }
	//	}
	//	SendMessage(hWndElement, WM_VSCROLL, SB_LINEDOWN, NULL);

	//	GetScrollBar(hWndElement, SB_HORZ, &siScrollInfo);
	//	while(siScrollInfo.nPos){ SendMessage(hWndElement, WM_HSCROLL, SB_PAGELEFT, NULL); GetScrollBar(hWndElement, SB_HORZ, &siScrollInfo); }
	//	ThreadSafe_End();
	//}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::GetScrollBar(_In_ BYTE ucBar, _In_ STScrollInfo& siScrollInfo)
{
	ucBar == SB_HORZ ? sbHorizontal->GetScrollInfo(siScrollInfo) : sbVertical->GetScrollInfo(siScrollInfo);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::DeSelect(void)
{
	/*
	if(cSelect == 2 || cSelect == -2){ rcSelect.left = 0; rcSelect.right = lEdge_right; }
	cSelect = 0; UpdateFenster(&rcSelect, true, false); ShowCaret(hWndElement);
	*/
}
//---------------------------------------------------------------------------------------------------------------------------------------
unsigned long __vectorcall RePag::DirectX::COTextBox::Zeilenanzahl(void)
{
	ThreadSafe_Begin();
	ULONG ulAnzahl = vliText->Number();
	ThreadSafe_End();
	return ulAnzahl;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::Scroll_Anfang(void)
{
	/*
	ThreadSafe_Begin();
	SCROLLINFO siScrollInfo; siScrollInfo.cbSize = sizeof(SCROLLINFO); siScrollInfo.fMask = SIF_POS;
	siScrollInfo.nPos = 0;
	SetScrollBar(hWndElement, SB_HORZ, &siScrollInfo, true);
	SetScrollBar(hWndElement, SB_VERT, &siScrollInfo, true);

	ptCaret.x = ptCaret.y = ulZeichenPos = lTextPos = 0;
	UpdateFenster(nullptr, true, false);
	SetCaretPos(ptCaret.x, ptCaret.y);
	ThreadSafe_End();
	*/
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::Scroll_Ende(void)
{
	/*
	ThreadSafe_Begin();
	SCROLLINFO siScrollInfo; siScrollInfo.cbSize = sizeof(SCROLLINFO); siScrollInfo.fMask = SIF_POS;
	siScrollInfo.nPos = 0;
	SetScrollBar(hWndElement, SB_HORZ, &siScrollInfo, true);

	siScrollInfo.fMask = SIF_POS | SIF_RANGE;
	GetScrollBar(hWndElement, SB_VERT, &siScrollInfo);
	siScrollInfo.nPos = siScrollInfo.nMax - lEdge_bottom / lSign_height;
	SetScrollBar(hWndElement, SB_VERT, &siScrollInfo, true);

	ptCaret.x = ptCaret.y = ulZeichenPos = lTextPos = 0;
	UpdateFenster(nullptr, true, false);
	SetCaretPos(ptCaret.x, ptCaret.y);
	ThreadSafe_End();
	*/
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::Scroll_Zeile(_In_ bool bAbwarts)
{
	if(bAbwarts) SendMessage(hWndElement, WM_VSCROLL, SB_LINEDOWN, NULL);
	else SendMessage(hWndElement, WM_VSCROLL, SB_LINEUP, NULL);
}
//---------------------------------------------------------------------------------------------------------------------------------------