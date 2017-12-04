//
// Lightpad - DisplayPanel.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
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

#include "ElementIDs.h"
#include "NightwaveCore/NightwaveCore.h"
#include "NightwaveCore/Colors.h"
#include "HOWL/Project.h"
#include "HOWL/Selection.h"
#include "NightwaveCore/SplashScreen.h"

#define MAXIMUM_LAUNCHPAD_IMAGE_SIZE 4096
#define MINIMUM_LAUNCHPAD_IMAGE_SIZE 128
#define MAXIMUM_LAUNCHPAD_BUTTON_SIZE 286
#define DISPLAY_LEFT_MARGIN 0.113525390625
#define DISPLAY_BUTTON_PADDING 0.078125

// Graphical interface panel
class DisplayPanel: public wxPanel {
	public:
		DisplayPanel(wxPanel *parent, SplashScreen *splash);
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
		void resize_images(int min_fit_size);
		void render_buttons();
		void render(wxDC &canvas);
		int get_button_style(int btn_x, int btn_y);
		float buttonIndexToPos(int index);
		float buttonPosToIndex(float pos);
		void colorButton(int button, int color);
		void colorButtons(wxCommandEvent &event);
		void selectButton(int button, bool state);
		void selectButton(int x, int y, bool state);
		void selectButton(HOWL::SelectionEvent &event);
		void MagickToWx(wxImage *out, Magick::Image *image, const int offset_x = 0, const int offset_y = 0);

	private:
		wxPanel *m_parent;
		Magick::Image *fullres_base_image;
		Magick::Image *fullres_button_images[768];
		Magick::Image *fullres_button_halo_images[6];
		Magick::Image *scaled_base_image;
		Magick::Image *scaled_button_images[768];
		Magick::Image *scaled_button_halo_images[6];
		wxImage *lp_img;
		std::string base_image_path;
		std::string button_image_path;
		std::string button_halo_image_path;
		int image_size = 0;
		double scale;
		int panel_width = -1;
		int panel_height = -1;
		int image_xpos = 0;
		int image_ypos = 0;
		int button_colors[100] = {0};
		bool changed_buttons[100] = {true};
		bool selected_buttons[100] = {false};
		bool selected_buttons_box[100] = {false};
		wxPoint clickpos = wxPoint(-1, -1);

	wxDECLARE_EVENT_TABLE();
};
