//
// Lightpad - GridRenderer.h
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
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
		LightpadGridRenderer();
		LightpadGridRenderer(int penwidth);
		wxGridCellRenderer *Clone() const;
		wxSize GetBestSize(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, int row, int col);	
		void Draw(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, const wxRect &rect, int row, int col, bool isSelected);

	private:
		int pen_width;
};
