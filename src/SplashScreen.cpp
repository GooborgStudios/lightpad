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

#include <sstream>
#include <queue>
#include <vector>

BEGIN_EVENT_TABLE(SplashScreen, wxFrame)
	EVT_CLOSE(SplashScreen::OnCloseWindow)
	#ifdef __WXGTK__
		EVT_PAINT(SplashScreen::OnPaint)
	#endif
	EVT_ERASE_BACKGROUND(SplashScreen::OnEraseBackground)
END_EVENT_TABLE()


void DrawWrappedText(std::string text, wxDC &dc, wxRect box) {
	std::queue<std::string> words;
	std::vector<std::string> *lines = new std::vector<std::string>();
	
	std::stringstream ssin(text);
	std::string word;
	while (ssin.good()) {
		ssin >> word;
		words.push(word);
	}
	
	std::string currentLine = words.front();
	words.pop();
	while (!words.empty()) {
		std::string c = currentLine + " " + words.front();
		wxSize size = dc.GetTextExtent(c);
		if (size.GetWidth() > box.GetWidth()) {
			lines->push_back(currentLine);
			currentLine = words.front();
		} else {
			currentLine = c;
		}
		
		words.pop();
	}
	
	lines->push_back(currentLine);
    
    for (std::string line : *lines) {
        dc.DrawLabel(line, box, wxALIGN_CENTER|wxALIGN_TOP);
        box.SetTop(box.GetTop() + dc.GetTextExtent(line).GetHeight() + 4);
    }
    
    delete lines;
}

static void DrawSplashBitmap(wxDC &dc, const wxBitmap &bitmap, const wxString copyright, const wxRect copyrightbox, const wxFont copyrightfont, const wxColor copyrightcolor) {
	wxMemoryDC dcMem;
	
	bool hiColour = (wxDisplayDepth() >= 16);
	if (bitmap.GetPalette() && !hiColour) dcMem.SetPalette(* bitmap.GetPalette());
	
	dcMem.SelectObjectAsSource(bitmap);
	dc.Blit(0, 0, bitmap.GetWidth(), bitmap.GetHeight(), &dcMem, 0, 0, wxCOPY, true);
	dcMem.SelectObject(wxNullBitmap);
	
	if (copyright.size() > 0) {
		dc.SetTextForeground(copyrightcolor);
		dc.SetFont(copyrightfont);
        DrawWrappedText(std::string(copyright.c_str()), dc, copyrightbox);
	}
	
	if (bitmap.GetPalette() && !hiColour) dcMem.SetPalette(wxNullPalette);
}

SplashScreen::SplashScreen() {
	Init();
}

SplashScreen::SplashScreen(wxWindow *parent, wxWindowID id, wxBitmap &bitmap, wxString copyright, wxRect copyrightbox, wxColor copyrightcolor, wxFont copyrightfont) : wxFrame() {
	Create(parent, id, wxEmptyString, wxPoint(0,0), wxSize(100, 100), wxSIMPLE_BORDER | wxSTAY_ON_TOP | wxFRAME_TOOL_WINDOW | wxFRAME_NO_TASKBAR | wxTRANSPARENT_WINDOW);
	
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
	wxPaintDC dc(this);
	dc.SetBackgroundMode(wxTRANSPARENT);
	dc.SetBackground(*wxTRANSPARENT_BRUSH);
	if (m_bitmap.IsOk()) DrawSplashBitmap(dc, m_bitmap, m_copyright, m_copyrightbox, m_copyrightfont, m_copyrightcolor);
}

void SplashScreen::OnEraseBackground(wxEraseEvent &event) {
	if (event.GetDC() && m_bitmap.IsOk()) {
		wxDC *dc = event.GetDC();
		dc->SetBackgroundMode(wxTRANSPARENT);
		dc->SetBackground(*wxTRANSPARENT_BRUSH);
		DrawSplashBitmap(*dc, m_bitmap, m_copyright, m_copyrightbox, m_copyrightfont, m_copyrightcolor);
	} else {
		wxClientDC dc(this);
		dc.SetBackgroundMode(wxTRANSPARENT);
		dc.SetBackground(*wxTRANSPARENT_BRUSH);
		if (m_bitmap.IsOk()) DrawSplashBitmap(dc, m_bitmap, m_copyright, m_copyrightbox, m_copyrightfont, m_copyrightcolor);
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
	const wxEventType t = event.GetEventType();
	if (t == wxEVT_KEY_DOWN || t == wxEVT_LEFT_DOWN || t == wxEVT_RIGHT_DOWN || t == wxEVT_MIDDLE_DOWN)
		Close(true);
	
	return -1;
}
