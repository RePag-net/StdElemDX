/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OListBoxD2.cpp
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
#include "OListBoxD2.h"
#define _Line ((COStringA*)vliText->Element(pvIterator))
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COListBox* __vectorcall RePag::DirectX::COListBoxV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																	 _In_ STDeviceResources* pstDeviceResources)
{
	COListBox* vListBox = (COListBox*)VMBlock(VMDialog(), sizeof(COListBox));
	vListBox->COListBoxV(VMDialog(), pcWindowName, uiIDElement, pstDeviceResources);
	return vListBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COListBox* __vectorcall RePag::DirectX::COListBoxV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName,
																																	 _In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources)
{
	COListBox* vListBox = (COListBox*)VMBlock(vmMemory, sizeof(COListBox));
	vListBox->COListBoxV(vmMemory, pcWindowName, uiIDElement, pstDeviceResources);
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
void __vectorcall RePag::DirectX::COListBox::COListBoxV(_In_ VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName,
																												_In_ unsigned int uiIDElementA,	_In_ STDeviceResources* pstDeviceResourcesA)
{
	// Note: three numbers uiIDElement, because COScrollBars by COTextBox!!!
	COTextBoxV(vmMemory, pcClassName, pcWindowName, uiIDElementA, pstDeviceResourcesA);
	ucIndex = 0;
 
	pfnWM_Command = nullptr;
	pfnWM_LButtonDown = nullptr;
	pfnWM_LButtonUp = nullptr;
	pfnWM_Char_Return = nullptr;
	pfnWM_Char_Escape = nullptr;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COListBox::COListBoxV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																												_In_ STDeviceResources* pstDeviceResourcesA)
{
	COListBoxV(vmMemory, pcRePag_ListBox, pcWindowName, uiIDElementA, pstDeviceResourcesA);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COListBox::COFreiV(void)
{
	return ((COTextBox*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COListBox::OnPaint(void)
{
	ThreadSafe_Begin();
	OnRender(false);
	rclDirty.left = 0; rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
	ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COListBox::WM_Size(_In_ LPARAM lParam)
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
void __vectorcall RePag::DirectX::COListBox::WM_HScroll(_In_ WPARAM wParam)
{
	ThreadSafe_Begin();
	STScrollInfo siLine{}; siLine.ucMask = SIF_PAGE; GetScrollBar(SB_VERT, siLine);
	STScrollInfo siCharacter{}; siCharacter.ucMask = SIF_PAGE; GetScrollBar(SB_HORZ, siCharacter);
	rclDirty.left = rclDirty.top = 0;
	rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
	OnRender(false);
	ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COListBox::WM_VScroll(_In_ WPARAM wParam)
{
	ThreadSafe_Begin();
	STScrollInfo siLine{}; siLine.ucMask = SIF_PAGE; GetScrollBar(SB_VERT, siLine);
	STScrollInfo siCharacter{}; siCharacter.ucMask = SIF_PAGE; GetScrollBar(SB_HORZ, siCharacter);
  rclDirty.left = rclDirty.top = 0;
	rclDirty.right = FloatToLong(siCharacter.fPage); rclDirty.bottom = FloatToLong(siLine.fPage);
	OnRender(false);
	ifDXGISwapChain4->Present1(1, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COListBox::WM_KeyDown(_In_ WPARAM wParam)
{
	STScrollInfo siLine{}; siLine.ucMask = SBI_POS | SBI_PAGE;
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
		STScrollInfo siLine{}; siLine.ucMask = SBI_POS | SBI_PAGE; GetScrollBar(SB_VERT, siLine);
		STScrollInfo siCharacter{}; siCharacter.ucMask = SBI_PAGE; GetScrollBar(SB_HORZ, siCharacter);

		RECT rcl2Dirty[2] = {0}; dxgiPresent.pDirtyRects = rcl2Dirty;
		if(cSelect){
			float fSelect_top_old = (float)ucIndex * szfCharacter.height;
			if(siLine.fPos) fSelect_top_old -= siLine.fPos;
			if(fSelect_top_old >= 0.0f){
				dxgiPresent.DirtyRectsCount = 2;
				rcl2Dirty[1].top = FloatToLong(fSelect_top_old);
				rcl2Dirty[1].bottom = rcl2Dirty[1].top + FloatToLong(szfCharacter.height) + 2;
				if(rcl2Dirty[1].bottom > FloatToLong(siLine.fPage))  rcl2Dirty[1].bottom = FloatToLong(siLine.fPage);
				rcl2Dirty[1].left = 0; rcl2Dirty[1].right = FloatToLong(siCharacter.fPage);
			}
			else dxgiPresent.DirtyRectsCount = 1;
		}
		else dxgiPresent.DirtyRectsCount = 1;

		cSelect = 1;
		ucIndex = GET_Y_LPARAM(lParam) / FloatToLong(szfCharacter.height) + FloatToLong(siLine.fPos / szfCharacter.height);
		if(ucIndex >= vliText->Number()) ucIndex = (BYTE)vliText->Number() - 1;
		COStringA* pasLine = (COStringA*)vliText->Element(ucIndex); D2D_SIZE_F szfTextPoint;
		ulCharacterPos = pasLine->Length();
		GetTextPoint(pasLine->c_Str(), ulCharacterPos, szfTextPoint);
    stSelect_top.lLine = ucIndex; stSelect_top.ulCharacterPos = 0; stSelect_top.fPosition = 0.0f; 
    stSelect_bottom.lLine = ucIndex; stSelect_bottom.ulCharacterPos = ulCharacterPos; stSelect_bottom.fPosition = szfTextPoint.width + 1.5f;

		rcl2Dirty[0].left = 0;
		stSelect_bottom.fPosition > siCharacter.fPage ? rcl2Dirty[0].right = FloatToLong(siCharacter.fPage) 
																									: rcl2Dirty[0].right = FloatToLong(stSelect_bottom.fPosition);
		rcl2Dirty[0].top = FloatToLong(((float)ucIndex - siLine.fPos / szfCharacter.height) * szfCharacter.height);
		rcl2Dirty[0].bottom = rcl2Dirty[0].top + FloatToLong(szfCharacter.height);
		OnRender(false);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		dxgiPresent.DirtyRectsCount = 1;
		dxgiPresent.pDirtyRects = &rclDirty;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COListBox::SetSelectIndex(_In_ unsigned char ucIndexA)
{
	ThreadSafe_Begin();
	if(ucIndexA < vliText->Number()){	D2D_SIZE_F szfTextPoint;
		STScrollInfo siCharacter{}; siCharacter.ucMask = SIF_PAGE; GetScrollBar(SB_HORZ, siCharacter);
		STScrollInfo siLine{}; siLine.ucMask = SBI_POS | SIF_PAGE; GetScrollBar(SB_VERT, siLine);

		RECT rcl2Dirty[2] = {0}; dxgiPresent.pDirtyRects = rcl2Dirty;
		if(cSelect){
			float fSelect_top_old = (float)ucIndex * szfCharacter.height;
			if(siLine.fPos) fSelect_top_old -= siLine.fPos;
			if(fSelect_top_old >= 0.0f){
				dxgiPresent.DirtyRectsCount = 2;
				rcl2Dirty[1].top = FloatToLong(fSelect_top_old);
				rcl2Dirty[1].bottom = rcl2Dirty[1].top + FloatToLong(szfCharacter.height) + 2;
				if(rcl2Dirty[1].bottom > FloatToLong(siLine.fPage))  rcl2Dirty[1].bottom = FloatToLong(siLine.fPage);
				rcl2Dirty[1].left = 0; rcl2Dirty[1].right = FloatToLong(siCharacter.fPage);
			}
			else dxgiPresent.DirtyRectsCount = 1;
		}
		else dxgiPresent.DirtyRectsCount = 1;

		if((float)ucIndexA < siLine.fPos / szfCharacter.height){
			siLine.fPos = (float)ucIndexA * szfCharacter.height;
			SetScrollBar(SB_VERT, siLine);
			rcl2Dirty[0].top = 0; rcl2Dirty[0].bottom = FloatToLong(siLine.fPage);
		}
		else{
			if((float)ucIndexA >= (siLine.fPos + siLine.fPage) / szfCharacter.height){
				float fLines = siLine.fPage / szfCharacter.height - 1.0f;
				siLine.fPos = (float)ucIndexA * szfCharacter.height - fLines * szfCharacter.height;
				SetScrollBar(SB_VERT, siLine);
				rcl2Dirty[0].top = 0; rcl2Dirty[0].bottom = FloatToLong(siLine.fPage);
			}
			else{
				rcl2Dirty[0].top = FloatToLong(((float)ucIndexA - siLine.fPos / szfCharacter.height) * szfCharacter.height);
				rcl2Dirty[0].bottom = rcl2Dirty[0].top + FloatToLong(szfCharacter.height);
			}
		}

		ucIndex = ucIndexA;
		cSelect = 1;

		COStringA* pasLine = (COStringA*)vliText->Element(ucIndex);
		ulCharacterPos = pasLine->Length();
		GetTextPoint(pasLine->c_Str(), ulCharacterPos, szfTextPoint);
		stSelect_top.lLine = stSelect_bottom.lLine = ucIndex;
		stSelect_top.ulCharacterPos = 0; stSelect_top.fPosition = 0.0f;
		stSelect_bottom.ulCharacterPos = ulCharacterPos; stSelect_bottom.fPosition = szfTextPoint.width + 1.5f;

		rcl2Dirty[0].left = 0; rcl2Dirty[0].right = FloatToLong(siCharacter.fPage);
		OnRender(false);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		dxgiPresent.DirtyRectsCount = 1;
		dxgiPresent.pDirtyRects = &rclDirty;
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
	STScrollInfo siLine{}; siLine.ucMask = SBI_POS | SBI_PAGE; GetScrollBar(SB_VERT, siLine);
	STScrollInfo siCharacter{}; siCharacter.ucMask = SBI_PAGE; GetScrollBar(SB_HORZ, siCharacter);

	float fSelect_top_old = (float)ucIndex * szfCharacter.height;
	if(siLine.fPos) fSelect_top_old -= siLine.fPos;
	if(fSelect_top_old >= 0.0f && fSelect_top_old < siLine.fPage){
		rclDirty.top = FloatToLong(fSelect_top_old);
		rclDirty.bottom = rclDirty.top + FloatToLong(szfCharacter.height) + 2;
		rclDirty.left = 0; rclDirty.right = FloatToLong(siCharacter.fPage);
	}
	cSelect = 0;
	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------