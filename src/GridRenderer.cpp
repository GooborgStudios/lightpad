//
// Lightpad - GridRenderer.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/grid.h>

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

wxSize LightpadGridRenderer::GetBestSize(wxGrid &grid, wxGridCellAttr &attr,
			wxDC &dc, int row, int col) {
	return wxSize(0, 0);
}

void LightpadGridRenderer::Draw(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc,
			const wxRect &rect, int row, int col, bool isSelected) {
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
	dc.DrawRectangle(rect.GetX()+pen_width, rect.GetY()+pen_width,
		rect.GetWidth()-(pen_width*2), rect.GetHeight()-(pen_width*2));
}
