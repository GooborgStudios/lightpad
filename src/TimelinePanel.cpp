//
// Lightpad - TimelinePanel.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "TimelinePanel.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/dcbuffer.h>
#include <wx/vscroll.h>

#include "ElementIDs.h"
#include "Project.h"
#include "Layer.h"
#include "MidiLayer.h"
#include "Launchpad.h"

TimelinePanel::TimelinePanel(wxPanel *parent): wxHVScrolledWindow(parent, ID_Panel_Timeline, wxPoint(-1, -1), wxSize(-1, 250), wxBORDER_SUNKEN) {
	m_parent = parent;
	sizer = new wxBoxSizer(wxHORIZONTAL);
	
	SetRowColumnCount(97, 100);
	
	Update();
}

TimelinePanel::~TimelinePanel() {
	
}

wxCoord TimelinePanel::OnGetRowHeight(size_t row) const {
	return 20;
}

wxCoord TimelinePanel::OnGetColumnWidth(size_t column) const {
	return 80;
//	return wxClientDC(this).GetSize().GetX();
}

void TimelinePanel::paintEvent(wxPaintEvent &event) {
	// depending on your system you may need to look at double-buffered dcs
	wxPaintDC canvas(this);
	render(canvas);
}

void TimelinePanel::paintNow() {
	// depending on your system you may need to look at double-buffered dcs
	wxClientDC canvas(this);
	render(canvas);
}

void TimelinePanel::render(wxDC &canvas) {
	int width = canvas.GetSize().GetX();
	int ypos = 30-(GetVisibleBegin().GetRow()*20);
	for (auto iter : activeProject->layer->keyframes) {
		if (ypos >= 10) render_row(canvas, iter.first, iter.second, wxRect(0, ypos, width, 20));
		ypos += 20;
		if (ypos >= canvas.GetSize().GetY()) break;
	}
	
	canvas.SetPen(*wxTRANSPARENT_PEN);
	canvas.SetBrush(*wxBLACK_BRUSH);
	
	canvas.DrawRectangle(0, 0, width, 30);
}

void TimelinePanel::render_row(wxDC &canvas, std::string rowname, KeyframeSet *keyframes, wxRect bounding_box) {
	canvas.SetPen(*wxBLACK_PEN);
	canvas.SetBrush(*wxTRANSPARENT_BRUSH);
	
	canvas.DrawText(rowname, bounding_box.GetTopLeft());
	canvas.DrawLine(bounding_box.GetBottomLeft(), bounding_box.GetBottomRight());
	
	canvas.SetPen(*wxTRANSPARENT_PEN);
	
	int colcount = 0;
	for (auto iter : keyframes->keyframes) {
		canvas.SetBrush(wxBrush(velocitycolors[((NoteKeyframe *)(iter))->velocity]));
		canvas.DrawRectangle(bounding_box.GetLeft()+30+(80*(iter->time)), bounding_box.GetTop(), bounding_box.GetWidth(), bounding_box.GetHeight());
		colcount++;
	}
	
	if (colcount > GetColumnCount()) SetColumnCount(colcount);
}

wxBEGIN_EVENT_TABLE(TimelinePanel, wxPanel)
	EVT_PAINT(TimelinePanel::paintEvent)
wxEND_EVENT_TABLE()

//draw the background rows
//button, beat labels
//click to create/move/recolor notes
//snap to grid
//zooming
