//
// Nightwave Global - SplashScreen.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// https://www.nightwave.co/
//

//
// This file is a modified copy from the wxWidgets wxSplashScreen class
//

#include "SplashScreen.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

// #ifdef __WXGTK20__
// 	#include <gtk/gtk.h>
// #endif

#include "wx/dcmemory.h"
#include "wx/dcclient.h"

#include "DCHelpers.h"

BEGIN_EVENT_TABLE(SplashScreen, wxFrame)
	EVT_CLOSE(SplashScreen::OnCloseWindow)
	EVT_PAINT(SplashScreen::OnPaint)
	EVT_ERASE_BACKGROUND(SplashScreen::OnEraseBackground)
END_EVENT_TABLE()

SplashScreen::SplashScreen() {
	// Init();
}

SplashScreen::SplashScreen(wxWindow *parent, wxWindowID window_id, wxBitmap &bitmap, wxString copyright, wxRect copyrightbox, wxColor textcolor, wxFont textfont, wxRect loadingtextbox, wxRect loadingbarbox) : wxFrame() {
	Create(parent, window_id, wxEmptyString, wxPoint(0,0), wxSize(100, 100), wxSIMPLE_BORDER | wxSTAY_ON_TOP | wxFRAME_TOOL_WINDOW | wxFRAME_NO_TASKBAR | wxTRANSPARENT_WINDOW);
	
	SetBackgroundColour(wxTransparentColor);
	Refresh();
	wxEvtHandler::AddFilter(this);

	// splash screen must not be used as parent by the other windows because it
	// is going to disappear soon, indicate it by giving it this special style
	SetExtraStyle(GetExtraStyle() | wxWS_EX_TRANSIENT);

	// #if defined(__WXGTK20__)
	// 	gtk_window_set_type_hint(GTK_WINDOW(m_widget), GDK_WINDOW_TYPE_HINT_SPLASHSCREEN);
	// #endif

	m_bitmap = bitmap;
	m_copyright = copyright;
	m_copyrightbox = copyrightbox;
	m_textfont = textfont;
	m_textcolor = textcolor;
	m_loadingtextbox = loadingtextbox;
	m_loadingbarbox = loadingbarbox;
	m_progress = 0;
	m_progresstext = "Loading...";
	
	#if !defined(__WXGTK__) && wxUSE_PALETTE
		bool hiColour = (wxDisplayDepth() >= 16);
		if (bitmap.GetPalette() && !hiColour) SetPalette(* bitmap.GetPalette());
	#endif

	SetClientSize(bitmap.GetWidth(), bitmap.GetHeight());
	CentreOnScreen();
	Show(true);
	Update(); // Without this, you see a blank screen for an instant
}

SplashScreen::~SplashScreen() {
	wxEvtHandler::RemoveFilter(this);
}

void SplashScreen::OnCloseWindow(wxCloseEvent &WXUNUSED(event)) {
	this->Destroy();
}

void SplashScreen::OnNotify(wxTimerEvent &WXUNUSED(event)) {
	Close(true);
}

void SplashScreen::OnEraseBackground(wxEraseEvent &event) {
	if (event.GetDC() && m_bitmap.IsOk()) {
		wxDC *canvas = event.GetDC();
		render(*canvas);
	} else {
		wxClientDC canvas(this);
		render(canvas);
	}
}

void SplashScreen::OnPaint(wxPaintEvent &WXUNUSED(event)) {
	wxPaintDC canvas(this);
	render(canvas);
}

void SplashScreen::render(wxDC &canvas) {
	canvas.SetBackgroundMode(wxTRANSPARENT);
	canvas.SetBackground(*wxTRANSPARENT_BRUSH);
	if (m_bitmap.IsOk()) {
		wxMemoryDC dcMem;
		
		bool hiColour = (wxDisplayDepth() >= 16);
		if (m_bitmap.GetPalette() && !hiColour) dcMem.SetPalette(* m_bitmap.GetPalette());
		
		dcMem.SelectObjectAsSource(m_bitmap);
		canvas.Blit(0, 0, m_bitmap.GetWidth(), m_bitmap.GetHeight(), &dcMem, 0, 0, wxCOPY, true);
		dcMem.SelectObject(wxNullBitmap);
		
		if (m_copyright.size() > 0) {
			canvas.SetTextForeground(m_textcolor);
			canvas.SetFont(m_textfont);
			DrawWrappedText(std::string(m_copyright.c_str()), canvas, m_copyrightbox);
		}
		
		if (m_loadingtextbox.GetSize() != wxSize(0, 0)) {
			canvas.SetTextForeground(m_textcolor);
			canvas.SetFont(m_textfont);
			canvas.DrawLabel(m_progresstext, m_loadingtextbox, wxALIGN_CENTER|wxALIGN_TOP);
		}
		
		if (m_loadingbarbox.GetSize() != wxSize(0, 0)) {
			canvas.SetPen(wxPen(m_textcolor, 2));
			canvas.SetBrush(*wxTRANSPARENT_BRUSH);
			canvas.DrawRectangle(m_loadingbarbox);
			
			canvas.SetPen(*wxTRANSPARENT_PEN);
			wxColor halfopaque = wxColour(m_textcolor.Red(), m_textcolor.Green(), m_textcolor.Blue(), 127);
			canvas.SetBrush(wxBrush(halfopaque));
			int right = m_loadingbarbox.GetWidth() * m_progress / 100.0;
			if (right < 5) right = 5;
			canvas.DrawRectangle(m_loadingbarbox.GetX() + 1, m_loadingbarbox.GetY() + 1, right - (m_progress == 100.0 ? 2 : 5), m_loadingbarbox.GetHeight() - 2);
			canvas.SetBrush(wxBrush(m_textcolor));
			if (right < 9) right = 9;
			canvas.DrawRectangle(m_loadingbarbox.GetX() + 4, m_loadingbarbox.GetY() + 4, right - 9, m_loadingbarbox.GetHeight() - 9);
		}
		
		if (m_bitmap.GetPalette() && !hiColour) dcMem.SetPalette(wxNullPalette);
	}
}

void SplashScreen::SetBitmap(wxBitmap &bitmap) {
	m_bitmap = bitmap;
}

wxBitmap &SplashScreen::GetBitmap() {
	return m_bitmap;
}

void SplashScreen::SetCopyright(wxString copyright) {
	m_copyright = copyright;
}

wxString SplashScreen::GetCopyright() {
	return m_copyright;
}

void SplashScreen::SetCopyrightBox(wxRect copyrightbox) {
	m_copyrightbox = copyrightbox;
}

wxRect SplashScreen::GetCopyrightBox() {
	return m_copyrightbox;
}

void SplashScreen::SetTextColor(wxColor textcolor) {
	m_textcolor = textcolor;
}

wxColor SplashScreen::GetTextColor() {
	return m_textcolor;
}

void SplashScreen::SetTextFont(wxFont textfont) {
	m_textfont = textfont;
}

wxFont SplashScreen::GetTextFont() {
	return m_textfont;
}

void SplashScreen::SetProgress(int progress) {
	m_progress = progress;
	Refresh();
	wxYield();
}

void SplashScreen::SetProgress(int progress, std::string progresstext) {
	m_progresstext = progresstext;
	SetProgress(progress);
}

int SplashScreen::GetProgress() {
	return m_progress;
}

int SplashScreen::FilterEvent(wxEvent &event) {
	const wxEventType type = event.GetEventType();
	if (type == wxEVT_KEY_DOWN || type == wxEVT_LEFT_DOWN || type == wxEVT_RIGHT_DOWN || type == wxEVT_MIDDLE_DOWN) {
		if (m_progress == 100.0 || m_loadingbarbox.GetSize() == wxSize(0, 0)) Close(true);
	}
	
	return -1;
}
