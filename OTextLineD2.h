/******************************************************************************
MIT License

Copyright(c) 2025 René Pagel

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
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace DirectX
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		typedef struct STFont
		{
			const WCHAR* fontFamilyName;
			IDWriteFontCollection* fontCollection;
			DWRITE_FONT_WEIGHT fontWeight;
			DWRITE_FONT_STYLE fontSytle;
			DWRITE_FONT_STRETCH fontStretch;
			float fontSize;
			const WCHAR* localeName;
		} STFont;
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COTextLine : public COGraphic
		{
			friend LRESULT CALLBACK WndProc_TextLine(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:			
			void __vectorcall OnRender(void);

		protected:
			COStringA* vasContent;
			STFont stFont;
			IDWriteTextFormat* ifText;
			D2D1_COLOR_F crfText;
			ID2D1SolidColorBrush* ifTextColor;
			D2D_SIZE_F szfCharacter;
			float fTextLine_maxwidth;
			unsigned char ucTextAlignment;
			void __vectorcall WM_Create(void);
			void __vectorcall CharacterMetric(void);
			void __vectorcall TextAlignment(_In_ IDWriteTextLayout* ifTextLayout, _Out_ float& fTextWidth, _Out_ D2D1_POINT_2F& ptfText);
			void __vectorcall TextAlignment(_In_ IDWriteTextLayout* ifTextLayout, _Out_ float& fTextWidth, _Out_ D2D1_RECT_F& rcfText);
			void __vectorcall COTextLineV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcClassName, _In_z_ const char* pcWindowName,
																		_In_ unsigned int uiIDElementA,	_In_ STDeviceResources* pstDeviceResourcesA);

		public:
			void __vectorcall COTextLineV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_z_ unsigned int uiIDElementA,
																		_In_ STDeviceResources* pstDeviceResourcesA);
			VMEMORY __vectorcall COFreiV(void);
			void __vectorcall Text(_In_z_ char* pcText);
			void __vectorcall Font(STFont& stFont);
			COStringA* __vectorcall Content(_Out_ COStringA* vasContentA);
			void __vectorcall SetTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ unsigned char ucAlpha);
			void __vectorcall SetTextColor(_In_ D2D1_COLOR_F& crfTextA);
			void __vectorcall TextAlignment(_In_ unsigned char ucTextAlignment);
			void __vectorcall OnPaint(void);
		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COTextLine* __vectorcall COTextLineV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement, _In_ STDeviceResources* pstDeviceResources);
		__declspec(dllexport) COTextLine* __vectorcall COTextLineV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcWindowName, unsigned int uiIDElement,
																															 _In_ STDeviceResources* pstDeviceResourcesA);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
