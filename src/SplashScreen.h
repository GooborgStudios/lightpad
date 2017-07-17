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

class SplashScreen: public wxFrame, public wxEventFilter {
	public:
		SplashScreen();
		SplashScreen(wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, wxString text = "", wxRect textbox = wxRect(0, 0, 0, 0), wxColor textcolor = *wxBLACK, wxFont textfont = wxFont());
		virtual ~SplashScreen();

		void OnCloseWindow(wxCloseEvent& event);
		void OnNotify(wxTimerEvent& event);
		void OnPaint(wxPaintEvent& event);
		void OnEraseBackground(wxEraseEvent& event);
		
		void SetBitmap(const wxBitmap& bitmap);
		wxBitmap& GetBitmap();

		virtual int FilterEvent(wxEvent& event);

	protected:
		wxBitmap m_bitmap;
		wxString m_text;
		wxRect m_textbox;
		wxFont m_textfont;
		wxColor m_textcolor;

		DECLARE_EVENT_TABLE()
		wxDECLARE_NO_COPY_CLASS(SplashScreen);
};
