/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: OSwitchGroupD2.h
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
#include "OSwitchD2.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace DirectX
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COSwitchGroup : public COGraphic
		{
			friend LRESULT CALLBACK WndProc_SwitchGroup(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);
			friend LRESULT CALLBACK WndProc_Two_waySwitch(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);

			private:
				class COTwo_waySwitch : public COSwitch
				{
					friend LRESULT CALLBACK WndProc_Two_waySwitch(_In_ HWND hWnd, _In_ unsigned int uiMessage, _In_ WPARAM wParam, _In_ LPARAM lParam);

					private:
						COSwitchGroup* pSwitchGroup;
						void __vectorcall WM_LButtonUp(_In_ WPARAM wParam, _In_ LPARAM lParam);

					protected:

					public:
						void __vectorcall COTwo_waySwitchV(_In_ VMEMORY vmMemory, _In_ const char* pcWindowName, _In_ unsigned int uiIDElementA,
																							 _In_ COSwitchGroup* pSwitchGroupA, _In_ STDeviceResources* pstDeviceResources);
						void __vectorcall OnPaint(void);
						bool bGroup;

				}; COTwo_waySwitch** vpTwo_waySwitch;
				IDWriteTextFormat* ifText;
				D2D1_COLOR_F crfText;
				ID2D1SolidColorBrush* ifTextColor;
				unsigned char ucModus;
				unsigned char ucCount;
				unsigned char ucIndex;
				short sMausPos_x;
				unsigned char ucSwitch;
				void __vectorcall WM_Create(void);
				void __vectorcall WM_LButtonDown(_In_ LPARAM lParam);
				void __vectorcall WM_MouseMove(_In_ WPARAM wParam, _In_ LPARAM lParam);
				void __vectorcall OnRender(void);

			protected:

			public:
				void __vectorcall COSwitchGroupV(_In_ VMEMORY vmMemory, _In_ const char* pcWindowName, _In_ unsigned int uiIDElementA, _In_ unsigned char ucNumberOfSwitches,
																				 _In_ unsigned char ucWechselmodus, _In_ STDeviceResources* pstDeviceResources); // Note: one uiIDElement for each switch plus one for group !!!
				//void __vectorcall COTabellenKopfV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA, unsigned char ucSchalteranzahl);
				VMEMORY __vectorcall COFreiV(void);
				void __vectorcall CreateSwitchInGroup(_In_ unsigned char ucSchalterA, _In_ long lHoheA, _In_ long lBreiteA, _In_ long lPos_x, _In_ long lPos_y);
				void __vectorcall SetAllSwitch_Off(void);
				void __vectorcall SetAllSwitch_Active(_In_ bool bAktiv);
				unsigned char __vectorcall SelectSwitch(void);
				void __vectorcall Lock(void);
				void __vectorcall Release(void);
				void(__vectorcall* pfnChangeColumnWidth)(_In_ HWND hWndTabelle, _In_ unsigned char ucSpalte, _In_ long lBreite);
				void __vectorcall Switch_Label(_In_ unsigned char ucSwitchA, _In_z_ const char* pcLabel);
				void __vectorcall Switch_Label_On(_In_ unsigned char ucSwitchA, _In_z_ const char* pcLabel);
				void __vectorcall Switch_Label_Off(_In_ unsigned char ucSwitchA, _In_z_ const char* pcLabel);
				void __vectorcall Switch_SetSwitch(_In_ unsigned char ucSwitchA, _In_ bool bOn);
				bool __vectorcall Switch_Active(_In_ unsigned char ucSwitchA);
				void __vectorcall Switch_SetActive(_In_ unsigned char ucSwitchA, _In_ bool bActive);
				void __vectorcall Switch_SetGroup(_In_ unsigned char ucSwitchA, _In_ bool bGroup);
				void __vectorcall Switch_SetWidth(_In_ unsigned char ucSwitchA, _In_ long lWidthA);
				void __vectorcall Switch_SetVisible(_In_ unsigned char ucSwitchA, _In_ bool bVisible);
				bool __vectorcall Switch_IsOn(_In_ unsigned char ucSwitchA);
				void __vectorcall Switch_Font(_In_ unsigned char ucSwitchA, _In_ STFont& stFont);
				void __vectorcall Switch_SetBackgroundColor(_In_ unsigned char ucSwitchA, _In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall Switch_SetBackgroundColor(_In_ unsigned char ucSwitchA, _In_ D2D1_COLOR_F& crfBackgroundA);
				void __vectorcall Switch_SetBackgroundColor_1(_In_ unsigned char ucSwitchA, _In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall Switch_SetBackgroundColor_1(_In_ unsigned char ucSwitchA, _In_ D2D1_COLOR_F& crfBackgroundA);
				void __vectorcall Switch_SetBackgroundColor_2(_In_ unsigned char ucSwitchA, _In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall Switch_SetBackgroundColor_2(_In_ unsigned char ucSwitchA, _In_ D2D1_COLOR_F& crfBackgroundA);
				void __vectorcall Switch_SetBackgroundColor_3(_In_ unsigned char ucSwitchA, _In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall Switch_SetBackgroundColor_3(_In_ unsigned char ucSwitchA, _In_ D2D1_COLOR_F& crfBackgroundA);
				long& __vectorcall Switch_Pos_X(_In_ unsigned char ucSwitchA, _Out_ long& lPos_x);
				long& __vectorcall Switch_Pos_Y(_In_ unsigned char ucSwitchA, _Out_ long& lPos_y);
				void __vectorcall Switch_NewPosition(_In_ unsigned char ucSwitchA, _In_ long lPos_x, _In_ long lPos_y);
				void __vectorcall Switch_ChangePosition(_In_ unsigned char ucSwitchA, _In_ long lPos_x, _In_ long lPos_y);
				void __vectorcall Switch_pfnWM_LButtonUp(_In_ unsigned char ucSwitchA, void(__vectorcall* pfnSwitch_WM_LButtonUp)(COSwitch*));
				void __vectorcall Switch_pfnWM_LButtonDown(_In_ unsigned char ucSwitchA, void(__vectorcall* pfnSwitch_WM_LButtonDown)(COSwitch*));
				void(__vectorcall* pfnOnRender)(_In_ COSwitchGroup* pSwitchGroup);
				void __vectorcall OnPaint(void);
				void __vectorcall SetFont(STFont& stFont);
				void __vectorcall SetTextColor(_In_ unsigned char ucRed, _In_ unsigned char ucGreen, _In_ unsigned char ucBlue, _In_ float fAlpha);
				void __vectorcall SetTextColor(_In_ D2D1_COLOR_F& crfTextA);
				void __vectorcall DrawText(_In_z_ const wchar_t* wcInhalt, _In_ size_t szBytes_Text, _In_ const D2D1_RECT_F& rcfText);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COSwitchGroup* __vectorcall COSwitchGroupV(_In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement, _In_ unsigned char ucNumberOfSwitches,
																																		 _In_ unsigned char ucWechselmodus, _In_ STDeviceResources* pstDeviceResources);
		__declspec(dllexport) COSwitchGroup* __vectorcall COSwitchGroupV(_In_ VMEMORY vmMemory, _In_z_ const char* pcWindowName, _In_ unsigned int uiIDElement, 
																																		 _In_ unsigned char ucNumberOfSwitches, _In_ unsigned char ucWechselmodus,
																																		 _In_ STDeviceResources* pstDeviceResources);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
