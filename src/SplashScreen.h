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

/*
 * SplashScreen
 */

class SplashScreen: public wxFrame, public wxEventFilter {
    public:
        SplashScreen() { Init(); }
        SplashScreen(wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, std::string text, wxRect textbox, wxFont textfont, wxColor textcolor);
        virtual ~SplashScreen();

        void OnCloseWindow(wxCloseEvent& event);
        void OnNotify(wxTimerEvent& event);
        
        void OnPaint(wxPaintEvent& event);
        void OnEraseBackground(wxEraseEvent& event);
        
        void SetBitmap(const wxBitmap& bitmap) { m_bitmap = bitmap; }
        wxBitmap& GetBitmap() { return m_bitmap; }

        // Override wxEventFilter method to hide splash screen on any user input.
        virtual int FilterEvent(wxEvent& event);

    protected:
        wxBitmap m_bitmap;
		wxString m_text;
		wxRect m_textbox;
		wxFont m_textfont;
		wxColor m_textcolor;

        DECLARE_DYNAMIC_CLASS(SplashScreen)
        DECLARE_EVENT_TABLE()
        wxDECLARE_NO_COPY_CLASS(SplashScreen);
};
