/******************************************************************************
MIT License

Copyright(c) 2025 René Pagel

Filename: HStdElemDX.h
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

#define WIN32_LEAN_AND_MEAN             // Selten verwendete Komponenten aus Windows-Headern ausschließen
// Windows-Headerdateien
#include <windows.h>
#include <windowsx.h>

#include "stdlib.h"
#include "Menu.h"
#include "Fieldsizes.h"

#include <ADT.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dcommon.h>
#include <dwrite.h>
#include <cstdlib>
#include <sal.h>

#include "CompSys.h"
using namespace RePag::System;

#include "ProgDlgDX.h"

extern char pcRePag_TextLine[];
extern char pcRePag_EditLine[];
extern char pcRePag_ScrollBar[];
extern char pcRePag_TextBox[];
extern char pcRePag_EditBox[];
extern char pcRePag_ListBox[];
extern char pcRePag_Passwort[];
extern char pcRePag_Knopf[];
extern char pcRePag_KlappBox[];
extern char pcRePag_KlappBoxEintrage[];
extern char pcRePag_Schalter[];
extern char pcRePag_SchalterGruppe[];
extern char pcRePag_WechselSchalter[];
extern char pcRePag_TabellenKopf[];
extern char pcRePag_Balken[];
extern char pcRePag_Datum[];
extern char pcRePag_EditDatum[];
extern char pcRePag_Leuchte[];

constexpr BYTE  TXA_TOP = 0;
constexpr BYTE  TXA_LEFT = 1;
constexpr BYTE  TXA_RIGHT = 2;
constexpr BYTE  TXA_BOTTOM = 4;
constexpr BYTE  TXA_CENTERVERTICAL = 8;
constexpr BYTE  TXA_CENTERHORIZONTAL = 16;

#define SHIFTED 0x8000
#define ZV_KEINE 0
#define ZV_BUCHSTABEN 1
#define ZV_ZIFFERN 2
#define ZV_SONDERBUCHSTABEN 4
#define ZV_SONDERZEICHEN 8
#define ZV_UBERLANGE 16
#define ZV_SICHTBAR 32
#define ZV_ALLE 15

#define SGM_KEINWECHSEL 0
#define SGM_EINAUS 1
#define SGM_EINAUSINAKTIV 2

#define LEF_RUND 0
#define LEF_ECKIG 1