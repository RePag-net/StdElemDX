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