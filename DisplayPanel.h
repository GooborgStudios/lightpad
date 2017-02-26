//
// Lightpad - DisplayPanel.h
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.nightwave.co/lightpad
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
        float getButtonPosition(int digit);
        void colorButton(int button, wxColor color);

        wxDECLARE_EVENT_TABLE();
	private:
		wxPanel *m_parent;
		wxImage *launchpad_base_image;
		wxImage *launchpad_button_image;
		wxImage *launchpad_button_images[6];
		wxBitmap resized;
		std::string base_image_path;
		std::string button_image_path;
		int image_size;
		int button_size;
		int button_radius;
		int panel_width;
		int panel_height;
		int image_xpos;
		int image_ypos;
		wxColor button_colors[100];
};

#endif
