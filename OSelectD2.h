/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OTextD2.h
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
#include "OTextD2.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace DirectX
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport)  COSelect : public COText
		{
			private:

			protected:
				bool bDoNotCopy;
				char cSelect;
				HANDLE heCaret;
				HANDLE htCaret;
				HMENU hMenu;
				BYTE ucCaretStrength;
				D2D_POINT_2F ptfCaret;
				D2D1_COLOR_F crfCaret;
				ID2D1SolidColorBrush* ifCaretColor;
				D2D1_COLOR_F crfSelectBack;
				ID2D1SolidColorBrush* ifSelectBackColor;
				D2D1_COLOR_F crfSelectText;
				unsigned long ulCharacterPos;
				unsigned char ucCharacterSpecification;
				bool __vectorcall CharacterCheck(_In_ WPARAM wParam);
				bool __vectorcall GetTextPoint(_In_ char* pcText, _In_ unsigned long ulTextLength, _Out_ D2D_SIZE_F& szfTextPoint);
				inline long __vectorcall FloatToLong(_In_ float fNumber);
				void __vectorcall COSelectV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName,
																	_In_ unsigned int uiIDElementA, _In_ STDeviceResources* pstDeviceResourcesA);

			public:
				void __vectorcall COSelectV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																	_In_ STDeviceResources* pstDeviceResourcesA);
				void __vectorcall SetSelectTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetSelectTextColor(_In_ D2D1_COLOR_F& crfSelectTextA);
				void __vectorcall SetSelectBackgroundColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetSelectBackgroundColor(_In_ D2D1_COLOR_F& crfSelectBackA);
				void __vectorcall SetCaretColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetCaretColor(_In_ D2D1_COLOR_F& crfCaretA);
				void __vectorcall CaretStrength(_In_ BYTE ucCaretStrengthA);
				void __vectorcall DoNotCopy(_In_ bool bDoNotCopyA);

		};
	}
}