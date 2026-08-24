/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OSwitchD2.h
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
#include "OButtonD2.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace DirectX
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		 class __declspec(dllexport) COSwitch : public COButton
		{
			friend LRESULT CALLBACK WndProc_Switch(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);

			private:
				bool bOn;
				void __vectorcall WM_LButtonUp(_In_ WPARAM wParam, _In_ LPARAM lParam);

			protected:
				COStringA asLabel_On;
				COStringA asLabel_Off;
				void __vectorcall Switch(_In_ bool bOn);

				void __vectorcall COSwitchV(_In_ VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																		_In_ STDeviceResources* pstDeviceResourcesA);

			public:
				void __vectorcall COSwitchV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																		_In_ STDeviceResources* pstDeviceResources);
				void(__vectorcall* pfnWM_LButtonDown)(COSwitch*);
				void(__vectorcall* pfnWM_LButtonUp)(COSwitch*);
				bool __vectorcall IsOn(void);
				void __vectorcall SetSwitch(_In_ bool bOn);
				void __vectorcall Label(_In_z_ const char* pcLabel);
				void __vectorcall Label_On(_In_z_ const char* pcLabel);
				void __vectorcall Label_Off(_In_z_ const char* pcLabel);
		
		};
//---------------------------------------------------------------------------------------------------------------------------------------
__declspec(dllexport) COSwitch* __vectorcall COSwitchV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources);
__declspec(dllexport) COSwitch* __vectorcall COSwitchV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																											 _In_ STDeviceResources* pstDeviceResources);
//---------------------------------------------------------------------------------------------------------------------------------------
	}
}