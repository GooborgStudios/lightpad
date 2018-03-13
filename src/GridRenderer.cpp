//
// Lightpad - GridRenderer.cpp
// ©2018 Gooborg Studios: Vinyl Darkscratch, Light Apacha.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.gooborg.com/lightpad
//

#include "GridRenderer.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/grid.h>

LightpadGridRenderer::LightpadGridRenderer(int penwidth) {
	pen_width = penwidth;
}

wxGridCellRenderer *LightpadGridRenderer::Clone() const {
	return new LightpadGridRenderer();
}

wxSize LightpadGridRenderer::GetBestSize(wxGrid &grid, wxGridCellAttr &attr,
        wxDC &canvas, int row, int col) {
	return wxSize(0, 0); // XXX Implement me
}

void LightpadGridRenderer::Draw(wxGrid &grid, wxGridCellAttr &attr, wxDC &canvas,
                                const wxRect &rect, int row, int col, bool isSelected) {
	wxPen pen = *wxTRANSPARENT_PEN;
	if (isSelected) pen = *wxWHITE_PEN;
	pen.SetWidth(pen_width);
	canvas.SetPen(pen);

	canvas.SetBrush(wxBrush(attr.GetBackgroundColour()));
	canvas.DrawRectangle(rect);

	if (isSelected) {
		pen = *wxBLACK_PEN;
		pen.SetWidth(pen_width);
		canvas.SetPen(pen);
	}
	canvas.DrawRectangle(rect.GetX() + pen_width, rect.GetY() + pen_width,
	                     rect.GetWidth() - (pen_width * 2), rect.GetHeight() - (pen_width * 2));
}
