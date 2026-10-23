/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OButtonD2.h
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
#include "OTextD2.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace DirectX
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		#define _Button ((RePag::DirectX::COButton*)pvParam)
		constexpr BYTE  BUTTON_TRIGGER = 0;
		constexpr BYTE  BUTTON_BYPASS = 1;
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COButton : public COText
		{
				friend LRESULT CALLBACK WndProc_Button(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);
				friend void CALLBACK Timer_Button(_In_ void* pvParam, _In_ bool bTimerOrWaitFired);
				friend DWORD WINAPI thRender_Button(void* pvParam);

			private:
				HANDLE htTimer_Button;
				HANDLE he2Render[2];
				HANDLE hRenderThread;
				bool bThreadEnd;
				bool bBlink;
				D2D1_COLOR_F crfText_Focus;
				ID2D1RoundedRectangleGeometry* ifButtom;
				float fEdgeRadius_X;
				float fEdgeRadius_Y;
				bool bLinearGradient_LButton;
				ID2D1LinearGradientBrush* ifLinearGradientBrush;
				D2D1_POINT_2F ptfStartPoint;
				D2D1_POINT_2F ptfEndPoint;

			protected:
				bool bEnabled;
				bool bLinearGradient;
				D2D1_COLOR_F crfButtonColor_1_0;
				D2D1_COLOR_F crfButtonColor_1_1;
				D2D1_COLOR_F crfButtonColor_2_0;
				D2D1_COLOR_F crfButtonColor_2_1;
				D2D1_COLOR_F crfButtonColor_3;
				ID2D1SolidColorBrush* ifButtonColor;
				D2D1_COLOR_F crfGradientStopsColor_0;
				D2D1_COLOR_F crfGradientStopsColor_1;
				void __vectorcall OnRender(void);
				void __vectorcall WM_Create(void);
				void __vectorcall WM_Size(_In_ LPARAM lParam);
				void __vectorcall WM_SetFocus(void);
				void __vectorcall WM_KillFocus(void);
				void __vectorcall WM_Char(_In_ WPARAM wParam);
				void __vectorcall WM_LButtonDown(void);
				void __vectorcall WM_LButtonUp(_In_ WPARAM wParam, _In_ LPARAM lParam);
				void __vectorcall COButtonV(_In_ VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																		_In_ STDeviceResources* pstDeviceResources);

			public:
				void __vectorcall COButtonV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																		_In_ STDeviceResources* pstDeviceResources);
				VMEMORY __vectorcall COFreiV(void);
				void(__vectorcall* pfnWM_LButtonDown)(COButton*);
				void(__vectorcall* pfnWM_LButtonUp)(COButton*);
				void __vectorcall SetTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetTextColor(_In_ D2D1_COLOR_F& crfTextA);
				void __vectorcall SetTextColor_Focus(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetTextColor_Focus(_In_ D2D1_COLOR_F& crfTextA);
				void __vectorcall SetButtonColor_1_0(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetButtonColor_1_0(_In_ D2D1_COLOR_F& crfButtonColor);
				void __vectorcall SetButtonColor_1_1(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetButtonColor_1_1(_In_ D2D1_COLOR_F& crfButtonColor);
				void __vectorcall SetButtonColor_2_0(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetButtonColor_2_0(_In_ D2D1_COLOR_F& crfButtonColor);
				void __vectorcall SetButtonColor_2_1(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetButtonColor_2_1(_In_ D2D1_COLOR_F& crfButtonColor);
				void __vectorcall SetButtonColor_3(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetButtonColor_3(_In_ D2D1_COLOR_F& crfButtonColor);
				void __vectorcall LinearGradient(_In_ bool bLinearGradientA);
				void __vectorcall StartPoint(D2D1_POINT_2F ptfStartPointA);
				void __vectorcall EndPoint(D2D1_POINT_2F ptfEndPointA);
				void __vectorcall Label(const char* pcLabel);
				void __vectorcall Enable(_In_ bool bEnable);
				void __vectorcall Edge(_In_ float fRadius_X, _In_ float fRadius_Y);
				void __vectorcall BlinkPeriod(_In_ unsigned long ulPeriod_msec);
				void __vectorcall Blink_Stop(void);
				void __vectorcall Blink_Go(void);
				void __vectorcall OnPaint(void);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COButton* __vectorcall COButtonV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources);
		__declspec(dllexport) COButton* __vectorcall COButtonV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																													 _In_ STDeviceResources* pstDeviceResources);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}