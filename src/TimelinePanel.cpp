//
// Lightpad - TimelinePanel.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "TimelinePanel.h"

#include <iostream>
#include <vector>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/grid.h>
#include <wx/clrpicker.h>

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
	this->DestroyChildren();

	grid = new wxGrid(this, ID_TimelinePanel_TimelineGrid, wxPoint(0, 0), wxSize(-1, -1));

	renderer = new LightpadGridRenderer(1);
	grid->CreateGrid(ROWS, COLS);
	grid->SetDefaultRenderer(renderer);
	grid->SetCellHighlightPenWidth(0);
	grid->SetCellHighlightROPenWidth(0);
	grid->SetDefaultColSize(75, true);
	grid->SetDefaultRowSize(18, true);
	for (int c = 0; c < COLS; c++) {
		snprintf(buf, 7, "%d.%d.%d", c / 16 + 1, c / 4 % 4 + 1, c % 4 + 1);
		grid->SetColLabelValue(c, buf);
		grid->DisableColResize(c);
		unsigned char *newframe = new unsigned char[96];
		bzero(newframe, sizeof(newframe));
		frames.push_back(newframe);
	}
	for (int r = 0; r < ROWS; r++) {
		grid->DisableRowResize(r);
	}

	sizer->Add(grid, 1, wxEXPAND);
	this->SetSizer(sizer);
	sizer->Layout();

	Update();
}

TimelinePanel::~TimelinePanel() {
	delete grid;
}

void TimelinePanel::ChangeNoteColor(wxColourPickerEvent &event) {
	wxGridCellCoordsArray cells = grid->GetSelectedCells();
	wxGridCellCoordsArray btops = grid->GetSelectionBlockTopLeft();
	wxGridCellCoordsArray bbots = grid->GetSelectionBlockBottomRight();

	if (cells.Count() > 0) {
		for (int i = 0; i < cells.Count(); i++) {
			int col = cells[i].GetCol();
			int row = cells[i].GetRow();
			grid->SetCellBackgroundColour(row, col, velocitycolors[event.GetInt()]);
			frames[col][row] = event.GetInt();
		}
	}
	if (btops.Count() > 0 && bbots.Count() > 0) {
		for (int row = btops[0].GetRow(); row <= bbots[0].GetRow(); row++) {
			for (int col = btops[0].GetCol(); col <= bbots[0].GetCol(); col++) {
				grid->SetCellBackgroundColour(row, col, velocitycolors[event.GetInt()]);
				frames[col][row] = event.GetInt();
			}
		}
	}

	Refresh();
	Update();
}

void TimelinePanel::OnSingleSelectCell(wxGridEvent &event) {
	grid->SelectBlock(event.GetRow(), event.GetCol(), event.GetRow(), event.GetCol());

	wxColourPickerEvent evt(this, ID_TimelinePanel_TimelineGrid,
	                        grid->GetCellBackgroundColour(event.GetRow(), event.GetCol()));
	wxPostEvent(wxWindow::FindWindowById(ID_Panel_Properties), evt);
}

wxBEGIN_EVENT_TABLE(TimelinePanel, wxPanel)
	EVT_GRID_SELECT_CELL(TimelinePanel::OnSingleSelectCell)
	EVT_COMMAND(ID_Panel_Timeline, COLOR_SELECT, TimelinePanel::ChangeNoteColor)
wxEND_EVENT_TABLE()
