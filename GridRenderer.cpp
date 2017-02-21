//
// Lightpad - GridRenderer.cpp
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.nightwave.co/lightpad
//

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/grid.h>

#ifdef _WIN32
	//define something for Windows (32-bit and 64-bit, this part is common)
	#define WINDOWS
	#ifdef _WIN64
		//define something for Windows (64-bit only)
		#define WINDOWS_64
	#endif
#elif __APPLE__
	//define something for Mac
	#define MACOS
#else
	//#error "Unknown/unsupported compiler/operating system"
#endif

#include "GridRenderer.h"

LightpadGridRenderer::LightpadGridRenderer() {
	pen_width = 1;
}

LightpadGridRenderer::LightpadGridRenderer(int penwidth) {
	pen_width = penwidth;
}

wxGridCellRenderer *LightpadGridRenderer::Clone() const {
	return new LightpadGridRenderer();
}

wxSize LightpadGridRenderer::GetBestSize(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, int row, int col) {
	return wxSize(0, 0);
}

void LightpadGridRenderer::Draw(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, const wxRect &rect, int row, int col, bool isSelected) {
	wxPen pen = *wxTRANSPARENT_PEN;
	if (isSelected) pen = *wxWHITE_PEN;
	pen.SetWidth(pen_width);
	dc.SetPen(pen);

	dc.SetBrush(wxBrush(attr.GetBackgroundColour()));
	dc.DrawRectangle(rect);

	if (isSelected) {
		pen = *wxBLACK_PEN;
		pen.SetWidth(pen_width);
		dc.SetPen(pen);
	}
	dc.DrawRectangle(rect.GetX()+pen_width, rect.GetY()+pen_width, rect.GetWidth()-(pen_width*2), rect.GetHeight()-(pen_width*2));
}