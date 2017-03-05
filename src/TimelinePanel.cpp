//
// Lightpad - TimelinePanel.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include <iostream>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/grid.h>
#include <wx/clrpicker.h>

#include "TimelinePanel.h"
#include "Colors.h"
#include "Helpers.h"
#include "GridRenderer.h"

// Initialize the timeline panel and it's elements
const int COLS = 32;
const int ROWS = 16;

TimelinePanel::TimelinePanel(wxPanel *parent)
	   : wxPanel(parent, ID_Panel_Timeline, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;

	sizer = new wxBoxSizer(wxHORIZONTAL);

	char buf[8];

	this->DestroyChildren();

	// Create a wxGrid object
	grid = new wxGrid( this,
						ID_TimelinePanel_TimelineGrid,
						wxPoint( 0, 0 ),
						wxSize( -1, -1 ));
	// Then we call CreateGrid to set the dimensions of the grid
	// (100 rows and 10 columns in this example)

	grid->CreateGrid( ROWS, COLS );
	grid->SetDefaultRenderer(new LightpadGridRenderer(1));
	grid->SetCellHighlightPenWidth(0);
	grid->SetCellHighlightROPenWidth(0);
	// We can set the sizes of individual rows and columns
	// in pixels
	grid->SetDefaultColSize(75, true);
	grid->SetDefaultRowSize(18, true);
	for (int c = 0; c < COLS; c++) {
		snprintf(buf, 7, "%d.%d.%d", c/16+1, c/4%4+1, c%4+1);
		grid->SetColLabelValue(c, buf);
		grid->DisableColResize(c);
	}
	for (int r = 0; r < ROWS; r++) {
		grid->DisableRowResize(r);
	}

	sizer->Add(grid, 1, wxEXPAND);
	this->SetSizer(sizer);
	sizer->Layout();

	Update();
}


void TimelinePanel::ChangeNoteColor(wxColourPickerEvent &event) {
	wxGridCellCoordsArray cells = grid->GetSelectedCells();
    wxGridCellCoordsArray btops = grid->GetSelectionBlockTopLeft();
    wxGridCellCoordsArray bbots = grid->GetSelectionBlockBottomRight();

    if (cells.Count() > 0) {
		for (int i=0; i < cells.Count(); i++ )
			grid->SetCellBackgroundColour(cells[i].GetRow(), cells[i].GetCol(), event.GetColour());
    }
    if (btops.Count() > 0 && bbots.Count() > 0) {
		for (int i=btops[0].GetRow(); i <= bbots[0].GetRow(); i++ ) {
			for (int j=btops[0].GetCol(); j <= bbots[0].GetCol(); j++ )
				grid->SetCellBackgroundColour(i, j, event.GetColour());
		}
	}

	Refresh();
	Update();
}

void TimelinePanel::OnSingleSelectCell(wxGridEvent& event) {
	grid->SelectBlock(event.GetRow(), event.GetCol(), event.GetRow(), event.GetCol());

	wxColourPickerEvent evt(this, ID_TimelinePanel_TimelineGrid, grid->GetCellBackgroundColour(event.GetRow(), event.GetCol()));
	wxPostEvent(wxWindow::FindWindowById(ID_Panel_Properties), evt);
}

wxBEGIN_EVENT_TABLE(TimelinePanel, wxPanel)
	EVT_GRID_SELECT_CELL(TimelinePanel::OnSingleSelectCell)
	EVT_COLOURPICKER_CHANGED(ID_PropertiesPanel_ColorSelector, TimelinePanel::ChangeNoteColor)
wxEND_EVENT_TABLE()
