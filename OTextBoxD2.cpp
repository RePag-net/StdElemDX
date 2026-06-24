/******************************************************************************
MIT License

Copyright(c) 2025 Ren� Pagel

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
		case WM_VSCROLL			:
		case WM_HSCROLL			: ((COTextBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_VHScroll(wParam);
													return NULL;
		case WM_KEYDOWN			: ((COTextBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KeyDown(wParam);
													return NULL;
		case WM_LBUTTONDOWN	: ((COTextBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDown();
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

	ucScrollBarSize = 20;
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
void __vectorcall RePag::DirectX::COTextBox::OnRender(_In_ bool bCaret, _In_ long lCaretLine, _In_ long lAnchorLine)
{
	WaitForSingleObjectEx(heRender, INFINITE, false);
	ifTextColor->SetColor(crfText);
	bool bTextClip = false, bTextTransform = false;
	D2D1::Matrix3x2F tfPrevTransform;

	ifD2D1Context6->BeginDraw();
	ifD2D1Context6->Clear(crfBackground);

	void* pvIterator = vliText->IteratorToBegin();
	if(pvIterator){
		float fLine = 0; size_t szBytes_Text; WCHAR wcInhalt[255];
		STScrollInfo siLine; siLine.ucMask = SBI_POS | SBI_PAGE | SBI_CHARACTER_HEIGHT;
		STScrollInfo siCharacter; siCharacter.ucMask = SBI_POS | SBI_PAGE | SBI_MAX | SBI_CHARACTER_WIDTH;
		sbVertical->GetScrollInfo(siLine); sbHorizontal->GetScrollInfo(siCharacter);

		while(pvIterator && fLine < siLine.fPos){	vliText->NextElement(pvIterator); fLine += siLine.szfCharacter.height; }
		D2D1_RECT_F rcfText = D2D1::RectF(0.0f, 0.0f, siCharacter.fMax, 0.0f);
		D2D1_RECT_F rcfViewport = D2D1::RectF(0.0f, 0.0f, siCharacter.fPage, siLine.fPage);
		ifD2D1Context6->PushAxisAlignedClip(&rcfViewport, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		bTextClip = true;
		ifD2D1Context6->GetTransform(&tfPrevTransform);
		ifD2D1Context6->SetTransform(D2D1::Matrix3x2F::Translation(-siCharacter.fPos, 0.0f));
		bTextTransform = true;

		do{
			rcfText.bottom += siLine.szfCharacter.height;
			if(mbstowcs_s(&szBytes_Text, wcInhalt, 255, _Line->c_Str(), _Line->Length())) goto Error;
			ifD2D1Context6->DrawText(wcInhalt, (UINT32)szBytes_Text, ifText, rcfText, ifTextColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);
			vliText->NextElement(pvIterator);
			rcfText.top += siLine.szfCharacter.height;
		}
		while(pvIterator && rcfText.top < siLine.fPage);

		if(!cSelect){
			if(bCaret){
				D2D1_POINT_2F ptfTop, ptfBottom;
				ptfTop.x = ptfCaret.x;
				ptfTop.y = ptfCaret.y + szfCharacter.height;
				ptfBottom.x = ptfCaret.x;
				ptfBottom.y = ptfCaret.y;
				ifD2D1Context6->DrawLine(ptfTop, ptfBottom, ifCaretColor, (float)ucCaretStrength, nullptr);
			}
		}
		else{
			long lStartLine = lAnchorLine, lEndLine = lCaretLine;
			ULONG ulStartPos = ulSelectPos, ulEndPos = ulCharacterPos;
			if(lStartLine > lEndLine){
				long lLineTemp = lStartLine; lStartLine = lEndLine; lEndLine = lLineTemp;
				ULONG ulPosTemp = ulStartPos; ulStartPos = ulEndPos; ulEndPos = ulPosTemp;
			}
			else if(lStartLine == lEndLine && ulStartPos > ulEndPos){
				ULONG ulPosTemp = ulStartPos; ulStartPos = ulEndPos; ulEndPos = ulPosTemp;
			}

			if(lStartLine < 0) lStartLine = 0;
			if(lEndLine >= (long)vliText->Number()) lEndLine = (long)vliText->Number() - 1;
			ifTextColor->SetColor(crfSelectText);

			for(long lSelectLine = lStartLine; lSelectLine <= lEndLine; lSelectLine++){
				COStringA* pSelectLine = (COStringA*)vliText->Element(lSelectLine);
				ULONG ulLineLength = pSelectLine->Length();
				ULONG ulLineStart = 0, ulLineEnd = ulLineLength;
				if(lStartLine == lEndLine){ ulLineStart = ulStartPos; ulLineEnd = ulEndPos; }
				else if(lSelectLine == lStartLine) ulLineStart = ulStartPos;
				else if(lSelectLine == lEndLine) ulLineEnd = ulEndPos;

				if(ulLineStart > ulLineLength) ulLineStart = ulLineLength;
				if(ulLineEnd > ulLineLength) ulLineEnd = ulLineLength;
				if(ulLineStart >= ulLineEnd) continue;

				float fTop = (float)lSelectLine * siLine.szfCharacter.height - siLine.fPos;
				if(fTop + siLine.szfCharacter.height <= 0.0f) continue;
				if(fTop >= siLine.fPage) break;

				D2D_SIZE_F szfLeft = {0}, szfRight = {0};
				if(ulLineStart) GetTextPoint(pSelectLine->c_Str(), ulLineStart, szfLeft);
				GetTextPoint(pSelectLine->c_Str(), ulLineEnd, szfRight);
				D2D1_RECT_F rcfSelectLine = D2D1::RectF(szfLeft.width, fTop, szfRight.width, fTop + siLine.szfCharacter.height);
				ifD2D1Context6->FillRectangle(&rcfSelectLine, ifSelectBackColor);

				VMBLOCK vbCharacter = nullptr;
				ULONG ulCharacters = pSelectLine->SubString(vbCharacter, ulLineStart + 1, ulLineEnd);
				if(ulCharacters && !mbstowcs_s(&szBytes_Text, wcInhalt, 255, vbCharacter, ulCharacters))
					ifD2D1Context6->DrawText(wcInhalt, (UINT32)szBytes_Text, ifText, rcfSelectLine, ifTextColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);
				if(vbCharacter) VMFrei(vbCharacter);
			}
		}
	}

Error:
	if(bTextTransform) ifD2D1Context6->SetTransform(tfPrevTransform);
	if(bTextClip) ifD2D1Context6->PopAxisAlignedClip();
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

	sbVertical->CreateWindowGraphic(hWndElement, lHeight - ucScrollBarSize, ucScrollBarSize, lWidth - ucScrollBarSize, 0);
	sbHorizontal->CreateWindowGraphic(hWndElement, ucScrollBarSize, lWidth - ucScrollBarSize, 0, lHeight - ucScrollBarSize);

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

	rcfSelect.left = rcfSelect.top = 	rcfSelect.bottom = 0.0f; rcfSelect.right = (float)lWidth;

	if(vasContent->Length()) CreateText();

	OnRender(false);
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
void __vectorcall RePag::DirectX::COTextBox::WM_VHScroll(_In_ WPARAM wParam)
{
	ThreadSafe_Begin();
	OnRender(false);
	ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::WM_KeyDown(_In_ WPARAM wParam)
{
	switch(wParam){
		case VK_HOME	: Scroll_Begin();
										break;
		case VK_END		: Scroll_End();
										break;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::WM_LButtonDown(void)
{
	SetFocus(hWndElement);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::WM_MouseWheel(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	POINTS ptPoints = MAKEPOINTS(lParam);
	POINT ptPoint; ptPoint.x = ptPoints.x; ptPoint.y = ptPoints.y;
	STScrollInfo siLine; siLine.ucMask = SBI_PAGE;

	ThreadSafe_Begin();
	ScreenToClient(hWndElement, &ptPoint);
	sbVertical->GetScrollInfo(siLine);
	if(ptPoint.y > 0 && ptPoint.y < FloatToLong(siLine.fPage))
		GET_WHEEL_DELTA_WPARAM(wParam) < 0 ? Scroll_Line(SB_LINEDOWN) : Scroll_Line(SB_LINEUP);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
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
	ThreadSafe_Begin();
	void* pvIterator = vliText->IteratorToBegin();
	while(pvIterator){ VMFreiV((COStringA*)vliText->Element(pvIterator)); vliText->DeleteFirstElement(pvIterator, false); }

	//if(cSelect) DeSelect();

	if(pcText && StrLength(pcText)){
		*vasContent = pcText;
		if(vasContent->Length() && (*vasContent)[vasContent->Length() - 1] != 0x0A) *vasContent += "\n";

		if(hWndElement) CreateText();
	}
	else{
		STScrollInfo siScrollInfo;
		siScrollInfo.ucMask = SBI_ALL;
		siScrollInfo.fMax = siScrollInfo.fPos = 0;
		siScrollInfo.fPage = (float)lHeight;
		siScrollInfo.szfCharacter = szfCharacter;
		sbVertical->SetVisible(false);
		sbVertical->SetScrollInfo(siScrollInfo);

		siScrollInfo.fPage = (float)lWidth;
		siScrollInfo.szfCharacter = szfCharacter;
		sbHorizontal->SetVisible(false);
		sbHorizontal->SetScrollInfo(siScrollInfo);
	}
	ulCharacterPos = 0;
	ptfCaret.x = ptfCaret.y;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::Text_NewLine(_In_ char* pcText, _In_ bool bDraw)
{
	if(!pcText) return;

	if(!hWndElement){ *vasContent += pcText; *vasContent += "\n"; }
	else{	D2D_SIZE_F szfTextPoint; COStringA* vasLine; float fWidestLine = 0;
		ThreadSafe_Begin();
		vasLine = COStringAV(vmMemory, pcText); vliText->ToEnd(vasLine);
		GetTextPoint(vasLine->c_Str(), vasLine->Length(), szfTextPoint);
		if(fWidestLine < szfTextPoint.width) fWidestLine = szfTextPoint.width;

		STScrollInfo siLine; siLine.ucMask = SBI_MAX | SBI_CHARACTER_HEIGHT;
		sbVertical->GetScrollInfo(siLine);
		siLine.fMax += siLine.szfCharacter.height;
		sbVertical->SetScrollInfo(siLine);

		STScrollInfo siCharacter; siCharacter.ucMask = SBI_MAX;
		sbHorizontal->GetScrollInfo(siCharacter);
		if(fWidestLine > siCharacter.fMax){	siCharacter.fMax = fWidestLine;	sbHorizontal->SetScrollInfo(siCharacter);	}
		ChangeSizeVisibleScrollBars();

		if(bDraw){
			siCharacter.ucMask = SBI_PAGE | SBI_POS; sbHorizontal->GetScrollInfo(siCharacter);
			siLine.ucMask = SBI_ALL; sbVertical->GetScrollInfo(siLine);
			if(siCharacter.fPos){
				if(siLine.fPos + siLine.fPage < siLine.fMax){
					siLine.ucMask = SBI_POS;
					siLine.fPos = siLine.fMax - siLine.fPage;
					sbVertical->SetScrollInfo(siLine);
				}
				siCharacter.fPos = 0;
				siCharacter.ucMask ^= SBI_PAGE;
				sbHorizontal->SetScrollInfo(siCharacter);
			}
			else if(siLine.fPos + siLine.fPage < siLine.fMax){
					siLine.ucMask = SBI_POS;
					siLine.fPos = siLine.fMax - siLine.fPage;
					sbVertical->SetScrollInfo(siLine);
			}
			OnRender(false);
			ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		}
		ThreadSafe_End();
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::GetScrollBar(_In_ BYTE ucBar, _Out_ STScrollInfo& siScrollInfo)
{
	ucBar == SB_HORZ ? sbHorizontal->GetScrollInfo(siScrollInfo) : sbVertical->GetScrollInfo(siScrollInfo);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::SetScrollBar(_In_ BYTE ucBar, _In_ STScrollInfo& siScrollInfo)
{
	ucBar == SB_HORZ ? sbHorizontal->SetScrollInfo(siScrollInfo) : sbVertical->SetScrollInfo(siScrollInfo);
}
//---------------------------------------------------------------------------------------------------------------------------------------
unsigned long __vectorcall RePag::DirectX::COTextBox::LineNumbers(void)
{
	ThreadSafe_Begin();
	ULONG ulNumber = vliText->Number();
	ThreadSafe_End();
	return ulNumber;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::Scroll_Begin(void)
{
	ThreadSafe_Begin();
	STScrollInfo siCharacter; siCharacter.ucMask = SBI_POS;
	STScrollInfo siLine; siLine.ucMask = SBI_POS;
	siCharacter.fPos = 0;	sbHorizontal->SetScrollInfo(siCharacter);
	siLine.fPos = 0; sbVertical->SetScrollInfo(siLine);
	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ulCharacterPos = 0;
	ptfCaret.x = ptfCaret.y = 0.0f;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::Scroll_End(void)
{
	ThreadSafe_Begin();
	STScrollInfo siCharacter; siCharacter.ucMask = SBI_POS;
	STScrollInfo siLine; siLine.ucMask = SBI_POS | SBI_MAX | SBI_PAGE;
	siCharacter.fPos = 0;	sbHorizontal->SetScrollInfo(siCharacter);
	sbVertical->GetScrollInfo(siLine);
	siLine.fPos = siLine.fMax - siLine.fPage; sbVertical->SetScrollInfo(siLine);
	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ulCharacterPos = 0;
	ptfCaret.x = 0.0f;
	ptfCaret.y = siLine.fPage;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::Scroll_Line(_In_ BYTE ucDown_UP)
{
	STScrollInfo siCharacter; siCharacter.ucMask = SBI_PAGE;
	STScrollInfo siLine; siLine.ucMask = SBI_POS | SBI_MAX | SBI_PAGE | SBI_CHARACTER_HEIGHT;
	ThreadSafe_Begin();
	sbVertical->GetScrollInfo(siLine);
	sbHorizontal->GetScrollInfo(siCharacter);
	if(ucDown_UP == SB_LINEDOWN){
		if(siLine.fPos + siLine.fPage < siLine.fMax){
			siLine.fPos += siLine.szfCharacter.height;
			if(cSelect){ rcfSelect.top -= szfCharacter.height; rcfSelect.bottom -= szfCharacter.height;	}
			sbVertical->SetScrollInfo(siLine);
			OnRender(false);
			ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		}
	}
	else if(ucDown_UP == SB_LINEUP){
		if(siLine.fPos){
			siLine.fPos -= siLine.szfCharacter.height;
			if(cSelect){ rcfSelect.top += szfCharacter.height; rcfSelect.bottom += szfCharacter.height; }
			sbVertical->SetScrollInfo(siLine);
			OnRender(false);
			ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		}
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::SetScrollBarSize(_In_ BYTE ucWidth_Height)
{
	STScrollInfo siCharacter; siCharacter.ucMask = SBI_PAGE;
	STScrollInfo siLine; siLine.ucMask = SBI_PAGE;

	ThreadSafe_Begin();
	ucScrollBarSize = ucWidth_Height;
	siLine.fPage = (float)(lHeight - ucScrollBarSize);
	sbVertical->SetScrollInfo(siLine);
	sbVertical->NewSize(lHeight - ucScrollBarSize, ucScrollBarSize, lWidth - ucScrollBarSize, 0);

	siCharacter.fPage = (float)(lWidth - ucScrollBarSize);
	sbHorizontal->SetScrollInfo(siCharacter);
	sbHorizontal->NewSize(ucScrollBarSize, lWidth - ucScrollBarSize, 0, lHeight - ucScrollBarSize);

	siCharacter.ucMask |= SBI_MAX;
	siLine.ucMask |= SBI_MAX;
	sbVertical->GetScrollInfo(siLine);
	if(siLine.fMax > siLine.fPage){
		sbVertical->SetVisible(true);
		sbHorizontal->GetScrollInfo(siCharacter);
		siCharacter.fMax <= siCharacter.fPage ?	sbHorizontal->SetVisible(false) : sbHorizontal->SetVisible(true);
	}
	else{
		sbVertical->SetVisible(false);
		sbHorizontal->GetScrollInfo(siCharacter);
		siCharacter.fMax <= siCharacter.fPage ?	sbHorizontal->SetVisible(false) : sbHorizontal->SetVisible(true);
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
BYTE __vectorcall RePag::DirectX::COTextBox::GetScrollBarSize(_In_ BYTE ucBar, _Out_ BYTE ucWidth_Height)
{
	ThreadSafe_Begin();
	if(ucBar == SB_HORZ){ IsWindowVisible(sbHorizontal->HWND_Element()) ?	ucWidth_Height = ucScrollBarSize : ucWidth_Height = 0; }
	else{	IsWindowVisible(sbVertical->HWND_Element()) ? ucWidth_Height = ucScrollBarSize : ucWidth_Height = 0; }
	ThreadSafe_End();
  return ucWidth_Height;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::DeSelect(void)
{
  cSelect = 0; SetEvent(heCaret); ulSelectPos = ulCharacterPos;

	rclDirty.top = FloatToLong(rcfSelect.top); rclDirty.bottom = FloatToLong(rcfSelect.bottom);
	rclDirty.left = FloatToLong(rcfSelect.left); rclDirty.right = FloatToLong(rcfSelect.right);
	OnRender(true);
	ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
}
//---------------------------------------------------------------------------------------------------------------------------------------
