//
// Lightpad - PropertiesPanel.cpp
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.nightwave.co/lightpad
//

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/sizer.h>
#include <wx/grid.h>
#include <wx/clrpicker.h>

#include <string>

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

#include "PropertiesPanel.h"
#include "Colors.h"
#include "Helpers.h"
#include "GridRenderer.h"

// Initialize the properties panel and it's elements
PropertiesPanel::PropertiesPanel(wxPanel *parent)
	   : wxPanel(parent, ID_Panel_Properties, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;

	sizer = new wxBoxSizer(wxVERTICAL);
	grid = new wxGrid(this, ID_PropertiesPanel_ColorSelector);

	grid->CreateGrid(16, 8);
	grid->DisableCellEditControl();
	grid->HideColLabels();
	grid->HideRowLabels();
	grid->SetDefaultColSize(28, true);
	grid->SetDefaultRowSize(28, true);
	grid->SetDefaultRenderer(new LightpadGridRenderer(2));
	grid->SetCellHighlightPenWidth(0);
	grid->SetCellHighlightROPenWidth(0);

	for (int i = 0; i < 8; i++) {
		grid->DisableColResize(i);
	}
	for (int i = 0; i < 16; i++) {
		grid->DisableRowResize(i);
	}

	for (int i = 0; i < COLORCOUNT; i++) {
		int r = i/8;
		int c = i%8;
		grid->SetCellBackgroundColour(r, c, velocitycolors[i]);
		// wxToggleButton *color_button = new wxToggleButton(this, wxID_ANY, std::to_string(i), wxPoint(-1, -1), wxSize(20, 20));
		// gs->Add(color_button); //velocitycolors[i];
	}

	sizer->Add(grid, 1, wxSHRINK);
	this->SetSizer(sizer);
	Update();
}

void PropertiesPanel::Update() {

}

void PropertiesPanel::OnSelectCell(wxGridEvent& event) {
	grid->SelectBlock(event.GetRow(), event.GetCol(), event.GetRow(), event.GetCol());

	wxColourPickerEvent evt(this, ID_PropertiesPanel_ColorSelector, grid->GetCellBackgroundColour(event.GetRow(), event.GetCol()));
	wxPostEvent(wxWindow::FindWindowById(ID_Panel_Timeline), evt);
}

void PropertiesPanel::SelectColor(wxColourPickerEvent& event) {
	int color = get_color_velocity(event.GetColour());
	if (color > -1) {
		int x = color/8;
		int y = color%8;
		grid->SelectBlock(x, y, x, y);
	}
	// wxMessageBox("Blah blah blah...", "PropertiesPanel::SelectColor()", wxOK | wxICON_INFORMATION);
}

wxBEGIN_EVENT_TABLE(PropertiesPanel, wxPanel)
	EVT_GRID_CELL_LEFT_CLICK(PropertiesPanel::OnSelectCell)
	EVT_COLOURPICKER_CHANGED(ID_TimelinePanel_TimelineGrid, PropertiesPanel::SelectColor)
wxEND_EVENT_TABLE()
