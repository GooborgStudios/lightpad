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
#include <wx/clrpicker.h>

#include "ElementIDs.h"
#include "Project.h"
#include "Layer.h"
#include "MidiLayer.h"
#include "Launchpad.h"
#include "DisplayPanel.h"

wxDEFINE_EVENT(DISPLAY_REFRESH, wxCommandEvent);
wxDEFINE_EVENT(PLAYHEAD_MOVED, wxCommandEvent);

TimelinePanel::TimelinePanel(wxPanel *parent): wxHVScrolledWindow(parent, ID_Panel_Timeline, wxPoint(-1, -1), wxSize(-1, 250), wxBORDER_SUNKEN) {
	m_parent = parent;
	sizer = new wxBoxSizer(wxHORIZONTAL);
	
	rowsize = 20;
	colsize = 80;
	headersize = 30;
	labelsize = 40;
	
	SetRowColumnCount(97, 1);
	
	Update();
	
	movePlayhead(0);
}

TimelinePanel::~TimelinePanel() {
	
}

wxCoord TimelinePanel::OnGetRowHeight(size_t row) const {
	return rowsize;
}

wxCoord TimelinePanel::OnGetColumnWidth(size_t column) const {
	return colsize;
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
	int ypos = headersize-(GetVisibleBegin().GetRow()*rowsize);
	for (auto iter : activeProject->layer->keyframes) {
		if (ypos >= headersize-rowsize) render_row(canvas, iter.first, iter.second, wxRect(0, ypos, width, rowsize));
		ypos += rowsize;
		if (ypos >= canvas.GetSize().GetY()) break;
	}
	
	render_header(canvas);
	render_playhead(canvas);
}

void TimelinePanel::render_row(wxDC &canvas, std::string rowname, KeyframeSet *keyframes, wxRect bounding_box) {
	canvas.SetPen(*wxTRANSPARENT_PEN);
	int colsPerBeat = 4;
	int ticksPerCol = activeProject->ticksPerBeat / colsPerBeat;
	int col1time = GetVisibleBegin().GetCol() * ticksPerCol;
	int colNtime = GetVisibleEnd().GetCol() * ticksPerCol;
	int lastCol = 0;
	
	for (auto iter: keyframes->keyframes) {
		auto time = iter->time;
		if (time < (col1time - activeProject->ticksPerBeat)) continue;
		//if (time > colNtime ) return;
		int col = (time - col1time) / ticksPerCol;
		if (col < 0) col = 0;
		if (col > lastCol) lastCol = col;
		canvas.SetBrush(wxBrush(velocitycolors[((NoteKeyframe *)(iter))->velocity]));
		canvas.DrawRectangle(bounding_box.GetLeft()+labelsize+(col*colsize), bounding_box.GetTop(), bounding_box.GetWidth(), bounding_box.GetHeight());
	}
	
	if (lastCol > GetColumnCount()) SetColumnCount(lastCol);
	
	/*int colcount = 0;
	int xpos = GetVisibleBegin().GetCol() * colsize;
	int colsPerBeat = 4;
	for (auto iter : keyframes->keyframes) {
		int left = (colsize*(iter->time)/activeProject->ticksPerBeat * colsPerBeat);
		if (left < xpos) continue;
		canvas.SetBrush(wxBrush(velocitycolors[((NoteKeyframe *)(iter))->velocity]));
		canvas.DrawRectangle(bounding_box.GetLeft()+labelsize+left, bounding_box.GetTop(), bounding_box.GetWidth(), bounding_box.GetHeight());
		colcount++;
	}*/
	
	//if (colcount > GetColumnCount()) SetColumnCount(colcount);
	
	canvas.SetPen(*wxBLACK_PEN);
	canvas.SetBrush(*wxTRANSPARENT_BRUSH);
	
	canvas.DrawText(rowname, bounding_box.GetTopLeft());
	canvas.DrawLine(bounding_box.GetBottomLeft(), bounding_box.GetBottomRight());
}

void TimelinePanel::render_header(wxDC &canvas) {
	int width = canvas.GetSize().GetX();
	int col = GetVisibleBegin().GetCol();
	char buf[16];
	
	canvas.SetPen(*wxTRANSPARENT_PEN);
	canvas.SetBrush(*wxWHITE_BRUSH);
	
	canvas.DrawRectangle(0, 0, width, headersize);
	
	canvas.SetPen(*wxBLACK_PEN);
	canvas.SetBrush(*wxTRANSPARENT_BRUSH);
	
	for (int x = labelsize; x < width; x += colsize) {
		snprintf(buf, 7, "%d.%d.%d", col / 16 + 1, col / 4 % 4 + 1, col % 4 + 1);
		canvas.DrawLine(x, 0, x, headersize-2);
		canvas.DrawText(buf, x+4, 0);
		col++;
	}
	
	canvas.SetPen(wxPen(*wxBLACK, 3));
	
	canvas.DrawLine(0, headersize-2, width, headersize-2);
}

void TimelinePanel::render_playhead(wxDC &canvas) {
	int x = (colsize * activeProject->currentTime / activeProject->ticksPerBeat * 4) - (colsize * GetVisibleBegin().GetCol()) + labelsize;
	
	if (x >= labelsize) {
		canvas.SetPen(wxPen(*wxBLACK, 6));
		canvas.DrawLine(x, headersize, x, canvas.GetSize().GetHeight());
		canvas.SetPen(wxPen(*wxWHITE, 3));
		canvas.DrawLine(x, headersize, x, canvas.GetSize().GetHeight());
		canvas.DrawCircle(x, headersize, 5.0);
	}
}

void TimelinePanel::nextBeat() {
	movePlayhead(playhead+activeProject->ticksPerBeat);
}

void TimelinePanel::nextQuarterBeat() {
	movePlayhead(playhead+(activeProject->ticksPerBeat / 4));
}

void TimelinePanel::movePlayhead(int time) {
	int phCol = time / activeProject->ticksPerBeat * 4;
	activeProject->seek(time);
	for (auto iter : activeProject->layer->keyframes) {
		wxColourPickerEvent evt(this, ID_Panel_Timeline, velocitycolors[activeProject->layer->getVelocity(iter.first)]);
		evt.SetInt(std::stoi(iter.first));
		wxPostEvent(wxWindow::FindWindowById(ID_Panel_Display), evt);
	}
	
	wxCommandEvent fin_evt(DISPLAY_REFRESH, ID_Panel_Display);
	fin_evt.SetEventObject(this);
	wxPostEvent(wxWindow::FindWindowById(ID_Panel_Display), fin_evt);
	playhead = time;
	
	if (phCol >= GetVisibleEnd().GetCol() || phCol < GetVisibleBegin().GetCol()) {
		ScrollToColumn(phCol == 0 ? 0 : phCol - 1);
	}

	
	Refresh();
}

wxBEGIN_EVENT_TABLE(TimelinePanel, wxPanel)
	EVT_PAINT(TimelinePanel::paintEvent)
wxEND_EVENT_TABLE()

//draw the background rows
//button, beat labels
//click to create/move/recolor notes
//snap to grid
//zooming
