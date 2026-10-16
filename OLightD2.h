/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OLightD2.h
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
		class __declspec(dllexport) COLight : public COGraphic
		{
				friend LRESULT CALLBACK WndProc_Light(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);
				friend void CALLBACK Timer_Light(_In_ void* pvParam, _In_ bool bTimerOrWaitFired);

			private:
				HANDLE heBlink;
				bool bLight_On;
				float fEllipseRadius_X;
				float fEllipseRadius_Y;
				bool bRadialGradient;
				D2D1_POINT_2F ptfRadialGradientCenter;
				D2D1_POINT_2F ptfRadialGradientOriginOffset;
				float fRadialGradientRadius_X;
				float fRadialGradientRadius_Y;
				D2D1_COLOR_F crfLight_1;
				D2D1_COLOR_F crfLight_2;
				ID2D1SolidColorBrush* ifLight_1;
				ID2D1SolidColorBrush* ifLight_2;
				D2D1_ELLIPSE ellipse;
				ID2D1EllipseGeometry* ifEllipseLight;
				void __vectorcall OnRender(_In_ bool bBlink);
				void __vectorcall WM_Create(void);
				void __vectorcall WM_Size(_In_ LPARAM lParam);

			protected:

			public:
				void __vectorcall COLightV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																	 _In_ STDeviceResources* pstDeviceResources);
				VMEMORY __vectorcall COFreiV(void);
				void __vectorcall Lightcolor_1(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall Lightcolor_1(_In_ D2D1_COLOR_F& crfLightColor);
				void __vectorcall Lightcolor_2(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall Lightcolor_2(_In_ D2D1_COLOR_F& crfLightColor);
				void __vectorcall EllipseRadius(_In_ float fRadius_X, _In_ float fRadius_Y);
				void __vectorcall RadialGradientCenter(_In_ float fCenter_X, _In_ float fCenter_Y);
				void __vectorcall RadialGradientCenter(_In_ D2D1_POINT_2F ptfCenter);
				void __vectorcall RadialGradientOriginOffset(_In_ float fOffset_X, _In_ float fOffset_Y);
				void __vectorcall RadialGradientOriginOffset(_In_ D2D1_POINT_2F ptfOffset);
				void __vectorcall RadialGradientRadius(_In_ float fRadiusA_X, _In_ float fRadiusA_Y);
				void __vectorcall RadialGradient(_In_ bool bRadialGradientA);
				void __vectorcall BlinkPeriod(_In_ unsigned long ulPeriod_msec);
				void __vectorcall Blink_Stop(void);
				void __vectorcall Blink_Go(void);
				void __vectorcall Light_On(_In_ bool bLight_OnA);
				void __vectorcall OnPaint(void);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COLight* __vectorcall COLightV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																												 _In_ STDeviceResources* pstDeviceResources);
		__declspec(dllexport) COLight* __vectorcall COLightV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																												 _In_ STDeviceResources* pstDeviceResources);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}