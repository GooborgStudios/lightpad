//
// Lightpad - DisplayPanel.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/sizer.h>

#include "Magick++.h"

#include "Colors.h"
#include "Helpers.h"

#define MAXIMUM_LAUNCHPAD_IMAGE_SIZE 4096

// Graphical interface panel
class DisplayPanel: public wxPanel {
	public:
		DisplayPanel(wxPanel *parent);
		void paintEvent(wxPaintEvent &evt);
		void paintNow();
		void OnSize(wxSizeEvent &event);
		void startstop(wxCommandEvent &event);
		void render(wxDC &dc);
		float getButtonPosition(int digit);
		void colorButton(int button, wxColor color);

		wxDECLARE_EVENT_TABLE();
	private:
		void play_next_frame(wxTimerEvent &event);

		wxPanel *m_parent;
		wxTimer *m_timer;
		#if wxDRAW_BUTTONS
		wxImage *launchpad_base_image;
		wxImage *launchpad_button_image;
		wxImage *launchpad_button_images[6];
		wxBitmap resized;
		#else
		Magick::Image launchpad_base_image;
		Magick::Image launchpad_button_image;
		Magick::Image *launchpad_button_images[6];
		Magick::Image resized;
		#endif
		std::string base_image_path;
		std::string button_image_path;
		int image_size;
		int button_size;
		int button_radius;
		int panel_width;
		int panel_height;
		int image_xpos;
		int image_ypos;
		int button_colors[100];
		int frame;
};
