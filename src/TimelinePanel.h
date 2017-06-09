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

// Timeline panel
class TimelinePanel: public wxPanel {
public:
	TimelinePanel(wxPanel *parent);
	~TimelinePanel();
//	void SetCellColor(int row, int col, int velocity);
	void MovePlayhead(int index);
	void RefreshDisplay();
	void ChangeNoteColor(wxCommandEvent &event);
//	void OnSingleSelectCell(wxGridEvent &event);
//	void OnCellLeftClick(wxGridEvent &event);
private:
	wxPanel *m_parent;
//	wxGrid *grid;
	wxBufferedDC dc;
	wxBoxSizer *sizer;
//	LightpadGridRenderer *renderer;
	int id;
//	char buf[8];
	
	wxDECLARE_EVENT_TABLE();
};
