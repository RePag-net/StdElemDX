/******************************************************************************
MIT License

Copyright(c) 2025 René Pagel

Filename: OEditBox.h
For more information see https://github.com/RePag-net/StdElem

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
#include "OTextBoxD2.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace DirectX
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COEditBox : public COTextBox
		{
			friend LRESULT CALLBACK WndProc_EditBox(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);
			friend void CALLBACK Timer_Caret_EditBox(_In_ void* pvParam, _In_ bool bTimerOrWaitFired);

		private:
			long lSelectLine;
			long lLine;
			void* pvLine;
			void __vectorcall WM_SetFocus(void);
			void __vectorcall WM_VScroll(_In_ WPARAM wParam, _In_ LPARAM lParam);
			void __vectorcall WM_HScroll(_In_ WPARAM wParam);
			void __vectorcall WM_LButtonDown(_In_ LPARAM lParam);
			void __vectorcall WM_KeyDown(_In_ WPARAM wParam, _In_ LPARAM lParam);
			void __vectorcall WM_Char(_In_ WPARAM wParam);
			bool __vectorcall WM_Command(_In_ WPARAM wParam);
			void __vectorcall WM_ContexMenu(_In_ LPARAM lParam);
			void __vectorcall WM_MouseMove(_In_ WPARAM wParam, _In_ LPARAM lParam);
			void __vectorcall BreitesteZeile(_In_ HDC hdc);
			void __vectorcall Select_Loschen(void);

		protected:

		public:
			void __vectorcall COEditBoxV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																	 _In_ STDeviceResources* pstDeviceResources); // Note: three numbers uiIDElement, because COScrollBars by COTextBox!!!
			void(__vectorcall* pfnWM_Char_ShiftReturn)(_In_ COEditBox*);
			void(__vectorcall* pfnWM_Char_Escape)(_In_ COEditBox*);
			void(__vectorcall* pfnWM_KillFocus)(_In_ COEditBox*);
			bool(__vectorcall* pfnWM_Command)(_In_ COEditBox*, _In_ WPARAM);
			COStringA* __vectorcall Content(_Out_ COStringA* vasInhaltA);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COEditBox* __vectorcall COEditBoxV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources);
		__declspec(dllexport) COEditBox* __vectorcall COEditBoxV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																																						 _In_ STDeviceResources* pstDeviceResources);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
