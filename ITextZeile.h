#pragma once
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace GUI
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class COTextZeile
		{
			friend LRESULT CALLBACK WndProc_TextZeile(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

			private:

			protected:
			LOGFONT lfSchrift;
			HFONT hFont;
			//STRGBFarbe stSchriftfarbe;
			COStringA* vasInhalt;
			long lZeichen_mittel;
			long lZeichen_hohe;
			float fZeichen_hohe;
			unsigned char ucSchriftausrichtung;
			void __vectorcall WM_Create(HWND hWnd);
			void __vectorcall WM_Paint(void);
			void __vectorcall ZeichenMetric(HWND hWnd);
			void __vectorcall Schriftausrichtung(HDC hdc, SIZE& stZeichengrosse, POINT& ptText);
			void __vectorcall COTextZeileV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA);

			public:
			void __vectorcall COTextZeileV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			VMEMORY __vectorcall COFreiV(void);
			void __vectorcall Text(char* pcText);
			void __vectorcall Schriftart(LOGFONT& lfSchriftA);
			LOGFONT& __vectorcall Schrift(LOGFONT& lfSchriftA);
			COStringA* __vectorcall Inhalt(COStringA* vasInhaltA);
			void __vectorcall Schriftfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Schriftausrichtung(unsigned char ucSchriftausrichtungA);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COTextZeile* __vectorcall COTextZeileV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllexport) COTextZeile* __vectorcall COTextZeileV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}

