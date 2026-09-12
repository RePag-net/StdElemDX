/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: ODateD2.cpp
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
#include "ODateD2.h"

#define _EditDate ((RePag::DirectX::CODate::COEditDate*)pvParam)

constexpr BYTE BUTTON = 1;
constexpr BYTE TEXT = 2;
constexpr BYTE NONCLIENT = 3;
constexpr BYTE ARROW_LEFT = 4;
constexpr BYTE ARROW_RIGHT = 5;
constexpr BYTE CALENDAR = 6;

//-------------------------------------------------------------------------------------------------------------------------------------------
char pcIncorrectDateTimeText[] = "The date is invalid.\nIt is being reset to its previous value.";
char pcIncorrectDate[] = "Incorrect date";
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::DirectX::CODate* __vectorcall RePag::DirectX::CODateV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources)
{
	CODate* vDate = (CODate*)VMBlock(VMDialog(), sizeof(CODate));
	vDate->CODateV(VMDialog(), pcWindowName, uiIDElement, pstDeviceResources);
	return vDate;
}
//---------------------------------------------------------------------------
RePag::DirectX::CODate* __vectorcall RePag::DirectX::CODateV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																										 _In_ STDeviceResources* pstDeviceResources)
{
	CODate* vDate = (CODate*)VMBlock(vmMemory, sizeof(CODate));
	vDate->CODateV(vmMemory, pcWindowName, uiIDElement, pstDeviceResources);
	return vDate;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_Date(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	CODate* pCalendar;
	switch(uiMessage){
		case WM_CREATE			: ((CODate*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
													((CODate*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
													return NULL;
		case WM_SIZE				: pCalendar = (CODate*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pCalendar) pCalendar->WM_Size_Element(hWnd, lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_KILLFOCUS		: ((CODate*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KillFocus();
													return NULL;
		case WM_COMMAND			: pCalendar = (CODate*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(!pCalendar->WM_Command(wParam)) return NULL;
													else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													break;
		case WM_MOUSEHOVER	: ((CODate*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseOver(lParam);
													return NULL;
		case WM_MOUSELEAVE	: ((CODate*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseLeave();
													return NULL;
		case WM_MOUSEMOVE		: ((CODate*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseMove(wParam, lParam);
													return NULL;
		case WM_LBUTTONUP		: pCalendar = (CODate*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													pCalendar->ThreadSafe_Begin();
													if(pCalendar->WM_LButtonUp(lParam)){
														if(pCalendar->pfnWM_LButtonUp)	pCalendar->pfnWM_LButtonUp(pCalendar);
														else PostMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWnd, GWLP_ID), wParam), lParam);
													}
													pCalendar->ThreadSafe_End();
													return NULL;
		case WM_NCDESTROY		: pCalendar = (CODate*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pCalendar->htEffect_Timer) DeleteTimerQueueTimer(TimerQueue(), pCalendar->htEffect_Timer, INVALID_HANDLE_VALUE);
													VMFreiV(pCalendar);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//---------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::DirectX::WndProc_EditDate(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	CODate::COEditDate* pEditDate;
	switch(uiMessage){
		case WM_CREATE				: ((CODate::COEditDate*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_Element(hWnd);
														((CODate::COEditDate*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create();
														return NULL;
		case WM_SIZE					: pEditDate = (CODate::COEditDate*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
														if(pEditDate) pEditDate->WM_Size_Element(hWnd, lParam);
														else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
														return NULL;
		case WM_SETFOCUS			: ((CODate::COEditDate*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_SetFocus();
														return NULL;
		case WM_KILLFOCUS			: ((CODate::COEditDate*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KillFocus();
														return NULL;
		case WM_KEYDOWN				:	((CODate::COEditDate*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KeyDown(wParam, lParam);
														return NULL;
		case WM_CHAR					:	((CODate::COEditDate*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Char(wParam);
														return NULL;
		case WM_COMMAND				: pEditDate = (CODate::COEditDate*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
														if(!pEditDate->WM_Command(wParam)) return NULL;
														else if(!pEditDate->WM_Command_EditDate(wParam)) return NULL;
														else if(pEditDate->pfnWM_Command){
															pEditDate->ThreadSafe_Begin();
															if(!pEditDate->pfnWM_Command(pEditDate, wParam)){ pEditDate->ThreadSafe_End(); return NULL; }
															pEditDate->ThreadSafe_End();
														}
														else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
														break;
		case WM_CONTEXTMENU		: ((CODate::COEditDate*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_ContexMenu(lParam);
														return NULL;
		case WM_MOUSEHOVER		: ((CODate::COEditDate*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseOver(lParam);
														return NULL;
		case WM_MOUSELEAVE		: ((CODate::COEditDate*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseLeave();
														return NULL;
		case WM_MOUSEMOVE			: ((CODate::COEditDate*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseMove(wParam, lParam);
														return NULL;
		case WM_LBUTTONDOWN		: ((CODate::COEditDate*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDown(wParam, lParam);
														return NULL;
		case WM_LBUTTONUP			: ((CODate::COEditDate*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonUp(wParam, lParam);
														return NULL;
		case WM_LBUTTONDBLCLK	: ((CODate::COEditDate*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDBClick(lParam);
														return NULL;
		case WM_NCDESTROY			: pEditDate = (CODate::COEditDate*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
														if(pEditDate->htEffect_Timer) DeleteTimerQueueTimer(TimerQueue(), pEditDate->htEffect_Timer, INVALID_HANDLE_VALUE);
														VMFreiV(pEditDate);
														return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::CODateV(_In_ VMEMORY vmMemory, _In_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																									_In_ STDeviceResources* pstDeviceResources)
{
	COGraphicV(vmMemory, pcRePag_Date, pcWindowName, uiIDElementA, pstDeviceResources);
	WindowStyle(WS_CHILDWINDOW | WS_CLIPSIBLINGS);

	fColumnWidth = fRowHeight = 0.0f;
	lWidth_Calendar = lYear_Pos_x = 0;
	ucFirstDayOfWeek = ucLastDay = 0;

	COStringA asTemp = COStringA(pcWindowName); asTemp += "_edEditDate";
	edEditDate = (COEditDate*)VMBlock(vmMemory, sizeof(COEditDate));
	edEditDate->COEditDateV(vmMemory, asTemp.c_Str(), ++uiIDElementA, this, pstDeviceResources);
	lHeight_EditDate = 20;

	asTemp = pcWindowName; asTemp += "_lbMonth";
	lbMonth = COListBoxV(vmMemory, asTemp.c_Str(), ++uiIDElementA, pstDeviceResources);
	lbMonth->SetBackgroundColor(255, 255, 255, 1.0f);
	lbMonth->SetTextColor(0, 0, 128, 1.0f);

	DWORD dwCalendarInfo = CAL_SMONTHNAME1; int iBytes; char c20Month[20];
	for(BYTE ucMonth = 0; ucMonth < 12; ucMonth++){
		iBytes = GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, dwCalendarInfo, nullptr, NULL, nullptr);
		c20Month[iBytes] = 0;
		GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, dwCalendarInfo, c20Month, iBytes, nullptr);
		lbMonth->Text_NewLine(c20Month, true);
		dwCalendarInfo++;
	}

  crfText = D2D1::ColorF(RGB(0, 0, 0), 1.0f);
	STFont stFont;
	stFont.fontFamilyName = L"Arial";
	stFont.fontCollection = NULL;
	stFont.fontWeight = DWRITE_FONT_WEIGHT_MEDIUM;
	stFont.fontSytle = DWRITE_FONT_STYLE_NORMAL;
	stFont.fontStretch = DWRITE_FONT_STRETCH_MEDIUM;
	stFont.fontSize = 15.0f;
	stFont.localeName = L"en-en";
	SetFont(stFont);

	crfHead_Background = D2D1::ColorF(RGB(180, 125, 90), 1.0f);

	crfArrow = D2D1::ColorF(RGB(255, 255, 0), 1.0f);
	crfArrow_Move = D2D1::ColorF(RGB(255, 50, 50), 1.f);
	crfArrow_Click = D2D1::ColorF(RGB(255, 255, 0), 1.0f);

	bMouseTracking = false;
	ucDirty = NONCLIENT;

	GetLocalTime(&stDate);
	bOrder_DateTime = true;

	pfnWM_LButtonUp = nullptr;
	pfnWM_Char_Return = nullptr;
	pfnWM_Char_Escape = nullptr;
	pfnWM_Command = nullptr;
}
//---------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::CODate::COFreiV(void)
{
	SafeRelease(&ifText); SafeRelease(&ifTextColor); SafeRelease(&ifHead_Background); SafeRelease(&ifHead);
	SafeRelease(&ifArrow_left); SafeRelease(&ifArrow_right); SafeRelease(&ifArrowColor_left);  SafeRelease(&ifArrowColor_right);

	if(edEditDate) DestroyWindow(edEditDate->HWND_Element());
	if(lbMonth) DestroyWindow(lbMonth->HWND_Element());
	return ((COElement*)this)->COFreiV();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void CALLBACK RePag::DirectX::Timer_Caret_EditDate(_In_ void* pvParam, _In_ bool bTimerOrWaitFired)
{
	WaitForSingleObject(_EditDate->heCaret, INFINITE);
	static bool bCaret = false;
	bCaret ? bCaret = false : bCaret = true;

	_EditDate->ThreadSafe_Begin();
	_EditDate->rclDirty.left = _EditDate->FloatToLong(_EditDate->ptfCaret.x);
	_EditDate->rclDirty.right = _EditDate->rclDirty.left + _EditDate->ucCaretStrength;

	_EditDate->OnRender(bCaret);
	_EditDate->ifDXGISwapChain4->Present1(0, NULL, &_EditDate->dxgiPresent);
	_EditDate->ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::COEditDate::COEditDateV(_In_ VMEMORY vmSpeicher, _In_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																																	_In_ CODate* pCalendarA,_In_ STDeviceResources* pstDeviceResources)
{
	COEditLineV(vmSpeicher, pcRePag_EditDate, pcWindowName, uiIDElementA, pstDeviceResources);

	pCalendar = pCalendarA;

	bNewMask = true;
	ucCharacterSpecification = ZV_ZIFFERN;
	CharacterMask("99'.'99'.'9999");
	asFormat_Date = "dd'.'MM'.'yyyy";
	asFormat_Time = NULL;

	crfButton = D2D1::ColorF(RGB(125, 125, 125), 1.0f);
	crfArrow = D2D1::ColorF(RGB(255, 255, 0), 1.0f);

	crfButton_Move = D2D1::ColorF(RGB(255, 255, 0), 1.0f);
	crfArrow_Move = D2D1::ColorF(RGB(255, 50, 50), 1.f);

	crfButton_Click = D2D1::ColorF(RGB(255, 50, 50), 1.0f);
	crfArrow_Click = D2D1::ColorF(RGB(255, 255, 0), 1.0f);

	bMouseTracking = false;
	ucDirty = NONCLIENT;

	AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hMenu, MF_STRING, IDM_DATUM, "Calendar	Strg+K");
}
//---------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::DirectX::CODate::COEditDate::COFreiV(void)
{
	SafeRelease(&ifButton); SafeRelease(&ifButtonColor);
	SafeRelease(&ifArrow); SafeRelease(&ifArrowColor);

	return ((COEditLine*)this)->COFreiV();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::OnRender(void)
{
	WaitForSingleObjectEx(heRender, INFINITE, false);

	SYSTEMTIME stMonth = stDate; stMonth.wDay = 1; DWORD dwCalendarInfo = CAL_SABBREVDAYNAME1;
	WCHAR pwcWeekday_1[3]; pwcWeekday_1[2] = 0; WCHAR pwcWeekday_2[3]; pwcWeekday_2[2] = 0;
	GetDateFormatEx(LOCALE_NAME_USER_DEFAULT, DATE_USE_ALT_CALENDAR, &stMonth, L"ddd", pwcWeekday_1, 3, nullptr);
	for(BYTE ucTag = 0; ucTag < 7; ucTag++){
		GetCalendarInfoEx(LOCALE_NAME_USER_DEFAULT, CAL_GREGORIAN, nullptr, dwCalendarInfo++, pwcWeekday_2, 3, nullptr);
		if(!wcscmp(pwcWeekday_1, pwcWeekday_2)){ ucTag == 6 ? stMonth.wDayOfWeek = 0 : stMonth.wDayOfWeek = ucTag + 1; break; }
	}
	ucFirstDayOfWeek = stMonth.wDayOfWeek;

	BYTE ucWeeksInMonth = 5;
	switch(stMonth.wMonth){
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12: if(!stMonth.wDayOfWeek || stMonth.wDayOfWeek == 6) ucWeeksInMonth++;
						 ucLastDay = 31;
						 break;
		case 4:
		case 6:
		case 9:
		case 11: if(!stMonth.wDayOfWeek) ucWeeksInMonth++;
						 ucLastDay = 30;
						 break;
		case 2: stMonth.wDay = 29;
						if(!GetDateFormatEx(LOCALE_NAME_USER_DEFAULT, CAL_GREGORIAN, &stMonth, nullptr, nullptr, NULL, nullptr)){
							if(stMonth.wDayOfWeek == 1) ucWeeksInMonth--;
							ucLastDay = 28;
						}
						else ucLastDay = 29;
						break;
	}

	fColumnWidth = (float)lWidth / 7.0f; fRowHeight = (float)lHeight / (float)(ucWeeksInMonth + 1);

	ifD2D1Context6->BeginDraw();
	ifD2D1Context6->Clear(crfBackground);

	D2D1_RECT_F rcfKopf = D2D1::RectF(0.0f, 0.0f, (float)lWidth, fRowHeight);
	SafeRelease(&ifHead);
	pstDeviceResources->ifd2d1Factory7->CreateRectangleGeometry(rcfKopf, &ifHead);
	ifD2D1Context6->DrawGeometry(ifHead, ifHead_Background, 1);
	ifD2D1Context6->FillGeometry(ifHead, ifHead_Background);

	D2D_SIZE_F szfTextPoint; WCHAR wc20Text[20]; size_t szBytes_Text; char c11Zahl[11];

	ULONGtoCHAR(c11Zahl, stMonth.wYear);
	GetTextPoint(c11Zahl, 4, szfTextPoint);
	mbstowcs_s(&szBytes_Text, wc20Text, 20, c11Zahl, 4);
	lYear_Pos_x = lWidth - (long)szfTextPoint.width - (long)fRowHeight;
	D2D1_RECT_F rcfText = D2D1::RectF((float)lYear_Pos_x, (fRowHeight - szfTextPoint.height) / 2.0f, (float)lWidth, fRowHeight);
	ifD2D1Context6->DrawText(wc20Text, (UINT32)szBytes_Text, ifText, rcfText, ifTextColor);

	dwCalendarInfo = CAL_SMONTHNAME1 + stMonth.wMonth; int iBytes;
	iBytes = GetCalendarInfoEx(LOCALE_NAME_USER_DEFAULT, CAL_GREGORIAN, nullptr, --dwCalendarInfo, nullptr, NULL, nullptr);
  wc20Text[iBytes] = 0;
	GetCalendarInfoEx(LOCALE_NAME_USER_DEFAULT, CAL_GREGORIAN, nullptr, dwCalendarInfo, wc20Text, iBytes, nullptr);
	rcfText.left = 5;
	ifD2D1Context6->DrawText(wc20Text, iBytes, ifText, rcfText, ifTextColor);

	ID2D1GeometrySink* ifSink = nullptr;
	D2D1_POINT_2F ptfArrow_right[3] = {
		D2D1::Point2F((float)lWidth - fRowHeight / 4.0f, fRowHeight / 2.0f),
		D2D1::Point2F((float)lWidth - fRowHeight * 0.75f, fRowHeight / 4.0f),
		D2D1::Point2F((float)lWidth - fRowHeight * 0.75f, fRowHeight * 0.75f)
	};
	
	SafeRelease(&ifArrow_right);
	pstDeviceResources->ifd2d1Factory7->CreatePathGeometry(&ifArrow_right);
	ifArrow_right->Open(&ifSink);
	ifSink->SetFillMode(D2D1_FILL_MODE_WINDING);
	ifSink->BeginFigure(ptfArrow_right[0], D2D1_FIGURE_BEGIN_FILLED);
	ifSink->AddLines(ptfArrow_right, 3);
	ifSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	ifSink->Close();
	SafeRelease(&ifSink);

	ifD2D1Context6->DrawGeometry(ifArrow_right, ifArrowColor_right, 1);
	ifD2D1Context6->FillGeometry(ifArrow_right, ifArrowColor_right);

	D2D1_POINT_2F ptfArrow_left[3] = {
		D2D1::Point2F((float)lWidth - szfTextPoint.width - fRowHeight * 1.75f, fRowHeight / 2.0f),
		D2D1::Point2F((float)lWidth - szfTextPoint.width - fRowHeight * 1.25f, fRowHeight / 4.0f),
		D2D1::Point2F((float)lWidth - szfTextPoint.width - fRowHeight * 1.25f, fRowHeight * 0.75f)
	};

	SafeRelease(&ifArrow_left);
	pstDeviceResources->ifd2d1Factory7->CreatePathGeometry(&ifArrow_left);
	ifArrow_left->Open(&ifSink);
	ifSink->SetFillMode(D2D1_FILL_MODE_WINDING);
	ifSink->BeginFigure(ptfArrow_left[0], D2D1_FIGURE_BEGIN_FILLED);
	ifSink->AddLines(ptfArrow_left, 3);
	ifSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	ifSink->Close();
	SafeRelease(&ifSink);

	ifD2D1Context6->DrawGeometry(ifArrow_left, ifArrowColor_left, 1);
	ifD2D1Context6->FillGeometry(ifArrow_left, ifArrowColor_left);

	BYTE ucWeekday, ucDay = 0; float fText_left;
	if(!stMonth.wDayOfWeek){ ucWeekday = 6; fText_left = fColumnWidth * 6.0f; }
	else{ ucWeekday = stMonth.wDayOfWeek - 1; fText_left = (float)ucWeekday * fColumnWidth; }

	ULONGtoCHAR(c11Zahl, ucDay);
	GetTextPoint(c11Zahl, 1, szfTextPoint);
	rcfText.top = fRowHeight + (fRowHeight - szfTextPoint.height) / 2.0f;
	rcfText.bottom = fRowHeight * 2;
	for(BYTE ucWeek = 0; ucWeek < ucWeeksInMonth; ucWeek++){
		do{
			if(++ucDay < 10){
				ULONGtoCHAR(c11Zahl, ucDay);
				GetTextPoint(c11Zahl, 1, szfTextPoint);
				mbstowcs_s(&szBytes_Text, wc20Text, 20, c11Zahl, 1);
				rcfText.left = fText_left + (fColumnWidth - szfTextPoint.width) / 2.0f;
				rcfText.right = fText_left + fColumnWidth;
				ifD2D1Context6->DrawText(wc20Text, (UINT32)szBytes_Text, ifText, rcfText, ifTextColor);
			}
			else{
				ULONGtoCHAR(c11Zahl, ucDay);
				GetTextPoint(c11Zahl, 2, szfTextPoint);
				mbstowcs_s(&szBytes_Text, wc20Text, 20, c11Zahl, 2);
				rcfText.left = fText_left + (fColumnWidth - szfTextPoint.width) / 2.0f; 
				rcfText.right = fText_left + fColumnWidth;
				ifD2D1Context6->DrawText(wc20Text, (UINT32)szBytes_Text, ifText, rcfText, ifTextColor);
			}
			fText_left += fColumnWidth;
		}
		while(++ucWeekday < 7 && ucDay < ucLastDay);
		ucWeekday = 0; fText_left = 0.0f; rcfText.top += fRowHeight; rcfText.bottom += fRowHeight;
	}
	ifD2D1Context6->EndDraw();

	SetEvent(heRender);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::OnPaint(void)
{
	ThreadSafe_Begin();
	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::COEditDate::OnRender(_In_ bool bCaret)
{
	IDWriteTextLayout* ifTextLayout; WCHAR wc255Content[255]; size_t szBytes_Text; float fTextWidth;	D2D1_RECT_F rcfText;

	WaitForSingleObjectEx(heRender, INFINITE, false);
	ifTextColor->SetColor(crfText);
	if(mbstowcs_s(&szBytes_Text, wc255Content, 255, vasContent->c_Str(), vasContent->Length())) goto Error;
	if(pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wc255Content, (UINT32)szBytes_Text, ifText, (float)lWidth, (float)lHeight, &ifTextLayout)) goto Error;
	TextAlignment(ifTextLayout, fTextWidth, rcfText);
	if(ucTextAlignment & TXA_RIGHT){
		rcfText.left -= (float)pCalendar->lHeight_EditDate; rcfText.right -= (float)pCalendar->lHeight_EditDate;
	}
	else if(ucTextAlignment & TXA_CENTERHORIZONTAL){
		rcfText.left -= (float)pCalendar->lHeight_EditDate / 2.0f; rcfText.right -= (float)pCalendar->lHeight_EditDate / 2.0f;
	}
	SafeRelease(&ifTextLayout);

	ifD2D1Context6->BeginDraw();
	ifD2D1Context6->Clear(crfBackground);
	ifD2D1Context6->DrawText(wc255Content, (UINT32)szBytes_Text, ifText, rcfText, ifTextColor);

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
		if(mbstowcs_s(&szBytes_Text, wc255Content, 255, vbCharacter, ulZeichen)){ VMFrei(vbCharacter); goto Error; }
		ifD2D1Context6->DrawText(wc255Content, (UINT32)szBytes_Text, ifText, rcfSelect_1, ifTextColor);
	}

	ID2D1GeometrySink* ifSink = nullptr;
	D2D1_POINT_2F ptfArrow[3] = {
		D2D1::Point2F(fButton_left + fButtonSize * 0.25f, fButton_top + fButtonSize * 0.25f),
		D2D1::Point2F(fButton_left + fButtonSize * 0.75f, fButton_top + fButtonSize * 0.25f),
		D2D1::Point2F(fButton_left + fButtonSize * 0.50f, fButton_top + fButtonSize * 0.75f)
	};

	SafeRelease(&ifArrow);
	pstDeviceResources->ifd2d1Factory7->CreatePathGeometry(&ifArrow);
	ifArrow->Open(&ifSink);
	ifSink->SetFillMode(D2D1_FILL_MODE_WINDING);
	ifSink->BeginFigure(ptfArrow[0], D2D1_FIGURE_BEGIN_FILLED);
	ifSink->AddLines(ptfArrow, 3);
	ifSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	ifSink->Close();
	SafeRelease(&ifSink);

	D2D1_RECT_F rcfButton;
	rcfButton.left = fButton_left;
	rcfButton.top = fButton_top;
	rcfButton.right = (float)lWidth;
	rcfButton.bottom = fButton_top + fButtonSize;

	SafeRelease(&ifButton);
	pstDeviceResources->ifd2d1Factory7->CreateRectangleGeometry(rcfButton, &ifButton);

	ifD2D1Context6->DrawGeometry(ifButton, ifButtonColor, 1);
	ifD2D1Context6->FillGeometry(ifButton, ifButtonColor);
	ifD2D1Context6->DrawGeometry(ifArrow, ifArrowColor, 1);
	ifD2D1Context6->FillGeometry(ifArrow, ifArrowColor);

	ifD2D1Context6->EndDraw();

Error:
	SetEvent(heRender);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::COEditDate::OnPaint(void)
{
	ThreadSafe_Begin();
	OnRender(true);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::COEditDate::WM_Create(void)
{
	stTrackMouseEvent.cbSize = sizeof(stTrackMouseEvent);
	stTrackMouseEvent.dwFlags = TME_HOVER | TME_LEAVE;
	stTrackMouseEvent.hwndTrack = hWndElement;
	stTrackMouseEvent.dwHoverTime = 10;

	float fTextHeight = (float)lHeight;
	float fTextWidth = (float)lWidth;
	fButtonSize = fTextHeight < fTextWidth ? fTextHeight : fTextWidth;
	fButton_left = fTextWidth - fButtonSize;
	fButton_top = (fTextHeight - fButtonSize) / 2.0f;

	ifD2D1Context6->CreateSolidColorBrush(crfText, &ifTextColor);
	ifD2D1Context6->CreateSolidColorBrush(crfButton, &ifButtonColor);
	ifD2D1Context6->CreateSolidColorBrush(crfArrow, &ifArrowColor);
	ifD2D1Context6->CreateSolidColorBrush(crfSelectBack, &ifSelectBackColor);
	ifD2D1Context6->CreateSolidColorBrush(crfCaret, &ifCaretColor);

	OnRender(true);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::COEditDate::WM_SetFocus(void)
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

	if(vasCharacterMask->Length()){
    if(ucTextAlignment & TXA_RIGHT) ptfText.x -= pCalendar->lHeight_EditDate + ucCaretStrength;
    else if(ucTextAlignment & TXA_CENTERHORIZONTAL) ptfText.x -= pCalendar->lHeight_EditDate / 2.0f + ucCaretStrength;
		ptfCaret.x = ptfText.x;
		if(!cSelect) ulCharacterPos = 0;
		EnableMenuItem(hMenu, IDM_CUT, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_PASTE, MF_BYCOMMAND | MF_GRAYED);
	}

	if(!cSelect){
		if(!htCaret) CreateTimerQueueTimer(&htCaret, TimerQueue(), (WAITORTIMERCALLBACK)Timer_Caret_EditDate, this, 0, 500, 0);
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::COEditDate::WM_MouseMove(_In_ LPARAM wParam, _In_ WPARAM lParam)
{
	int iCursor_x = GET_X_LPARAM(lParam);
	ThreadSafe_Begin();
	if(!bMouseTracking){ bMouseTracking = true;	TrackMouseEvent(&stTrackMouseEvent); }
	else{
		switch(ucDirty){
			case BUTTON	: if(iCursor_x < fButton_left){
											ifButtonColor->SetColor(crfButton);
											ifArrowColor->SetColor(crfArrow);
											rclDirty.left = (long)fButton_left; rclDirty.top = 0;
											rclDirty.right = lWidth; rclDirty.bottom = pCalendar->lHeight_EditDate;
											OnRender(true);
											ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											ucDirty = TEXT;
										}
										break;
			case TEXT		: if(iCursor_x >= fButton_left){
											ifButtonColor->SetColor(crfButton_Move);
											ifArrowColor->SetColor(crfArrow_Move);
											rclDirty.left = (long)fButton_left; rclDirty.top = 0;
											rclDirty.right = lWidth; rclDirty.bottom = pCalendar->lHeight_EditDate;
											OnRender(true);
											ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
											ucDirty = BUTTON;
										}
		}
	}

	if(hWndElement == GetFocus() && wParam == MK_LBUTTON){
		if(iCursor_x < FloatToLong(ptfCaret.x - szfCharacter.width)) SendMessage(hWndElement, WM_KEYDOWN, VK_LEFT, NULL);
		else if(iCursor_x > FloatToLong(ptfCaret.x + szfCharacter.width)) SendMessage(hWndElement, WM_KEYDOWN, VK_RIGHT, NULL);
	}

	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::COEditDate::WM_MouseOver(_In_ LPARAM lParam)
{
	int iCursor_x = GET_X_LPARAM(lParam);
	ThreadSafe_Begin();
	if(iCursor_x >= fButton_left){
		ifButtonColor->SetColor(crfButton_Move);
		ifArrowColor->SetColor(crfArrow_Move);
		rclDirty.left = (long)fButton_left; rclDirty.top = 0;
		rclDirty.right = lWidth; rclDirty.bottom = pCalendar->lHeight_EditDate;
		OnRender(true);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		ucDirty = BUTTON;
	}
	else ucDirty = TEXT;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::COEditDate::WM_MouseLeave(void)
{
	ThreadSafe_Begin();
	ifButtonColor->SetColor(crfButton);
	ifArrowColor->SetColor(crfArrow);
	rclDirty.left = (long)fButton_left; rclDirty.top = 0;
	rclDirty.right = lWidth; rclDirty.bottom = pCalendar->lHeight_EditDate;
	OnRender(true);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	ucDirty = NONCLIENT;
	bMouseTracking = false;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::CODate::COEditDate::WM_Command_EditDate(_In_ WPARAM wParam)
{
	ThreadSafe_Begin();
	if(LOWORD(wParam) == IDM_DATUM){
		if(TextToSYSTEMTIME()){ SetWindowPos(pCalendar->HWND_Element(), HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE); SetFocus(pCalendar->HWND_Element()); }
		else{ MessageBox(hWndElement, pcIncorrectDateTimeText, pcIncorrectDate, MB_ICONERROR); DateTimeText(); }
		ThreadSafe_End();
		return false;
	}
	else{
		if(pCalendar->pfnWM_Command){
			bool bReturn = pCalendar->pfnWM_Command(pCalendar, wParam);
			ThreadSafe_End();
			return bReturn;
		}
	}
	ThreadSafe_End();
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::COEditDate::WM_KillFocus(void)
{
	ThreadSafe_Begin();
	DeleteTimerQueueTimer(TimerQueue(), htCaret, NULL); htCaret = nullptr;

	if(!TextToSYSTEMTIME()){ MessageBox(hWndElement, pcIncorrectDateTimeText, pcIncorrectDate, MB_ICONERROR);	DateTimeText(); }
	else{
		rclDirty.left = FloatToLong(ptfCaret.x); rclDirty.right = FloatToLong(ptfCaret.x) + ucCaretStrength;
		rclDirty.left = 0; rclDirty.right = lWidth;
		OnRender(false);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}

	if(pfnWM_KillFocus) pfnWM_KillFocus(this);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::COEditDate::WM_KeyDown(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	D2D_SIZE_F szfTextPoint;
	switch(wParam){
		case VK_HOME	: ThreadSafe_Begin();
										DeleteCaretPos();
										if(cSelect) DeSelect();
										ulCharacterPos = 0;
										if(ucTextAlignment & TXA_LEFT) ptfCaret.x = 0.0f;
										else{
											GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
											if(szfTextPoint.width > (float)(lWidth - pCalendar->lHeight_EditDate)) ptfCaret.x = 0.0f;
											else if(ucTextAlignment & TXA_RIGHT) ptfCaret.x = (float)(lWidth - pCalendar->lHeight_EditDate) - szfTextPoint.width;
											else ptfCaret.x = ((float)(lWidth - pCalendar->lHeight_EditDate) - szfTextPoint.width) / 2.0f;
										}

										ThreadSafe_End();
										break;
		case VK_END		: ThreadSafe_Begin();
										DeleteCaretPos();
										if(cSelect) DeSelect();
										ulCharacterPos = vasContent->Length();
										GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
										if(ucTextAlignment & TXA_LEFT) ptfCaret.x = szfTextPoint.width;
										else if(ucTextAlignment & TXA_RIGHT) ptfCaret.x = (float)(lWidth - pCalendar->lHeight_EditDate - ucCaretStrength);
										else ptfCaret.x = ((float)(lWidth - pCalendar->lHeight_EditDate) - szfTextPoint.width) / 2.0f + szfTextPoint.width;
										ThreadSafe_End();
										break;
		case VK_LEFT	: ThreadSafe_Begin();
										if(ulCharacterPos){
											if(cSelect) DeleteCaretPos();
											rclDirty.right = FloatToLong(ptfCaret.x);
											GetTextPoint(vasContent->c_Str(), --ulCharacterPos, szfTextPoint);
											if(ucTextAlignment & TXA_LEFT){
												if(CharacterMask_FixLeft()) GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
												ptfCaret.x = szfTextPoint.width - fTextPos;
												rclDirty.left = FloatToLong(ptfCaret.x); rclDirty.right += ucCaretStrength;
											}
											else{
												GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
												CharacterMask_FixLeft();
												rclDirty.left = FloatToLong(ptfCaret.x); rclDirty.right += ucCaretStrength;

												ucTextAlignment& TXA_RIGHT ? ptfCaret.x = (float)(lWidth - pCalendar->lHeight_EditDate) - szfTextPoint.width
													: ptfCaret.x = ((float)(lWidth - pCalendar->lHeight_EditDate) - szfTextPoint.width) / 2.0f;

												GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
												ptfCaret.x += szfTextPoint.width;
											}

											if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
												if(cSelect < 0){ rcfSelect.left = ptfCaret.x;	rclDirty.left = FloatToLong(ptfCaret.x); }
												else if(cSelect > 0){
													if(ulSelectPos < ulCharacterPos){
														rcfSelect.right = ptfCaret.x;
														rclDirty.right += ucCaretStrength;
														rclDirty.left = FloatToLong(ptfCaret.x);
													}
													else{
														cSelect = 0; SetEvent(heCaret);
														rclDirty.left = FloatToLong(ptfCaret.x);
													}
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
		case VK_RIGHT	: ThreadSafe_Begin();
										if(ulCharacterPos < vasContent->Length()){
											if(!cSelect) DeleteCaretPos();
											rclDirty.left = FloatToLong(ptfCaret.x);
											GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint);
											if(szfTextPoint.width - fTextPos == (float)(lWidth - pCalendar->lHeight_EditDate))
												ptfCaret.x = (float)(lWidth - pCalendar->lHeight_EditDate - ucCaretStrength);
											else if(ucTextAlignment & TXA_LEFT){
												if(CharacterMask_FixRight()) GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
												rclDirty.left -= ucCaretStrength;
												ptfCaret.x = szfTextPoint.width - fTextPos;
												rclDirty.right = FloatToLong(ptfCaret.x);
											}
											else{
												GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
												CharacterMask_FixRight();
												rclDirty.left = FloatToLong(ptfCaret.x); rclDirty.right += ucCaretStrength;

												ucTextAlignment& TXA_RIGHT ? ptfCaret.x = (float)(lWidth - pCalendar->lHeight_EditDate) - szfTextPoint.width
																									 : ptfCaret.x = ((float)(lWidth - pCalendar->lHeight_EditDate) - szfTextPoint.width) / 2.0f;

												GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
												ptfCaret.x += szfTextPoint.width;
											}

											if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
												if(cSelect > 0){ rcfSelect.right = ptfCaret.x; rclDirty.right = FloatToLong(rcfSelect.right); }
												else if(cSelect < 0){
													if(ulSelectPos > ulCharacterPos) rcfSelect.left = ptfCaret.x;
													else{ cSelect = 0; SetEvent(heCaret); }
												}
												else{
													cSelect = 1;
													ulSelectPos = ulCharacterPos - 1;
													rcfSelect.left = (float)rclDirty.left;
													rcfSelect.right = ptfCaret.x - (float)ucCaretStrength;
													rclDirty.right = FloatToLong(rcfSelect.right);
													ResetEvent(heCaret);
												}
											}
											else if(cSelect) DeSelect();
											OnRender(true);
											ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
										}
										ThreadSafe_End();
										break;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::COEditDate::WM_Char(_In_ WPARAM wParam)
{
	D2D_SIZE_F szfTextPoint, szfTextPoint_1; BYTE ucPosition;
	switch(wParam){
		case VK_RETURN	: ThreadSafe_Begin();
											if(pCalendar->pfnWM_Char_Return) pCalendar->pfnWM_Char_Return(pCalendar);
											ThreadSafe_End();
											break;
		case VK_ESCAPE	: ThreadSafe_Begin();
											if(pCalendar->pfnWM_Char_Escape) pCalendar->pfnWM_Char_Escape(pCalendar);
											ThreadSafe_End();
											break;
		case VK_BACK		: ThreadSafe_Begin();
											if(!ucCharacterSpecification){ ThreadSafe_End(); break; }
											if(ulCharacterPos || !ulCharacterPos && cSelect){
												if(cSelect){
													if(cSelect < 0) ulCharacterPos = ulSelectPos;
													DeSelect();
													GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
													ptfCaret.x = szfTextPoint.width;
												}

												if(CharacterMask_Delete()){
													vasContent->Delete(--ulCharacterPos, 1);
													(ucCharacterSpecification & ZV_SICHTBAR ? vasContent->Insert("_", ulCharacterPos) : vasContent->Insert(" ", ulCharacterPos));
													rclDirty.right = 1;
												}
												else rclDirty.right = 0;

												if(ucTextAlignment & TXA_LEFT) ptfCaret.x = 0.0f;
												else if(ucTextAlignment & TXA_RIGHT){
													GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
													ptfCaret.x = (float)(lWidth - pCalendar->lHeight_EditDate) - szfTextPoint.width;
												}
												else{
													GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
													ptfCaret.x = ((float)(lWidth - pCalendar->lHeight_EditDate) - szfTextPoint.width) / 2.0f;
												}
												GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
												ptfCaret.x += szfTextPoint.width;

												if(rclDirty.right){
													rclDirty.left = FloatToLong(ptfCaret.x); rclDirty.right = lWidth;
													OnRender(true);
													ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												}
											}
											ThreadSafe_End();
											break;
		default					: ThreadSafe_Begin();
											if(CharacterCheck(wParam) && ulCharacterPos < ulCharacter_max){
												if(cSelect){
													if(cSelect > 0){
														ulCharacterPos = ulSelectPos;
														GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
														ptfCaret.x = szfTextPoint.width;
													}
													DeSelect();
												}
												else{
													if(CharacterMask_Insert(wParam)){
														vasContent->Delete(ulCharacterPos, 1); vasContent->Insert((char*)&wParam, ulCharacterPos++);
														GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
														GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint_1);

														if(ucTextAlignment & TXA_LEFT){
															ptfCaret.x = szfTextPoint_1.width;
															rclDirty.left = 0; rclDirty.right = FloatToLong(szfTextPoint.width);
														}
														else if(ucTextAlignment & TXA_RIGHT){
															ptfCaret.x = (float)(lWidth - pCalendar->lHeight_EditDate) - szfTextPoint.width + szfTextPoint_1.width;
															rclDirty.left = lWidth - pCalendar->lHeight_EditDate - FloatToLong(szfTextPoint.width);
															rclDirty.right = lWidth - pCalendar->lHeight_EditDate;
														}
														else{ // TXA_MITTE
															rclDirty.left = FloatToLong(((float)(lWidth - pCalendar->lHeight_EditDate) - szfTextPoint.width) / 2.0f);
															rclDirty.right = rclDirty.left + FloatToLong(szfTextPoint.width);
															ptfCaret.x = (float)rclDirty.left + szfTextPoint_1.width;
														}

														OnRender(true);
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
													}
												}
											}
											ThreadSafe_End();
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::COEditDate::WM_LButtonDown(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	long lCursor_x = GET_X_LPARAM(lParam);
	ThreadSafe_Begin();
	if((float)lCursor_x >= fButton_left){
		ifButtonColor->SetColor(crfButton_Click);
		ifArrowColor->SetColor(crfArrow_Click);
		rclDirty.left = (long)fButton_left; rclDirty.top = 0;
		rclDirty.right = lWidth; rclDirty.bottom = pCalendar->lHeight_EditDate;
		OnRender(false);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
	else{
		SetCapture(hWndElement);
		if(hWndElement == GetFocus()) DeleteCaretPos();
		else SetFocus(hWndElement);
		if(cSelect) DeSelect();

		ulCharacterPos = 0;
		if(vasContent->Length()){
			D2D_SIZE_F szfTextPoint;
			if(fTextPos || ucTextAlignment & TXA_LEFT){
				do{ GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint); }
				while(szfTextPoint.width - fTextPos < (float)lCursor_x && ulCharacterPos < vasContent->Length());
				ptfCaret.x = szfTextPoint.width - fTextPos;

				if(vasCharacterMask->Length()){
					if(CharacterMask_FixRight()){
						GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
						ptfCaret.x = szfTextPoint.width;
					}
				}
			}
			else{
				GetTextPoint(vasContent->c_Str(), vasContent->Length(), szfTextPoint);
				if(szfTextPoint.width > (float)(lWidth - pCalendar->lHeight_EditDate)){
					do{ GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint); }
					while(szfTextPoint.width < (float)lCursor_x && ulCharacterPos < vasContent->Length());
					ptfCaret.x = szfTextPoint.width;
				}
				else if(ucTextAlignment & TXA_RIGHT) ptfCaret.x = (float)lWidth - (float)pCalendar->lHeight_EditDate - szfTextPoint.width;
				else ptfCaret.x = ((float)(lWidth - pCalendar->lHeight_EditDate) - szfTextPoint.width) / 2.0f;

				if((float)lCursor_x > ptfCaret.x){
					do{ GetTextPoint(vasContent->c_Str(), ++ulCharacterPos, szfTextPoint); }
					while(szfTextPoint.width + ptfCaret.x < (float)lCursor_x && ulCharacterPos < vasContent->Length());

					if(vasCharacterMask->Length()){
						if(CharacterMask_FixRight()) GetTextPoint(vasContent->c_Str(), ulCharacterPos, szfTextPoint);
					}
					ptfCaret.x += szfTextPoint.width;
				}
			}
			if(ptfCaret.x == (float)(lWidth - pCalendar->lHeight_EditDate)) ptfCaret.x -= (float)ucCaretStrength;
		}
		if(pfnWM_LButtonDown) pfnWM_LButtonDown(this, wParam, lParam);
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::COEditDate::WM_LButtonUp(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	long lCursor_x = GET_X_LPARAM(lParam);
	ThreadSafe_Begin();
	if(lCursor_x >= fButton_left){
		if(TextToSYSTEMTIME()){
			pCalendar->OnPaint();
			SetWindowPos(pCalendar->HWND_Element(), HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
			SetFocus(pCalendar->HWND_Element());

			ifButtonColor->SetColor(crfButton_Move);
			ifArrowColor->SetColor(crfArrow_Move);
			rclDirty.left = (long)fButton_left; rclDirty.top = 0;
			rclDirty.right = lWidth; rclDirty.bottom = pCalendar->lHeight_EditDate;
			OnRender(true);
			ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);		
		}
		else{ MessageBox(hWndElement, pcIncorrectDateTimeText, pcIncorrectDate, MB_ICONERROR); DateTimeText(); }
	}

	if(pfnWM_LButtonUp) pfnWM_LButtonUp(this, wParam, lParam);
	else PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), lParam);
	ThreadSafe_End();
	ReleaseCapture();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::COEditDate::WM_LButtonDBClick(_In_ LPARAM lParam)
{
	ThreadSafe_Begin();
	if(TextToSYSTEMTIME()){ SetWindowPos(pCalendar->HWND_Element(), HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE); SetFocus(pCalendar->HWND_Element()); }
	else{ MessageBox(hWndElement, pcIncorrectDateTimeText, pcIncorrectDate, MB_ICONERROR); DateTimeText(); }
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::COEditDate::DeleteCaretPos(void)
{
	rclDirty.left = FloatToLong(ptfCaret.x);	rclDirty.right = rclDirty.left + ucCaretStrength;
	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::COEditDate::DeSelect(void)
{
	cSelect = 0;
	rclDirty.left = FloatToLong(rcfSelect.left); rclDirty.right = FloatToLong(rcfSelect.right);
	OnRender(false);
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	SetEvent(heCaret);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::WM_Create(void)
{
	stTrackMouseEvent.cbSize = sizeof(stTrackMouseEvent);
	stTrackMouseEvent.dwFlags = TME_HOVER | TME_LEAVE;
	stTrackMouseEvent.hwndTrack = hWndElement;
	stTrackMouseEvent.dwHoverTime = 10;

	ifD2D1Context6->CreateSolidColorBrush(crfArrow, &ifArrowColor_left);
	ifD2D1Context6->CreateSolidColorBrush(crfArrow, &ifArrowColor_right);

	ifD2D1Context6->CreateSolidColorBrush(crfText, &ifTextColor);
	ifD2D1Context6->CreateSolidColorBrush(crfHead_Background, &ifHead_Background);

	lbMonth->CreateWindowGraphic(hWndElement, lHeight - lHeight_EditDate, lWidth, 0, 0);
	lbMonth->SetVisible(false);

	edEditDate->CreateWindowGraphic(GetParent(hWndElement), lHeight_EditDate, lWidth, ptPosition.x, ptPosition.y - lHeight_EditDate);
	SetDayOfWeek();
	edEditDate->DateTimeText();

	//if(lWidth_Calendar){
	//	lWidth = lWidth_Calendar;
	//	CreateWindowSizeDependentResources();
	//	MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, IsWindowVisible(hWndElement));
	//}

	OnRender();
	ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::WM_MouseMove(_In_ WPARAM wParam, _In_ LPARAM lParam)
{
	POINTS ptlCursor = MAKEPOINTS(lParam);
	ThreadSafe_Begin();

	if(!bMouseTracking){ bMouseTracking = true;	TrackMouseEvent(&stTrackMouseEvent); }
	else{
		switch(ucDirty){
			case ARROW_LEFT	:		if(ptlCursor.x >= lYear_Pos_x || ptlCursor.x <= lYear_Pos_x - (long)fRowHeight || ptlCursor.y > (long)fRowHeight){
														ifArrowColor_left->SetColor(crfArrow);
														rclDirty.left = lYear_Pos_x - (long)fRowHeight;; rclDirty.top = 0;
														rclDirty.right = lYear_Pos_x; rclDirty.bottom = (long)fRowHeight;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = CALENDAR;
													}
													break;
			case ARROW_RIGHT	: if(ptlCursor.x < lWidth - (long)fRowHeight || ptlCursor.y > (long)fRowHeight){
														ifArrowColor_right->SetColor(crfArrow);
														rclDirty.left = lWidth - (long)fRowHeight; rclDirty.top = 0;
														rclDirty.right = lWidth; rclDirty.bottom = (long)fRowHeight;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = CALENDAR;
													}
													break;
			case CALENDAR			: if(ptlCursor.x <= lYear_Pos_x && ptlCursor.x >= lYear_Pos_x - (long)fRowHeight && ptlCursor.y < (long)fRowHeight){
														ifArrowColor_left->SetColor(crfArrow_Move);
														rclDirty.left = lYear_Pos_x - (long)fRowHeight; rclDirty.top = 0;
														rclDirty.right = lYear_Pos_x; rclDirty.bottom = (long)fRowHeight;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = ARROW_LEFT;
													}
													else if(ptlCursor.x >= lWidth - (long)fRowHeight && ptlCursor.y < (long)fRowHeight){
														ifArrowColor_right->SetColor(crfArrow_Move);
														rclDirty.left = lWidth - (long)fRowHeight; rclDirty.top = 0;
														rclDirty.right = lWidth; rclDirty.bottom = (long)fRowHeight;
														OnRender();
														ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
														ucDirty = ARROW_RIGHT;
													}
		}
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::WM_MouseOver(_In_ LPARAM lParam)
{
	POINTS ptlCursor = MAKEPOINTS(lParam);
	ThreadSafe_Begin();
	if(ptlCursor.x <= lYear_Pos_x && ptlCursor.x >= lYear_Pos_x - (long)fRowHeight && ptlCursor.y < (long)fRowHeight){
		ifArrowColor_left->SetColor(crfArrow_Move);
		rclDirty.left = lYear_Pos_x - (long)fRowHeight; rclDirty.top = 0;
		rclDirty.right = lYear_Pos_x; rclDirty.bottom = (long)fRowHeight;
		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		ucDirty = ARROW_LEFT;
	}
	else if(ptlCursor.x >= lWidth - (long)fRowHeight && ptlCursor.y < (long)fRowHeight){
		ifArrowColor_right->SetColor(crfArrow_Move);
		rclDirty.left = lWidth - (long)fRowHeight; rclDirty.top = 0;
		rclDirty.right = lWidth; rclDirty.bottom = (long)fRowHeight;
		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		ucDirty = ARROW_RIGHT;
	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::WM_MouseLeave(void)
{
	ThreadSafe_Begin();
	switch(ucDirty){
		case ARROW_LEFT		: ifArrowColor_left->SetColor(crfArrow);
												rclDirty.left = lYear_Pos_x - (long)fRowHeight; rclDirty.top = 0;
												rclDirty.right = lYear_Pos_x; rclDirty.bottom = (long)fRowHeight;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												break;
		case ARROW_RIGHT	:	ifArrowColor_right->SetColor(crfArrow);
												rclDirty.left = lWidth - (long)fRowHeight; rclDirty.top = 0;
												rclDirty.right = lWidth; rclDirty.bottom = (long)fRowHeight;
												OnRender();
												ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
												break;
	}
	ucDirty = NONCLIENT;
	bMouseTracking = false;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::WM_KillFocus(void)
{
	ThreadSafe_Begin();
  if(lbMonth->Visible()) lbMonth->SetVisible(false);
	if(IsWindowVisible(hWndElement)) ShowWindow(hWndElement, SW_HIDE);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::CODate::WM_Command(_In_ WPARAM wParam)
{
	if(LOWORD(wParam) == GetWindowLongPtr(hWndElement, GWLP_ID) + 2){
		ThreadSafe_Begin();
		stDate.wMonth = lbMonth->GetSelectIndex() + 1;
		lbMonth->SetVisible(false);
		rclDirty.left = 0; rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
		OnRender();
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		SetWindowPos(hWndElement, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE); SetFocus(hWndElement);
		ThreadSafe_End();
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::CODate::WM_LButtonUp(_In_ LPARAM lParam)
{
	float fRow = (float)GET_Y_LPARAM(lParam); int iCursor_x = GET_X_LPARAM(lParam); BYTE ucWoche;

	ThreadSafe_Begin();
	if(fRow < fRowHeight) ucWoche = 0;
	else if(fRow < fRowHeight * 2) ucWoche = 1;
	else if(fRow < fRowHeight * 3) ucWoche = 2;
	else if(fRow < fRowHeight * 4) ucWoche = 3;
	else if(fRow < fRowHeight * 5) ucWoche = 4;
	else if(fRow < fRowHeight * 6) ucWoche = 5;
	else ucWoche = 6;
	
	float fColumn = (float)GET_X_LPARAM(lParam); BYTE ucDayOfWeek;
	if(fColumn < fColumnWidth) ucDayOfWeek = 1;
	else if(fColumn < fColumnWidth * 2) ucDayOfWeek = 2;
	else if(fColumn < fColumnWidth * 3) ucDayOfWeek = 3;
	else if(fColumn < fColumnWidth * 4) ucDayOfWeek = 4;
	else if(fColumn < fColumnWidth * 5) ucDayOfWeek = 5;
	else if(fColumn < fColumnWidth * 6) ucDayOfWeek = 6;
	else ucDayOfWeek = 0;
	 
	if(!ucWoche){
		if(iCursor_x > lWidth - (long)fRowHeight){
			if(lbMonth->Visible()) lbMonth->SetVisible(false);
			stDate.wYear++;
      rclDirty.left = 0; rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
			OnRender();
			ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		}
		else if(iCursor_x < lYear_Pos_x && iCursor_x > lYear_Pos_x - (BYTE)fRowHeight){
			if(lbMonth->Visible()) lbMonth->SetVisible(false);
			stDate.wYear--; 
			rclDirty.left = 0; rclDirty.top = 0; rclDirty.right = lWidth; rclDirty.bottom = lHeight;
			OnRender();
			ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
		}
		else if(iCursor_x < lYear_Pos_x - (BYTE)fRowHeight){
			lbMonth->SetSelectIndex(stDate.wMonth - 1);
      lbMonth->NewWindowHeight(lHeight - fRowHeight);
      lbMonth->ChangeWindowPosition(0, fRowHeight);
			lbMonth->SetVisible(true);
		}
		return false;
	}
	else{
		ucFirstDayOfWeek ? stDate.wDay = 8 - ucFirstDayOfWeek : stDate.wDay = 1;
		if(!ucDayOfWeek){
			stDate.wDay += --ucWoche * 7;
			if(stDate.wDay > ucLastDay) stDate.wDay = ucLastDay;
		}
		else if(!--ucWoche){
			if(ucFirstDayOfWeek && ucDayOfWeek > ucFirstDayOfWeek) stDate.wDay -= 7 - ucDayOfWeek;
			else stDate.wDay = 1;
		}
		else{
			stDate.wDay += --ucWoche * 7 + ucDayOfWeek;
			if(stDate.wDay > ucLastDay) stDate.wDay = ucLastDay;
		}
	}

	SetDayOfWeek(); edEditDate->DateTimeText();
	SetFocus(edEditDate->HWND_Element());

	ThreadSafe_End();
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------
long& __vectorcall RePag::DirectX::CODate::GetHeight(_In_ long& lHeightA)
{
	ThreadSafe_Begin();
	lHeightA = lHeight + lHeight_EditDate;
	ThreadSafe_End();
	return lHeightA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
long& __vectorcall RePag::DirectX::CODate::GetHeight_EditDate(_In_ long& lHeightA)
{
	ThreadSafe_Begin();
	lHeightA = lHeight_EditDate;
	ThreadSafe_End();
	return lHeightA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
long& __vectorcall RePag::DirectX::CODate::GetWidth_EditDate(_In_ long& lWidthA)
{
	ThreadSafe_Begin();
	edEditDate->Width(lWidthA);
	ThreadSafe_End();
	return lWidthA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
RECT& __vectorcall RePag::DirectX::CODate::GetWindow(_In_ RECT& rcWindow)
{
	ThreadSafe_Begin();
	rcWindow.left = ptPosition.x;
	rcWindow.top = ptPosition.y - lHeight_EditDate;
	lWidth_Calendar ? rcWindow.right = ptPosition.x + lWidth_Calendar : rcWindow.right = ptPosition.x + lWidth;
	rcWindow.bottom = ptPosition.y + lHeight;
	ThreadSafe_End();
	return rcWindow;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::NewWindowPosition(_In_ long lPos_x, _In_ long lPos_y)
{
	ThreadSafe_Begin();
	ptPosition.x = lPos_x; ptPosition.y = lPos_y + lHeight_EditDate;

	MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, IsWindowVisible(hWndElement));
	edEditDate->NewWindowPosition(ptPosition.x, ptPosition.y - lHeight_EditDate);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::NewWindowPosition(_In_ POINT& ptPositionA)
{
	ThreadSafe_Begin();
	ptPosition = ptPositionA;
	ptPosition.y += lHeight_EditDate;

	MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, IsWindowVisible(hWndElement));
	edEditDate->NewWindowPosition(ptPosition.x, ptPosition.y - lHeight_EditDate);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::NewWindow(long lHeightA, long lWidthA, long lPos_x, long lPos_y)
{
	ThreadSafe_Begin();
	lHeight = lHeightA;	ptPosition.x = lPos_x; ptPosition.y = lPos_y + lHeight_EditDate;
	lWidth_Calendar ? lWidth = lWidth_Calendar : lWidth = lWidthA;

	CreateWindowSizeDependentResources();
	MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, IsWindowVisible(hWndElement));

	edEditDate->NewWindow(lHeight_EditDate, lWidth, lPos_x, lPos_y);
	edEditDate->OnPaint();
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::NewWindowSize(long lHeightA, long lWidthA)
{
	ThreadSafe_Begin();
	lHeight = lHeightA - lHeight_EditDate;
	lWidth_Calendar > lWidthA ? lWidth = lWidth_Calendar : lWidth = lWidthA;

	CreateWindowSizeDependentResources();
	MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, IsWindowVisible(hWndElement));
	edEditDate->NewWindowSize(lHeight_EditDate, lWidthA);
	edEditDate->OnPaint();
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::NewWindowHeight(long lHeightA)
{
	ThreadSafe_Begin();
	lHeight = lHeightA - lHeight_EditDate;

	CreateWindowSizeDependentResources();
	MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, IsWindowVisible(hWndElement));
	edEditDate->NewWindowHeight(lHeight_EditDate);
	edEditDate->OnPaint();
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::NewWindowWidth(long lWidthA)
{
	ThreadSafe_Begin();
	lWidth_Calendar > lWidthA ? lWidth = lWidth_Calendar : lWidth = lWidthA;

	CreateWindowSizeDependentResources();
	MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, IsWindowVisible(hWndElement));
	edEditDate->NewWindowWidth(lWidthA);
	edEditDate->OnPaint();
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::ChangeWindowSize(long lHeightA, long lWidthA)
{
	ThreadSafe_Begin();
	lHeight += lHeightA;
	lWidth += lWidthA;

	CreateWindowSizeDependentResources();
	MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, IsWindowVisible(hWndElement));
	edEditDate->ChangeWindowSize(0, lWidthA);
	edEditDate->OnPaint();

	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::ChangeWindowPosition(long lPos_x, long lPos_y)
{
	ThreadSafe_Begin();
	ptPosition.x += lPos_x; ptPosition.y += lPos_y;

	MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lWidth, lHeight, IsWindowVisible(hWndElement));
	edEditDate->ChangeWindowPosition(ptPosition.x, ptPosition.y);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
  crfText = D2D1::ColorF(RGB(ucRed, ucGreen, ucBlue), fAlpha);
	edEditDate->SetTextColor(ucRed, ucGreen, ucBlue, fAlpha);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetBackgroundColor(_In_ unsigned char ucRot, _In_ unsigned char ucGrun, _In_ unsigned char ucBlau, _In_ float fAlpha)
{
	ThreadSafe_Begin();
  crfBackground = D2D1::ColorF(RGB(ucRot, ucGrun, ucBlau), fAlpha);
	edEditDate->SetBackgroundColor(ucRot, ucGrun, ucBlau, fAlpha);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetHead_BackgroundColor(_In_ unsigned char ucRot, _In_ unsigned char ucGrun, _In_ unsigned char ucBlau, _In_ float fAlpha)
{
	ThreadSafe_Begin();
  crfHead_Background = D2D1::ColorF(RGB(ucRot, ucGrun, ucBlau), fAlpha);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetButtonColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	edEditDate->crfButton = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetButtonColor(_In_ D2D1_COLOR_F& crfButtonA)
{
	ThreadSafe_Begin();
	edEditDate->crfButton = crfButtonA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetButtonColor_Move(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	edEditDate->crfButton_Move = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetButtonColor_Move(_In_ D2D1_COLOR_F& crfButton_MoveA)
{
	ThreadSafe_Begin();
	edEditDate->crfButton_Move = crfButton_MoveA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetButtonColor_Click(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	edEditDate->crfButton_Click = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetButtonColor_Click(_In_ D2D1_COLOR_F& crfButton_ClickA)
{
	ThreadSafe_Begin();
	edEditDate->crfButton_Click = crfButton_ClickA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetArrowColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	edEditDate->crfArrow = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetArrowColor(_In_ D2D1_COLOR_F& crfArrowA)
{
	ThreadSafe_Begin();
	edEditDate->crfArrow = crfArrowA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetArrowColor_Move(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	edEditDate->crfArrow_Move = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetArrowColor_Move(_In_ D2D1_COLOR_F& crfArrow_MoveA)
{
	ThreadSafe_Begin();
	edEditDate->crfArrow_Move = crfArrow_MoveA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetArrowColor_Click(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha)
{
	ThreadSafe_Begin();
	edEditDate->crfArrow_Click = D2D1::ColorF(RGB(ucBlue, ucGreen, ucRed), fAlpha);
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetArrowColor_Click(_In_ D2D1_COLOR_F& crfArrow_ClickA)
{
	ThreadSafe_Begin();
	edEditDate->crfArrow_Click = crfArrow_ClickA;
	ThreadSafe_End();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetFont(_In_ STFont& stFont)
{
	ThreadSafe_Begin();
	SafeRelease(&ifText);
	pstDeviceResources->ifdwriteFactory7->CreateTextFormat(stFont.fontFamilyName, stFont.fontCollection, stFont.fontWeight, stFont.fontSytle,
																												 stFont.fontStretch, stFont.fontSize, stFont.localeName, &ifText);
	edEditDate->SetFont(stFont);
	lbMonth->SetFont(stFont);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetAvailable(_In_ bool bAvailable)
{
	ThreadSafe_Begin();
	if(bAvailable){
		if(!IsWindowVisible(edEditDate->HWND_Element())) ShowWindow(edEditDate->HWND_Element(), SW_SHOW);
		if(!IsWindowEnabled(edEditDate->HWND_Element())) EnableWindow(edEditDate->HWND_Element(), bAvailable);
		else if(edEditDate->HWND_Element() == GetFocus()) SendMessage(edEditDate->HWND_Element(), WM_SETFOCUS, NULL, NULL);
	}
	else{ if(edEditDate->HWND_Element() == GetFocus()) SetFocus(GetParent(hWndElement)); EnableWindow(edEditDate->HWND_Element(), bAvailable); }
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetVisible(_In_ bool bVisible)
{
	ThreadSafe_Begin();
	if(bVisible) ShowWindow(edEditDate->HWND_Element(), SW_SHOWNA);
	else{ if(edEditDate->HWND_Element() == GetFocus()) SetFocus(GetParent(hWndElement)); ShowWindow(edEditDate->HWND_Element(), SW_HIDE); ShowWindow(hWndElement, SW_HIDE); }
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::CODate::Visible(void)
{
	return IsWindowVisible(edEditDate->HWND_Element());
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetExtendedWidth_Calendar(_In_ long lWidthA)
{
	ThreadSafe_Begin();
	lWidth_Calendar = lWidthA;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetHeight_EditDate(_In_ long lHeightA)
{
	ThreadSafe_Begin();
	lHeight_EditDate = lHeightA;
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::TextAlignment_EditDate(_In_ unsigned char ucTextAlignment)
{
	ThreadSafe_Begin();
	edEditDate->TextAlignment(ucTextAlignment);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::CharacterSpecification_EditDate_ZVVISIBLE(_In_ bool bVisible)
{
	ThreadSafe_Begin();
	if(bVisible) edEditDate->CharacterSpecification(ZV_ZIFFERN | ZV_SICHTBAR);
	else edEditDate->CharacterSpecification(ZV_ZIFFERN);
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::DirectX::CODate::GetDateformat(_In_ COStringA* pasFormat_Date)
{
	ThreadSafe_Begin();
	if(pasFormat_Date) *pasFormat_Date = edEditDate->asFormat_Date;
	ThreadSafe_End();
	return pasFormat_Date;
}
//---------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::DirectX::CODate::GetTimeformat(_In_ COStringA* pasFormat_Time)
{
	ThreadSafe_Begin();
	if(pasFormat_Time) *pasFormat_Time = edEditDate->asFormat_Time;
	ThreadSafe_End();
	return pasFormat_Time;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetDateTimeFormat(_In_ const char* pcFormat_Date, _In_ const char* pcFormat_Time, _In_ bool bOrder_DateTimeA)
{
	ThreadSafe_Begin();
	if(!pcFormat_Date) pcFormat_Date = "dd'.'MM'.'yyyy";
	BYTE ucMaske, ucStelle = 0, ucLange = (BYTE)StrLength(pcFormat_Date); bool bUngultig = false; char c2Buchstabe[2]; c2Buchstabe[1] = 0; COStringA asMask_Date;

	do{
		switch(pcFormat_Date[ucStelle]){
			case 0x64	: ucMaske = 1; while(pcFormat_Date[++ucStelle] == 0x64 && ucStelle < ucLange) ucMaske++;
									switch(ucMaske){
										case 1: asMask_Date += "d"; break;
										case 2: asMask_Date += "dd"; break;
										case 3: asMask_Date += "ddd"; break;
										case 4: asMask_Date += "dddd"; break;
										default: bUngultig = true; ucLange = 0;
									}
									break;
			case 0x4d	: ucMaske = 1; while(pcFormat_Date[++ucStelle] == 0x4d && ucStelle < ucLange) ucMaske++;
									switch(ucMaske){
										case 1: asMask_Date += "M"; break;
										case 2: asMask_Date += "MM"; break;
										case 3: asMask_Date += "MMM"; break;
										case 4: asMask_Date += "MMMM"; break;
										default: bUngultig = true; ucLange = 0;
									}
									break;
			case 0x79	: ucMaske = 1; while(pcFormat_Date[++ucStelle] == 0x79 && ucStelle < ucLange) ucMaske++;
									switch(ucMaske){
										case 1: asMask_Date += "y"; break;
										case 2: asMask_Date += "yy"; break;
										case 4: asMask_Date += "yyyy"; break;
										default: bUngultig = true; ucLange = 0;
									}
									break;
			case 0x27	: asMask_Date += "'";
									while(pcFormat_Date[++ucStelle] != 0x27 && ucStelle < ucLange){
										*c2Buchstabe = pcFormat_Date[ucStelle];
										asMask_Date += c2Buchstabe;
									}
									asMask_Date += "'"; ucStelle++;
									break;
			default		: bUngultig = true; ucLange = 0; break;
		}
	}
	while(ucStelle < ucLange);

	if(pcFormat_Time && !bUngultig){ COStringA asMask_Time; ucStelle = 0, ucLange = (BYTE)StrLength(pcFormat_Time);
		do{
			switch(pcFormat_Time[ucStelle]){
				case 0x68	: ucMaske = 1; while(pcFormat_Time[++ucStelle] == 0x68 && ucStelle < ucLange) ucMaske++;
										switch(ucMaske){
											case 1: asMask_Time += "h"; break;
											case 2: asMask_Time += "hh"; break;
											default: bUngultig = true; ucLange = 0;
										}
										break;
				case 0x48	: ucMaske = 1; while(pcFormat_Time[++ucStelle] == 0x48 && ucStelle < ucLange) ucMaske++;
										switch(ucMaske){
											case 1: asMask_Time += "H"; break;
											case 2: asMask_Time += "HH"; break;
											default: bUngultig = true; ucLange = 0;
										}
										break;
				case 0x6d	: ucMaske = 1; while(pcFormat_Time[++ucStelle] == 0x6d && ucStelle < ucLange) ucMaske++;
										switch(ucMaske){
											case 1: asMask_Time += "m"; break;
											case 2: asMask_Time += "mm"; break;
											default: bUngultig = true; ucLange = 0;
										}
										break;
				case 0x73	: ucMaske = 1; while(pcFormat_Time[++ucStelle] == 0x73 && ucStelle < ucLange) ucMaske++;
										switch(ucMaske){
											case 1: asMask_Time += "s"; break;
											case 2: asMask_Time += "ss"; break;
											default: bUngultig = true; ucLange = 0;
										}
										break;
				case 0x74	: ucMaske = 1; while(pcFormat_Time[++ucStelle] == 0x74 && ucStelle < ucLange) ucMaske++;
										switch(ucMaske){
											case 1: asMask_Time += "t"; break;
											case 2: asMask_Time += "tt"; break;
											default: bUngultig = true; ucLange = 0;
										}
										break;
				case 0x27	: asMask_Time += "'";
										while(pcFormat_Time[++ucStelle] != 0x27 && ucStelle < ucLange){
											*c2Buchstabe = pcFormat_Time[ucStelle];
											asMask_Time += c2Buchstabe;
										}
										asMask_Time += "'"; ucStelle++;
										break;
				default		: bUngultig = true; ucLange = 0; break;
			}
		}
		while(ucStelle < ucLange);

		if(!bUngultig) edEditDate->asFormat_Time = asMask_Time;
	}

	if(!bUngultig){ bOrder_DateTime = bOrder_DateTimeA;	edEditDate->bNewMask = true; edEditDate->asFormat_Date = asMask_Date; edEditDate->DateTimeText();	}
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::COEditDate::DateTimeText(void)
{
	size_t szBytes_Text; WCHAR wc50Format_Date[50];
	mbstowcs_s(&szBytes_Text, wc50Format_Date, 50, asFormat_Date.c_Str(), asFormat_Date.Length());
	int iBytes_Datum = GetDateFormatEx(LOCALE_NAME_USER_DEFAULT, DATE_USE_ALT_CALENDAR, &pCalendar->stDate, wc50Format_Date, nullptr, NULL, nullptr);
	VMBLOCK vbDatum = VMBlock(iBytes_Datum);  vbDatum[iBytes_Datum - 1] = 0; VMBLOCK vbDatum_A = nullptr;
	GetDateFormat(LOCALE_USER_DEFAULT, NULL, &pCalendar->stDate, asFormat_Date.c_Str(), vbDatum, iBytes_Datum);

	int iBytes_Zeit = 0; VMBLOCK vbZeit = nullptr; VMBLOCK vbZeit_A = nullptr;
	if(asFormat_Time.Length()){
		iBytes_Zeit = GetTimeFormat(LOCALE_USER_DEFAULT, NULL, &pCalendar->stDate, asFormat_Time.c_Str(), nullptr, NULL);
		vbZeit = VMBlock(iBytes_Zeit); vbZeit[iBytes_Zeit - 1] = 0;
		GetTimeFormat(LOCALE_USER_DEFAULT, NULL, &pCalendar->stDate, asFormat_Time.c_Str(), vbZeit, iBytes_Zeit);
	}

	if(bNewMask){
		BYTE ucMaske, ucStelle_Maske = 0, ucStelle_DatumZeit = 0, ucLange = asFormat_Date.Length(); COStringA asMaske_Datum;
		char c2Buchstabe[2]; c2Buchstabe[1] = 0; WORD wJahr;	bNewMask = false; char pc5KurzText[5]; int iBytes; DWORD dwKalenderInfo; VMBLOCK vbText;
		do{
			switch((asFormat_Date)[ucStelle_Maske]){
				case 0x64	: ucMaske = 1; while((asFormat_Date)[++ucStelle_Maske] == 0x64 && ucStelle_Maske < ucLange) ucMaske++;
										switch(ucMaske){
											case 1	: if(pCalendar->stDate.wDay < 10){
																	if(!vbDatum_A){
																		vbDatum_A = VMBlock(iBytes_Datum + 1); vbDatum_A[iBytes_Datum] = 0;
																		vbDatum_A[ucStelle_DatumZeit] = 0x20;
																		MemCopy(vbDatum_A, vbDatum, ucStelle_DatumZeit);
																		MemCopy(&vbDatum_A[ucStelle_DatumZeit + 1], &vbDatum[ucStelle_DatumZeit], iBytes_Datum - ucStelle_DatumZeit - 1);
																		VMFrei(vbDatum); vbDatum = nullptr;
																	}
																	else{
																		vbDatum = VMBlock(iBytes_Datum + 1); vbDatum[iBytes_Datum] = 0;
																		vbDatum[ucStelle_DatumZeit] = 0x20;
																		MemCopy(vbDatum, vbDatum_A, ucStelle_DatumZeit);
																		MemCopy(&vbDatum[ucStelle_DatumZeit + 1], &vbDatum_A[ucStelle_DatumZeit], iBytes_Datum - ucStelle_DatumZeit - 1);
																		VMFrei(vbDatum_A); vbDatum_A = nullptr;
																	}
																	iBytes_Datum++;
																}
											case 2	: asMaske_Datum += "99"; ucStelle_DatumZeit += 2; break;
											case 3	: (asMaske_Datum.Length() && asMaske_Datum[asMaske_Datum.Length() - 1] == 0x27 ? asMaske_Datum.ShortRightOne() : asMaske_Datum += "'");
																(!pCalendar->stDate.wDayOfWeek ? GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, CAL_SABBREVDAYNAME7, pc5KurzText, 3, nullptr)
																	: GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, CAL_SABBREVDAYNAME1 + pCalendar->stDate.wDayOfWeek - 1, pc5KurzText, 3, nullptr));
																pc5KurzText[2] = 0x27; pc5KurzText[3] = 0x00; asMaske_Datum += pc5KurzText;
																ucStelle_DatumZeit += 2; bNewMask = true; break;
											case 4	: (asMaske_Datum.Length() && asMaske_Datum[asMaske_Datum.Length() - 1] == 0x27 ? asMaske_Datum.ShortRightOne() : asMaske_Datum += "'");
																(!pCalendar->stDate.wDayOfWeek ? dwKalenderInfo = CAL_SDAYNAME7 : dwKalenderInfo = CAL_SDAYNAME1 + pCalendar->stDate.wDayOfWeek - 1);
																iBytes = GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, dwKalenderInfo, nullptr, NULL, nullptr);
																vbText = VMBlock(iBytes + 1); vbText[iBytes - 1] = 0x27; vbText[iBytes] = 0x00;
																GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, dwKalenderInfo, vbText, --iBytes, nullptr);
																asMaske_Datum += vbText; ucStelle_DatumZeit += iBytes; VMFrei(vbText);
																bNewMask = true; break;
										} break;
				case 0x4d	: ucMaske = 1; while((asFormat_Date)[++ucStelle_Maske] == 0x4d && ucStelle_Maske < ucLange) ucMaske++;
										switch(ucMaske){
											case 1	: if(pCalendar->stDate.wMonth < 10){
																	if(!vbDatum_A){
																		vbDatum_A = VMBlock(iBytes_Datum + 1); vbDatum_A[iBytes_Datum] = 0;
																		vbDatum_A[ucStelle_DatumZeit] = 0x20;
																		MemCopy(vbDatum_A, vbDatum, ucStelle_DatumZeit);
																		MemCopy(&vbDatum_A[ucStelle_DatumZeit + 1], &vbDatum[ucStelle_DatumZeit], iBytes_Datum - ucStelle_DatumZeit - 1);
																		VMFrei(vbDatum); vbDatum = nullptr;
																	}
																	else{
																		vbDatum = VMBlock(iBytes_Datum + 1); vbDatum[iBytes_Datum] = 0;
																		vbDatum[ucStelle_DatumZeit] = 0x20;
																		MemCopy(vbDatum, vbDatum_A, ucStelle_DatumZeit);
																		MemCopy(&vbDatum[ucStelle_DatumZeit + 1], &vbDatum_A[ucStelle_DatumZeit], iBytes_Datum - ucStelle_DatumZeit - 1);
																		VMFrei(vbDatum_A); vbDatum_A = nullptr;
																	}
																	iBytes_Datum++;
																}
											case 2	: asMaske_Datum += "99"; ucStelle_DatumZeit += 2; break;
											case 3	: (asMaske_Datum.Length() && asMaske_Datum[asMaske_Datum.Length() - 1] == 0x27 ? asMaske_Datum.ShortRightOne() : asMaske_Datum += "'");
																GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, CAL_SABBREVMONTHNAME1 + pCalendar->stDate.wMonth - 1, pc5KurzText, 4, nullptr);
																pc5KurzText[3] = 0x27; pc5KurzText[4] = 0x00; asMaske_Datum += pc5KurzText;
																ucStelle_DatumZeit += 3; bNewMask = true; break;
											case 4	: (asMaske_Datum.Length() && asMaske_Datum[asMaske_Datum.Length() - 1] == 0x27 ? asMaske_Datum.ShortRightOne() : asMaske_Datum += "'");
																dwKalenderInfo = CAL_SMONTHNAME1 + pCalendar->stDate.wMonth;
																iBytes = GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, --dwKalenderInfo, nullptr, NULL, nullptr);
																vbText = VMBlock(iBytes + 1); vbText[iBytes - 1] = 0x27; vbText[iBytes] = 0x00;
																GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, dwKalenderInfo, vbText, --iBytes, nullptr);
																asMaske_Datum += vbText; ucStelle_DatumZeit += iBytes; VMFrei(vbText);
																bNewMask = true; break;
										} break;
				case 0x79	: ucMaske = 1; while((asFormat_Date)[++ucStelle_Maske] == 0x79 && ucStelle_Maske < ucLange) ucMaske++;
										switch(ucMaske){
											case 1	: wJahr = pCalendar->stDate.wYear; wJahr <<= 12; wJahr >>= 12;
																if(wJahr < 10){
																	if(!vbDatum_A){
																		vbDatum_A = VMBlock(iBytes_Datum + 1); vbDatum_A[iBytes_Datum] = 0;
																		vbDatum_A[ucStelle_DatumZeit] = 0x20;
																		MemCopy(vbDatum_A, vbDatum, ucStelle_DatumZeit);
																		MemCopy(&vbDatum_A[ucStelle_DatumZeit + 1], &vbDatum[ucStelle_DatumZeit], iBytes_Datum - ucStelle_DatumZeit - 1);
																		VMFrei(vbDatum); vbDatum = NULL;
																	}
																	else{
																		vbDatum = VMBlock(iBytes_Datum + 1); vbDatum[iBytes_Datum] = 0;
																		vbDatum[ucStelle_DatumZeit] = 0x20;
																		MemCopy(vbDatum, vbDatum_A, ucStelle_DatumZeit);
																		MemCopy(&vbDatum[ucStelle_DatumZeit + 1], &vbDatum_A[ucStelle_DatumZeit], iBytes_Datum - ucStelle_DatumZeit - 1);
																		VMFrei(vbDatum_A); vbDatum_A = NULL;
																	}
																	iBytes_Datum++;
																}
											case 2	: asMaske_Datum += "99"; ucStelle_DatumZeit += 2; break;
											case 4	: asMaske_Datum += "9999"; ucStelle_DatumZeit += 4; break;
										}
										break;
				case 0x27	: (asMaske_Datum.Length() && asMaske_Datum[asMaske_Datum.Length() - 1] == 0x27 ? asMaske_Datum.ShortRightOne() : asMaske_Datum += "'");
										while((asFormat_Date)[++ucStelle_Maske] != 0x27 && ucStelle_Maske < ucLange){
											*c2Buchstabe = (asFormat_Date)[ucStelle_Maske];
											asMaske_Datum += c2Buchstabe;
											ucStelle_DatumZeit++;
										}
										asMaske_Datum += "'"; ucStelle_Maske++;
										break;
			}
		}
		while(ucStelle_Maske < ucLange);

		if(iBytes_Zeit){
			ucStelle_Maske = 0, ucStelle_DatumZeit = 0, ucLange = asFormat_Time.Length(); COStringA asMaske_Zeit;
			do{
				switch((asFormat_Time)[ucStelle_Maske]){
					case 0x68	: ucMaske = 1; while((asFormat_Time)[++ucStelle_Maske] == 0x68 && ucStelle_Maske < ucLange) ucMaske++;
											switch(ucMaske){
												case 1	: if(pCalendar->stDate.wHour < 10){
																		if(!vbZeit_A){
																			vbZeit_A = VMBlock(iBytes_Zeit + 1); vbZeit_A[iBytes_Zeit] = 0;
																			vbZeit_A[ucStelle_DatumZeit] = 0x20;
																			MemCopy(vbZeit_A, vbZeit, ucStelle_DatumZeit);
																			MemCopy(&vbZeit_A[ucStelle_DatumZeit + 1], &vbZeit[ucStelle_DatumZeit], iBytes_Zeit - ucStelle_DatumZeit - 1);
																			VMFrei(vbZeit); vbZeit = nullptr;
																		}
																		else{
																			vbZeit = VMBlock(iBytes_Zeit + 1); vbZeit[iBytes_Zeit] = 0;
																			vbZeit[ucStelle_DatumZeit] = 0x20;
																			MemCopy(vbZeit, vbZeit_A, ucStelle_DatumZeit);
																			MemCopy(&vbZeit[ucStelle_DatumZeit + 1], &vbZeit_A[ucStelle_DatumZeit], iBytes_Zeit - ucStelle_DatumZeit - 1);
																			VMFrei(vbZeit_A); vbZeit_A = nullptr;
																		}
																		iBytes_Zeit++;
																	}
												case 2	: asMaske_Zeit += "99"; ucStelle_DatumZeit += 2; break;
											} break;
					case 0x48	: ucMaske = 1; while((asFormat_Time)[++ucStelle_Maske] == 0x48 && ucStelle_Maske < ucLange) ucMaske++;
											switch(ucMaske){
												case 1	: if(pCalendar->stDate.wHour < 10){
																		if(!vbZeit_A){
																			vbZeit_A = VMBlock(iBytes_Zeit + 1); vbZeit_A[iBytes_Zeit] = 0;
																			vbZeit_A[ucStelle_DatumZeit] = 0x20;
																			MemCopy(vbZeit_A, vbZeit, ucStelle_DatumZeit);
																			MemCopy(&vbZeit_A[ucStelle_DatumZeit + 1], &vbZeit[ucStelle_DatumZeit], iBytes_Zeit - ucStelle_DatumZeit - 1);
																			VMFrei(vbZeit); vbZeit = nullptr;
																		}
																		else{
																			vbZeit = VMBlock(iBytes_Zeit + 1); vbZeit[iBytes_Zeit] = 0;
																			vbZeit[ucStelle_DatumZeit] = 0x20;
																			MemCopy(vbZeit, vbZeit_A, ucStelle_DatumZeit);
																			MemCopy(&vbZeit[ucStelle_DatumZeit + 1], &vbZeit_A[ucStelle_DatumZeit], iBytes_Zeit - ucStelle_DatumZeit - 1);
																			VMFrei(vbZeit_A); vbZeit_A = nullptr;
																		}
																		iBytes_Zeit++;
																	}
												case 2	: asMaske_Zeit += "99"; ucStelle_DatumZeit += 2; break;
											} break;
					case 0x6d	: ucMaske = 1; while((asFormat_Time)[++ucStelle_Maske] == 0x6d && ucStelle_Maske < ucLange) ucMaske++;
											switch(ucMaske){
												case 1: if(pCalendar->stDate.wMinute < 10){
																	if(!vbZeit_A){
																		vbZeit_A = VMBlock(iBytes_Zeit + 1); vbZeit_A[iBytes_Zeit] = 0;
																		vbZeit_A[ucStelle_DatumZeit] = 0x20;
																		MemCopy(vbZeit_A, vbZeit, ucStelle_DatumZeit);
																		MemCopy(&vbZeit_A[ucStelle_DatumZeit + 1], &vbZeit[ucStelle_DatumZeit], iBytes_Zeit - ucStelle_DatumZeit - 1);
																		VMFrei(vbZeit); vbZeit = nullptr;
																	}
																	else{
																		vbZeit = VMBlock(iBytes_Zeit + 1); vbZeit[iBytes_Zeit] = 0;
																		vbZeit[ucStelle_DatumZeit] = 0x20;
																		MemCopy(vbZeit, vbZeit_A, ucStelle_DatumZeit);
																		MemCopy(&vbZeit[ucStelle_DatumZeit + 1], &vbZeit_A[ucStelle_DatumZeit], iBytes_Zeit - ucStelle_DatumZeit - 1);
																		VMFrei(vbZeit_A); vbZeit_A = nullptr;
																	}
																	iBytes_Zeit++;
																}
												case 2	: asMaske_Zeit += "99"; ucStelle_DatumZeit += 2; break;
											}
											break;
					case 0x73	: ucMaske = 1; while((asFormat_Time)[++ucStelle_Maske] == 0x73 && ucStelle_Maske < ucLange) ucMaske++;
											switch(ucMaske){
												case 1	: if(pCalendar->stDate.wSecond < 10){
																		if(!vbZeit_A){
																			vbZeit_A = VMBlock(iBytes_Zeit + 1); vbZeit_A[iBytes_Zeit] = 0;
																			vbZeit_A[ucStelle_DatumZeit] = 0x20;
																			MemCopy(vbZeit_A, vbZeit, ucStelle_DatumZeit);
																			MemCopy(&vbZeit_A[ucStelle_DatumZeit + 1], &vbZeit[ucStelle_DatumZeit], iBytes_Zeit - ucStelle_DatumZeit - 1);
																			VMFrei(vbZeit); vbZeit = nullptr;
																		}
																		else{
																			vbZeit = VMBlock(iBytes_Zeit + 1); vbZeit[iBytes_Zeit] = 0;
																			vbZeit[ucStelle_DatumZeit] = 0x20;
																			MemCopy(vbZeit, vbZeit_A, ucStelle_DatumZeit);
																			MemCopy(&vbZeit[ucStelle_DatumZeit + 1], &vbZeit_A[ucStelle_DatumZeit], iBytes_Zeit - ucStelle_DatumZeit - 1);
																			VMFrei(vbZeit_A); vbZeit_A = nullptr;
																		}
																		iBytes_Zeit++;
																	}
												case 2	: asMaske_Zeit += "99"; ucStelle_DatumZeit += 2; break;
											}
											break;
					case 0x74	: ucMaske = 1; while((asFormat_Time)[++ucStelle_Maske] == 0x74 && ucStelle_Maske < ucLange) ucMaske++;
											switch(ucMaske){
												case 1	: (asMaske_Zeit.Length() && asMaske_Zeit[asMaske_Zeit.Length() - 1] == 0x74 ? asMaske_Zeit.ShortRightOne() : asMaske_Zeit += "'");
																	if(pCalendar->stDate.wHour < 12) asMaske_Zeit += "A'";
																	else asMaske_Zeit += "P'";
																	ucStelle_DatumZeit++; bNewMask = true; break;
												case 2	: (asMaske_Zeit.Length() && asMaske_Zeit[asMaske_Zeit.Length() - 1] == 0x74 ? asMaske_Zeit.ShortRightOne() : asMaske_Zeit += "'");
																	if(pCalendar->stDate.wHour < 12) asMaske_Zeit += "AM'";
																	else asMaske_Zeit += "PM'";
																	ucStelle_DatumZeit += 2; bNewMask = true; break;
											}
											break;
					case 0x27	: (asMaske_Zeit.Length() && asMaske_Zeit[asMaske_Zeit.Length() - 1] == 0x27 ? asMaske_Zeit.ShortRightOne() : asMaske_Zeit += "'");
											while((asFormat_Time)[++ucStelle_Maske] != 0x27 && ucStelle_Maske < ucLange){
												*c2Buchstabe = (asFormat_Time)[ucStelle_Maske];
												asMaske_Zeit += c2Buchstabe;
												ucStelle_DatumZeit++;
											}
											asMaske_Zeit += "'"; ucStelle_Maske++;
											break;
				}
			}
			while(ucStelle_Maske < ucLange);

			if(pCalendar->bOrder_DateTime){ asMaske_Datum += asMaske_Zeit;	CharacterMask(asMaske_Datum.c_Str()); }
			else{ asMaske_Zeit += asMaske_Datum; CharacterMask(asMaske_Zeit.c_Str()); }
		}
		else CharacterMask(asMaske_Datum.c_Str());
	}

	if(!iBytes_Zeit) (vbDatum_A ? *vasContent = vbDatum_A, VMFrei(vbDatum_A) : *vasContent = vbDatum, VMFrei(vbDatum));
	else{
		//COStringA asDatumZeit;
		if(pCalendar->bOrder_DateTime){
			(vbDatum_A ? *vasContent = vbDatum_A, VMFrei(vbDatum_A) : *vasContent = vbDatum, VMFrei(vbDatum));
			(vbZeit_A ? *vasContent += vbZeit_A, VMFrei(vbZeit_A) : *vasContent += vbZeit, VMFrei(vbZeit));
		}
		else{
			(vbZeit_A ? *vasContent = vbZeit_A, VMFrei(vbZeit_A) : *vasContent = vbZeit, VMFrei(vbZeit));
			(vbDatum_A ? *vasContent += vbDatum_A, VMFrei(vbDatum_A) : *vasContent += vbDatum, VMFrei(vbDatum));
		//Text(asDatumZeit.c_Str());
		}
	}
	if(hWndElement){
		rclDirty.left = 0; rclDirty.right = lWidth - pCalendar->lHeight_EditDate;
		OnRender(false);
		ifDXGISwapChain4->Present1(0, NULL, &dxgiPresent);
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetDayOfWeek(void)
{
	DWORD dwCalendarInfo = CAL_SABBREVDAYNAME1; WCHAR pwcWeekday_1[3]; pwcWeekday_1[2] = 0; WCHAR pwcWeekday_2[3]; pwcWeekday_2[2] = 0;
	GetDateFormatEx(LOCALE_NAME_USER_DEFAULT, DATE_USE_ALT_CALENDAR, &stDate, L"ddd", pwcWeekday_1, 3, nullptr);
	for(BYTE ucTag = 0; ucTag < 7; ucTag++){
		GetCalendarInfoEx(LOCALE_NAME_USER_DEFAULT, CAL_GREGORIAN, nullptr, dwCalendarInfo++, pwcWeekday_2, 3, nullptr);
		if(!wcscmp(pwcWeekday_1, pwcWeekday_2)){ ucTag == 6 ? stDate.wDayOfWeek = 0 : stDate.wDayOfWeek = ucTag + 1; break; }
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::CODate::COEditDate::TextToSYSTEMTIME(void)
{
	BYTE ucStelle_DatumZeit = 0;
	SYSTEMTIME stSystemTime = pCalendar->stDate;

	if(pCalendar->bOrder_DateTime){
		if(DateTextToSYSTEMTIME(stSystemTime, ucStelle_DatumZeit)){
			if(asFormat_Time.Length()){
				if(!TimeTextToSYSTEMTIME(stSystemTime, ucStelle_DatumZeit)) return false;
			}
			pCalendar->stDate = stSystemTime; pCalendar->SetDayOfWeek(); return true;
		}
	}
	else{
		if(asFormat_Time.Length()){
			if(!TimeTextToSYSTEMTIME(stSystemTime, ucStelle_DatumZeit)) return false;
		}
		if(DateTextToSYSTEMTIME(stSystemTime, ucStelle_DatumZeit)){ pCalendar->stDate = stSystemTime; pCalendar->SetDayOfWeek(); return true; }
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::CODate::COEditDate::DateTextToSYSTEMTIME(SYSTEMTIME& stSystemTime, unsigned char& ucStelle_DatumZeit)
{
	BYTE ucMaske, ucStelle_Maske = 0, ucLange = asFormat_Date.Length(); COStringA asZahl;
	do{
		switch((asFormat_Date)[ucStelle_Maske]){
			case 0x64	: ucMaske = 1; while((asFormat_Date)[++ucStelle_Maske] == 0x64 && ucStelle_Maske < ucLange) ucMaske++;
									switch(ucMaske){
										case 1	:
										case 2	: vasContent->SubString(&asZahl, ucStelle_DatumZeit + 1, ucStelle_DatumZeit + 2);
															if(asZahl[0] == 0x20 || asZahl[0] == 0x5f){ asZahl[0] = 0x30; SendMessage(hWndElement, WM_CHAR, 0x30, NULL); }
															if(asZahl[1] == 0x20 || asZahl[1] == 0x5f) return false;
															asZahl.USHORT(stSystemTime.wDay); ucStelle_DatumZeit += 2; break;
										case 3	: ucStelle_DatumZeit += 2; break;
										case 4	: (!pCalendar->stDate.wDayOfWeek ? ucStelle_DatumZeit += GetCalendarInfoEx(LOCALE_NAME_USER_DEFAULT, CAL_GREGORIAN, nullptr, CAL_SDAYNAME7, nullptr, NULL, nullptr) - 1
																: ucStelle_DatumZeit += GetCalendarInfoEx(LOCALE_NAME_USER_DEFAULT, CAL_GREGORIAN, nullptr, CAL_SDAYNAME1 + pCalendar->stDate.wDayOfWeek - 1, nullptr, NULL, nullptr) - 1);
															break;
									} break;
			case 0x4d	: ucMaske = 1; while((asFormat_Date)[++ucStelle_Maske] == 0x4d && ucStelle_Maske < ucLange) ucMaske++;
									switch(ucMaske){
										case 1	:
										case 2	: vasContent->SubString(&asZahl, ucStelle_DatumZeit + 1, ucStelle_DatumZeit + 2);
															if(asZahl[0] == 0x20 || asZahl[0] == 0x5f){ asZahl[0] = 0x30; SendMessage(hWndElement, WM_CHAR, 0x30, NULL); }
															if(asZahl[1] == 0x20 || asZahl[1] == 0x5f) return false;
															asZahl.USHORT(stSystemTime.wMonth); ucStelle_DatumZeit += 2; break;
										case 3	: ucStelle_DatumZeit += 3; break;
										case 4: ucStelle_DatumZeit += GetCalendarInfoEx(LOCALE_NAME_USER_DEFAULT, CAL_GREGORIAN, nullptr, CAL_SMONTHNAME1 + pCalendar->stDate.wMonth - 1, nullptr, NULL, nullptr) - 1;
															break;
									} break;
			case 0x79	: ucMaske = 1; while((asFormat_Date)[++ucStelle_Maske] == 0x79 && ucStelle_Maske < ucLange) ucMaske++;
									switch(ucMaske){
										case 1	:
										case 2	: vasContent->SubString(&asZahl, ucStelle_DatumZeit + 1, ucStelle_DatumZeit + 2);
															if(asZahl[0] == 0x20 || asZahl[0] == 0x5f){ asZahl[0] = 0x30; SendMessage(hWndElement, WM_CHAR, 0x30, NULL); }
															if(asZahl[1] == 0x20 || asZahl[1] == 0x5f) return false;
															asZahl.USHORT(stSystemTime.wYear);
															stSystemTime.wYear += 2000; ucStelle_DatumZeit += 2; break;
										case 4	: vasContent->SubString(&asZahl, ucStelle_DatumZeit + 1, ucStelle_DatumZeit + 4);
															ucMaske = 1;
															do{
																if(asZahl[ucMaske] == 0x20 || asZahl[ucMaske] == 0x5f) return false;
															}
															while(++ucMaske < 4);
															asZahl.USHORT(stSystemTime.wYear); ucStelle_DatumZeit += 4; break;
									}
									break;
			case 0x27	: while((asFormat_Date)[++ucStelle_Maske] != 0x27 && ucStelle_Maske < ucLange) ucStelle_DatumZeit++;
									ucStelle_Maske++; break;
		}
	}
	while(ucStelle_Maske < ucLange);

	size_t szBytes_Text; WCHAR wc50Format_Date[50];
	mbstowcs_s(&szBytes_Text, wc50Format_Date, 50, asFormat_Date.c_Str(), asFormat_Date.Length());
	if(!GetDateFormatEx(LOCALE_NAME_USER_DEFAULT, DATE_USE_ALT_CALENDAR, &stSystemTime, wc50Format_Date, nullptr, NULL, nullptr)) return false;
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::CODate::COEditDate::TimeTextToSYSTEMTIME(SYSTEMTIME& stSystemTime, unsigned char& ucStelle_DatumZeit)
{
	BYTE ucMaske, ucStelle_Maske = 0, ucLange = asFormat_Time.Length(); COStringA asZahl;
	do{
		switch((asFormat_Time)[ucStelle_Maske]){
			case 0x68	: ucMaske = 1; while((asFormat_Time)[++ucStelle_Maske] == 0x68 && ucStelle_Maske < ucLange) ucMaske++;
									switch(ucMaske){
										case 1:
										case 2: vasContent->SubString(&asZahl, ucStelle_DatumZeit + 1, ucStelle_DatumZeit + 2);
											if(asZahl[1] == 0x20 || asZahl[1] == 0x5f) return false;
											asZahl.USHORT(stSystemTime.wHour); ucStelle_DatumZeit += 2; break;
									} break;
			case 0x48	: ucMaske = 1; while((asFormat_Time)[++ucStelle_Maske] == 0x48 && ucStelle_Maske < ucLange) ucMaske++;
									switch(ucMaske){
										case 1:
										case 2: vasContent->SubString(&asZahl, ucStelle_DatumZeit + 1, ucStelle_DatumZeit + 2);
											if(asZahl[1] == 0x20 || asZahl[1] == 0x5f) return false;
											asZahl.USHORT(stSystemTime.wHour); ucStelle_DatumZeit += 2; break;
									} break;
			case 0x6d	: ucMaske = 1; while((asFormat_Time)[++ucStelle_Maske] == 0x6d && ucStelle_Maske < ucLange) ucMaske++;
									switch(ucMaske){
										case 1:
										case 2: vasContent->SubString(&asZahl, ucStelle_DatumZeit + 1, ucStelle_DatumZeit + 2);
											if(asZahl[1] == 0x20 || asZahl[1] == 0x5f) return false;
											asZahl.USHORT(stSystemTime.wMinute); ucStelle_DatumZeit += 2; break;
									}
									break;
			case 0x73	: ucMaske = 1; while((asFormat_Time)[++ucStelle_Maske] == 0x73 && ucStelle_Maske < ucLange) ucMaske++;
									switch(ucMaske){
										case 1:
										case 2: vasContent->SubString(&asZahl, ucStelle_DatumZeit + 1, ucStelle_DatumZeit + 2);
											if(asZahl[1] == 0x20 || asZahl[1] == 0x5f) return false;
											asZahl.USHORT(stSystemTime.wSecond); ucStelle_DatumZeit += 2; break;
									}
									break;
			case 0x74	: ucMaske = 1; while((asFormat_Time)[++ucStelle_Maske] == 0x74 && ucStelle_Maske < ucLange) ucMaske++;
									switch(ucMaske){
										case 1:
										case 2: ucStelle_DatumZeit += 2; break;
									}
									break;
			case 0x27	: while((asFormat_Time)[++ucStelle_Maske] != 0x27 && ucStelle_Maske < ucLange) ucStelle_DatumZeit++;
									ucStelle_Maske++; break;
		}
	}
	while(ucStelle_Maske < ucLange);
	if(!GetTimeFormat(LOCALE_USER_DEFAULT, NULL, &stSystemTime, asFormat_Time.c_Str(), NULL, NULL)) return false;
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::SetDate(COTime* pzDate)
{
	ThreadSafe_Begin();
	if(pzDate){ stDate = pzDate->SystemTime(); SetDayOfWeek(); edEditDate->DateTimeText(); }
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::CODate::SetDate(STTime& stZeit)
{
	ThreadSafe_Begin();
	SYSTEMTIME stSystemTime;
	stSystemTime.wYear = stZeit.usJahr; stSystemTime.wMonth = stZeit.ucMonat; stSystemTime.wDay = stZeit.ulTag; stSystemTime.wHour = stZeit.ucStunde;
	stSystemTime.wMinute = stZeit.ucMinute; stSystemTime.wSecond = stZeit.ucSekunde; stSystemTime.wMilliseconds = stZeit.usMillisekunde;
	if(GetDateFormatEx(LOCALE_NAME_USER_DEFAULT, DATE_USE_ALT_CALENDAR, &stSystemTime, nullptr, nullptr, NULL, nullptr)){
		stDate = stSystemTime; SetDayOfWeek();
		edEditDate->DateTimeText();
		ThreadSafe_End();
		return true;
	}
	ThreadSafe_End();
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::Today(void)
{
	ThreadSafe_Begin();
	GetLocalTime(&stDate); stDate.wHour = 0; stDate.wMinute = 0; stDate.wSecond = 0; stDate.wMilliseconds = 0;
	SetDayOfWeek();
	edEditDate->DateTimeText();
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::Tomorrow(void)
{
	ThreadSafe_Begin();
	FILETIME stFileTime; ULARGE_INTEGER unZeit;
	GetLocalTime(&stDate); stDate.wHour = 0; stDate.wMinute = 0; stDate.wSecond = 0; stDate.wMilliseconds = 0;

	SystemTimeToFileTime(&stDate, &stFileTime);
	unZeit.HighPart = stFileTime.dwHighDateTime;
	unZeit.LowPart = stFileTime.dwLowDateTime;
	unZeit.QuadPart += 864000000000;
	stFileTime.dwHighDateTime = unZeit.HighPart;
	stFileTime.dwLowDateTime = unZeit.LowPart;
	FileTimeToSystemTime(&stFileTime, &stDate);

	SetDayOfWeek();
	edEditDate->DateTimeText();
	ThreadSafe_End();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::Read(char* pcInhalt)
{
	if(pcInhalt){
		FILETIME stFZeit;
		ThreadSafe_Begin();
		SystemTimeToFileTime(&stDate, &stFZeit);
		MemCopy(pcInhalt, &stFZeit.dwHighDateTime, 4); MemCopy(&pcInhalt[4], &stFZeit.dwLowDateTime, 4);
		ThreadSafe_End();
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::DirectX::CODate::Write(const char* pcInhalt)
{
	if(pcInhalt){
		FILETIME stFZeit = {0};
		ThreadSafe_Begin();
		MemCopy(&stFZeit.dwHighDateTime, pcInhalt, 4); MemCopy(&stFZeit.dwLowDateTime, &pcInhalt[4], 4);
		if(FileTimeToSystemTime(&stFZeit, &stDate)){ SetDayOfWeek(); edEditDate->DateTimeText(); }
		else{ MessageBox(hWndElement, pcIncorrectDateTimeText, pcIncorrectDate, MB_ICONERROR); edEditDate->DateTimeText(); }
		ThreadSafe_End();
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::DirectX::CODate::GetDate(COStringA* pasDatum)
{
	ThreadSafe_Begin();
	if(pasDatum) edEditDate->Content(pasDatum);
	ThreadSafe_End();
	return pasDatum;
}
//---------------------------------------------------------------------------------------------------------------------------------------
COTime* __vectorcall RePag::DirectX::CODate::GetTime(COTime* pzDatum)
{
	ThreadSafe_Begin();
	if(pzDatum) *pzDatum = stDate;
	ThreadSafe_End();
	return pzDatum;
}
//---------------------------------------------------------------------------------------------------------------------------------------
SYSTEMTIME __vectorcall RePag::DirectX::CODate::GetSystemTime(void)
{
	ThreadSafe_Begin();
	SYSTEMTIME stDatumA = stDate;
	ThreadSafe_End();
	return stDatumA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::DirectX::CODate::GetTextPoint(_In_ char* pcText, _In_ unsigned long ulTextLength, _Out_ D2D_SIZE_F& szfTextPoint)
{
	IDWriteTextLayout* ifTextLayout; DWRITE_TEXT_METRICS stTextMetrics;
	size_t szBytes_Text; WCHAR wcInhalt[255];
	if(mbstowcs_s(&szBytes_Text, wcInhalt, 255, pcText, ulTextLength)) return false;
	if(pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wcInhalt, (UINT)szBytes_Text, ifText, 100.0f, 100.0f, &ifTextLayout)) return false;
	ifTextLayout->GetMetrics(&stTextMetrics);
	SafeRelease(&ifTextLayout);

	szfTextPoint.width = stTextMetrics.width;
	szfTextPoint.height = stTextMetrics.height;
	return true;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

