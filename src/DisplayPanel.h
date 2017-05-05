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
#include <wx/clrpicker.h>

#include "Magick++.h"

#include "Colors.h"
#include "Helpers.h"
#include "Project.h"

#define MAXIMUM_LAUNCHPAD_IMAGE_SIZE 4096
#define MINIMUM_LAUNCHPAD_IMAGE_SIZE 128
#define MAXIMUM_LAUNCHPAD_BUTTON_SIZE 286
#define DISPLAY_LEFT_MARGIN 0.113525390625
#define DISPLAY_BUTTON_PADDING 0.078125
// #define BUTTON_RATIO 0.06982421875

// #define ANIMATED_BUTTON_COLOR
// #define RAINBOW_BUTTON_COLOR

// Graphical interface panel
class DisplayPanel: public wxPanel {
	public:
		DisplayPanel(wxPanel *parent);
		~DisplayPanel();
		void paintEvent(wxPaintEvent &event);
		void paintNow();
		wxRealPoint buttonAtCoords(wxPoint coords);
		void onLeftDown(wxMouseEvent &event);
		void onMouseMove(wxMouseEvent &event);
		void onLeftUp(wxMouseEvent &event);
		void refreshNow();
		void refreshNow(wxCommandEvent &event);
		void onSize(wxSizeEvent &event);
		void startstop(wxCommandEvent &event);
		void resize_images(int min_fit_size);
		// void set_debug_button_colors();
		void render_buttons();
		void render(wxDC &dc);
		int get_button_style(int btn_x, int btn_y);
		float buttonIndexToPos(int index);
		float buttonPosToIndex(float pos);
		void colorButton(int button, wxColor color);
		void colorButton(wxColourPickerEvent &event);

		wxDECLARE_EVENT_TABLE();
	private:
		void play_next_frame(wxTimerEvent &event);

		wxPanel *m_parent;
		wxTimer *m_timer;
		Magick::Image *fullres_base_image;
		Magick::Image *fullres_button_images[6];
		Magick::Image *fullres_button_halo_images[6];
		Magick::Image *scaled_base_image;
		Magick::Image *scaled_button_images[6];
		Magick::Image *scaled_button_halo_images[6];
		Magick::Image *lp_img;
		std::string base_image_path;
		std::string button_image_path;
		std::string button_halo_image_path;
		int image_size;
		int panel_width;
		int panel_height;
		int image_xpos;
		int image_ypos;
		int button_colors[100];
		bool selected_buttons[100];
		bool selected_buttons_box[100];
		int frame;
		wxPoint clickpos;
};

wxDECLARE_EVENT(DISPLAY_REFRESH, wxCommandEvent);
