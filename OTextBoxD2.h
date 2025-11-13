/******************************************************************************
MIT License

Copyright(c) 2025 René Pagel

Filename: OTextBoxD2.h
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
#include "OEditLineD2.h"
#include "OScrollBarD2.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace DirectX
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COTextBox : public COEditLine
		{
			friend LRESULT CALLBACK WndProc_TextBox(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);

			private:
				COScrollBar* sbHorizontal;
				COScrollBar* sbVertical;
				void __vectorcall CreateText(void);
				void __vectorcall OnRender(void);
				void __vectorcall ChangeSizeVisibleScrollBars(void);

			protected:
				COList* vliText;
				void __vectorcall WM_Create(void);
				void __vectorcall WM_Size(_In_ LPARAM lParam);
				void __vectorcall WM_VScroll(_In_ WPARAM wParam);
				void __vectorcall WM_HScroll(_In_ WPARAM wParam);
				void __vectorcall WM_LButtonDown(void);
				void __vectorcall WM_MouseWheel(_In_ WPARAM wParam, _In_ LPARAM lParam);
				void __vectorcall GetScrollBar(_In_ BYTE ucBar, _In_ STScrollInfo& stScrollInfo);
				void __vectorcall GetTextPoint(_In_ char* pcText, _In_ unsigned long ulTextLength, _Out_ D2D_SIZE_F& szfTextPoint);
				inline long& __vectorcall FloatToLong(_In_ float fZahl, _Out_ long& lZahl);
				void __vectorcall DeSelect(void);
				void __vectorcall COTextBoxV(_In_ VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																		 _In_ STDeviceResources* pstDeviceResourcesA); // Note: three numbers uiIDElement, because COScrollBars !!!

			public:
				void __vectorcall COTextBoxV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																		 _In_ STDeviceResources* pstDeviceResources);
				VMEMORY __vectorcall COFreiV(void);
				void __vectorcall Text(_In_ char* pcText);
				void __vectorcall Text_NeueZeile(_In_ char* pcText);
				unsigned long __vectorcall Zeilenanzahl(void);
				void __vectorcall Scroll_Anfang(void);
				void __vectorcall Scroll_Ende(void);
				void __vectorcall Scroll_Zeile(_In_ bool bAbwarts);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COTextBox* __vectorcall COTextBoxV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources);
		__declspec(dllexport) COTextBox* __vectorcall COTextBoxV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																														 _In_ STDeviceResources* pstDeviceResources);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}

