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
			long lMin;
			long lMax;
			ULONG ulPage;
			long  lPos;
		} STScrollInfo;
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COScrollBar : public COGraphic
		{

			friend LRESULT CALLBACK WndProc_ScrollBar(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

			private:
				bool bHorizontal;
				STScrollInfo stScrollInfo;
				BYTE ucDirty;
				bool bMouseTracking;
				TRACKMOUSEEVENT stTrackMouseEvent;
				ID2D1RectangleGeometry* ifButton_Up;
				ID2D1RectangleGeometry* ifButton_Down;
				D2D1_COLOR_F crfButton;
				D2D1_COLOR_F crfButton_Click;
				D2D1_COLOR_F crfButton_Move;
				ID2D1SolidColorBrush* ifButtonColor;
				ID2D1PathGeometry* ifArrow_Up;
				ID2D1PathGeometry* ifArrow_Down;
				D2D1_COLOR_F crfArrow;
				D2D1_COLOR_F crfArrow_Click;
				D2D1_COLOR_F crfArrow_Move;
				ID2D1SolidColorBrush* ifArrowColor;
				FLOAT fScaleArrowThumb;
				ID2D1RoundedRectangleGeometry* ifThumb;
				D2D1_COLOR_F crfThumb;
				D2D1_COLOR_F crfThumb_Click;
				D2D1_COLOR_F crfThumb_Move;
				ID2D1SolidColorBrush* ifThumbColor;
				D2D1_RECT_F rcfThumb;
				float fThumbSize;
				void __vectorcall CreateThumb(bool bRender);

			protected:
				void __vectorcall OnRender(void);
				void __vectorcall OnPaint(void);
				void __vectorcall WM_Create(void);
				void __vectorcall WM_MouseMove(WPARAM wParam, LPARAM lParam);
				void __vectorcall WM_MouseOver(WPARAM wParam, LPARAM lParam);
				void __vectorcall WM_MouseLeave(void);
				void __vectorcall WM_LButtonDown(WPARAM wParam, LPARAM lParam);
				void __vectorcall WM_LButtonUp(WPARAM wParam, LPARAM lParam);
				void __vectorcall COScrollBarV(_In_ const VMEMORY vmSpeicher, _In_z_ const char* pcKlassenName, _In_z_ const char* pcFensterName,
																			 _In_ unsigned int uiIDElementA,	_In_ STDeviceResources* pstDeviceResourcesA, bool bHorizontalA);

			public:
				void __vectorcall COScrollBarV(_In_ VMEMORY vmSpeicher, _In_z_ const char* pcFensterName, _In_ unsigned int uiIDElementA,
																			 _In_ STDeviceResources* pstDeviceResources, bool bHorizontalA);
				VMEMORY __vectorcall COFreiV(void);
				void __vectorcall GetScrollInfo(_In_ STScrollInfo& stScrollInfoA);
				void __vectorcall SetScrollInfo(_In_ STScrollInfo& stScrollInfoA);
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
		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COScrollBar* __vectorcall COScrollBarV(_In_z_ const char* pcFensterName, _In_ unsigned int uiIDElement,
																																 _In_ STDeviceResources* pstDeviceResources, bool bHorizontal);
		__declspec(dllexport) COScrollBar* __vectorcall COScrollBarV(_In_ const VMEMORY vmSpeicher, _In_z_ const char* pcFensterName, _In_ unsigned int uiIDElement,
																																 _In_ STDeviceResources* pstDeviceResources, bool bHorizontal);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}