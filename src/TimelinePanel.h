//
// Lightpad - TimelinePanel.h
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.nightwave.co/lightpad
//

#ifndef TIMELINEPANEL_H
#define TIMELINEPANEL_H

// Attempt to load precompiled, if compiler doesn't support then load normal
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
		// void Update();
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

#endif
