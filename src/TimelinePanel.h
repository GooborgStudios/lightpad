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
	public:
		TimelinePanel(wxPanel *parent);
		~TimelinePanel();
		void paintEvent(wxPaintEvent &event);
		void paintNow();
		void render(wxDC &canvas);
		void render_row(wxDC &canvas, std::string rowname, KeyframeSet *keyframes, wxRect bounding_box);
		void render_header(wxDC &canvas);
		void render_playhead(wxDC &canvas);
		void nextBeat();
		void nextQuarterBeat();
		void movePlayhead(int time);
//		void RefreshDisplay();
//		void ChangeNoteColor(wxCommandEvent &event);
	
	protected:
		wxCoord OnGetRowHeight(size_t row) const;
		wxCoord OnGetColumnWidth(size_t column) const;
		int playhead;
	
	private:
		wxPanel *m_parent;
		wxBoxSizer *sizer;
	
		int rowsize;
		int colsize;
		int headersize;
		int labelsize;
	
		int playhead_in_pixels();
		wxPoint offset_in_pixels();
		wxPoint mousepos_to_buttons(wxPoint mousepos);
	
		wxDECLARE_EVENT_TABLE();
};

wxDECLARE_EVENT(DISPLAY_REFRESH, wxCommandEvent);
wxDECLARE_EVENT(PLAYHEAD_MOVED, wxCommandEvent);
