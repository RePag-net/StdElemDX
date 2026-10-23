#include "HStdElemDX.h"
#include "TStdElemDX.h"
#include "OButtonD2.h"
#include "OLightD2.h"
//------------------------------------------------------------------------------------------------------------------------------------------
DWORD WINAPI RePag::DirectX::thRender_Button(void* pvParam)
{
	SetThreadPriority(_Button->hRenderThread, THREAD_PRIORITY_NORMAL);
	IDWriteTextLayout* ifTextLayout; float fTextWidth; 	size_t szBytes_Text; WCHAR wc255Inhalt[255];
	D2D1_RECT_F rcfText = D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);
	D2D1_RECT_F rcfButton = {0.0f, 0.0f, (float)_Button->lWidth, (float)_Button->lHeight};
	D2D1_ROUNDED_RECT rrcfButton = {rcfButton, _Button->fEdgeRadius_X, _Button->fEdgeRadius_Y};
	bool bBlink = true;

	while(TRUE){
		WaitForMultipleObjectsEx(2, _Button->he2Render, true, INFINITE, false);
		if(_Button->bThreadEnd) break;
		if(!mbstowcs_s(&szBytes_Text, wc255Inhalt, 255, _Button->vasContent->c_Str(), _Button->vasContent->Length())){
			if(!_Button->pstDeviceResources->ifdwriteFactory7->CreateTextLayout(wc255Inhalt, (UINT32)szBytes_Text, _Button->ifText,
																																				 (float)_Button->lWidth, (float)_Button->lHeight, &ifTextLayout)){
				_Button->TextAlignment(ifTextLayout, fTextWidth, rcfText);
				SafeRelease(&ifTextLayout);

				_Button->ifD2D1Context6->BeginDraw();
				_Button->ifD2D1Context6->Clear(_Button->crfBackground);

				bBlink ? bBlink = false : bBlink = true;
				if(!bBlink){
					SafeRelease(&_Button->ifButtom);
					_Button->pstDeviceResources->ifd2d1Factory7->CreateRoundedRectangleGeometry(rrcfButton, &_Button->ifButtom);
					_Button->ifD2D1Context6->DrawGeometry(_Button->ifButtom, _Button->ifButtonColor, 0);
					if(!_Button->bLinearGradient)	_Button->ifD2D1Context6->FillGeometry(_Button->ifButtom, _Button->ifButtonColor);
				}
				else{
					D2D1_GRADIENT_STOP gradientStops[2];
					gradientStops[0].color = _Button->crfGradientStopsColor_0;  gradientStops[0].position = 0.0f;
					gradientStops[1].color = _Button->crfGradientStopsColor_1;  gradientStops[1].position = 1.0f;

					ID2D1GradientStopCollection* ifGradientStops = nullptr;
					_Button->ifD2D1Context6->CreateGradientStopCollection(gradientStops, 2, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &ifGradientStops);

					ID2D1LinearGradientBrush* ifLinearGradientBrush = nullptr;
					_Button->ifD2D1Context6->CreateLinearGradientBrush(
						D2D1::LinearGradientBrushProperties(D2D1::Point2F((float)_Button->lWidth / 2.0f, 0), D2D1::Point2F((float)_Button->lWidth / 2.0f, (float)_Button->lHeight)),
						D2D1::BrushProperties(1.0f, D2D1::IdentityMatrix()), ifGradientStops, &ifLinearGradientBrush);

					_Button->ifD2D1Context6->FillGeometry(_Button->ifButtom, ifLinearGradientBrush);
					SafeRelease(&ifGradientStops); SafeRelease(&ifLinearGradientBrush);
				}
				_Button->ifD2D1Context6->DrawText(wc255Inhalt, (UINT32)szBytes_Text, _Button->ifText, rcfText, _Button->ifTextColor);
				_Button->ifD2D1Context6->EndDraw();

				_Button->ifDXGISwapChain4->Present1(0, NULL, &_Button->dxgiPresent);
			}
		}
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
		if(_Light->bThreadEnd) break;
		_Light->ifD2D1Context6->BeginDraw();
		_Light->ifD2D1Context6->Clear(_Light->crfBackground);

		bBlink ? bBlink = false : bBlink = true;
		if(!bBlink){
			_Light->ellipse = D2D1::Ellipse(D2D1::Point2F((float)_Light->lWidth / 2.0f, (float)_Light->lHeight / 2.0f), _Light->fEllipseRadius_X, _Light->fEllipseRadius_Y);
			SafeRelease(&_Light->ifEllipseLight);
			_Light->pstDeviceResources->ifd2d1Factory7->CreateEllipseGeometry(_Light->ellipse, &_Light->ifEllipseLight);
			_Light->ifD2D1Context6->DrawGeometry(_Light->ifEllipseLight, _Light->ifStroke, _Light->fStroke_Width);

			if(!_Light->bRadialGradient) _Light->ifD2D1Context6->FillGeometry(_Light->ifEllipseLight, _Light->ifLight_1_0);
			else{
				D2D1_GRADIENT_STOP gradientStops[2];
				gradientStops[0].color = _Light->crfLight_1_0; gradientStops[0].position = 0.0f;
				gradientStops[1].color = _Light->crfLight_2_1; gradientStops[1].position = 1.0f;

				ID2D1GradientStopCollection* ifGradientStops;
				_Light->ifD2D1Context6->CreateGradientStopCollection(gradientStops, 2, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &ifGradientStops);

				ID2D1RadialGradientBrush* ifRadialGradientBrush;
				_Light->ifD2D1Context6->CreateRadialGradientBrush(
					D2D1::RadialGradientBrushProperties(_Light->ptfRadialGradientCenter, _Light->ptfRadialGradientOriginOffset,
																							_Light->fRadialGradientRadius_X, _Light->fRadialGradientRadius_Y),
																							ifGradientStops, &ifRadialGradientBrush);

				_Light->ifD2D1Context6->FillGeometry(_Light->ifEllipseLight, ifRadialGradientBrush);
				SafeRelease(&ifGradientStops); SafeRelease(&ifRadialGradientBrush);
			}
		}
		_Light->ifD2D1Context6->EndDraw();

		_Light->ifDXGISwapChain4->Present1(0, NULL, &_Light->dxgiPresent);
	}
	return NULL;
}
//------------------------------------------------------------------------------------------------------------------------------------------