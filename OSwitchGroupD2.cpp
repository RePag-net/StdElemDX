/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename : OSwitchGroupD2.cpp
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
#include "OSwitchGroupD2.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COSwitchGroup* __vectorcall RePag::DirectX::COSwitchGroupV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement, _In_ unsigned char ucNumberOfSwitches,
																																					 _In_ unsigned char ucWechselmodus, _In_ STDeviceResources* pstDeviceResources)
{
	COSwitchGroup* vSwitchGroup = (COSwitchGroup*)VMBlock(VMDialog(), sizeof(COSwitchGroup));
	vSwitchGroup->COSwitchGroupV(VMDialog(), pcWindowName, uiIDElement, ucNumberOfSwitches, ucWechselmodus, pstDeviceResources);
	return vSwitchGroup;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::COSwitchGroup* __vectorcall RePag::DirectX::COSwitchGroupV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																					 _In_ unsigned char ucNumberOfSwitches, _In_ unsigned char ucWechselmodus,
																																					 _In_ STDeviceResources* pstDeviceResources)
{
	COSwitchGroup* vSwitchGroup = (COSwitchGroup*)VMBlock(vmMemory, sizeof(COSwitchGroup));
	vSwitchGroup->COSwitchGroupV(vmMemory, pcWindowName, uiIDElement, ucNumberOfSwitches, ucWechselmodus, pstDeviceResources);
	return vSwitchGroup;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_SwitchGroup(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	COSwitchGroup* pSwitchGroup;
	switch(uiMessage){
		case WM_CREATE			: ((COSwitchGroup*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
													((COSwitchGroup*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
		case WM_SIZE				: pSwitchGroup = (COSwitchGroup*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pSwitchGroup) pSwitchGroup->WM_Size_Element(hWnd, lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_COMMAND			: PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													return NULL;
		case WM_LBUTTONDOWN	: ((COSwitchGroup*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDown(lParam);
													return NULL;
		case WM_LBUTTONUP		: ReleaseCapture();
													PostMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWnd, GWLP_ID), wParam), lParam);
													return NULL;
		case WM_MOUSEMOVE		: ((COSwitchGroup*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseMove(wParam, lParam);
													return NULL;
		case WM_NCDESTROY		: pSwitchGroup = (COSwitchGroup*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pSwitchGroup->htEffect_Timer) DeleteTimerQueueTimer(TimerQueue(), pSwitchGroup->htEffect_Timer, INVALID_HANDLE_VALUE);
													VMFreiV(pSwitchGroup);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_Two_waySwitch(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	COSwitchGroup::COTwo_waySwitch* pTwo_waySwitch;
	switch(uiMessage){
		case WM_CREATE			: ((COSwitchGroup::COTwo_waySwitch*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
													((COSwitchGroup::COTwo_waySwitch*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
													return NULL;
		case WM_SIZE				: pTwo_waySwitch = (COSwitchGroup::COTwo_waySwitch*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pTwo_waySwitch) pTwo_waySwitch->WM_Size_Element(hWnd, lParam);
													return NULL;
		case WM_COMMAND			: PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													break;
		case WM_LBUTTONDOWN	: pTwo_waySwitch = (COSwitchGroup::COTwo_waySwitch*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													SetCapture(hWnd);
													if(pTwo_waySwitch->pfnWM_LButtonDown){
														pTwo_waySwitch->ThreadSafe_Begin();
														pTwo_waySwitch->pfnWM_LButtonDown(pTwo_waySwitch); pTwo_waySwitch->ThreadSafe_End();
													}
													return NULL;
		case WM_LBUTTONUP		: ((COSwitchGroup::COTwo_waySwitch*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonUp(wParam, lParam);
													return NULL;
		case WM_NCDESTROY		: pTwo_waySwitch = (COSwitchGroup::COTwo_waySwitch*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													pTwo_waySwitch->pSwitchGroup->vpTwo_waySwitch[pTwo_waySwitch->IDElement() - pTwo_waySwitch->pSwitchGroup->IDElement() - 1] = nullptr;
													VMFreiV(pTwo_waySwitch);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::COSwitchGroupV(_In_ VMEMORY vmMemory, _In_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																																_In_ unsigned char ucNumberOfSwitches, _In_ unsigned char ucWechselmodus,
																																_In_ STDeviceResources* pstDeviceResources)
{
	COGraphicV(vmMemory, pcRePag_SwitchGroup, pcWindowName, uiIDElementA, pstDeviceResources);

	ExtendStyle(WS_EX_STATICEDGE | WS_EX_NOPARENTNOTIFY);
	crfBackground = {RGB(100, 100, 100), 1.0f};

	ucModus = ucWechselmodus;
	pfnChangeColumnWidth = nullptr;
  pfnOnRender = nullptr;
	sMausPos_x = 0;
	ucIndex = 0xFF;

	crfText = D2D1::ColorF(RGB(0, 0, 0), 1.0f);
	pstDeviceResources->ifdwriteFactory7->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
																												 13.0f, L"de-DE", &ifText);

#ifndef _64bit
	vpTwo_waySwitch = (COTwo_waySwitch**)VMBlock(vmMemory, ucNumberOfSwitches * 4);
#else
	vpTwo_waySwitch = (COTwo_waySwitch**)VMBlock(vmMemory, ucNumberOfSwitches * 8);
#endif
	char pc11Zahl[11]; COStringA asName = pcWindowName; asName += "_Two_waySwitch_";
	ucCount = 0; uiIDElementA++; // Note: one uiIDElement for each switch plus one for group !!!
	while(ucCount < ucNumberOfSwitches){
		asName += ULONGtoCHAR(pc11Zahl, ucCount);
		vpTwo_waySwitch[ucCount] = (COTwo_waySwitch*)VMBlock(vmMemory, sizeof(COTwo_waySwitch));
		vpTwo_waySwitch[ucCount]->COTwo_waySwitchV(vmMemory, asName.c_Str(), uiIDElementA + ucCount, this, pstDeviceResources);
		if(ucWechselmodus) vpTwo_waySwitch[ucCount]->bGroup = true;
		vpTwo_waySwitch[ucCount]->SetTextColor(100, 100, 100, 1);
		vpTwo_waySwitch[ucCount]->SetBackgroundColor(150, 150, 150, 1);
		vpTwo_waySwitch[ucCount]->SetBackgroundColor_2(250, 250, 250, 1);
		vpTwo_waySwitch[ucCount]->SetBackgroundColor_3(200, 200, 200, 1);
		vpTwo_waySwitch[ucCount]->TextAlignment(TXA_CENTERVERTICAL | TXA_CENTERHORIZONTAL);
		(ucCount < 10 ? asName.Delete(asName.Length() - 1, 1) : asName.Delete(asName.Length(), 2));
		ucCount++;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::COSwitchGroup::COFreiV(void)
{
	SafeRelease(&ifText); SafeRelease(&ifTextColor);

	while(--ucCount){ if(vpTwo_waySwitch[ucCount]) DestroyWindow(vpTwo_waySwitch[ucCount]->HWND_Element()); }
	if(vpTwo_waySwitch[ucCount]) DestroyWindow(vpTwo_waySwitch[ucCount]->HWND_Element());
	VMFrei(vmMemory, vpTwo_waySwitch);
	return ((COElement*)this)->COFreiV();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::OnRender(void)
{
	WaitForSingleObjectEx(heRender, INFINITE, false);
	ifD2D1Context6->BeginDraw();
	ifD2D1Context6->Clear(crfBackground);
	if(pfnOnRender) pfnOnRender(this);
	ifD2D1Context6->EndDraw();
	SetEvent(heRender);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::OnPaint(void)
{
	ThreadSafe_Begin();
	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::WM_Create(void)
{
	ifD2D1Context6->CreateSolidColorBrush(crfText, &ifTextColor);

	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::SetFont(STFont& stFont)
{
	ThreadSafe_Begin();
	SafeRelease(&ifText);
	pstDeviceResources->ifdwriteFactory7->CreateTextFormat(stFont.fontFamilyName, stFont.fontCollection, stFont.fontWeight, stFont.fontSytle,
																												 stFont.fontStretch, stFont.fontSize, stFont.localeName, &ifText);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::SetTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	crfText = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	if(ifTextColor) ifTextColor->SetColor(crfText);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::SetTextColor(_In_ D2D1_COLOR_F& stTextA)
{
	ThreadSafe_Begin();
	crfText = stTextA;
	if(ifTextColor) ifTextColor->SetColor(crfText);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::DrawText(_In_z_ const wchar_t* wcInhalt, _In_ size_t szBytes_Text, _In_ const D2D1_RECT_F& rcfText)
{
	ifD2D1Context6->DrawText(wcInhalt, (UINT32)szBytes_Text, ifText, rcfText, ifTextColor);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::COTwo_waySwitch::COTwo_waySwitchV(_In_ VMEMORY vmMemory, _In_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																																									 _In_ COSwitchGroup* pSwitchGroupA, _In_ STDeviceResources* pstDeviceResources)
{
	COSwitchV(vmMemory, pcRePag_Two_waySwitch, pcWindowName, uiIDElementA, pstDeviceResources);
	pSwitchGroup = pSwitchGroupA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::COTwo_waySwitch::WM_LButtonUp(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	if(pSwitchGroup->ucModus){
		BYTE ucIndexA = GetWindowLongPtr(hWndElement, GWLP_ID) - GetWindowLongPtr(pSwitchGroup->HWND_Element(), GWLP_ID) - 1;
		if(pSwitchGroup->ucIndex < pSwitchGroup->ucCount){
			if(pSwitchGroup->ucModus == SGM_EINAUS){
				if(pSwitchGroup->ucIndex != ucIndexA){
					Switch(true);
					if(pSwitchGroup->vpTwo_waySwitch[pSwitchGroup->ucIndex]->bGroup) pSwitchGroup->vpTwo_waySwitch[pSwitchGroup->ucIndex]->SetSwitch(false);
				}
			}
			else{
				SetSwitch(!IsOn());
				if(pSwitchGroup->ucIndex != ucIndexA && pSwitchGroup->vpTwo_waySwitch[pSwitchGroup->ucIndex]->bGroup)
					pSwitchGroup->vpTwo_waySwitch[pSwitchGroup->ucIndex]->SetActive(false);
			}
		}
		else Switch(true);
		pSwitchGroup->ucIndex = ucIndexA;
	}

	if(pfnWM_LButtonUp) pfnWM_LButtonUp(this);
	else PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), lParam);
	ThreadSafe_End();
	ReleaseCapture();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::COTwo_waySwitch::OnPaint(void)
{
	ThreadSafe_Begin();
	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::WM_LButtonDown(_In_ LPARAM lParam)
{
	SetCapture(hWndElement);
	ThreadSafe_Begin();
	sMausPos_x = LOWORD(lParam);

	long lPos_x;
	ucSwitch = 0;
	while(ucSwitch < ucCount && vpTwo_waySwitch[ucSwitch]->Pos_X(lPos_x) != sMausPos_x + 1) ucSwitch++;
	ucSwitch--;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::WM_MouseMove(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	if(pfnChangeColumnWidth && wParam == MK_LBUTTON){
		short sWidth = 0; long lSwitchWidth;
		ThreadSafe_Begin();
		if(LOWORD(lParam) >= sMausPos_x) sWidth = LOWORD(lParam) - sMausPos_x;
		else sWidth = LOWORD(lParam) - sMausPos_x;

		if(vpTwo_waySwitch[ucSwitch]->Width(lSwitchWidth) + sWidth >= 3){
			//AndernFensterGrosse(0, sBreite);
			lWidth += sWidth;
			//InvalidateRect(hWndElement, NULL, false);
			MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, true);

			vpTwo_waySwitch[ucSwitch]->ChangeWindowSize(0, sWidth);
			//vpTwo_waySwitch[ucSwitch]->UpdateFenster(NULL, true, false);
			vpTwo_waySwitch[ucSwitch]->OnPaint();

			for(BYTE ucNummer = ucSwitch + 1; ucNummer < ucCount; ucNummer++) vpTwo_waySwitch[ucNummer]->ChangeWindowPosition(sWidth, 0);

			sMausPos_x = LOWORD(lParam);
			pfnChangeColumnWidth(GetParent(hWndElement), ucSwitch, sWidth);
		}
		ThreadSafe_End();
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::CreateSwitchInGroup(unsigned char ucSwitchA, long lHeight, long lWidth, long lPos_x, long lPos_y)
{
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->CreateWindowGraphic(hWndElement, lHeight, lWidth, lPos_x, lPos_y);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::SetAllSwitch_Off(void)
{
	ThreadSafe_Begin();
	for(BYTE ucNummer = 0; ucNummer < ucCount; ucNummer++) vpTwo_waySwitch[ucNummer]->SetSwitch(false);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::SetAllSwitch_Active(_In_ bool bActive)
{
	ThreadSafe_Begin();
	for(BYTE ucNummer = 0; ucNummer < ucCount; ucNummer++) vpTwo_waySwitch[ucNummer]->SetSwitch(bActive);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
unsigned char __vectorcall RePag::DirectX::COSwitchGroup::SelectSwitch(void)
{
	ThreadSafe_Begin();
	BYTE ucIndexA = ucIndex;
	ThreadSafe_End();
	return ucIndexA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Lock(void)
{
	ThreadSafe_Begin();
	for(BYTE ucNummer = 0; ucNummer < ucCount; ucNummer++) vpTwo_waySwitch[ucNummer]->Lock();
	EnableWindow(hWndElement, false);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Release(void)
{
	ThreadSafe_Begin();
	for(BYTE ucNummer = 0; ucNummer < ucCount; ucNummer++) vpTwo_waySwitch[ucNummer]->Release();
	EnableWindow(hWndElement, true);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Switch_Label(_In_ unsigned char ucSwitchA, _In_z_ const char* pcLabel)
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->Label(pcLabel);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Switch_Label_On(_In_ unsigned char ucSwitchA, _In_z_ const char* pcLabel)
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->Label_On(pcLabel);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Switch_Label_Off(_In_ unsigned char ucSwitchA, _In_z_ const char* pcLabel)
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->Label_Off(pcLabel);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Switch_SetSwitch(_In_ unsigned char ucSwitchA, _In_ bool bOn)
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount){ vpTwo_waySwitch[ucSwitchA]->SetSwitch(bOn); if(bOn) ucIndex = ucSwitchA; }
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COSwitchGroup::Switch_Active(_In_ unsigned char ucSwitchA)
{
	ThreadSafe_Begin();
	bool bReturn = false;
	if(ucSwitchA < ucCount) bReturn = vpTwo_waySwitch[ucSwitchA]->Active();
	ThreadSafe_End();
	return bReturn;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Switch_SetActive(_In_ unsigned char ucSwitchA, _In_ bool bActive)
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->SetActive(bActive);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Switch_SetGroup(_In_ unsigned char ucSwitchA, _In_ bool bGroup)
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->bGroup = bGroup;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Switch_SetWidth(_In_ unsigned char ucSwitchA, _In_ long lWidthA)
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->ChangeWindowSize(0, lWidthA);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Switch_SetVisible(_In_ unsigned char ucSwitchA, _In_ bool bVisible)
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->SetVisible(bVisible);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::COSwitchGroup::Switch_IsOn(_In_ unsigned char ucSwitchA)
{
	ThreadSafe_Begin();
	bool bReturn = false;
	if(ucSwitchA < ucCount) bReturn = vpTwo_waySwitch[ucSwitchA]->IsOn();
	ThreadSafe_End();
	return bReturn;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Switch_Font(_In_ unsigned char ucSwitchA, _In_ STFont& stFont)
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->SetFont(stFont);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Switch_SetBackgroundColor(_In_ unsigned char ucSwitchA, _In_ unsigned char ucRed,
																																					 _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->SetBackgroundColor(ucRed, ucGreen, ucBlue, fAlpha);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Switch_SetBackgroundColor_1(_In_ unsigned char ucSwitchA, _In_ unsigned char ucRed,
																																					 _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->SetBackgroundColor_1(ucRed, ucGreen, ucBlue, fAlpha);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Switch_SetBackgroundColor_2(_In_ unsigned char ucSwitchA, _In_ unsigned char ucRed,
																																					 _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->SetBackgroundColor_2(ucRed, ucGreen, ucBlue, fAlpha);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Switch_SetBackgroundColor_3(_In_ unsigned char ucSwitchA, _In_ unsigned char ucRed,
																																					 _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->SetBackgroundColor_3(ucRed, ucGreen, ucBlue, fAlpha);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
long& __vectorcall RePag::DirectX::COSwitchGroup::Switch_Pos_X(_In_ unsigned char ucSwitchA, _Out_ long& lPos_x)
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->Pos_X(lPos_x);
	ThreadSafe_End();
	return lPos_x;
}
//---------------------------------------------------------------------------------------------------------------------------------------
long& __vectorcall RePag::DirectX::COSwitchGroup::Switch_Pos_Y(_In_ unsigned char ucSwitchA, _Out_ long& lPos_y)
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->Pos_Y(lPos_y);
	ThreadSafe_End();
	return lPos_y;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Switch_NewPosition(_In_ unsigned char ucSwitchA, _In_ long lPos_x, _In_ long lPos_y)
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->NewWindowPosition(lPos_x, lPos_y);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Switch_ChangePosition(_In_ unsigned char ucSwitchA, _In_ long lPos_x, _In_ long lPos_y)
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->ChangeWindowPosition(lPos_x, lPos_y);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Switch_pfnWM_LButtonUp(_In_ unsigned char ucSwitchA, void(__vectorcall* pfnSwitch_WM_LButtonUp)(COSwitch*))
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->pfnWM_LButtonUp = pfnSwitch_WM_LButtonUp;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::COSwitchGroup::Switch_pfnWM_LButtonDown(_In_ unsigned char ucSwitchA, void(__vectorcall* pfnSwitch_WM_LButtonDown)(COSwitch*))
{
	ThreadSafe_Begin();
	if(ucSwitchA < ucCount) vpTwo_waySwitch[ucSwitchA]->pfnWM_LButtonDown = pfnSwitch_WM_LButtonDown;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
