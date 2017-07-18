//
// Lightpad - GridRenderer.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/grid.h>

class LightpadGridRenderer: public wxGridCellRenderer {
	public:
		LightpadGridRenderer(int penwidth = 1);
		wxGridCellRenderer *Clone() const;
		wxSize GetBestSize(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, int row, int col);
		void Draw(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, const wxRect &rect, int row, int col,
		          bool isSelected);

	private:
		int pen_width;
};
