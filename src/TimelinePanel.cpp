//
// HOWL - Music-synced animation library
// File: TimelinePanel.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// https://www.nightwave.co
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
#include "LightpadProject.h"
#include "Layer.h"
#include "DisplayPanel.h"

using namespace HOWL;

wxDEFINE_EVENT(HOWL::DISPLAY_REFRESH, wxCommandEvent);
wxDEFINE_EVENT(HOWL::PLAYHEAD_MOVED, wxCommandEvent);

TimelinePanel::TimelinePanel(wxPanel *parent, wxWindowID window_id): wxHVScrolledWindow(parent, window_id, wxPoint(-1, -1), wxSize(-1, 250), wxBORDER_SUNKEN) {
	m_parent = parent;
	sizer = new wxBoxSizer(wxHORIZONTAL);
	
	rowsize = 20;
	colsize = 80;
	headersize = 30;
	labelsize = 40;
	ticksPerCol = activeProject->ticksPerBeat / 4;
	active_button = wxPoint(0, 0);
	
	int rows = 0;
	for (auto l: activeProject->layers) rows += l->keyframes.size();
	
	SetRowColumnCount(rows, 1);
	
	Update();
	
	movePlayhead(0);
}

TimelinePanel::~TimelinePanel() {
	
}

void TimelinePanel::paintEvent(wxPaintEvent &WXUNUSED(event)) {
	// depending on your system you may need to look at double-buffered dcs
	wxPaintDC canvas(this);
	render(canvas);
}

void TimelinePanel::paintNow() {
	// depending on your system you may need to look at double-buffered dcs
	wxClientDC canvas(this);
	render(canvas);
}

void TimelinePanel::onLeftDown(wxMouseEvent &event) {
	wxPoint mousepos = event.GetLogicalPosition(wxClientDC(this));
	wxPoint btn = mousepos_to_buttons(mousepos);
	if (mousepos.y < headersize) {
		headerclicked = true;
		movePlayhead(btn.x * ticksPerCol);
	} else {
		active_button = btn;
	}
	
	/*if (!event.ControlDown()) {
		for (int i = 0; i < 100; i++) selected_buttons[i] = false;
	}
	
	if (event.ShiftDown()) {
		wxRealPoint first_btn = buttonAtCoords(clickpos);
		for (int x = ceil(std::min(btn.x, first_btn.x)); x <= floor(std::max(btn.x, first_btn.x)); x++) {
			for (int y = ceil(std::min(btn.y, first_btn.y)); y <= floor(std::max(btn.y, first_btn.y)); y++) {
				selected_buttons[x + (y * 10)] = true;
			}
		}
	} else {
		if (btn.x == floor(btn.x) && btn.y == floor(btn.y)) selected_buttons[(int)(btn.x + (btn.y * 10))] = !selected_buttons[(int)(btn.x + (btn.y * 10))] ;
		clickpos = mousepos;
	}*/
	
	paintNow();
}

void TimelinePanel::onMouseMove(wxMouseEvent &event) {
	if (!event.LeftIsDown()) return;
	
	wxPoint mousepos = event.GetLogicalPosition(wxClientDC(this));
	wxPoint btn = mousepos_to_buttons(mousepos);
	if (headerclicked) movePlayhead(btn.x * ticksPerCol);

	/*wxRealPoint btn = buttonAtCoords(mousepos);
	wxRealPoint first_btn = buttonAtCoords(clickpos);
	
	if (btn.x == first_btn.x && btn.y == first_btn.y) return;
	
	for (int x = ceil(std::min(btn.x, first_btn.x)); x <= floor(std::max(btn.x, first_btn.x)); x++) {
		for (int y = ceil(std::min(btn.y, first_btn.y)); y <= floor(std::max(btn.y, first_btn.y)); y++) {
			selected_buttons_box[x + (y * 10)] = true;
		}
	}*/
	
	//paintNow();
}

void TimelinePanel::onLeftUp(wxMouseEvent &WXUNUSED(event)) {
	headerclicked = false;
}

void TimelinePanel::render(wxDC &canvas) {
	int width = canvas.GetSize().GetX();
	int xpos = labelsize-(GetVisibleBegin().GetCol()*colsize);
	int ypos = headersize-(GetVisibleBegin().GetRow()*rowsize);
	int cypos = ypos;
	for (auto l: activeProject->layers) {
		if (true /* l->isExpanded */) {
			for (auto kf : l->keyframes) {
				if (cypos >= headersize-rowsize) render_row(canvas, kf.first, kf.second, wxRect(0, cypos, width, rowsize));
				cypos += rowsize;
				if (cypos >= canvas.GetSize().GetY()) break;
			}
		}
	}
	
	render_header(canvas);
	render_playhead(canvas);
	
	canvas.SetPen(wxPen(*wxBLACK, 2));
	canvas.SetBrush(*wxWHITE_BRUSH);
	int left = active_button.x * colsize + xpos;
	int top = active_button.y * rowsize + ypos;
	if (left >= labelsize && top >= headersize) canvas.DrawRectangle(left, top, colsize, rowsize);
}

void TimelinePanel::render_row(wxDC &canvas, std::string rowname, KeyframeSet *keyframes, wxRect bounding_box) {
	int col1time = GetVisibleBegin().GetCol() * ticksPerCol;
	int colNtime = GetVisibleEnd().GetCol() * ticksPerCol;
	int lastCol = 0;
	
	canvas.SetPen(*wxBLACK_PEN);
	canvas.DrawLine(bounding_box.GetLeft(), bounding_box.GetHeight(), bounding_box.GetRight(), bounding_box.GetHeight());
	
	for (auto iter: keyframes->keyframes) {
		auto time = iter->time;
		// XXX Need to check currently active keyframes, not just keyframes starting before first column time
		//if (time < (col1time - activeProject->ticksPerBeat)) {
			// starting_velocity = velocity;
			// continue;
		// }
		// draw the box from 0 to iter->time with starting_velocity if it exists
		double col = (time - col1time) / (ticksPerCol * 1.0);
		if (col < 0.0) col = 0.0;
		if (col > lastCol) lastCol = (int)(col);
		int left = bounding_box.GetLeft()+labelsize+(col*colsize);

		wxRect kfbox(bounding_box);
		kfbox.SetLeft(left);
		iter->render(canvas, kfbox);
	}
	
	if (lastCol > GetColumnCount()) SetColumnCount(lastCol);
	
	canvas.SetPen(*wxBLACK_PEN);
	canvas.SetBrush(*wxTRANSPARENT_BRUSH);
	
	canvas.DrawText(rowname, bounding_box.GetTopLeft());
	canvas.DrawLine(bounding_box.GetBottomLeft(), bounding_box.GetBottomRight());
}

std::string TimelinePanel::get_header_string(int col) {
	char buf[16];
	int colsPerBeat = activeProject->ticksPerBeat / ticksPerCol;
	
	int measure = col / (colsPerBeat * activeProject->beatsPerMeasure) + 1;
	int beat = col / colsPerBeat % activeProject->beatsPerMeasure + 1;
	int fb = col % colsPerBeat + 1;
	if (beat == 1 && fb == 1) snprintf(buf, sizeof(buf), "%d", measure);
	else if (fb == 1) snprintf(buf, sizeof(buf), "%d.%d", measure, beat);
	else snprintf(buf, sizeof(buf), "%d.%d.%d", measure, beat, fb);
	
	return std::string(buf);
}

void TimelinePanel::render_header(wxDC &canvas) {
	int width = canvas.GetSize().GetX();
	int col = GetVisibleBegin().GetCol();
	
	canvas.SetPen(*wxTRANSPARENT_PEN);
	canvas.SetBrush(*wxWHITE_BRUSH);
	canvas.DrawRectangle(0, 0, width, headersize);
	
	canvas.SetPen(*wxBLACK_PEN);
	canvas.SetBrush(*wxTRANSPARENT_BRUSH);
	
	for (int x = labelsize; x < width; x += colsize) {
		canvas.DrawLine(x, 0, x, headersize-2);
		canvas.DrawText(get_header_string(col), x+4, 0);
		col++;
	}
	
	canvas.SetPen(wxPen(*wxBLACK, 3));
	canvas.DrawLine(0, headersize-2, width, headersize-2);
}

void TimelinePanel::render_playhead(wxDC &canvas) {
	int xpos = playhead_in_pixels() - offset_in_pixels().x + labelsize;
	
	if (xpos >= labelsize) {
		canvas.SetPen(wxPen(*wxBLACK, 6));
		canvas.DrawLine(xpos, headersize, xpos, canvas.GetSize().GetHeight());
		canvas.SetPen(wxPen(*wxWHITE, 3));
		canvas.DrawLine(xpos, headersize, xpos, canvas.GetSize().GetHeight());
		canvas.DrawCircle(xpos, headersize, 5.0);
	}
}

void TimelinePanel::nextBeat() {
	movePlayhead(playhead+activeProject->ticksPerBeat);
}

void TimelinePanel::nextQuarterBeat() {
	movePlayhead(playhead+ticksPerCol);
}

void TimelinePanel::movePlayhead(int time) {
	int phCol = time / ticksPerCol;
	activeProject->seek(time);
	
	wxCommandEvent fin_evt(PLAYHEAD_MOVED, ID_Panel_Display);
	fin_evt.SetEventObject(this);
	wxPostEvent(wxWindow::FindWindowById(ID_Panel_Display), fin_evt);
	playhead = time;
	
	if (phCol >= GetVisibleEnd().GetCol() || phCol < GetVisibleBegin().GetCol()) {
		ScrollToColumn(phCol == 0 ? 0 : phCol - 1);
	}

	Refresh();
}

wxCoord TimelinePanel::OnGetRowHeight(size_t row) const {
	return rowsize;
}

wxCoord TimelinePanel::OnGetColumnWidth(size_t column) const {
	return colsize;
	//	return wxClientDC(this).GetSize().GetX();
}

int TimelinePanel::playhead_in_pixels() {
	return colsize * activeProject->currentTime / activeProject->ticksPerBeat * 4;
}

wxPoint TimelinePanel::offset_in_pixels() {
	return wxPoint(colsize * GetVisibleBegin().GetCol(), rowsize * GetVisibleBegin().GetRow());
}

wxPoint TimelinePanel::mousepos_to_buttons(wxPoint mousepos) {
	return wxPoint(
		(mousepos.x + offset_in_pixels().x - labelsize)  / colsize,
		(mousepos.y + offset_in_pixels().y - headersize) / rowsize
	);
}

wxBEGIN_EVENT_TABLE(TimelinePanel, wxPanel)
	EVT_PAINT(TimelinePanel::paintEvent)
	EVT_LEFT_DOWN(TimelinePanel::onLeftDown)
	EVT_MOTION(TimelinePanel::onMouseMove)
	EVT_LEFT_UP(TimelinePanel::onLeftUp)
wxEND_EVENT_TABLE()

//click to create/move/recolor notes
//snap to grid
//zooming
