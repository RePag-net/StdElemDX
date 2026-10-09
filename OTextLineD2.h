/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OTextLineD2.h
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
#include "OSelectD2.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace DirectX
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COTextLine : public COSelect
		{
				friend LRESULT CALLBACK WndProc_TextLine(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

			private:			

			protected:
				float fTextPos;
				unsigned long ulSelectPos;
				D2D1_RECT_F rcfSelect;
				void __vectorcall OnRender(_In_ bool bCaret);
				void __vectorcall WM_Create(void);
				void __vectorcall WM_Size(_In_ LPARAM lParam);
				void __vectorcall WM_SetFocus(void);
				void __vectorcall WM_KeyDown(_In_ WPARAM wParam, _In_ LPARAM lParam);
				void __vectorcall WM_LButtonDown(_In_ WPARAM wParam, _In_ LPARAM lParam);
				void __vectorcall WM_ContexMenu(_In_ LPARAM lParam);
				void __vectorcall WM_MouseMove(_In_ WPARAM wParam, _In_ LPARAM lParam);
				void __vectorcall CharacterMetric(void);
				void __vectorcall DeSelect(void);
				void __vectorcall DeleteCaretPos(void);
				void __vectorcall SelectText_Left(void);
				void __vectorcall SelectText_Right(void);
				void __vectorcall COTextLineV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName,
																			_In_ unsigned int uiIDElementA,	_In_ STDeviceResources* pstDeviceResourcesA);

			public:
				void __vectorcall COTextLineV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																			_In_ STDeviceResources* pstDeviceResourcesA);
				VMEMORY __vectorcall COFreiV(void);
				void __vectorcall Text(_In_z_ char* pcText);
				COStringA* __vectorcall Content(_Out_ COStringA* vasContentA);
				void __vectorcall OnPaint(void);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COTextLine* __vectorcall COTextLineV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources);
		__declspec(dllexport) COTextLine* __vectorcall COTextLineV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																															 _In_ STDeviceResources* pstDeviceResourcesA);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
