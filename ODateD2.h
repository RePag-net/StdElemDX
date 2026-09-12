/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: ODateD2.h
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
#pragma once
#include "OEditLineD2.h"
#include "OListBoxD2.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace DirectX
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) CODate : public COGraphic
		{
			friend LRESULT CALLBACK WndProc_Date(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);
			friend LRESULT CALLBACK WndProc_EditDate(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);
			friend void CALLBACK Timer_Caret_EditDate(_In_ void* pvParam, _In_ bool bTimerOrWaitFired);

			private:
			class COEditDate : public COEditLine
			{
				friend LRESULT CALLBACK WndProc_EditDate(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);
				friend void CALLBACK Timer_Caret_EditDate(_In_ void* pvParam, _In_ bool bTimerOrWaitFired);

				private:
					float fButtonSize;
					float fButton_left;
					float fButton_top;
					BYTE ucDirty;
					bool bMouseTracking;
					TRACKMOUSEEVENT stTrackMouseEvent;
					ID2D1RectangleGeometry* ifButton;
					ID2D1SolidColorBrush* ifButtonColor;
					ID2D1PathGeometry* ifArrow;
					ID2D1SolidColorBrush* ifArrowColor;
					CODate* pCalendar;
					void __vectorcall WM_Create(void);
					void __vectorcall WM_SetFocus(void);
					void __vectorcall WM_KillFocus(void);
					bool __vectorcall WM_Command_EditDate(_In_ WPARAM wParam);
					void __vectorcall WM_KeyDown(_In_ WPARAM wParam, _In_ LPARAM lParam);
					void __vectorcall WM_Char(_In_ WPARAM wParam);
					void __vectorcall WM_LButtonUp(_In_ WPARAM wParam, _In_ LPARAM lParam);
					void __vectorcall WM_LButtonDown(_In_ WPARAM wParam, _In_ LPARAM lParam);
					void __vectorcall WM_LButtonDBClick(_In_ LPARAM lParam);
					void __vectorcall WM_MouseMove(_In_ LPARAM wParam, _In_ WPARAM lParam);
					void __vectorcall WM_MouseLeave(void);
					void __vectorcall WM_MouseOver(_In_ LPARAM lParam);
					void __vectorcall OnRender(_In_ bool bCaret);
					void __vectorcall DeleteCaretPos(void);
					void __vectorcall DeSelect(void);
					bool __vectorcall DateTextToSYSTEMTIME(_In_ SYSTEMTIME& stSystemTime, _In_ unsigned char& ucPosition_DateTime);
					bool __vectorcall TimeTextToSYSTEMTIME(_In_ SYSTEMTIME& stSystemTime, _In_ unsigned char& ucPosition_DateTime);
					bool __vectorcall TextToSYSTEMTIME(void);

				protected:

				public:
					void __vectorcall COEditDateV(_In_ VMEMORY vmSpeicher, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA, _In_ CODate* pDateA,
																				_In_ STDeviceResources* pstDeviceResources);
					// Note: five numbers uiIDElement, because COEditDate and COListBox !!!
					VMEMORY __vectorcall COFreiV(void);
					void __vectorcall OnPaint(void);
					void __vectorcall DateTimeText(void);
					COStringA asFormat_Date;
					COStringA asFormat_Time;
					bool bNewMask;
					D2D1_COLOR_F crfButton;
					D2D1_COLOR_F crfButton_Move;
					D2D1_COLOR_F crfButton_Click;
					D2D1_COLOR_F crfArrow;
					D2D1_COLOR_F crfArrow_Move;
					D2D1_COLOR_F crfArrow_Click;

				}; COEditDate* edEditDate;
				COListBox* lbMonth;
				SYSTEMTIME stDate;
				IDWriteTextFormat* ifText;
				D2D1_COLOR_F crfText;
				ID2D1SolidColorBrush* ifTextColor;
				D2D1_COLOR_F crfHead_Background;
				ID2D1SolidColorBrush* ifHead_Background;
				ID2D1RectangleGeometry* ifHead;
				BYTE ucDirty;
				bool bMouseTracking;
				TRACKMOUSEEVENT stTrackMouseEvent;
				D2D1_COLOR_F crfArrow;
				D2D1_COLOR_F crfArrow_Move;
				D2D1_COLOR_F crfArrow_Click;
				ID2D1PathGeometry* ifArrow_left;
				ID2D1PathGeometry* ifArrow_right;
				ID2D1SolidColorBrush* ifArrowColor_left;
				ID2D1SolidColorBrush* ifArrowColor_right;
				bool bOrder_DateTime;
				long lWidth_Calendar;
				long lHeight_EditDate;
				long lYear_Pos_x;
				float fColumnWidth;
				float fRowHeight;
				unsigned char ucFirstDayOfWeek;
				unsigned char ucLastDay;
				void __vectorcall WM_Create(void);
				void __vectorcall WM_MouseMove(_In_ WPARAM wParam, _In_ LPARAM lParam);
				void __vectorcall WM_MouseOver(_In_ LPARAM lParam);
				void __vectorcall WM_MouseLeave(void);
				void __vectorcall WM_KillFocus(void);
				bool __vectorcall WM_Command(_In_ WPARAM wParam);
				bool __vectorcall WM_LButtonUp(_In_ LPARAM lParam);
				void __vectorcall SetDayOfWeek(void);
				void __vectorcall OnRender(void);
				void __vectorcall OnPaint(void);
				bool __vectorcall GetTextPoint(_In_ char* pcText, _In_ unsigned long ulTextLength, _Out_ D2D_SIZE_F& szfTextPoint);

			protected:

			public:
				void __vectorcall CODateV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																	 _In_ STDeviceResources* pstDeviceResources);
				VMEMORY __vectorcall COFreiV(void);
				long& __vectorcall GetHeight(_In_ long& lHeightA);
				long& __vectorcall GetHeight_EditDate(_In_ long& lHeightA);
				long& __vectorcall GetWidth_EditDate(_In_ long& lWidthA);
				RECT& __vectorcall GetWindow(_In_ RECT& rcWindow);
				void __vectorcall NewWindowPosition(_In_ long lPos_x, _In_ long lPos_y);
				void __vectorcall NewWindowPosition(_In_ POINT& ptPositionA);
				void __vectorcall NewWindow(_In_ long lHoheA, _In_ long lBreiteA, _In_ long lPos_x, _In_ long lPos_y);
				void __vectorcall NewWindowSize(_In_ long lHoheA, _In_ long lBreiteA);
				void __vectorcall NewWindowHeight(_In_ long lHoheA);
				void __vectorcall NewWindowWidth(_In_ long lBreiteA);
				void __vectorcall ChangeWindowSize(_In_ long lHoheA, _In_ long lBreiteA);
				void __vectorcall ChangeWindowPosition(_In_ long lPos_x, _In_ long lPos_y);
				void __vectorcall SetFont(_In_ STFont& stFont);
				void __vectorcall SetTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetBackgroundColor(_In_ unsigned char ucRot, _In_ unsigned char ucGrun, _In_ unsigned char ucBlau, _In_ float fAlpha);
				void __vectorcall SetHead_BackgroundColor(_In_ unsigned char ucRot, _In_ unsigned char ucGrun, _In_ unsigned char ucBlau, _In_ float fAlpha);
				void __vectorcall SetButtonColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetButtonColor(_In_ D2D1_COLOR_F& crfButtonA);
				void __vectorcall SetButtonColor_Move(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetButtonColor_Move(_In_ D2D1_COLOR_F& crfButton_MoveA);
				void __vectorcall SetButtonColor_Click(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetButtonColor_Click(_In_ D2D1_COLOR_F& crfButton_ClickA);
				void __vectorcall SetArrowColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetArrowColor(_In_ D2D1_COLOR_F& crfArrowA);
				void __vectorcall SetArrowColor_Move(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetArrowColor_Move(_In_ D2D1_COLOR_F& crfArrow_MoveA);
				void __vectorcall SetArrowColor_Click(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetArrowColor_Click(_In_ D2D1_COLOR_F& crfArrow_ClickA);
				void __vectorcall SetAvailable(_In_ bool bAvailable);
				void __vectorcall SetVisible(_In_ bool bVisible);
				bool __vectorcall Visible(void);
				void(__vectorcall* pfnWM_LButtonUp)(_In_ CODate*);
				void(__vectorcall* pfnWM_Char_Return)(_In_ CODate*);
				void(__vectorcall* pfnWM_Char_Escape)(_In_ CODate*);
				bool(__vectorcall* pfnWM_Command)(_In_ CODate*, WPARAM);
				void __vectorcall SetExtendedWidth_Calendar(_In_ long lWidthA);
				void __vectorcall SetHeight_EditDate(_In_ long lHeightA);
				void __vectorcall CharacterSpecification_EditDate_ZVVISIBLE(_In_ bool bVisible);
				void __vectorcall TextAlignment_EditDate(_In_ unsigned char ucTextAlignment);
				void __vectorcall SetDateTimeFormat(_In_ const char* pcFormat_Date, _In_ const char* pcFormat_Time, _In_ bool bPosition_DateTimeA);
				COStringA* __vectorcall GetDateformat(_In_ COStringA* pasFormat_Date);
				COStringA* __vectorcall GetTimeformat(_In_ COStringA* pasFormat_Time);
				void __vectorcall SetDate(_In_ COTime* pzDate);
				bool __vectorcall SetDate(_In_ STTime& stTime);
				void __vectorcall Today(void);
				void __vectorcall Tomorrow(void);
				void __vectorcall Read(_In_z_ char* pcInhalt);
				void __vectorcall Write(_In_z_ const char* pcInhalt);
				COStringA* __vectorcall GetDate(_In_ COStringA* pasDate);
				COTime* __vectorcall GetTime(_In_ COTime* ptDate);
				SYSTEMTIME __vectorcall GetSystemTime(void);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) CODate* __vectorcall CODateV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources);
		__declspec(dllexport) CODate* __vectorcall CODateV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																											 _In_ STDeviceResources* pstDeviceResources);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}