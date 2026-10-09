/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

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
			friend LRESULT CALLBACK WndProc_EditLine(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);
			friend void CALLBACK Timer_Caret_EditLine(_In_ void* pvParam, _In_ bool bTimerOrWaitFired);

		private:
			bool __vectorcall CharacterMask_Insert_Check(_In_ WPARAM wParam, _In_ VMBLOCK vbCharacter_Mask);
			void __vectorcall Select_Loschen(void);

		protected:
			COStringA* vasCharacterMask;
			unsigned long ulCharacter_max;
			void __vectorcall WM_Create(void);
			void __vectorcall WM_Size(_In_ LPARAM lParam);
			void __vectorcall WM_SetFocus(void);
			void __vectorcall WM_KillFocus(void);
			void __vectorcall WM_KeyDown(_In_ WPARAM wParam, _In_ LPARAM lParam);
			void __vectorcall WM_Char(_In_ WPARAM wParam);
			bool __vectorcall WM_Command(_In_ WPARAM wParam);
			void __vectorcall WM_ContexMenu(_In_ LPARAM lParam);
			void __vectorcall WM_LButtonUp(_In_ WPARAM wParam, _In_ LPARAM lParam);
			void __vectorcall WM_LButtonDBClick(_In_ WPARAM wParam, _In_ LPARAM lParam);
			void __vectorcall WM_LButtonDown(_In_ WPARAM wParam, _In_ LPARAM lParam);
			bool __vectorcall CharacterMask_FixLeft(void);
			bool __vectorcall CharacterMask_FixRight(void);
			bool __vectorcall CharacterMask_Insert(_In_ WPARAM wParam);
			bool __vectorcall CharacterMask_Delete(void);
			void __vectorcall COEditLineV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																		_In_ STDeviceResources* pstDeviceResourcesA);

		public:
			void __vectorcall COEditLineV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																		_In_ STDeviceResources* pstDeviceResourcesA);
			VMEMORY __vectorcall COFreiV(void);
			void(__vectorcall* pfnWM_Char_Return)(_In_ COEditLine*);
			void(__vectorcall* pfnWM_Char_Escape)(_In_ COEditLine*);
			void(__vectorcall* pfnWM_KillFocus)(_In_ COEditLine*);
			bool(__vectorcall* pfnWM_Command)(_In_ COEditLine*, _In_  WPARAM);
			void(__vectorcall* pfnWM_LButtonDown)(_In_ COEditLine*, _In_ WPARAM, _In_ LPARAM);
			void(__vectorcall* pfnWM_LButtonUp)(_In_ COEditLine*, _In_ WPARAM, _In_ LPARAM);
			void(__vectorcall* pfnWM_LButtonDBClick)(_In_ COEditLine*, _In_ WPARAM, _In_ LPARAM);
			void __vectorcall SetzVerfugbar(_In_ bool bVerfugbar);
			void __vectorcall Text(_In_ char* pcText);
			void __vectorcall SetCharacter_Max(_In_ unsigned long ulCharacter);
			unsigned long __vectorcall Character_Max(void);
			void __vectorcall CharacterSpecification(_In_ unsigned char ucCharacterSpecificationA);
			void __vectorcall CharacterMask(_In_ const char* pcCharacterMask);
			COStringA* __vectorcall CharacterMask(_Out_ COStringA* pasCharacterMask);
			void __vectorcall SelectAlles(void);
			void __vectorcall SelectEntfernen(void);
			void __vectorcall OnPaint(void);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COEditLine* __vectorcall COEditLineV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources);
		__declspec(dllexport) COEditLine* __vectorcall COEditLineV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement,
																															 _In_ STDeviceResources* pstDeviceResources);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}