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

#ifdef __WXGTK20__
	#include <gtk/gtk.h>
#endif

#include "wx/dcmemory.h"
#include "wx/dcclient.h"

#include "DCHelpers.h"

BEGIN_EVENT_TABLE(SplashScreen, wxFrame)
	EVT_CLOSE(SplashScreen::OnCloseWindow)
	#ifdef __WXGTK__
		EVT_PAINT(SplashScreen::OnPaint)
	#endif
	EVT_ERASE_BACKGROUND(SplashScreen::OnEraseBackground)
END_EVENT_TABLE()

static void DrawSplashBitmap(wxDC &canvas, const wxBitmap &bitmap, const wxString copyright, const wxRect copyrightbox, const wxFont copyrightfont, const wxColor copyrightcolor) {
	wxMemoryDC dcMem;
	
	bool hiColour = (wxDisplayDepth() >= 16);
	if (bitmap.GetPalette() && !hiColour) dcMem.SetPalette(* bitmap.GetPalette());
	
	dcMem.SelectObjectAsSource(bitmap);
	canvas.Blit(0, 0, bitmap.GetWidth(), bitmap.GetHeight(), &dcMem, 0, 0, wxCOPY, true);
	dcMem.SelectObject(wxNullBitmap);
	
	if (copyright.size() > 0) {
		canvas.SetTextForeground(copyrightcolor);
		canvas.SetFont(copyrightfont);
        DrawWrappedText(std::string(copyright.c_str()), canvas, copyrightbox);
	}
	
	if (bitmap.GetPalette() && !hiColour) dcMem.SetPalette(wxNullPalette);
}

SplashScreen::SplashScreen() {
	Init();
}

SplashScreen::SplashScreen(wxWindow *parent, wxWindowID window_id, wxBitmap &bitmap, wxString copyright, wxRect copyrightbox, wxColor copyrightcolor, wxFont copyrightfont) : wxFrame() {
	Create(parent, window_id, wxEmptyString, wxPoint(0,0), wxSize(100, 100), wxSIMPLE_BORDER | wxSTAY_ON_TOP | wxFRAME_TOOL_WINDOW | wxFRAME_NO_TASKBAR | wxTRANSPARENT_WINDOW);
	
	SetBackgroundColour(wxTransparentColor);
	Refresh();
	wxEvtHandler::AddFilter(this);

	// splash screen must not be used as parent by the other windows because it
	// is going to disappear soon, indicate it by giving it this special style
	SetExtraStyle(GetExtraStyle() | wxWS_EX_TRANSIENT);

	#if defined(__WXGTK20__)
		gtk_window_set_type_hint(GTK_WINDOW(m_widget), GDK_WINDOW_TYPE_HINT_SPLASHSCREEN);
	#endif

	m_bitmap = bitmap;
	m_copyright = copyright;
	m_copyrightbox = copyrightbox;
	m_copyrightfont = copyrightfont;
	m_copyrightcolor = copyrightcolor;
	
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

void SplashScreen::OnPaint(wxPaintEvent &WXUNUSED(event)) {
	wxPaintDC canvas(this);
	canvas.SetBackgroundMode(wxTRANSPARENT);
	canvas.SetBackground(*wxTRANSPARENT_BRUSH);
	if (m_bitmap.IsOk()) DrawSplashBitmap(canvas, m_bitmap, m_copyright, m_copyrightbox, m_copyrightfont, m_copyrightcolor);
}

void SplashScreen::OnEraseBackground(wxEraseEvent &event) {
	if (event.GetDC() && m_bitmap.IsOk()) {
		wxDC *canvas = event.GetDC();
		canvas->SetBackgroundMode(wxTRANSPARENT);
		canvas->SetBackground(*wxTRANSPARENT_BRUSH);
		DrawSplashBitmap(*canvas, m_bitmap, m_copyright, m_copyrightbox, m_copyrightfont, m_copyrightcolor);
	} else {
		wxClientDC canvas(this);
		canvas.SetBackgroundMode(wxTRANSPARENT);
		canvas.SetBackground(*wxTRANSPARENT_BRUSH);
		if (m_bitmap.IsOk()) DrawSplashBitmap(canvas, m_bitmap, m_copyright, m_copyrightbox, m_copyrightfont, m_copyrightcolor);
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

void SplashScreen::SetCopyrightColor(wxColor copyrightcolor) {
	m_copyrightcolor = copyrightcolor;
}

wxColor SplashScreen::GetCopyrightColor() {
	return m_copyrightcolor;
}

void SplashScreen::SetCopyrightFont(wxFont copyrightfont) {
	m_copyrightfont = copyrightfont;
}

wxFont SplashScreen::GetCopyrightFont() {
	return m_copyrightfont;
}

int SplashScreen::FilterEvent(wxEvent &event) {
	const wxEventType type = event.GetEventType();
	if (type == wxEVT_KEY_DOWN || type == wxEVT_LEFT_DOWN || type == wxEVT_RIGHT_DOWN || type == wxEVT_MIDDLE_DOWN)
		Close(true);
	
	return -1;
}
