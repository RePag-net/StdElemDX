/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OPasswortD2.h
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
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace DirectX
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COPassword : public COEditLine
		{
			friend LRESULT CALLBACK WndProc_Password(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);
			friend void CALLBACK Timer_Caret_Password(_In_ void* pvParam, _In_ bool bTimerOrWaitFired);

			private:
			COStringA* vasPassword;
			void __vectorcall WM_SetFocus(void);
			void __vectorcall WM_Char(_In_ WPARAM wParam);
			void __vectorcall WM_LButtonDown(_In_ LPARAM lParam);
			void __vectorcall OnRender(_In_ bool bCaret);

			protected:

			public:
			void __vectorcall COPasswordV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																		_In_ STDeviceResources* pstDeviceResources);
			VMEMORY __vectorcall COFreiV(void);
			void __vectorcall Text(_In_z_ const char* pcText);

		};
		//---------------------------------------------------------------------------
		__declspec(dllexport) COPassword* __vectorcall COPasswordV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																															 _In_ STDeviceResources* pstDeviceResources);
		__declspec(dllexport) COPassword* __vectorcall COPasswordV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																															 _In_ STDeviceResources* pstDeviceResources);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}