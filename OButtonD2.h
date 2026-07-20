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
#include "OTextLineD2.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace DirectX
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COButton : public COTextLine
		{
			friend LRESULT CALLBACK WndProc_Button(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);

			private:
			D2D1_COLOR_F crfText_1;
			ID2D1SolidColorBrush* ifTextColor_1;
			D2D1_COLOR_F crfText_Focus;
			ID2D1SolidColorBrush* ifTextColor_Focus;

			protected:
			bool bActive;
			D2D1_COLOR_F crfBackground_1;
			ID2D1SolidColorBrush* ifBackgroundColor_1;
			D2D1_COLOR_F crfBackground_2;
			ID2D1SolidColorBrush* ifBackgroundColor_2;
			D2D1_COLOR_F crfBackground_3;
			ID2D1SolidColorBrush* ifBackgroundColor_3;
			void __vectorcall WM_Create(void);
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
			void __vectorcall SetTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
			void __vectorcall SetTextColor(_In_ D2D1_COLOR_F& crfTextA);
			void __vectorcall SetTextColor_Focus(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
			void __vectorcall SetTextColor_Focus(_In_ D2D1_COLOR_F& crfTextA);
			void __vectorcall SetBackgroundColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
			void __vectorcall SetBackgroundColor(_In_ D2D1_COLOR_F& crfBackgroundA);
			void __vectorcall SetBackgroundColor_1(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
			void __vectorcall SetBackgroundColor_1(_In_ D2D1_COLOR_F& crfBackgroundA);
			void __vectorcall SetBackgroundColor_2(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
			void __vectorcall SetBackgroundColor_2(_In_ D2D1_COLOR_F& crfBackgroundA);
			void __vectorcall SetBackgroundColor_3(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
			void __vectorcall SetBackgroundColor_3(_In_ D2D1_COLOR_F& crfBackgroundA);
			void __vectorcall Label(const char* pcLabel);
			void __vectorcall SetActive(_In_ bool bActiveA);
			bool __vectorcall Active(void);
			void __vectorcall Lock(void);
			void __vectorcall Release(void);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COButton* __vectorcall COButtonV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources);
		__declspec(dllexport) COButton* __vectorcall COButtonV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																													 _In_ STDeviceResources* pstDeviceResources);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}