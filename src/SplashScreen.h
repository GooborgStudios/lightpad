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
 * A window for displaying a splash screen
 */

class WXDLLIMPEXP_FWD_ADV SplashScreenWindow;

/*
 * SplashScreen
 */

class WXDLLIMPEXP_ADV SplashScreen: public wxFrame, public wxEventFilter {
    public:
        SplashScreen() { Init(); }
        SplashScreen(const wxBitmap& bitmap, wxWindow* parent, wxWindowID id);
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

        DECLARE_DYNAMIC_CLASS(SplashScreen)
        DECLARE_EVENT_TABLE()
        wxDECLARE_NO_COPY_CLASS(SplashScreen);
};

/*
 * SplashScreenWindow
 */

/*class WXDLLIMPEXP_ADV SplashScreenWindow: public wxWindow
{
public:
    SplashScreenWindow(const wxBitmap& bitmap, wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxNO_BORDER);

protected:

    DECLARE_EVENT_TABLE()
    wxDECLARE_NO_COPY_CLASS(SplashScreenWindow);
};*/
