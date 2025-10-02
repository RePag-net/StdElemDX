#include "HStdElemD2.h"
#include "ITextLine.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
//RePag::GUI::CITextZeile* __vectorcall RePag::GUI::COTextZeileV(const char* pcFensterName, unsigned int uiIDElement)
//{
//	CITextZeile* vTextZeile = (CITextZeile*)VMBlock(VMDialog(), sizeof(CITextZeile));
//	vTextZeile->CITextLineV((const PBYTE)VMDialog(), (const PBYTE)pcFensterName, uiIDElement);
//	return vTextZeile;
//}
////-------------------------------------------------------------------------------------------------------------------------------------------
//RePag::GUI::CITextZeile* __vectorcall RePag::GUI::COTextZeileV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
//{
//	CITextZeile* vTextZeile = (CITextZeile*)VMBlock(vmSpeicher, sizeof(CITextZeile));
//	vTextZeile->CITextLineV((const PBYTE)vmSpeicher, (const PBYTE)pcFensterName, uiIDElement);
//	return vTextZeile;
//}
////-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_TextZeile(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	CITextZeile* pTextZeile;
	switch(uiMessage){
		case WM_CREATE		: (((CITextZeile*)((LPCREATESTRUCT)lParam)->lpCreateParams))->WM_Create(hWnd);
												return NULL;	
		case WM_SIZE			:	pTextZeile = (CITextZeile*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
												if(pTextZeile) pTextZeile->WM_Size_Element(hWnd, lParam);
												else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
												return NULL;
		case WM_PAINT			:	pTextZeile = (CITextZeile*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
												pTextZeile->WM_Paint();
												return NULL;
		case WM_NCDESTROY	:	pTextZeile = (CITextZeile*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
												if(pTextZeile->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pTextZeile->htEffekt_Timer, INVALID_HANDLE_VALUE);
												VMFreiV(pTextZeile);
												return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
//HRESULT __vectorcall RePag::GUI::CITextZeile::COTextLineV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA, IGraphic ifGraphic)
//{
//	//CIGrafikV(vmSpeicher, pcKlassenName, pcFensterName, uiIDElementA);
//
//
//	hFont = nullptr;
//	lfSchrift.lfHeight = -13;
//	lfSchrift.lfWidth = 0;
//	lfSchrift.lfFaceName[0] = '\0';
//	lfSchrift.lfEscapement = GM_COMPATIBLE;
//	lfSchrift.lfOrientation = GM_COMPATIBLE;
//	lfSchrift.lfItalic = FALSE;
//	lfSchrift.lfUnderline = FALSE;
//	lfSchrift.lfStrikeOut = FALSE;
//	lfSchrift.lfWeight = FW_DONTCARE;
//	lfSchrift.lfCharSet = DEFAULT_CHARSET;
//	lfSchrift.lfOutPrecision = OUT_DEVICE_PRECIS;
//	lfSchrift.lfClipPrecision = CLIP_DEFAULT_PRECIS;
//	lfSchrift.lfQuality = DEFAULT_QUALITY;
//	lfSchrift.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
//
//	vasInhalt = COStringAV(vmSpeicher);
//
//	stSchriftfarbe.ucRot = stSchriftfarbe.ucGrun = stSchriftfarbe.ucBlau = 0;
//	ucSchriftausrichtung = TXA_LINKS | TXA_MITTEVERTICAL;
//
//	return S_OK;
//}
//-------------------------------------------------------------------------------------------------------------------------------------------
HRESULT __vectorcall RePag::GUI::CITextZeile::COTextLineV(IGraphic* pifGraphic)
{
	hFont = nullptr;
	lfSchrift.lfHeight = -13;
	lfSchrift.lfWidth = 0;
	lfSchrift.lfFaceName[0] = '\0';
	lfSchrift.lfEscapement = GM_COMPATIBLE;
	lfSchrift.lfOrientation = GM_COMPATIBLE;
	lfSchrift.lfItalic = FALSE;
	lfSchrift.lfUnderline = FALSE;
	lfSchrift.lfStrikeOut = FALSE;
	lfSchrift.lfWeight = FW_DONTCARE;
	lfSchrift.lfCharSet = DEFAULT_CHARSET;
	lfSchrift.lfOutPrecision = OUT_DEVICE_PRECIS;
	lfSchrift.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lfSchrift.lfQuality = DEFAULT_QUALITY;
	lfSchrift.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

	vasInhalt = COStringAV(vmSpeicher);

	stSchriftfarbe.ucRot = stSchriftfarbe.ucGrun = stSchriftfarbe.ucBlau = 0;
	ucSchriftausrichtung = TXA_LINKS | TXA_MITTEVERTICAL;

	return S_OK;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::GUI::CITextZeile::COFreiV(void)
{
	VMFreiV(vasInhalt);
	if(hFont) DeleteObject(hFont);
	return ((COElement*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CITextZeile::WM_Paint(void)
{
	ThreadSicher_Anfang();
	PAINTSTRUCT stPaint; SIZE stZeichengrosse; POINT ptText = {0,0};
	BeginPaint(hWndElement, &stPaint);
	WM_Paint_Hintergrund(stPaint);
	SelectObject(stPaint.hdc, hFont);
	Schriftausrichtung(stPaint.hdc, stZeichengrosse, ptText);
	SetBkMode(stPaint.hdc, TRANSPARENT);
	SetTextColor(stPaint.hdc, RGB(stSchriftfarbe.ucRot, stSchriftfarbe.ucGrun, stSchriftfarbe.ucBlau));
	ExtTextOut(stPaint.hdc, ptText.x, ptText.y, ETO_CLIPPED, &stPaint.rcPaint, vasInhalt->c_Str(), vasInhalt->Length(), NULL);
	EndPaint(hWndElement, &stPaint);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CITextZeile::WM_Create(HWND hWnd)
{
	ZeichenMetric(hWnd);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CITextZeile::ZeichenMetric(HWND hWnd)
{
	TEXTMETRIC stTextmetric;
	HDC hdc = GetDC(hWnd);
	if(hFont) DeleteObject(hFont);
	hFont = CreateFontIndirect(&lfSchrift);
	SelectObject(hdc, hFont);
	GetTextMetrics(hdc, &stTextmetric);
	lZeichen_mittel = stTextmetric.tmAveCharWidth;
	lZeichen_hohe = stTextmetric.tmHeight + stTextmetric.tmExternalLeading;
	fZeichen_hohe = static_cast<float>(stTextmetric.tmHeight + stTextmetric.tmExternalLeading);
	ReleaseDC(hWnd, hdc);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CITextZeile::Schriftausrichtung(HDC hdc, SIZE& stZeichengrosse, POINT& ptText)
{
	TEXTMETRIC stTextmetric;
	GetTextMetrics(hdc, &stTextmetric);
	GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
	ptText.x = ptText.y = 0;
	if(ucSchriftausrichtung & TXA_RECHTS) ptText.x = lBreite - stZeichengrosse.cx;
	if(ucSchriftausrichtung & TXA_MITTEHORIZONTAL) ptText.x = (lBreite - stZeichengrosse.cx) / 2;
	if(ucSchriftausrichtung & TXA_UNTEN) ptText.y = lHohe - stTextmetric.tmHeight + stTextmetric.tmExternalLeading;
	if(ucSchriftausrichtung & TXA_MITTEVERTICAL) ptText.y = (lHohe - stTextmetric.tmHeight + stTextmetric.tmExternalLeading) / 2;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CITextZeile::Text(char* pcText)
{
	ThreadSicher_Anfang();
	*vasInhalt = pcText;
	UpdateFenster(NULL, true, false);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CITextZeile::Schriftart(LOGFONT& lfSchriftA)
{
	if(!hWndElement) lfSchrift = lfSchriftA;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LOGFONT& __vectorcall RePag::GUI::CITextZeile::Schrift(LOGFONT& lfSchriftA)
{
	ThreadSicher_Anfang();
	lfSchriftA = lfSchrift;
	ThreadSicher_Ende();
	return lfSchriftA;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::GUI::CITextZeile::Inhalt(COStringA* vasInhaltA)
{
	ThreadSicher_Anfang();
	if(vasInhaltA) *vasInhaltA = *vasInhalt;
	ThreadSicher_Ende();
	return vasInhaltA;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CITextZeile::Schriftfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
	ThreadSicher_Anfang();
	stSchriftfarbe.ucRot = ucRot; stSchriftfarbe.ucGrun = ucGrun; stSchriftfarbe.ucBlau = ucBlau;
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CITextZeile::Schriftausrichtung(unsigned char ucSchriftausrichtungA)
{
	ThreadSicher_Anfang();
	ucSchriftausrichtung = ucSchriftausrichtungA;
	ThreadSicher_Ende();
}