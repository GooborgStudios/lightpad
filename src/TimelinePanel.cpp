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
#include "PropertiesPanel.h"
#include "DisplayPanel.h"

// Initialize the timeline panel and it's elements
const int COLS = 32;
const int ROWS = 96;

// Set debugger colors
// #define ANIMATED_BUTTON_COLOR
#define RAINBOW_BUTTON_COLOR

TimelinePanel::TimelinePanel(wxPanel *parent)
	: wxPanel(parent, ID_Panel_Timeline, wxPoint(-1, -1), wxSize(-1, 250), wxBORDER_SUNKEN) {
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
		unsigned char *newframe = new unsigned char[100];
		bzero(newframe, sizeof(newframe));
		set_debug_button_colors(newframe, col); // Remove me after development
		frames.push_back(newframe);
	}
	for (int row = 0; row < ROWS; row++) {
		grid->DisableRowResize(row);
	}
	for (int col = 0; col < COLS; col++) {
		for (int row = 0; row < ROWS; row++) {
			int b = row;
			if (row >= 9) b += 1;
			if (row >= 90) b += 1;
			grid->SetCellBackgroundColour(row, col, velocitycolors[frames[col][b]]);
			wxColourPickerEvent evt(this, ID_TimelinePanel_TimelineGrid, velocitycolors[frames[col][b]]);
			evt.SetInt(row);
			wxPostEvent(wxWindow::FindWindowById(ID_Panel_Display), evt);
		}
	}

	wxCommandEvent fin_evt(DISPLAY_REFRESH, ID_TimelinePanel_CellSelect);
	fin_evt.SetEventObject(this);
	wxPostEvent(wxWindow::FindWindowById(ID_Panel_Display), fin_evt);

	sizer->Add(grid, 1, wxEXPAND);
	this->SetSizer(sizer);
	sizer->Layout();

	Update();

	Bind(COLOR_SELECT, &TimelinePanel::ChangeNoteColor, this, ID_PropertiesPanel_ColorSelect);
}

TimelinePanel::~TimelinePanel() {
	delete grid;
}

void TimelinePanel::set_debug_button_colors(unsigned char *frame, int frame_num) {
	// Set the button colors
	#if defined(RAINBOW_BUTTON_COLOR)
	Color rainbow[18];
	for (int j = 0; j < 18; j++) {
		int red = 0, grn = 0, blu = 0;
		if (j >= 15) red = 255 * ((18 - j) / 3.0);
		else red = 255 * ((j - 9) / 3.0);
		if (j >= 12) grn = 255 * ((15 - j) / 3.0);
		else grn = 255 * ((j - 3) / 3.0);
		if (j >= 6) blu = 255 * ((9 - j) / 3.0);
		else blu = 255 * ((j) / 3.0);
		rainbow[j] = Color(val_in_range(red, 0, 255), val_in_range(grn, 0, 255), val_in_range(blu, 0,
		                   255));
	}
	#endif

	for (int i = 1; i < 99; i++) {
		if (i == 9 || i == 90) continue;
		int btn_x = i % 10;
		int btn_y = 9 - (i / 10);

		#if defined(ANIMATED_BUTTON_COLOR)
		int offset = std::abs(sin(frame_num * PI / 6) * 3);

		if (btn_x == 0 || btn_y == 0 || btn_x == 9 || btn_y == 9) {
			frame[i] = 0;
		} else if (btn_x == 3 || btn_y == 3 || btn_x == 6 || btn_y == 6) {
			if ((btn_x == 3 || btn_x == 6) && (btn_y == 3 || btn_y == 6)) {
				frame[i] = 81;
			} else {
				if (btn_x < 3) offset += std::abs(sin(btn_x + 1 * PI / 6) * 3);
				if (btn_x > 6) offset += std::abs(sin(btn_x * PI / 6) * 3);
				if (btn_y < 3) offset += std::abs(sin(btn_y + 1 * PI / 6) * 3);
				if (btn_y > 6) offset += std::abs(sin(btn_y * PI / 6) * 3);
				frame[i] = 45 + offset;
			}
		} else {
			frame[i] = 0;
		}
		#elif defined(RAINBOW_BUTTON_COLOR)
		frame[i] = ColorConverter::get_closest_velocity(rainbow[btn_x + btn_y]);
		#else
		frame[i] = 0;
		#endif
	}
}

void TimelinePanel::ChangeNoteColor(wxCommandEvent &event) {
	wxGridCellCoordsArray cells = grid->GetSelectedCells();
	wxGridCellCoordsArray btops = grid->GetSelectionBlockTopLeft();
	wxGridCellCoordsArray bbots = grid->GetSelectionBlockBottomRight();

	if (cells.Count() > 0) {
		for (int i = 0; i < cells.Count(); i++) {
			int col = cells[i].GetCol();
			int row = cells[i].GetRow();
			grid->SetCellBackgroundColour(row, col, velocitycolors[event.GetInt()]);
			frames[col][row] = event.GetInt();
			wxColourPickerEvent evt(this, ID_TimelinePanel_TimelineGrid,
			                        grid->GetCellBackgroundColour(row, col));
			evt.SetInt(row);
			wxPostEvent(wxWindow::FindWindowById(ID_Panel_Display), evt);
		}
	}
	if (btops.Count() > 0 && bbots.Count() > 0) {
		for (int row = btops[0].GetRow(); row <= bbots[0].GetRow(); row++) {
			for (int col = btops[0].GetCol(); col <= bbots[0].GetCol(); col++) {
				grid->SetCellBackgroundColour(row, col, velocitycolors[event.GetInt()]);
				frames[col][row] = event.GetInt();
				wxColourPickerEvent evt(this, ID_TimelinePanel_TimelineGrid,
				                        grid->GetCellBackgroundColour(row, col));
				evt.SetInt(row);
				wxPostEvent(wxWindow::FindWindowById(ID_Panel_Display), evt);
			}
		}
	}

	wxCommandEvent fin_evt(DISPLAY_REFRESH, ID_TimelinePanel_CellSelect);
	fin_evt.SetEventObject(this);
	wxPostEvent(wxWindow::FindWindowById(ID_Panel_Display), fin_evt);

	Refresh();
	Update();
}

void TimelinePanel::OnSingleSelectCell(wxGridEvent &event) {
	int btn;
	int row = event.GetRow();
	int col = event.GetCol();
	grid->SelectBlock(row, col, row, col);

	wxColourPickerEvent evt(this, ID_TimelinePanel_TimelineGrid,
	                        grid->GetCellBackgroundColour(row, col));
	wxPostEvent(wxWindow::FindWindowById(ID_Panel_Properties), evt);
	for (int i = 0; i < 96; i++) {
		evt.SetColour(grid->GetCellBackgroundColour(i, col));
		evt.SetInt(i + 1);
		wxPostEvent(wxWindow::FindWindowById(ID_Panel_Display), evt);
	}

	wxCommandEvent fin_evt(DISPLAY_REFRESH, ID_TimelinePanel_CellSelect);
	fin_evt.SetEventObject(this);
	wxPostEvent(wxWindow::FindWindowById(ID_Panel_Display), fin_evt);
}

wxBEGIN_EVENT_TABLE(TimelinePanel, wxPanel)
	EVT_GRID_SELECT_CELL(TimelinePanel::OnSingleSelectCell)
	EVT_COMMAND(ID_Panel_Timeline, COLOR_SELECT, TimelinePanel::ChangeNoteColor)
wxEND_EVENT_TABLE()
