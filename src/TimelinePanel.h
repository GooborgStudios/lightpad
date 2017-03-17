//
// Lightpad - TimelinePanel.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

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

// Timeline panel
class TimelinePanel: public wxPanel {
	public:
		TimelinePanel(wxPanel *parent);
		~TimelinePanel();
		void set_debug_button_colors(unsigned char *frame, int frame_num);
		void ChangeNoteColor(wxCommandEvent &event);
		void OnSingleSelectCell(wxGridEvent &event);
		void OnCellLeftClick(wxGridEvent &event);
	private:
		wxPanel *m_parent;
		wxGrid *grid;
		wxBoxSizer *sizer;
		LightpadGridRenderer *renderer;
		int id;
		char buf[8];
		std::vector<unsigned char *> frames;

		wxDECLARE_EVENT_TABLE();
};
