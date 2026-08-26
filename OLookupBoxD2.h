/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OLookupBoxD2.h
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
#include "OListBoxD2.h"
#include "OButtonD2.h"
namespace RePag
{
	namespace DirectX
	{
		class __declspec(dllexport) COLookupBox : public COListBox
		{
			friend LRESULT CALLBACK WndProc_LookupBox(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);
			friend LRESULT CALLBACK WndProc_Entry(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);

			private:
				class COEntry : public COButton
				{
					friend LRESULT CALLBACK WndProc_Entry(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);

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
						COLookupBox* pLookupBox;
						void __vectorcall WM_Create(void);
						void __vectorcall WM_LButtonUp(_In_ WPARAM wParam, _In_ LPARAM lParam);
						void __vectorcall WM_LButtonDown(_In_ LPARAM lParam);
						void __vectorcall WM_MouseMove(_In_ LPARAM lParam);
						void __vectorcall WM_MouseLeave(void);
						void __vectorcall WM_MouseOver(_In_ LPARAM lParam);
						void __vectorcall OnRender(void);

					protected:

					public:
						void __vectorcall COEntryV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																			 _In_ COLookupBox* pLookupBoxA, _In_ STDeviceResources* pstDeviceResourcesA);
						VMEMORY __vectorcall COFreiV(void);
						void __vectorcall Geometry(void);
						void __vectorcall OnPaint(void);
						void __vectorcall Text(_In_z_ char* pcText);
						void __vectorcall Lock(void);
						void __vectorcall Release(void);
						D2D1_COLOR_F crfButton;
						D2D1_COLOR_F crfButton_Move;
						D2D1_COLOR_F crfButton_Click;
						D2D1_COLOR_F crfArrow;
						D2D1_COLOR_F crfArrow_Move;
						D2D1_COLOR_F crfArrow_Click;
				}; COEntry* eEntry;
				unsigned char ucHeight_Entry;
				void __vectorcall WM_Create_LookupBox(_In_ HWND hWnd);
				void __vectorcall WM_LButtonUp_LookupBox(void);

			protected:

			public:
				void __vectorcall COLookupBoxV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																			 _In_ STDeviceResources* pstDeviceResourcesA);
																			 // Note: four numbers uiIDElement, because COScrollBars by COTextBox and COEntry !!!
				VMEMORY __vectorcall COFreiV(void);
				long& __vectorcall GetHeight(_Out_ long& lHeightA);
				RECT& __vectorcall GetWindow(_Out_ RECT& rcWindow);
				void __vectorcall NewWindowPosition(_In_ long lPos_x, _In_ long lPos_y);
				void __vectorcall NewWindowPosition(_In_ POINT& ptPositionA);
				void __vectorcall NewWindow(_In_ long lHeightA, _In_ long lWidthA, _In_ long lPos_x, _In_ long lPos_y);
				void __vectorcall NewWindowSize(_In_ long lHeightA, _In_ long lWidthA);
				void __vectorcall NewWindowHeight(_In_ long lHeightA);
				void __vectorcall NewWindowWidth(_In_ long lWidthA);
				void __vectorcall ChangeWindowSize(_In_ long lHeightA, _In_ long lWidthA);
				void __vectorcall ChangeWindowPosition(_In_ long lPos_x, _In_ long lPos_y);
				void __vectorcall SetFont(_In_ STFont& stFontA);
				void __vectorcall SetHeight_Entry(_In_ unsigned char ucHohe);
				void __vectorcall SetTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetBackgroundColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
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
				void __vectorcall Lock(void);
				void __vectorcall Release(void);
				void __vectorcall Text(_In_z_ char* pcText);
				bool __vectorcall SetSelectIndex(_In_ unsigned char ucIndex);
				bool __vectorcall SearchAndSetEntry(_In_ COStringA* vasEnum, _In_ unsigned char& ucIndexA);

		};
		__declspec(dllexport) COLookupBox* __vectorcall COLookupBoxV(const char* pcWindowName, unsigned int uiIDElement,
																																 _In_ STDeviceResources* pstDeviceResources);
		__declspec(dllexport) COLookupBox* __vectorcall COLookupBoxV(VMEMORY vmMemory, const char* pcWindowName, unsigned int uiIDElement,
																																 _In_ STDeviceResources* pstDeviceResources);
	}
}
