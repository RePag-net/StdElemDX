/******************************************************************************
MIT License

Copyright(c) 2026 René Pagel

Filename: TStdElemDX.cpp
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
#include "TStdElemDX.h"
#include "OButtonD2.h"
#include "OLightD2.h"
//------------------------------------------------------------------------------------------------------------------------------------------
DWORD WINAPI RePag::DirectX::thRender_Button(void* pvParam)
{
	SetThreadPriority(_Button->hRenderThread, THREAD_PRIORITY_NORMAL);
	bool bBlink = true;

	while(TRUE){
		WaitForMultipleObjectsEx(2, _Button->he2Render, true, INFINITE, false);
		if(_Button->bThreadEnd) break;

		bBlink ? bBlink = false : bBlink = true;
		_Button->OnRender(bBlink);
		_Button->ifDXGISwapChain4->Present1(0, NULL, &_Button->dxgiPresent);
	}
	return NULL;
}
//------------------------------------------------------------------------------------------------------------------------------------------
DWORD WINAPI RePag::DirectX::thRender_Light(void* pvParam)
{
	SetThreadPriority(_Light->hRenderThread, THREAD_PRIORITY_NORMAL);
	bool bBlink = true;

	while(TRUE){
	    WaitForMultipleObjectsEx(2, _Light->he2Render, true, INFINITE, false);
	    if (_Light->bThreadEnd) break;

	    bBlink ? bBlink = false : bBlink = true;
	    _Light->OnRender(bBlink);
	    _Light->ifDXGISwapChain4->Present1(0, NULL, &_Light->dxgiPresent);
	}
	return NULL;
}
//------------------------------------------------------------------------------------------------------------------------------------------