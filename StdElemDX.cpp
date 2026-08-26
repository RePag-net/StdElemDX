/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

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
#include "OTextBoxD2.h"
#include "OScrollBarD2.h"
#include "OListBoxD2.h"
#include "OEditBoxD2.h"
#include "OPasswordD2.h"
#include "OButtonD2.h"
#include "OLookupBoxD2.h"
#include "OSwitchD2.h"
#include "OSwitchGroupD2.h"
#include "OProgressBarD2.h"

using namespace RePag::DirectX;
//---------------------------------------------------------------------------
HKEY hSchlussel; DWORD dwBytes = 4;
//---------------------------------------------------------------------------
char pcRePag_TextLine[] = "RePag_TextLine";
char pcRePag_EditLine[] = "RePag_EditLine";
char pcRePag_TextBox[] = "RePag_TextBox";
char pcRePag_ScrollBar[] = "RePag_ScrollBar";
char pcRePag_ListBox[] = "RePag_ListBox";
char pcRePag_EditBox[] = "RePag_EditBox";
char pcRePag_Password[] = "RePag_Password";
char pcRePag_Button[] = "RePag_Button";
char pcRePag_LookupBox[] = "RePag_LookupBox";
char pcRePag_LookupEntry[] = "RePag_LookupEntry";
char pcRePag_Switch[] = "RePag_Switch";
char pcRePag_SwitchGroup[] = "RePag_SwitchGroup";
char pcRePag_Two_waySwitch[] = "RePag_Two_waySwitch";
char pcRePag_ProgressBar[] = "RePag_ProgressBar";

//---------------------------------------------------------------------------
void __vectorcall RegisterClassName(void)
{
  WNDCLASSEX wndClass;
  wndClass.cbSize = sizeof(WNDCLASSEX);
  wndClass.style = CS_OWNDC;
  wndClass.cbClsExtra = 0;
  wndClass.cbWndExtra = 16;
  wndClass.hInstance = NULL;
  wndClass.hIcon = NULL;
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndClass.hbrBackground = NULL;
  wndClass.lpszMenuName = NULL;
  wndClass.hIconSm = NULL;

  wndClass.lpszClassName = pcRePag_TextLine;
  wndClass.lpfnWndProc = WndProc_TextLine;
  RegisterClassEx(&wndClass);

  wndClass.lpszClassName = pcRePag_EditLine;
  wndClass.lpfnWndProc = WndProc_EditLine;
  wndClass.style |= CS_DBLCLKS;
  wndClass.hCursor = LoadCursor(NULL, IDC_IBEAM);
  wndClass.hbrBackground = NULL;
  RegisterClassEx(&wndClass);
  wndClass.style = CS_OWNDC;

  wndClass.lpszClassName = pcRePag_TextBox;
  wndClass.lpfnWndProc = WndProc_TextBox;
  wndClass.hCursor = LoadCursor(NULL, IDC_IBEAM);
  wndClass.hbrBackground = NULL;
  RegisterClassEx(&wndClass);

  wndClass.lpszClassName = pcRePag_ScrollBar;
  wndClass.lpfnWndProc = WndProc_ScrollBar;
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndClass.hbrBackground = NULL;
  RegisterClassEx(&wndClass);

  wndClass.lpszClassName = pcRePag_ListBox;
  wndClass.lpfnWndProc = WndProc_ListBox;
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndClass.hbrBackground = NULL;
  RegisterClassEx(&wndClass);

  wndClass.lpszClassName = pcRePag_EditBox;
  wndClass.lpfnWndProc = WndProc_EditBox;
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndClass.hbrBackground = NULL;
  RegisterClassEx(&wndClass);

  wndClass.lpszClassName = pcRePag_Password;
  wndClass.lpfnWndProc = WndProc_Password;
  wndClass.hCursor = LoadCursor(NULL, IDC_IBEAM);
  RegisterClassEx(&wndClass);

  wndClass.lpszClassName = pcRePag_Button;
  wndClass.lpfnWndProc = WndProc_Button; 
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  RegisterClassEx(&wndClass);

  wndClass.lpszClassName = pcRePag_LookupBox;
  wndClass.lpfnWndProc = WndProc_LookupBox;
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  RegisterClassEx(&wndClass);

  wndClass.lpszClassName = pcRePag_LookupEntry;
  wndClass.lpfnWndProc = WndProc_Entry;
  RegisterClassEx(&wndClass);

  wndClass.lpszClassName = pcRePag_Switch;
  wndClass.lpfnWndProc = WndProc_Switch;
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  RegisterClassEx(&wndClass);

  wndClass.lpszClassName = pcRePag_SwitchGroup;
  wndClass.lpfnWndProc = WndProc_SwitchGroup;
  RegisterClassEx(&wndClass);

  wndClass.lpszClassName = pcRePag_Two_waySwitch;
  wndClass.lpfnWndProc = WndProc_Two_waySwitch;
  RegisterClassEx(&wndClass);

  wndClass.lpszClassName = pcRePag_ProgressBar;
  wndClass.lpfnWndProc = WndProc_ProgressBar;
  RegisterClassEx(&wndClass);
}
//---------------------------------------------------------------------------
void __vectorcall UnRegisterClassName(void)
{
  WNDCLASSEX wndClass; char pcUnregister[] = "Unregister";
  wndClass.cbSize = sizeof(WNDCLASSEX);
  wndClass.style = CS_OWNDC;
  wndClass.cbClsExtra = 0;
  wndClass.cbWndExtra = 0;
  wndClass.hInstance = NULL;
  wndClass.hIcon = NULL;
  wndClass.hCursor = NULL;
  wndClass.hbrBackground = NULL;
  wndClass.lpszMenuName = NULL;
  wndClass.hIconSm = NULL;
  wndClass.lpszClassName = pcUnregister;
  wndClass.lpfnWndProc = NULL;
  RegisterClassEx(&wndClass);

  UnregisterClass(pcRePag_TextLine, wndClass.hInstance);
  UnregisterClass(pcRePag_EditLine, wndClass.hInstance);
  UnregisterClass(pcRePag_TextBox, wndClass.hInstance);
  UnregisterClass(pcRePag_ScrollBar, wndClass.hInstance);
  UnregisterClass(pcRePag_ListBox, wndClass.hInstance);
  UnregisterClass(pcRePag_EditBox, wndClass.hInstance);
  UnregisterClass(pcRePag_Password, wndClass.hInstance);
  UnregisterClass(pcRePag_Button, wndClass.hInstance);
  UnregisterClass(pcRePag_LookupBox, wndClass.hInstance);
  UnregisterClass(pcRePag_LookupEntry, wndClass.hInstance);
  UnregisterClass(pcRePag_Switch, wndClass.hInstance);
  UnregisterClass(pcRePag_SwitchGroup, wndClass.hInstance);
  UnregisterClass(pcRePag_Two_waySwitch, wndClass.hInstance);
  UnregisterClass(pcRePag_ProgressBar, wndClass.hInstance);
}
//---------------------------------------------------------------------------