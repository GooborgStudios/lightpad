//
// Lightpad - TimelinePanel.cpp (Old version)
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "TimelinePanel😣.h"

#include <iostream>
#include <vector>
#include <string>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/grid.h>
#include <wx/clrpicker.h>

#include "ElementIDs.h"
#include "Colors.h"
#include "Helpers.h"
#include "Project.h"
#include "GridRenderer.h"
#include "PropertiesPanel.h"
#include "DisplayPanel.h"
#include "Launchpad.h"

// Initialize the timeline panel and it's elements
const int COLS = 32;
const int ROWS = 96;

TimelinePanel::TimelinePanel(wxPanel *parent): wxPanel(parent, ID_Panel_Timeline, wxPoint(-1, -1), wxSize(-1, 250), wxBORDER_SUNKEN) {
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
	grid->SetDefaultRowSize(10, true);
	grid->EnableEditing(false);

	for (int col = 0; col < COLS; col++) {
		snprintf(buf, 7, "%d.%d.%d", col / 16 + 1, col / 4 % 4 + 1, col % 4 + 1);
		grid->SetColLabelValue(col, buf);
		grid->DisableColResize(col);
	}
	
	for (int row = ROWS - 1; row >= 0; row--) {
		int btn = ROWS - row;
		if (row <= 7) btn++;
		if (row <= 87) btn++;
		grid->SetRowLabelValue(row, std::to_string(btn));
		grid->DisableRowResize(row);
	}
	
	activeProject->layer->seek(0.0);
	
//	for (int col = COLS - 1; col >= 0; col--) { // Reversed to start the display with the first frame's colors
	for (int col = 0; col < COLS; col++) {
		for (int row = 0; row < ROWS; row++) {
			SetCellColor(col, row, activeProject->layer->getVelocity(row));
		}
		activeProject->layer->advanceFrame(1.0);
	}

	RefreshDisplay();

	sizer->Add(grid, 1, wxEXPAND);
	this->SetSizer(sizer);
	sizer->Layout();

	Update();

	Bind(COLOR_SELECT, &TimelinePanel::ChangeNoteColor, this, ID_PropertiesPanel_ColorSelect);
}

TimelinePanel::~TimelinePanel() {
	delete grid;
}


void TimelinePanel::SetCellColor(int col, int row, int velocity) {
	// We're mapping from 96 rows to 98 buttons, since two of them are non-existant, and from index start 0 to 1
	int btn = ROWS - row;
	if (row <= 7) btn++;
	if (row <= 87) btn++;
	grid->SetCellBackgroundColour(row, col, velocitycolors[velocity]);
	activeProject->layer->setVelocity(row, velocity);
	wxColourPickerEvent evt(this, ID_TimelinePanel_TimelineGrid, velocitycolors[velocity]);
	evt.SetInt(btn);
	wxPostEvent(wxWindow::FindWindowById(ID_Panel_Display), evt);
}

void TimelinePanel::MovePlayhead(int frame_num) {
	activeProject->layer->seek(frame_num);
	for (int i = 0; i < ROWS; i++) SetCellColor(frame_num, i, activeProject->layer->getVelocity(i));
	RefreshDisplay();
}

void TimelinePanel::RefreshDisplay() {
	wxCommandEvent fin_evt(DISPLAY_REFRESH, ID_TimelinePanel_CellSelect);
	fin_evt.SetEventObject(this);
	wxPostEvent(wxWindow::FindWindowById(ID_Panel_Display), fin_evt);
}

void TimelinePanel::ChangeNoteColor(wxCommandEvent &event) {
	wxGridCellCoordsArray cells = grid->GetSelectedCells();
	wxGridCellCoordsArray btops = grid->GetSelectionBlockTopLeft();
	wxGridCellCoordsArray bbots = grid->GetSelectionBlockBottomRight();

	if (cells.Count() > 0) {
		for (int i = 0; i < cells.Count(); i++) {
			SetCellColor(cells[i].GetCol(), cells[i].GetRow(), event.GetInt());
		}
	}
	if (btops.Count() > 0 && bbots.Count() > 0) {
		for (int row = btops[0].GetRow(); row <= bbots[0].GetRow(); row++) {
			for (int col = btops[0].GetCol(); col <= bbots[0].GetCol(); col++) {
				SetCellColor(col, row, event.GetInt());
			}
		}
	}

	RefreshDisplay();

	Refresh();
	Update();
}

void TimelinePanel::OnSingleSelectCell(wxGridEvent &event) {
	int row = event.GetRow();
	int col = event.GetCol();
	grid->SelectBlock(row, col, row, col);

	wxColourPickerEvent evt(this, ID_TimelinePanel_TimelineGrid,
	                        grid->GetCellBackgroundColour(row, col));
	wxPostEvent(wxWindow::FindWindowById(ID_Panel_Properties), evt);
	MovePlayhead(col);
	RefreshDisplay();
}

wxBEGIN_EVENT_TABLE(TimelinePanel, wxPanel)
	EVT_GRID_SELECT_CELL(TimelinePanel::OnSingleSelectCell)
	EVT_COMMAND(ID_Panel_Timeline, COLOR_SELECT, TimelinePanel::ChangeNoteColor)
wxEND_EVENT_TABLE()
