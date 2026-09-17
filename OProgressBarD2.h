/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OProgressBarD2.h
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
		class __declspec(dllexport) COProgressBar : public COGraphic
		{
			friend LRESULT CALLBACK WndProc_ProgressBar(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);

			private:
				unsigned long ulPosition_max;
				unsigned long ulPosition_min;
				unsigned long ulPosition;
				double dStep;
				D2D1_COLOR_F crfFillColor;
				ID2D1SolidColorBrush* ifFillColor;
				ID2D1RectangleGeometry* ifProgressBar;
				void __vectorcall WM_Create(void);
				void __vectorcall WM_Size(_In_ LPARAM lParam);
				void __vectorcall WM_LButtonDown(_In_ WPARAM wParam, _In_ LPARAM lParam);
				void __vectorcall WM_MouseMove(_In_ WPARAM wParam, _In_ LPARAM lParam);
				void __vectorcall OnRender(void);

			protected:

			public:
				void __vectorcall COProgressBarV(_In_ VMEMORY vmSpeicher, _In_z_ const char* pcFensterName, _In_ unsigned int uiIDElementA, _In_ STDeviceResources* pstDeviceResources);
				VMEMORY __vectorcall COFreiV(void);
				void __vectorcall SetPosition_Max(_In_ unsigned long ulPosition);
				void __vectorcall SetPosition_Min(_In_ unsigned long ulPosition);
				void __vectorcall SetPosition(_In_ unsigned long ulPositionA);
				void __vectorcall FillColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall FillColor(_In_ D2D1_COLOR_F& crfTextA);
				void(__vectorcall* pfnWM_LButtonDown)(_In_ COProgressBar*, _In_ WPARAM, _In_ LPARAM);
				void(__vectorcall* pfnWM_MouseMove)(_In_ COProgressBar*, _In_ WPARAM, _In_ LPARAM);
				void __vectorcall OnPaint(void);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COProgressBar* __vectorcall COProgressBarV(_In_z_ const char* pcWindowsName, _In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources);
		__declspec(dllexport) COProgressBar* __vectorcall COProgressBarV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowsName, _In_ unsigned int uiIDElement,
																																		 _In_ STDeviceResources* pstDeviceResources);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}