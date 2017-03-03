//
// Lightpad - TimelinePanel.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/grid.h>
#include <wx/clrpicker.h>

#include "Colors.h"
#include "Helpers.h"
#include "GridRenderer.h"

// Timeline panel
class TimelinePanel: public wxPanel {
	public:
		TimelinePanel(wxPanel *parent);
		void ChangeNoteColor(wxColourPickerEvent &event);
		void OnSingleSelectCell(wxGridEvent& event);
		void OnCellLeftClick(wxGridEvent& event);
	private:
		wxPanel *m_parent;
		wxGrid *grid;
		wxBoxSizer *sizer;
		int id;

	wxDECLARE_EVENT_TABLE();
};
