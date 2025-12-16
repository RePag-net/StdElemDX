/******************************************************************************
MIT License

Copyright(c) 2025 René Pagel

Filename: OListBox.cpp
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
#include "OListBoxD2.h"
//#include "OKlappBox.h"
#define _Line ((COStringA*)vliText->Element(pvIterator))
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COListBox* __vectorcall RePag::DirectX::COListBoxV(_In_z_ const char* pcFensterName, _In_ unsigned int uiIDElement,
																																	 _In_ STDeviceResources* pstDeviceResources)
{
	COListBox* vListBox = (COListBox*)VMBlock(VMDialog(), sizeof(COListBox));
	vListBox->COListBoxV(VMDialog(), pcFensterName, uiIDElement, pstDeviceResources);
	return vListBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COListBox* __vectorcall RePag::DirectX::COListBoxV(_In_ VMEMORY vmSpeicher, _In_z_ const char* pcFensterName,
																																	 _In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources)
{
	COListBox* vListBox = (COListBox*)VMBlock(vmSpeicher, sizeof(COListBox));
	vListBox->COListBoxV(vmSpeicher, pcFensterName, uiIDElement, pstDeviceResources);
	return vListBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_ListBox(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	COListBox* pListBox;
	switch(uiMessage){
		case WM_CREATE      : ((COListBox*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
													((COListBox*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
													return NULL;
		case WM_SIZE        : pListBox = (COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pListBox) pListBox->WM_Size(lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_KILLFOCUS   : pListBox = (COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pListBox->pfnWM_KillFocus){ pListBox->ThreadSafe_Begin(); pListBox->pfnWM_KillFocus(pListBox); pListBox->ThreadSafe_End(); }
													return NULL;
		case WM_VSCROLL     : ((COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_VScroll(wParam);
													return NULL;
		case WM_HSCROLL     : ((COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_HScroll(wParam);
													return NULL;
		case WM_KEYDOWN     : ((COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KeyDown(wParam);
													return NULL;
		case WM_CHAR        : ((COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Char(wParam);
													return NULL;
		case WM_COMMAND     : pListBox = (COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pListBox->pfnWM_Command){
														pListBox->ThreadSafe_Begin();
														if(!pListBox->pfnWM_Command(pListBox, wParam)){ pListBox->ThreadSafe_End(); return NULL; }
														pListBox->ThreadSafe_End();
													}
													else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													break;
		case WM_LBUTTONDOWN : pListBox = (COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pListBox->pfnWM_LButtonDown){ pListBox->ThreadSafe_Begin(); pListBox->pfnWM_LButtonDown(pListBox); pListBox->ThreadSafe_End(); }
													return NULL;
		case WM_RBUTTONDOWN	: ((COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->Test();
													return NULL;
		case WM_LBUTTONUP   : pListBox = (COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													pListBox->ThreadSafe_Begin();
													pListBox->WM_LButtonUp(lParam);
													if(pListBox->pfnWM_LButtonUp) pListBox->pfnWM_LButtonUp(pListBox);
													else PostMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWnd, GWLP_ID), wParam), lParam);
													pListBox->ThreadSafe_End();
													return NULL;
		case WM_MOUSEWHEEL  : ((COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseWheel(wParam, lParam);
													return NULL;
		case WM_NCDESTROY   : pListBox = (COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pListBox->htEffect_Timer) DeleteTimerQueueTimer(TimerQueue(), pListBox->htEffect_Timer, INVALID_HANDLE_VALUE);
													VMFreiV(pListBox);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COListBox::COListBoxV(_In_ VMEMORY vmSpeicher, _In_z_ const char* pcKlassenName, _In_z_ const char* pcFensterName,
																												_In_ unsigned int uiIDElementA,	_In_ STDeviceResources* pstDeviceResourcesA)
{
	// Note: three numbers uiIDElement, because COScrollBars by COTextBox!!!
	COTextBoxV(vmSpeicher, pcKlassenName, pcFensterName, uiIDElementA, pstDeviceResourcesA);
	ucIndex = 0;
 
	pfnWM_Command = nullptr;
	pfnWM_LButtonDown = nullptr;
	pfnWM_LButtonUp = nullptr;
	pfnWM_Char_Return = nullptr;
	pfnWM_Char_Escape = nullptr;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COListBox::COListBoxV(_In_ VMEMORY vmSpeicher, _In_z_ const char* pcFensterName, _In_ unsigned int uiIDElementA,
																												_In_ STDeviceResources* pstDeviceResourcesA)
{
	COListBoxV(vmSpeicher, pcRePag_ListBox, pcFensterName, uiIDElementA, pstDeviceResourcesA);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COListBox::COFreiV(void)
{
	return ((COTextBox*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COListBox::WM_HScroll(_In_ WPARAM wParam)
{
	ThreadSafe_Begin();
	rclDirty.left = rclDirty.top = 0;
	rclDirty.right = lWidth; rclDirty.bottom = lHeight;
	OnRender();
	ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COListBox::WM_VScroll(_In_ WPARAM wParam)
{
	ThreadSafe_Begin();
	if(cSelect){ STScrollInfo siLine; siLine.ucMask = SIF_PAGE | SIF_POS; GetScrollBar(SB_VERT, siLine); BYTE ucLines;
		switch(LOWORD(wParam)){
			case SB_LINEUP		: rcfSelect.top += szfCharacter.height; rcfSelect.bottom += szfCharacter.height; break;
			case SB_LINEDOWN	: rcfSelect.top -= szfCharacter.height; rcfSelect.bottom -= szfCharacter.height; break;
			case SB_PAGEUP		:	
			case SB_PAGEDOWN	:	rcfSelect.top = (float)ucIndex * szfCharacter.height - siLine.fPos;
													rcfSelect.bottom = rcfSelect.top + szfCharacter.height; break;
		}
	}
  rclDirty.left = rclDirty.top = 0;
	rclDirty.right = lWidth; rclDirty.bottom = lHeight;
	OnRender();
	ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COListBox::WM_KeyDown(_In_ WPARAM wParam)
{
	STScrollInfo siLine; siLine.ucMask = SBI_POS | SBI_PAGE;
	switch(wParam){
		case VK_UP		: ThreadSafe_Begin();
										if(cSelect) SetSelectIndex(ucIndex - 1);
										ThreadSafe_End();
										break;
		case VK_DOWN	: ThreadSafe_Begin();
										if(cSelect) SetSelectIndex(ucIndex + 1);
										ThreadSafe_End();
										break;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COListBox::WM_Char(_In_ WPARAM wParam)
{
	switch(wParam){
		case VK_RETURN : ThreadSafe_Begin();
											if(pfnWM_Char_Return) pfnWM_Char_Return(this);
											ThreadSafe_End();
											break;
		case VK_ESCAPE : ThreadSafe_Begin();
											DeSelect();
											if(pfnWM_Char_Escape) pfnWM_Char_Escape(this);
											ThreadSafe_End();
											break;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COListBox::WM_LButtonUp(_In_ LPARAM lParam)
{
	if(vliText->Number()){ SetFocus(hWndElement);
		STScrollInfo siLine{}; siLine.ucMask = SBI_POS | SBI_MAX;
		GetScrollBar(SB_VERT, siLine);

		ucIndex = (BYTE)((float)(HIWORD(lParam) / szfCharacter.height) + (BYTE)(siLine.fPos / szfCharacter.height));

		RECT rcl2Dirty[2] = {0};
		dxgiPresent.pDirtyRects = rcl2Dirty;
		if(cSelect){
			if(rcfSelect.top >= 0.0f){
				dxgiPresent.DirtyRectsCount = 2;
				rcl2Dirty[1].left = 0; rcl2Dirty[1].right = lWidth;
				!rcfSelect.top ? rcl2Dirty[1].top = 0 : rcl2Dirty[1].top = FloatToLong(rcfSelect.top) - 1;
				rcl2Dirty[1].bottom = FloatToLong(rcfSelect.bottom) + 1;
			}
			else dxgiPresent.DirtyRectsCount = 1;
		}
		else dxgiPresent.DirtyRectsCount = 1;

		cSelect = 1;
		if(ucIndex == vliText->Number()) ucIndex--;
		ulCharacterPos = ((COStringA*)vliText->Element(ucIndex))->Length();
		rcfSelect.top = (ucIndex - siLine.fPos / szfCharacter.height) * szfCharacter.height;
		rcfSelect.bottom = rcfSelect.top + szfCharacter.height;
		rcfSelect.left = 0.0f; rcfSelect.right = (float)(lWidth - ucScrollBarSize);

		rcl2Dirty[0].left = 0; rcl2Dirty[0].right = lWidth;
		rcl2Dirty[0].top = FloatToLong(((float)ucIndex - siLine.fPos / szfCharacter.height) * szfCharacter.height);
		rcl2Dirty[0].bottom = rcl2Dirty[0].top + FloatToLong(szfCharacter.height);
		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		dxgiPresent.DirtyRectsCount = 1;
		dxgiPresent.pDirtyRects = &rclDirty;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COListBox::DeSelect(void)
{
	/*
	if(cSelect == 2 || cSelect == -2){ rcSelect.left = 0; rcSelect.right = lEdge_right; }
	cSelect = 0; UpdateFenster(&rcSelect, true, false); ShowCaret(hWndElement);
	*/
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COListBox::Test(void)
{
	//SetSelectIndex(5);
	//DeSelectEnum();
  BYTE ucIndexA;
  SearchAndSetEnum(COStringAV("Hallo TextBox Zeile 33 "), ucIndexA);
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COListBox::SetSelectIndex(_In_ unsigned char ucIndexA)
{
	ThreadSafe_Begin();
	if(ucIndexA < vliText->Number()){	STScrollInfo siLine{}; siLine.ucMask = SBI_POS | SIF_PAGE;
		GetScrollBar(SB_VERT, siLine);
		if((float)ucIndexA < siLine.fPos / szfCharacter.height){
			siLine.fPos = (float)ucIndexA * szfCharacter.height;
			rcfSelect.top = 0.0f; rcfSelect.bottom = szfCharacter.height;
		}
		else if((float)ucIndexA >= (siLine.fPos + siLine.fPage) / szfCharacter.height){
      BYTE ucLines = (BYTE)(siLine.fPage / szfCharacter.height) - 1;
			siLine.fPos = (float)ucIndexA * szfCharacter.height - (float)ucLines * szfCharacter.height;
			rcfSelect.top = ((float)ucIndexA - siLine.fPos / szfCharacter.height) * szfCharacter.height; rcfSelect.bottom = rcfSelect.top + szfCharacter.height;
		}
		else rcfSelect.top = ((float)ucIndexA - siLine.fPos / szfCharacter.height) * szfCharacter.height; rcfSelect.bottom = rcfSelect.top + szfCharacter.height;
		ucIndex = ucIndexA;
		cSelect = 1;	
		rcfSelect.left = 0.0f; rcfSelect.right = (float)(lWidth - ucScrollBarSize);
		siLine.ucMask ^= SIF_PAGE;
		SetScrollBar(SB_VERT, siLine);

		rclDirty.left = rclDirty.top = 0;
		rclDirty.right = lWidth; rclDirty.bottom = lHeight;
		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		ThreadSafe_End();
		return true;
	}
	ThreadSafe_End();
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
unsigned char __vectorcall RePag::DirectX::COListBox::GetSelectIndex(void)
{
	ThreadSafe_Begin();
	BYTE ucIndexA = ucIndex;
	ThreadSafe_End();
	return ucIndexA;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::DirectX::COListBox::SelectEnum(_Out_ COStringA* vasEnum)
{
	ThreadSafe_Begin();
	if(!cSelect) *vasEnum = NULL;
	else{ void* pvIterator = vliText->Element(ucIndex);
		if(pvIterator) *vasEnum = *((COStringA*)pvIterator);
		else *vasEnum = NULL;
	}
	ThreadSafe_End();
	return vasEnum;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COListBox::SearchEnum(_In_ COStringA* vasEnum, _Out_ unsigned char& ucIndexA)
{
	ThreadSafe_Begin();
	void* pvIterator = vliText->IteratorToBegin(); ucIndexA = 0;
	while(pvIterator){
		if(*(COStringA*)vliText->Element(pvIterator) == *vasEnum){ ThreadSafe_End(); return true; }
		vliText->NextElement(pvIterator); ucIndexA++;
	}
	ThreadSafe_End();
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COListBox::SearchAndSetEnum(_In_ COStringA* vasEnum, _Out_ unsigned char& ucIndexA)
{
	ThreadSafe_Begin();
	bool bReturn = false;
	if(SearchEnum(vasEnum, ucIndexA)) bReturn = SetSelectIndex(ucIndexA);
	ThreadSafe_End();
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::DirectX::COListBox::SetAndSearchEnum(_In_ unsigned char ucIndexA, _Out_ COStringA* vasEnum)
{
	ThreadSafe_Begin();
	if(SetSelectIndex(ucIndexA)) SelectEnum(vasEnum);
	else *vasEnum = NULL;
	ThreadSafe_End();
	return vasEnum;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
unsigned long __vectorcall RePag::DirectX::COListBox::NumberEnum(void)
{
	ThreadSafe_Begin();
	ULONG ulAnzahl = vliText->Number();
	ThreadSafe_End();
	return ulAnzahl;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COListBox::DeSelectEnum(void)
{
	ThreadSafe_Begin();
	cSelect = 0;
	rclDirty.left = 0; rclDirty.right = lWidth;
	!rcfSelect.top ? rclDirty.top = 0 : rclDirty.top = FloatToLong(rcfSelect.top) - 1;
	rclDirty.bottom = FloatToLong(rcfSelect.bottom) + 1;
	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------