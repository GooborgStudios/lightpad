//
// Lightpad - PropertiesPanel.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "PropertiesPanel.h"

#include <string>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/sizer.h>
#include <wx/grid.h>
#include <wx/clrpicker.h>

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
		int col = i / 8;
		int row = i % 8;
		grid->SetCellBackgroundColour(col, row, velocitycolors[i]);
	}

	sizer->Add(grid, 1, wxSHRINK);
	this->SetSizer(sizer);
	Update();
}

void PropertiesPanel::Update() {

}

void PropertiesPanel::OnSelectCell(wxGridEvent &event) {
	grid->SelectBlock(event.GetRow(), event.GetCol(), event.GetRow(), event.GetCol());

	wxColourPickerEvent evt(this, ID_PropertiesPanel_ColorSelector,
	                        grid->GetCellBackgroundColour(event.GetRow(), event.GetCol()));
	wxPostEvent(wxWindow::FindWindowById(ID_Panel_Timeline), evt);
}

void PropertiesPanel::SelectColor(wxColourPickerEvent &event) {
	int color = get_color_velocity(event.GetColour());
	if (color > -1) {
		int col = color / 8;
		int row = color % 8;
		grid->SelectBlock(col, row, col, row);
	}
}

wxBEGIN_EVENT_TABLE(PropertiesPanel, wxPanel)
	EVT_GRID_CELL_LEFT_CLICK(PropertiesPanel::OnSelectCell)
	EVT_COLOURPICKER_CHANGED(ID_TimelinePanel_TimelineGrid, PropertiesPanel::SelectColor)
wxEND_EVENT_TABLE()
