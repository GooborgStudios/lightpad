//
// Lightpad - DisplayPanel.h
// Created by Vinyl Darkscratch and Light Apacha, ©2017 Nightwave Studios.
//

#ifndef DISPLAY_H
#define DISPLAY_H

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/sizer.h>

#define MAXIMUM_LAUNCHPAD_IMAGE_SIZE 4096

#include "Colors.h"
#include "Helpers.h"

// Graphical interface panel
class DisplayPanel: public wxPanel {
	public:
		DisplayPanel(wxPanel *parent);
		void paintEvent(wxPaintEvent & evt);
        void paintNow();
        void OnSize(wxSizeEvent& event);
        void render(wxDC& dc);

        wxDECLARE_EVENT_TABLE();
	private:
		wxPanel *m_parent;
		wxImage *launchpad_image;
		wxBitmap resized;
		std::string image_path;
		int image_size;
		int panel_width;
		int panel_height;
		int xpos;
		int ypos;
};

#endif
