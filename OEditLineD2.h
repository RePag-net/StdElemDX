/******************************************************************************
MIT License

Copyright(c) 2025 René Pagel

Filename: OEditLineD2.h
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
		class __declspec(dllexport) COEditLine : public COTextLine
		{
			friend LRESULT CALLBACK WndProc_EditLine(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);
			friend void CALLBACK Timer_Caret(void* pvParam, bool bTimerOrWaitFired);

		private:
			COStringA* vasZeichenMaske;
			bool __vectorcall ZeichenMaske_Einfugen(WPARAM wParam);
			bool __vectorcall ZeichenMaske_Einfugen_Prufen(WPARAM wParam, VMBLOCK vbZeichen_Maske);
			bool __vectorcall ZeichenMaske_Loschen(void);
			bool __vectorcall ZeichenMaske_FestRechts(void);
			bool __vectorcall ZeichenMaske_FestLinks(void);
			void __vectorcall DeSelect(void);
			void __vectorcall Select_Loschen();
			void __vectorcall ScrollRight(SIZE& stZeichengrosse);
			void __vectorcall ScrollLeft(SIZE& stZeichengrosse);

		protected:
			HMENU hMenu;
			POINT ptlCaret;
			HANDLE htCaret;
			HANDLE heCaret;
			char cSelect;
			RECT rclSelect;
			RECT rclScroll;
			POINT ptlScrollOffset;
			long lTextPos;
			unsigned long ulZeichen_max;
			unsigned char ucZeichenVorgabe;
			unsigned long ulZeichenPos;
			unsigned long ulSelectPos;
			D2D1_COLOR_F crfSelectText;
			D2D1_COLOR_F crfSelectBack;
			D2D1_COLOR_F crfCaret;
			ID2D1SolidColorBrush* ifSelectBackColor;
			ID2D1SolidColorBrush* ifCaretColor;
			//STRGBFarbe stSelectSchriftfarbe;
			//STRGBFarbe stSelectHintergrundfarbe;
			void __vectorcall OnRender(_In_ bool bCaret);
			void __vectorcall OnPaint(void);
			void __vectorcall WM_Create(void);
			void __vectorcall WM_SetFocus(void);
			void __vectorcall WM_KillFocus(void);
			void __vectorcall WM_KeyDown(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_Char(WPARAM wParam);
			bool __vectorcall WM_Command(WPARAM wParam);
			void __vectorcall WM_ContexMenu(LPARAM lParam);
			void __vectorcall WM_LButtonUp(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_LButtonDown(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_MouseMove(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_LButtonDBClick(WPARAM wParam, LPARAM lParam);
			bool __vectorcall ZeichenVorgabe(WPARAM wParam);
			void __vectorcall GetTextPoint(_In_ char* pcText, _In_ unsigned long ulTextLength, _Out_ SIZE& stTextPoint);
			void __vectorcall DeleteCaretPos(void);
			void __vectorcall SetCaretColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
			void __vectorcall SetCaretColor(_In_ D2D1_COLOR_F& crfCaretA);
			void __vectorcall COEditLineV(_In_ const VMEMORY vmSpeicher, _In_z_ const char* pcKlassenName, _In_z_ const char* pcFensterName, _In_ unsigned int uiIDElementA,
																		_In_ STDeviceResources* pstDeviceResourcesA);

		public:
			void __vectorcall COEditLineV(_In_ const VMEMORY vmSpeicher, _In_z_ const char* pcFensterName, _In_ unsigned int uiIDElementA,
																		_In_ STDeviceResources* pstDeviceResourcesA);
			VMEMORY __vectorcall COFreiV(void);
			void(__vectorcall* pfnWM_Char_Return)(COEditLine*);
			void(__vectorcall* pfnWM_Char_Escape)(COEditLine*);
			void(__vectorcall* pfnWM_KillFocus)(COEditLine*);
			bool(__vectorcall* pfnWM_Command)(COEditLine*, WPARAM);
			void(__vectorcall* pfnWM_LButtonDown)(COEditLine*, WPARAM, LPARAM);
			void(__vectorcall* pfnWM_LButtonUp)(COEditLine*, WPARAM, LPARAM);
			void(__vectorcall* pfnWM_LButtonDBClick)(COEditLine*, WPARAM, LPARAM);
			void __vectorcall SetzVerfugbar(bool bVerfugbar);
			void __vectorcall Text(char* pcText);
			void __vectorcall SetSelectTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
			void __vectorcall SetSelectTextColor(_In_ D2D1_COLOR_F& crfSelectTextA);
			void __vectorcall SetSelectBackgroundColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
			void __vectorcall SetSelectBackgroundColor(_In_ D2D1_COLOR_F& crfSelectBackA);
			void __vectorcall SetzZeichen_Max(unsigned long ulZeichen);
			unsigned long __vectorcall Zeichen_Max(void);
			void __vectorcall Zeichenvorgabe(unsigned char ucZeichenvorgabeA);
			void __vectorcall Zeichenmaske(const char* pcZeichenmaske);
			COStringA* __vectorcall Zeichenmaske(COStringA* pasZeichenmaske);
			void __vectorcall SelectAlles(void);
			void __vectorcall SelectEntfernen(void);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COEditLine* __vectorcall COEditLineV(_In_ const char* pcFensterName, _In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources);
		__declspec(dllexport) COEditLine* __vectorcall COEditLineV(_In_ const VMEMORY vmSpeicher, _In_ const char* pcFensterName, _In_ unsigned int uiIDElement,
																															 _In_ STDeviceResources* pstDeviceResources);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
