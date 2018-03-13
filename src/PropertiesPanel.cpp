//
// Lightpad - PropertiesPanel.cpp
// ©2018 Gooborg Studios: Vinyl Darkscratch, Light Apacha.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.gooborg.com/lightpad
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

#include "ElementIDs.h"
#include "GooCore/Colors.h"
#include "GooCore/GooCore.h"
#include "GridRenderer.h"
#include "MidiLayer.h"
#include "LightpadProject.h"
#include "HOWL/TimelinePanel.h"

wxDEFINE_EVENT(COLOR_SELECT, wxCommandEvent);

// Initialize the properties panel and it's elements
PropertiesPanel::PropertiesPanel(wxPanel *parent, LightpadProject *project) : wxPanel(parent, ID_Panel_Properties, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;

	activeProject = project;

	sizer = new wxBoxSizer(wxVERTICAL);
	grid = new wxGrid(this, ID_PropertiesPanel_ColorSelector);

	renderer = new LightpadGridRenderer(2);
	grid->CreateGrid(16, 8);
	grid->DisableCellEditControl();
	grid->HideColLabels();
	grid->HideRowLabels();
	grid->SetDefaultColSize(28, true);
	grid->SetDefaultRowSize(28, true);
	grid->SetDefaultRenderer(renderer);
	grid->SetCellHighlightPenWidth(0);
	grid->SetCellHighlightROPenWidth(0);

	for (int i = 0; i < 8; i++) {
		grid->DisableColResize(i);
	}
	for (int i = 0; i < 16; i++) {
		grid->DisableRowResize(i);
	}

	for (int i = 0; i < COLORCOUNT; i++) {
		int row = i / 8;
		int col = i % 8;
		grid->SetCellBackgroundColour(row, col, velocitycolors[i]);
	}

	sizer->Add(grid, 1, wxSHRINK);
	this->SetSizer(sizer);
	Update();
}

PropertiesPanel::~PropertiesPanel() {
	delete grid;
}

void PropertiesPanel::Update() {

}

void PropertiesPanel::OnSelectCell(wxGridEvent &event) {
	grid->SelectBlock(event.GetRow(), event.GetCol(), event.GetRow(), event.GetCol());

	HOWL::Selection sel = activeProject->selection;
	unsigned char velocity = event.GetRow() * 8 + event.GetCol();
	unsigned char old_velocity;

	for (auto s: sel.sel) {
		HOWL::KeyframePair pair1 = activeProject->layer->getSurroundingKeyframes(s->set->name, s->start);
		HOWL::KeyframePair pair2 = activeProject->layer->getSurroundingKeyframes(s->set->name, s->end);
		old_velocity = pair2.first == NULL ? 0 : ((NoteKeyframe *)(pair2.first))->velocity;
		s->set->removeKeyframes(pair1.second, pair2.first);

		s->set->AddKeyframe(new NoteKeyframe(std::atoi(s->set->name.c_str()), s->start, velocity));
		s->set->AddKeyframe(new NoteKeyframe(std::atoi(s->set->name.c_str()), s->end, old_velocity), false);
	}

	wxCommandEvent evt(HOWL::DISPLAY_REFRESH, ID_PropertiesPanel_ColorSelect);
	evt.SetEventObject(this);
	wxPostEvent(wxWindow::FindWindowById(ID_Panel_Timeline), evt);
	wxPostEvent(wxWindow::FindWindowById(ID_Panel_Display), evt);

//	wxCommandEvent evt(COLOR_SELECT, ID_PropertiesPanel_ColorSelect);
//	evt.SetEventObject(this);
//	evt.SetInt(event.GetRow() * 8 + event.GetCol());
//	wxPostEvent(wxWindow::FindWindowById(ID_Panel_Timeline), evt);
}

void PropertiesPanel::SelectColor(wxColourPickerEvent &event) {
	int color = get_color_velocity(event.GetColour());
	if (color > -1) {
		int row = color / 8;
		int col = color % 8;
		grid->SelectBlock(row, col, row, col);
	}
}

wxBEGIN_EVENT_TABLE(PropertiesPanel, wxPanel)
	EVT_GRID_CELL_LEFT_CLICK(PropertiesPanel::OnSelectCell)
	EVT_COLOURPICKER_CHANGED(ID_TimelinePanel_TimelineGrid, PropertiesPanel::SelectColor)
wxEND_EVENT_TABLE()
