/******************************************************************************
MIT License

Copyright(c) 2025 René Pagel

Filename: OScrollBarD2.h
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
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace DirectX
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		typedef struct STScrollInfo
		{
			float fMax;
			float fPage;
			float fPos;
			BYTE ucMask;
			D2D_SIZE_F szfCharacter;
		} STScrollInfo;
		constexpr BYTE SBI_MAX = 1;
		constexpr BYTE SBI_PAGE = 2;
		constexpr BYTE SBI_POS = 4;
		constexpr BYTE SBI_CHARACTER_WIDTH = 8;
		constexpr BYTE SBI_CHARACTER_HEIGHT = 16;
		constexpr BYTE SBI_ALL = 31;
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COScrollBar : public COGraphic
		{

			friend LRESULT CALLBACK WndProc_ScrollBar(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);

			private:
				bool bHorizontal;
				STScrollInfo siScrollInfo;
				BYTE ucDirty;
				bool bMouseTracking;
				TRACKMOUSEEVENT stTrackMouseEvent;
				ID2D1RectangleGeometry* ifButton_Up;
				ID2D1RectangleGeometry* ifButton_Down;
				ID2D1SolidColorBrush* ifButtonColor_Up;
				ID2D1SolidColorBrush* ifButtonColor_Down;
				D2D1_COLOR_F crfButton;
				D2D1_COLOR_F crfButton_Click;
				D2D1_COLOR_F crfButton_Move;
				ID2D1PathGeometry* ifArrow_Up;
				ID2D1PathGeometry* ifArrow_Down;
				ID2D1SolidColorBrush* ifArrowColor_Up;
				ID2D1SolidColorBrush* ifArrowColor_Down;
				D2D1_COLOR_F crfArrow;
				D2D1_COLOR_F crfArrow_Click;
				D2D1_COLOR_F crfArrow_Move;
				ID2D1RoundedRectangleGeometry* ifThumb;
				ID2D1SolidColorBrush* ifThumbColor;
				D2D1_COLOR_F crfThumb;
				D2D1_COLOR_F crfThumb_Click;
				D2D1_COLOR_F crfThumb_Move;
				D2D1_RECT_F rcfThumb;
				float fScaleArrowThumb;
				float fThumb;
				float fStep;
				void __vectorcall Geometry(void);
				void __vectorcall CreateThumb(_In_ bool bRender);

			protected:
				void __vectorcall OnRender(void);
				void __vectorcall WM_Create(void);
				void __vectorcall WM_MouseMove(_In_ WPARAM wParam, _In_ LPARAM lParam);
				void __vectorcall WM_MouseOver(_In_ WPARAM wParam, _In_ LPARAM lParam);
				void __vectorcall WM_MouseLeave(void);
				void __vectorcall WM_LButtonDown(_In_ WPARAM wParam, _In_ LPARAM lParam);
				void __vectorcall WM_LButtonUp(_In_ WPARAM wParam, _In_ LPARAM lParam);
				void __vectorcall WM_VScroll(_In_ WPARAM wParam);
				void __vectorcall WM_HScroll(_In_ WPARAM wParam);
				void __vectorcall COScrollBarV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName,
																			 _In_ unsigned int uiIDElementA,	_In_ STDeviceResources* pstDeviceResourcesA, bool bHorizontalA);

			public:
				void __vectorcall COScrollBarV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																			 _In_ STDeviceResources* pstDeviceResources, bool bHorizontalA);
				VMEMORY __vectorcall COFreiV(void);
				void __vectorcall GetScrollInfo(_In_ STScrollInfo& siScrollInfoA);
				void __vectorcall SetScrollInfo(_In_ STScrollInfo& siScrollInfoA);
				void __vectorcall NewSize(_In_ long lHeightA, _In_ long lWidthA);
				void __vectorcall NewHeight(_In_ long lHeightA);
				void __vectorcall NewWidth(_In_ long lWidth);
				void __vectorcall ScaleArrowThumb(_In_ float fScale);
				void __vectorcall SetButtonColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
				void __vectorcall SetButtonColor(_In_ D2D1_COLOR_F& crfButtonA);
				void __vectorcall SetButtonColor_Move(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
				void __vectorcall SetButtonColor_Move(_In_ D2D1_COLOR_F& crfButton_MoveA);
				void __vectorcall SetButtonColor_Click(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
				void __vectorcall SetButtonColor_Click(_In_ D2D1_COLOR_F& crfButton_ClickA);
				void __vectorcall SetArrowColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
				void __vectorcall SetArrowColor(_In_ D2D1_COLOR_F& crfArrowA);
				void __vectorcall SetArrowColor_Move(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
				void __vectorcall SetArrowColor_Move(_In_ D2D1_COLOR_F& crfArrow_MoveA);
				void __vectorcall SetArrowColor_Click(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
				void __vectorcall SetArrowColor_Click(_In_ D2D1_COLOR_F& crfArrow_ClickA);
				void __vectorcall SetThumbColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
				void __vectorcall SetThumbColor(_In_ D2D1_COLOR_F& crfThumbA);
				void __vectorcall SetThumbColor_Move(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
				void __vectorcall SetThumbColor_Move(_In_ D2D1_COLOR_F& crfThumb_MoveA);
				void __vectorcall SetThumbColor_Click(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
				void __vectorcall SetThumbColor_Click(_In_ D2D1_COLOR_F& crfThumb_ClickA);
				void __vectorcall OnPaint(void);
		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COScrollBar* __vectorcall COScrollBarV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																 _In_ STDeviceResources* pstDeviceResources, bool bHorizontal);
		__declspec(dllexport) COScrollBar* __vectorcall COScrollBarV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																 _In_ STDeviceResources* pstDeviceResources, bool bHorizontal);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}