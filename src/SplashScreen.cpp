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

// ----------------------------------------------------------------------------
// SplashScreen
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(SplashScreen, wxFrame)
BEGIN_EVENT_TABLE(SplashScreen, wxFrame)
    EVT_CLOSE(SplashScreen::OnCloseWindow)
	#ifdef __WXGTK__
		EVT_PAINT(SplashScreen::OnPaint)
	#endif
	EVT_ERASE_BACKGROUND(SplashScreen::OnEraseBackground)
END_EVENT_TABLE()

static void DrawSplashBitmap(wxDC& dc, const wxBitmap& bitmap, int WXUNUSED(x), int WXUNUSED(y)) {
	wxMemoryDC dcMem;
	
	bool hiColour = (wxDisplayDepth() >= 16);
	if (bitmap.GetPalette() && !hiColour) dcMem.SetPalette(* bitmap.GetPalette());
	
	dcMem.SelectObjectAsSource(bitmap);
	dc.Blit(0, 0, bitmap.GetWidth(), bitmap.GetHeight(), &dcMem, 0, 0, wxCOPY, true);
	dcMem.SelectObject(wxNullBitmap);
	
	if (bitmap.GetPalette() && !hiColour) dcMem.SetPalette(wxNullPalette);
}

void SplashScreen::Init() {
    //m_window = NULL;

    wxEvtHandler::AddFilter(this);
}

/* Note that unless we pass a non-default size to the frame, SetClientSize
 * won't work properly under Windows, and the splash screen frame is sized
 * slightly too small.
 */

SplashScreen::SplashScreen(const wxBitmap& bitmap, long splashStyle,
                               wxWindow* parent, wxWindowID id, const wxPoint& pos,
                               const wxSize& size, long style)
    : wxFrame() {
    //SetBackgroundStyle(wxBG_STYLE_TRANSPARENT);
    Create(parent, id, wxEmptyString, wxPoint(0,0), wxSize(100, 100),
            style | wxFRAME_TOOL_WINDOW | wxFRAME_NO_TASKBAR);
    Init();

    // splash screen must not be used as parent by the other windows because it
    // is going to disappear soon, indicate it by giving it this special style
    SetExtraStyle(GetExtraStyle() | wxWS_EX_TRANSIENT);

	#if defined(__WXGTK20__)
		gtk_window_set_type_hint(GTK_WINDOW(m_widget), GDK_WINDOW_TYPE_HINT_SPLASHSCREEN);
	#endif

    m_splashStyle = splashStyle;
	m_bitmap = bitmap;

    //m_window = new SplashScreenWindow(bitmap, this, wxID_ANY, pos, size, wxNO_BORDER);
	
	#if !defined(__WXGTK__) && wxUSE_PALETTE
		bool hiColour = (wxDisplayDepth() >= 16);
		
		if (bitmap.GetPalette() && !hiColour) SetPalette(* bitmap.GetPalette());
	#endif

    SetClientSize(bitmap.GetWidth(), bitmap.GetHeight());

    if (m_splashStyle & SPLASH_CENTRE_ON_PARENT) CentreOnParent();
    else if (m_splashStyle & SPLASH_CENTRE_ON_SCREEN) CentreOnScreen();

    Show(true);
    //m_window->SetFocus();
	Update(); // Without this, you see a blank screen for an instant
}

SplashScreen::~SplashScreen() {
    wxEvtHandler::RemoveFilter(this);
}

int SplashScreen::FilterEvent(wxEvent& event) {
    const wxEventType t = event.GetEventType();
    if (t == wxEVT_KEY_DOWN || t == wxEVT_LEFT_DOWN || t == wxEVT_RIGHT_DOWN || t == wxEVT_MIDDLE_DOWN)
        Close(true);

    return -1;
}

void SplashScreen::OnNotify(wxTimerEvent& WXUNUSED(event)) {
    Close(true);
}

void SplashScreen::OnCloseWindow(wxCloseEvent& WXUNUSED(event)) {
    this->Destroy();
}

/*SplashScreenWindow::SplashScreenWindow(const wxBitmap& bitmap, wxWindow* parent,
                                           wxWindowID id, const wxPoint& pos,
                                           const wxSize& size, long style)
    : wxWindow() {
    Create(parent, id, pos, size, style);
}*/

void SplashScreen::OnPaint(wxPaintEvent& WXUNUSED(event)) {
    wxPaintDC dc(this);
	dc.SetBackgroundMode(wxTRANSPARENT);
	dc.SetBackground(*wxTRANSPARENT_BRUSH);
    if (m_bitmap.IsOk()) DrawSplashBitmap(dc, m_bitmap, 0, 0);
}

void SplashScreen::OnEraseBackground(wxEraseEvent& event) {
    if (event.GetDC() && m_bitmap.IsOk()) {
		wxDC *dc = event.GetDC();
		dc->SetBackgroundMode(wxTRANSPARENT);
		dc->SetBackground(*wxTRANSPARENT_BRUSH);
        DrawSplashBitmap(*dc, m_bitmap, 0, 0);
    } else {
        wxClientDC dc(this);
		dc.SetBackgroundMode(wxTRANSPARENT);
		dc.SetBackground(*wxTRANSPARENT_BRUSH);
        if (m_bitmap.IsOk()) DrawSplashBitmap(dc, m_bitmap, 0, 0);
    }
}
