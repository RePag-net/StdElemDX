/******************************************************************************
MIT License

Copyright(c) 2025 René Pagel

Filename: OListBox.h
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
		class __declspec(dllexport) COListBox : public COTextBox
		{
			friend LRESULT CALLBACK WndProc_ListBox(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);

		private:
			unsigned char ucIndex;

		protected:
			void __vectorcall WM_LButtonUp(_In_ LPARAM lParam);
			void __vectorcall WM_VScroll(_In_ WPARAM wParam);
			void __vectorcall WM_HScroll(_In_ WPARAM wParam);
			void __vectorcall WM_KeyDown(_In_ WPARAM wParam);
			void __vectorcall WM_Char(_In_ WPARAM wParam);
			void __vectorcall DeSelect(void);
			void __vectorcall COListBoxV(_In_ VMEMORY vmSpeicher, _In_z_ const char* pcKlassenName, _In_z_ const char* pcFensterName, _In_ unsigned int uiIDElementA,
																	 _In_ STDeviceResources* pstDeviceResourcesA); // Note: three numbers uiIDElement, because COScrollBars by COTextBox!!!

		public:
			void __vectorcall COListBoxV(_In_ VMEMORY vmSpeicher, _In_z_ const char* pcFensterName, _In_ unsigned int uiIDElementA, _In_ STDeviceResources* pstDeviceResourcesA);
			VMEMORY __vectorcall COFreiV(void);
			void(__vectorcall* pfnWM_LButtonDown)(_In_ COListBox*);
			void(__vectorcall* pfnWM_LButtonUp)(_In_ COListBox*);
			void(__vectorcall* pfnWM_Char_Return)(_In_ COListBox*);
			void(__vectorcall* pfnWM_Char_Escape)(_In_ COListBox*);
			void(__vectorcall* pfnWM_KillFocus)(_In_ COListBox*);
			bool(__vectorcall* pfnWM_Command)(_In_ COListBox*, _In_ WPARAM);
			bool __vectorcall SetSelectIndex(_In_ unsigned char ucIndexA);
			unsigned char __vectorcall GetSelectIndex(void);
			COStringA* __vectorcall SelectEnum(_Out_ COStringA* vasEintrag);
			COStringA* __vectorcall SetAndSearchEnum(_In_ unsigned char ucIndexA, _Out_ COStringA* vasEnum);
			bool __vectorcall SearchEnum(_In_ COStringA* vasEnum, _Out_ unsigned char& ucIndexA);
			bool __vectorcall SearchAndSetEnum(_In_ COStringA* vasEnum, _Out_ unsigned char& ucIndexA);
			unsigned long __vectorcall NumberEnum(void);
			void __vectorcall DeSelectEnum(void);
			void __vectorcall Test(void);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COListBox* __vectorcall COListBoxV(_In_z_ const char* pcFensterName, _In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources);
		__declspec(dllexport) COListBox* __vectorcall COListBoxV(_In_ VMEMORY vmSpeicher, _In_z_ const char* pcFensterName, _In_ unsigned int uiIDElement,
																														 _In_ STDeviceResources* pstDeviceResources);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
