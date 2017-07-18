//
// Nightwave Global - SplashScreen.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// https://www.nightwave.co/
//

//
// This file is a modified copy from the wxWidgets wxSplashScreen class
//

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "wx/bitmap.h"
#include "wx/eventfilter.h"
#include "wx/frame.h"
#include "wx/timer.h"

void DrawWrappedText(std::string text, wxDC &dc, wxRect box);

class SplashScreen: public wxFrame, public wxEventFilter {
	public:
		SplashScreen();
		SplashScreen(wxWindow *parent, wxWindowID id, wxBitmap &bitmap, wxString copyright = "", wxRect copyrightbox = wxRect(0, 0, 0, 0), wxColor copyrightcolor = *wxBLACK, wxFont copyrightfont = wxFont());
		virtual ~SplashScreen();

		void OnCloseWindow(wxCloseEvent& event);
		void OnNotify(wxTimerEvent& event);
		void OnPaint(wxPaintEvent& event);
		void OnEraseBackground(wxEraseEvent& event);
		
		wxBitmap& GetBitmap();
		void SetBitmap(wxBitmap &bitmap);

		virtual int FilterEvent(wxEvent& event);

	protected:
		wxBitmap m_bitmap;
		wxString m_copyright;
		wxRect m_copyrightbox;
		wxFont m_copyrightfont;
		wxColor m_copyrightcolor;

		DECLARE_EVENT_TABLE()
		wxDECLARE_NO_COPY_CLASS(SplashScreen);
};
