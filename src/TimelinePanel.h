//
// Lightpad - TimelinePanel.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/dcbuffer.h>
#include <wx/vscroll.h>

#include "Project.h"
#include "MidiLayer.h"

// Timeline panel
class TimelinePanel: public wxHVScrolledWindow {
	protected:
		wxCoord OnGetRowHeight(size_t row) const;
		wxCoord OnGetColumnWidth(size_t column) const;
	
	public:
		TimelinePanel(wxPanel *parent);
		~TimelinePanel();
		void paintEvent(wxPaintEvent &event);
		void paintNow();
		void render(wxDC &canvas);
		void render_row(wxDC &canvas, std::string rowname, KeyframeSet *keyframes, wxRect bounding_box);
		void render_header(wxDC &canvas);
		void render_playhead(wxDC &canvas);
//		void MovePlayhead(int index);
//		void RefreshDisplay();
//		void ChangeNoteColor(wxCommandEvent &event);
	private:
		wxPanel *m_parent;
		wxBoxSizer *sizer;
	
		int rowsize;
		int colsize;
	
		wxDECLARE_EVENT_TABLE();
};
