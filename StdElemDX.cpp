/******************************************************************************
MIT License

Copyright(c) 2025 René Pagel

Filename: StdElemDX.cpp
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
#include "HStdElemDX.h"
#include "OTextLineD2.h"
#include "OEditLineD2.h"
//#include "OTextBoxD2.h"
#include "OScrollBarD2.h"

using namespace RePag::DirectX;
//---------------------------------------------------------------------------
HKEY hSchlussel; DWORD dwBytes = 4;
//---------------------------------------------------------------------------
char pcRePag_TextLine[] = "RePag_TextLine";
char pcRePag_EditLine[] = "RePag_EditLine";
//char pcRePag_TextBox[] = "RePag_TextBox";
char pcRePag_ScrollBar[] = "RePag_ScrollBar";

//---------------------------------------------------------------------------
void __vectorcall RegisterKlassenName(void)
{
  WNDCLASSEX wndKlasse;
  wndKlasse.cbSize = sizeof(WNDCLASSEX);
  wndKlasse.style = CS_OWNDC;
  wndKlasse.cbClsExtra = 0;
  wndKlasse.cbWndExtra = 16;
  wndKlasse.hInstance = NULL;
  wndKlasse.hIcon = NULL;
  wndKlasse.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndKlasse.hbrBackground = NULL;
  wndKlasse.lpszMenuName = NULL;
  wndKlasse.hIconSm = NULL;

  wndKlasse.lpszClassName = pcRePag_TextLine;
  wndKlasse.lpfnWndProc = WndProc_TextLine;
  RegisterClassEx(&wndKlasse);

  wndKlasse.lpszClassName = pcRePag_EditLine;
  wndKlasse.lpfnWndProc = WndProc_EditLine;
  wndKlasse.style |= CS_DBLCLKS;
  wndKlasse.hCursor = LoadCursor(NULL, IDC_IBEAM);
  wndKlasse.hbrBackground = NULL;
  RegisterClassEx(&wndKlasse);
  wndKlasse.style = CS_OWNDC;

  //wndKlasse.lpszClassName = pcRePag_TextBox;
  //wndKlasse.lpfnWndProc = WndProc_TextBox;
  //wndKlasse.style |= CS_DBLCLKS;
  //wndKlasse.hCursor = LoadCursor(NULL, IDC_IBEAM);
  //wndKlasse.hbrBackground = NULL;
  //RegisterClassEx(&wndKlasse);
  //wndKlasse.style = CS_OWNDC;

  wndKlasse.lpszClassName = pcRePag_ScrollBar;
  wndKlasse.lpfnWndProc = WndProc_ScrollBar;
  wndKlasse.style |= CS_DBLCLKS;
  wndKlasse.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndKlasse.hbrBackground = NULL;
  RegisterClassEx(&wndKlasse);
  wndKlasse.style = CS_OWNDC;
}
//---------------------------------------------------------------------------
void __vectorcall UnRegisterKlassenName(void)
{
  WNDCLASSEX wndKlasse; char pcUnregister[] = "Unregister";
  wndKlasse.cbSize = sizeof(WNDCLASSEX);
  wndKlasse.style = CS_OWNDC;
  wndKlasse.cbClsExtra = 0;
  wndKlasse.cbWndExtra = 0;
  wndKlasse.hInstance = NULL;
  wndKlasse.hIcon = NULL;
  wndKlasse.hCursor = NULL;
  wndKlasse.hbrBackground = NULL;
  wndKlasse.lpszMenuName = NULL;
  wndKlasse.hIconSm = NULL;
  wndKlasse.lpszClassName = pcUnregister;
  wndKlasse.lpfnWndProc = NULL;
  RegisterClassEx(&wndKlasse);

  UnregisterClass(pcRePag_TextLine, wndKlasse.hInstance);
  UnregisterClass(pcRePag_EditLine, wndKlasse.hInstance);
  //UnregisterClass(pcRePag_TextBox, wndKlasse.hInstance);
  UnregisterClass(pcRePag_ScrollBar, wndKlasse.hInstance);
}
//---------------------------------------------------------------------------