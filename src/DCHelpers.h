//
// Nightwave Global - DCHelpers.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// https://www.nightwave.co/
//

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

void DrawWrappedText(std::string text, wxDC &canvas, wxRect box);
