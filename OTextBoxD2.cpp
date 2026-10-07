/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

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
#define _CurrentLine ((COStringA*)pvLine)

constexpr CHAR LEFT = -1;
constexpr CHAR RIGHT = 1;
constexpr CHAR UP = -2;
constexpr	CHAR DOWN = 2;
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
		case WM_KEYDOWN			: ((COTextBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KeyDown(wParam, lParam);
													return NULL;
		case WM_MOUSEMOVE		: ((COTextBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseMove(wParam, lParam);
													return NULL;
		case WM_LBUTTONDOWN	: ((COTextBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDown(lParam);
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
	pvLine = nullptr;
	lLine = 0;

	ucScrollBarSize = 20;

	stSelect_top.lLine = 0; stSelect_top.fPosition = 0.0f;
	stSelect_bottom.lLine = 0; stSelect_bottom.fPosition = 0.0f;

	bDoNotCopy = false;
  CloseHandle(heCaret); heCaret = nullptr;
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
void __vectorcall RePag::DirectX::COTextBox::OnRender(_In_ bool bCaret)
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
		STScrollInfo siLine{}; siLine.ucMask = SBI_POS | SBI_PAGE | SBI_CHARACTER_HEIGHT;
		STScrollInfo siCharacter{}; siCharacter.ucMask = SBI_POS | SBI_PAGE | SBI_MAX | SBI_CHARACTER_WIDTH;
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
			long lSelectLine = 0; VMBLOCK vbCharacter; ULONG ulCharacters, ulCharacter_top = stSelect_top.ulCharacterPos; D2D_SIZE_F szfTextPoint; 
			pvIterator = vliText->IteratorToBegin(); D2D1_RECT_F rcfSelect = {0.0f, 0.0f, 0.0f, 0.0f};
			while(pvIterator && rcfSelect.top < siLine.fPos){	vliText->NextElement(pvIterator); rcfSelect.top += siLine.szfCharacter.height; lSelectLine++;	}
			if(lSelectLine > stSelect_bottom.lLine) goto Error;
			rcfSelect.top = 0;
			while(pvIterator && lSelectLine++ < stSelect_top.lLine){ vliText->NextElement(pvIterator); rcfSelect.top += siLine.szfCharacter.height; }
			if(stSelect_top.lLine >= --lSelectLine){ rcfSelect.left = stSelect_top.fPosition; lSelectLine = stSelect_top.lLine; }
			else{ rcfSelect.left = 0.0f; ulCharacter_top = 0; }
			ifTextColor->SetColor(crfSelectText);

			if(stSelect_top.lLine == stSelect_bottom.lLine){
				rcfSelect.right = stSelect_bottom.fPosition; rcfSelect.bottom = rcfSelect.top + siLine.szfCharacter.height;

				ifD2D1Context6->FillRectangle(&rcfSelect, ifSelectBackColor);
				ulCharacters = _Line->SubString(vbCharacter, ulCharacter_top + 1, stSelect_bottom.ulCharacterPos);
				if(ulCharacters && !mbstowcs_s(&szBytes_Text, wcInhalt, 255, vbCharacter, ulCharacters))
					ifD2D1Context6->DrawText(wcInhalt, (UINT32)szBytes_Text, ifText, rcfSelect, ifTextColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);
				VMFrei(vbCharacter);
			}
			else{
				if(stSelect_bottom.lLine == lSelectLine){ rcfSelect.bottom = siLine.szfCharacter.height; goto LastLine;	}

				ulCharacters = _Line->SubString(vbCharacter, ulCharacter_top + 1, _Line->Length());
				if(ulCharacters && !mbstowcs_s(&szBytes_Text, wcInhalt, 255, vbCharacter, ulCharacters)){
					GetTextPoint(vbCharacter, ulCharacters, szfTextPoint);
					stSelect_top.lLine == lSelectLine ?	rcfSelect.right = stSelect_top.fPosition + szfTextPoint.width
																						: rcfSelect.right = szfTextPoint.width;
					rcfSelect.bottom = rcfSelect.top + siLine.szfCharacter.height;
					ifD2D1Context6->FillRectangle(&rcfSelect, ifSelectBackColor);
					ifD2D1Context6->DrawText(wcInhalt, (UINT32)szBytes_Text, ifText, rcfSelect, ifTextColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);
				}
				VMFrei(vbCharacter);
				
				vliText->NextElement(pvIterator); lSelectLine++;
				rcfSelect.top += siLine.szfCharacter.height; rcfSelect.bottom = rcfSelect.top + siLine.szfCharacter.height;

				if(stSelect_bottom.lLine == lSelectLine) goto LastLine;
				else{
					do{
						ulCharacters = _Line->SubString(vbCharacter, 1, _Line->Length());
						if(ulCharacters && !mbstowcs_s(&szBytes_Text, wcInhalt, 255, vbCharacter, ulCharacters)){
							GetTextPoint(vbCharacter, ulCharacters, szfTextPoint);
							rcfSelect.left = 0.0f; rcfSelect.right = szfTextPoint.width;
							ifD2D1Context6->FillRectangle(&rcfSelect, ifSelectBackColor);
							ifD2D1Context6->DrawText(wcInhalt, (UINT32)szBytes_Text, ifText, rcfSelect, ifTextColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);
						}
						VMFrei(vbCharacter);

						vliText->NextElement(pvIterator); lSelectLine++;
						rcfSelect.top += siLine.szfCharacter.height; rcfSelect.bottom = rcfSelect.top + siLine.szfCharacter.height;
					}
					while(pvIterator && lSelectLine < stSelect_bottom.lLine);

LastLine:
					rcfSelect.left = 0.0f; rcfSelect.right = stSelect_bottom.fPosition;
					ulCharacters = _Line->SubString(vbCharacter, 1, stSelect_bottom.ulCharacterPos);
					if(ulCharacters && !mbstowcs_s(&szBytes_Text, wcInhalt, 255, vbCharacter, ulCharacters)){
						ifD2D1Context6->FillRectangle(&rcfSelect, ifSelectBackColor);
						ifD2D1Context6->DrawText(wcInhalt, (UINT32)szBytes_Text, ifText, rcfSelect, ifTextColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);
					}
					VMFrei(vbCharacter);
				}
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
void __vectorcall RePag::DirectX::COTextBox::OnPaint(void)
{
	ThreadSafe_Begin();
  rclDirty.left = 0; rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
	OnRender(false);
	ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
	ThreadSafe_End();
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

	STScrollInfo siScrollInfo{};
	siScrollInfo.ucMask = SBI_ALL;
	siScrollInfo.fMax = siScrollInfo.fPos = 0;
	siScrollInfo.fPage = (float)lHeight - ucScrollBarSize;
	siScrollInfo.szfCharacter = szfCharacter;
	sbVertical->SetVisible(false);
	sbVertical->SetScrollInfo(siScrollInfo);

	siScrollInfo.fPage = (float)lWidth - ucScrollBarSize;
	sbHorizontal->SetVisible(false);
	sbHorizontal->SetScrollInfo(siScrollInfo);

	if(vasContent->Length()) CreateText();

	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::WM_Size(_In_ LPARAM lParam)
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
void __vectorcall RePag::DirectX::COTextBox::WM_VHScroll(_In_ WPARAM wParam)
{
	ThreadSafe_Begin();
  rclDirty.left = 0; rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
	OnRender(false);
	ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::WM_KeyDown(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	D2D_SIZE_F szfTextPoint; D2D_POINT_2F ptfCaret_old; ULONG ulCharacterPos_old;
	STScrollInfo siCharacter{}; siCharacter.ucMask = SBI_POS | SBI_PAGE;
	STScrollInfo siLine{}; siLine.ucMask = SBI_POS | SBI_MAX | SBI_PAGE;
	switch(wParam){
		case VK_HOME	: ThreadSafe_Begin();
										sbHorizontal->GetScrollInfo(siCharacter);	sbVertical->GetScrollInfo(siLine);
										siCharacter.fPos = 0;	sbHorizontal->SetScrollInfo(siCharacter);
										siLine.fPos = 0; sbVertical->SetScrollInfo(siLine);
										rclDirty.left = rclDirty.top = 0; rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
										OnRender(false);
										ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
										ulCharacterPos = 0;
										ptfCaret.x = ptfCaret.y = 0.0f;
										ThreadSafe_End();
										break;
		case VK_END		:	ThreadSafe_Begin();
										sbHorizontal->GetScrollInfo(siCharacter);
										siCharacter.fPos = 0;	sbHorizontal->SetScrollInfo(siCharacter);
										sbVertical->GetScrollInfo(siLine);
										siLine.fPos = siLine.fMax - siLine.fPage; sbVertical->SetScrollInfo(siLine);
										rclDirty.left = rclDirty.top = 0; rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
										OnRender(false);
										ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
										ulCharacterPos = 0;
										ptfCaret.x = 0.0f; ptfCaret.y = siLine.fPage;
										ThreadSafe_End();
										break;
		case VK_LEFT	: ThreadSafe_Begin();
										if(!bDoNotCopy){
											if(!cSelect){
												GetTextPoint(_CurrentLine->c_Str(), --ulCharacterPos, szfTextPoint);
												ptfCaret_old = ptfCaret;
												ptfCaret.x = szfTextPoint.width;
											}
											if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam)	SelectText_Left(ptfCaret_old);
											else if(cSelect) DeSelect();
										}
										ThreadSafe_End();
										break;
    case VK_RIGHT	: ThreadSafe_Begin();
										if(!bDoNotCopy){
											if(!cSelect){
												GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint);
												ptfCaret_old = ptfCaret;
												ptfCaret.x = szfTextPoint.width;
											}
											if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam)	SelectText_Right(ptfCaret_old);
											else if(cSelect) DeSelect();
										}
										ThreadSafe_End();
										break;
    case VK_UP		: ThreadSafe_Begin();
										if(!bDoNotCopy){
											if(!cSelect){
												ptfCaret_old = ptfCaret; ulCharacterPos_old = ulCharacterPos;

												pvLine = vliText->Element(--lLine);
												ulCharacterPos = 0;
												if(ptfCaret.x > 0.0f && _CurrentLine->Length()){
													do{ GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint); }
													while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _CurrentLine->Length());
													ptfCaret.x = szfTextPoint.width;
												}
												else ptfCaret.x = 0.0f;
												ptfCaret.y -= szfCharacter.height;
											}
											if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam)	SelectText_Up(ptfCaret_old, ulCharacterPos_old);
											else if(cSelect) DeSelect();
										}
										ThreadSafe_End();
										break;
    case VK_DOWN	: ThreadSafe_Begin();
										if(!bDoNotCopy){
											if(!cSelect){
												ptfCaret_old = ptfCaret;
												ulCharacterPos_old = ulCharacterPos;

												pvLine = vliText->Element(++lLine);
												ulCharacterPos = 0;
												if(ptfCaret.x > 0.0f && _CurrentLine->Length()){
													do{ GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint); }
													while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _CurrentLine->Length());
													ptfCaret.x = szfTextPoint.width;
												}
												else ptfCaret.x = 0.0f;
												ptfCaret.y += szfCharacter.height;
											}
											if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam)	SelectText_Down(ptfCaret_old, ulCharacterPos_old);
											else if(cSelect) DeSelect();
										}
										ThreadSafe_End();
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::LButtonDown(_In_ LPARAM lParam)
{
	D2D_SIZE_F szfTextPoint; STScrollInfo siLine{}; siLine.ucMask = SBI_POS | SBI_MAX; GetScrollBar(SB_VERT, siLine);

	lLine = (long)(((float)GET_Y_LPARAM(lParam) + siLine.fPos) / szfCharacter.height);
	if(lLine < 0) lLine = 0;
	else if(lLine >= (long)vliText->Number()) lLine = (long)vliText->Number() - 1;

	ptfCaret.y = (float)lLine * szfCharacter.height - siLine.fPos;
	pvLine = vliText->Element(lLine);

	STScrollInfo siCharacter{}; siCharacter.ucMask = SBI_POS;	GetScrollBar(SB_HORZ, siCharacter);
	if(_CurrentLine->Length()){
		do{ GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint); }
		while(szfTextPoint.width - siCharacter.fPos < (float)GET_X_LPARAM(lParam) && ulCharacterPos < _CurrentLine->Length());
		ptfCaret.x = szfTextPoint.width;
	}
	else ptfCaret.x = 0.0f;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::WM_LButtonDown(_In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	if(hWndElement != GetFocus()) SetFocus(hWndElement);
	if(cSelect) DeSelect();
	LButtonDown(lParam);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::WM_MouseMove(_In_ WPARAM wParam, _In_ LPARAM lParam)
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
void __vectorcall RePag::DirectX::COTextBox::WM_MouseWheel(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	POINTS ptPoints = MAKEPOINTS(lParam);
	POINT ptPoint; ptPoint.x = ptPoints.x; ptPoint.y = ptPoints.y;
	STScrollInfo siCharacter{}; siCharacter.ucMask = SBI_PAGE;
	STScrollInfo siLine{}; siLine.ucMask = SBI_POS | SBI_MAX | SBI_PAGE | SBI_CHARACTER_HEIGHT;
	ThreadSafe_Begin();
	sbHorizontal->GetScrollInfo(siCharacter); sbVertical->GetScrollInfo(siLine);
	ScreenToClient(hWndElement, &ptPoint);
	if(ptPoint.y > 0 && ptPoint.y < FloatToLong(siLine.fPage)){
		if(GET_WHEEL_DELTA_WPARAM(wParam) < 0){
			if(siLine.fPos + siLine.fPage < siLine.fMax){
				siLine.fPos += siLine.szfCharacter.height;
				sbVertical->SetScrollInfo(siLine);
				rclDirty.left = rclDirty.top = 0;
				rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
				OnRender(false);
				ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
			}
		}
		else{
			if(siLine.fPos){
				siLine.fPos -= siLine.szfCharacter.height;
				if(siLine.fPos < 0.0f) siLine.fPos = 0.0f;
				sbVertical->SetScrollInfo(siLine);
				rclDirty.left = rclDirty.top = 0;
				rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
				OnRender(false);
				ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
			}
		}
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::ChangeSizeVisibleScrollBars(void)
{
	STScrollInfo siLine{}; siLine.ucMask = SBI_MAX | SBI_PAGE; STScrollInfo siCharacter{}; siCharacter.ucMask = SBI_MAX | SBI_PAGE;	long lSize;
	sbVertical->GetScrollInfo(siLine);
	if(siLine.fMax > siLine.fPage){
		sbVertical->SetVisible(true);
		sbHorizontal->GetScrollInfo(siCharacter);
		if(siCharacter.fMax <= siCharacter.fPage){
			sbHorizontal->SetVisible(false);
			sbVertical->NewWindowHeight(lHeight);
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
			sbHorizontal->NewWindowWidth(lWidth);
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

	STScrollInfo siScrollInfo{}; siScrollInfo.ucMask = SBI_MAX;
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

	if(cSelect) DeSelect();

	if(pcText && StrLength(pcText)){
		*vasContent = pcText;
		if(vasContent->Length() && (*vasContent)[vasContent->Length() - 1] != 0x0A) *vasContent += "\n";

		if(hWndElement) CreateText();
	}
	else{
		STScrollInfo siScrollInfo{};
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

		STScrollInfo siLine{}; siLine.ucMask = SBI_MAX | SBI_CHARACTER_HEIGHT;
		sbVertical->GetScrollInfo(siLine);
		siLine.fMax += siLine.szfCharacter.height;
		sbVertical->SetScrollInfo(siLine);

		STScrollInfo siCharacter{}; siCharacter.ucMask = SBI_MAX;
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
void __vectorcall RePag::DirectX::COTextBox::SetScrollBarSize(_In_ BYTE ucWidth_Height)
{
	STScrollInfo siCharacter{}; siCharacter.ucMask = SBI_PAGE;
	STScrollInfo siLine{}; siLine.ucMask = SBI_PAGE;

	ThreadSafe_Begin();
	ucScrollBarSize = ucWidth_Height;
	siLine.fPage = (float)(lHeight - ucScrollBarSize);
	sbVertical->SetScrollInfo(siLine);
	sbVertical->NewWindow(lHeight - ucScrollBarSize, ucScrollBarSize, lWidth - ucScrollBarSize, 0);

	siCharacter.fPage = (float)(lWidth - ucScrollBarSize);
	sbHorizontal->SetScrollInfo(siCharacter);
	sbHorizontal->NewWindow(ucScrollBarSize, lWidth - ucScrollBarSize, 0, lHeight - ucScrollBarSize);

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
void __vectorcall RePag::DirectX::COTextBox::SetScrollBarPos(_In_ BYTE ucBar, _In_ long lPos_x, _In_ long lPos_y)
{
	ucBar == SB_HORZ ? sbHorizontal->NewWindowPosition(lPos_x, lPos_y) : sbVertical->NewWindowPosition(lPos_x, lPos_y);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::DeSelect(void)
{
	rclDirty.left = 0; rclDirty.right = lWidth;
  cSelect = 0; stSelect_bottom = stSelect_top; 
	if(heCaret) SetEvent(heCaret);
	OnRender(false);
	ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::SelectText_Left(D2D_POINT_2F& ptfCaret_old)
{
	D2D_SIZE_F szfTextPoint;
	STScrollInfo siLine{}; siLine.ucMask = SBI_PAGE;
	STScrollInfo	siCharacter{}; siCharacter.ucMask = SBI_POS | SBI_PAGE;
	GetScrollBar(SB_HORZ, siCharacter);
	GetTextPoint(_CurrentLine->c_Str(), ulCharacterPos - 1, szfTextPoint);
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
											GetTextPoint(_CurrentLine->c_Str(), ulCharacterPos, szfTextPoint);
											stSelect_bottom.fPosition = ptfCaret.x = szfTextPoint.width;
											stSelect_bottom.ulCharacterPos = ulCharacterPos;
										}
										else{
											GetTextPoint(_CurrentLine->c_Str(), ulCharacterPos, szfTextPoint);
											stSelect_top.fPosition = ptfCaret.x = szfTextPoint.width;
											stSelect_top.ulCharacterPos = ulCharacterPos;
											rclDirty.left = FloatToLong(stSelect_top.fPosition - siCharacter.fPos);
										}
									}
									else if(lLine == stSelect_top.lLine){
										GetTextPoint(_CurrentLine->c_Str(), ulCharacterPos, szfTextPoint);
										stSelect_top.fPosition = ptfCaret.x = szfTextPoint.width;
										stSelect_top.ulCharacterPos = ulCharacterPos;
									}
									else{
										GetTextPoint(_CurrentLine->c_Str(), ulCharacterPos, szfTextPoint);
										stSelect_bottom.fPosition = ptfCaret.x = szfTextPoint.width;
										stSelect_bottom.ulCharacterPos = ulCharacterPos;
									}
									break;
		case	DOWN	:
		case	UP		:	ulCharacterPos--;
									GetTextPoint(_CurrentLine->c_Str(), ulCharacterPos, szfTextPoint);
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
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::SelectText_Right(D2D_POINT_2F& ptfCaret_old)
{
	D2D_SIZE_F szfTextPoint;
	STScrollInfo siLine{}; siLine.ucMask = SBI_PAGE;
	STScrollInfo	siCharacter{}; siCharacter.ucMask = SBI_POS | SBI_PAGE;
	GetScrollBar(SB_HORZ, siCharacter);
	GetTextPoint(_CurrentLine->c_Str(), ulCharacterPos + 1, szfTextPoint);
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
											GetTextPoint(_CurrentLine->c_Str(), ulCharacterPos, szfTextPoint);
											stSelect_bottom.fPosition = ptfCaret.x = szfTextPoint.width;
											stSelect_bottom.ulCharacterPos = ulCharacterPos;
											stSelect_bottom.fPosition > siCharacter.fPage ? rclDirty.right = FloatToLong(siCharacter.fPage)
												: rclDirty.right = FloatToLong(stSelect_bottom.fPosition);
										}
										else{
											rclDirty.left = FloatToLong(stSelect_top.fPosition);
											GetTextPoint(_CurrentLine->c_Str(), ulCharacterPos, szfTextPoint);
											stSelect_top.fPosition = ptfCaret.x = szfTextPoint.width;
											stSelect_top.ulCharacterPos = ulCharacterPos;
										}
									}
									else if(lLine == stSelect_top.lLine){
										GetTextPoint(_CurrentLine->c_Str(), ulCharacterPos, szfTextPoint);
										stSelect_top.fPosition = ptfCaret.x = szfTextPoint.width;
										stSelect_top.ulCharacterPos = ulCharacterPos;
									}
									else{
										GetTextPoint(_CurrentLine->c_Str(), ulCharacterPos, szfTextPoint);
										stSelect_bottom.fPosition = ptfCaret.x = szfTextPoint.width;
										stSelect_bottom.ulCharacterPos = ulCharacterPos;
									}
									break;
		case	DOWN	:
		case	UP		: ulCharacterPos++;
									GetTextPoint(_CurrentLine->c_Str(), ulCharacterPos, szfTextPoint);
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
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::SelectText_Up(D2D_POINT_2F& ptfCaret_old, ULONG &ulCharacterPos_old)
{
	D2D_SIZE_F szfTextPoint;
	STScrollInfo siLine{}; 	siLine.ucMask = SBI_PAGE | SBI_POS;	GetScrollBar(SB_VERT, siLine);
	STScrollInfo	siCharacter{}; siCharacter.ucMask = SBI_PAGE; GetScrollBar(SB_HORZ, siCharacter);
	if(siLine.fPos && ptfCaret.y < szfCharacter.height && cSelect){
		siLine.fPos -= szfCharacter.height;
		SetScrollBar(SB_VERT, siLine);
		rclDirty.left = rclDirty.top = 0;
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
										do{ GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint); }
										while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _CurrentLine->Length());
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
										do{ GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint); }
										while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _CurrentLine->Length());
										stSelect_top.fPosition = ptfCaret.x = szfTextPoint.width;
										stSelect_top.ulCharacterPos = ulCharacterPos;
										ptfCaret.y ? ptfCaret.y -= szfCharacter.height : ptfCaret.y = 0.0f;
										rclDirty.top = FloatToLong(ptfCaret.y);
									}
									else{
										stSelect_bottom.lLine--;
										ulCharacterPos = 0;
										do{ GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint); }
										while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _CurrentLine->Length());
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
										do{ GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint); }
										while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _CurrentLine->Length());
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
										do{ GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint); }
										while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _CurrentLine->Length());
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
										do{ GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint); }
										while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _CurrentLine->Length());
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
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::SelectText_Down(D2D_POINT_2F& ptfCaret_old, ULONG& ulCharacterPos_old)
{
	D2D_SIZE_F szfTextPoint;
	STScrollInfo siLine{}; 	siLine.ucMask = SBI_PAGE | SBI_POS;	GetScrollBar(SB_VERT, siLine);
	STScrollInfo	siCharacter{}; siCharacter.ucMask = SBI_PAGE; GetScrollBar(SB_HORZ, siCharacter);
	GetScrollBar(SB_VERT, siLine);
	if(cSelect && ptfCaret.y + szfCharacter.height * 2 > siLine.fPage){
		siLine.fPos += szfCharacter.height;
		SetScrollBar(SB_VERT, siLine);
		rclDirty.left = rclDirty.top = 0;
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
		case	UP		: pvLine = vliText->Element(++lLine);
									if(lLine == stSelect_bottom.lLine){
										stSelect_top.lLine++;
										ulCharacterPos = 0;
										do{ GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint); }
										while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _CurrentLine->Length());
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
										do{ GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint); }
										while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _CurrentLine->Length());
										stSelect_top.fPosition = ptfCaret.x = szfTextPoint.width;
										stSelect_top.ulCharacterPos = ulCharacterPos;
										rclDirty.top = FloatToLong(ptfCaret.y);
										ptfCaret.y += szfCharacter.height;
									}
									else{
										stSelect_bottom.lLine++;
										ulCharacterPos = 0;
										do{ GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint); }
										while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _CurrentLine->Length());
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
										do{ GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint); }
										while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _CurrentLine->Length());
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
										do{ GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint); }
										while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _CurrentLine->Length());
										stSelect_top.fPosition = ptfCaret.x = szfTextPoint.width;
										stSelect_top.ulCharacterPos = ulCharacterPos;
										ptfCaret.y += szfCharacter.height;
									}
									else{
										stSelect_bottom.lLine++;
										ulCharacterPos = 0;
										do{ GetTextPoint(_CurrentLine->c_Str(), ++ulCharacterPos, szfTextPoint); }
										while(szfTextPoint.width < ptfCaret.x && ulCharacterPos < _CurrentLine->Length());
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
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COTextBox::DoNotCopy(_In_ bool bDoNotCopyA)
{
	ThreadSafe_Begin();
	bDoNotCopy = bDoNotCopyA;
	ThreadSafe_End();
}