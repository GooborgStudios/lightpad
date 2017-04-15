//
// Lightpad - DisplayPanel.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "DisplayPanel.h"

#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/dcbuffer.h>
#include <wx/clrpicker.h>

#include "Magick++.h"

#include "Colors.h"
#include "Helpers.h"
#include "Project.h"

wxDEFINE_EVENT(DISPLAY_REFRESH, wxCommandEvent);

const float button_pos[10] = {0.113525390625, 0.199462890625, 0.277587890625, 0.355712890625, 0.433837890625, 0.511962890625, 0.590087890625, 0.668212890625, 0.746337890625, 0.832275390625};
const float button_size = 0.06982421875;

// Initialize the file panel and it's elements
DisplayPanel::DisplayPanel(wxPanel *parent)
	: wxPanel(parent, ID_Panel_Display, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;

	m_timer = new wxTimer(this, ID_DisplayPanel_Timer);

	// Initialize variables
	base_image_path = getResourcePath("launchpad_display/base/base_4096.png");
	button_image_path = getResourcePath("launchpad_display/buttons/buttons_4096.png");
	image_size = MAXIMUM_LAUNCHPAD_IMAGE_SIZE;
	panel_width = -1;
	panel_height = -1;
	image_xpos = 0;
	image_ypos = 0;
	frame = 0;

	fullres_base_image = new Magick::Image(base_image_path);
	Magick::Image button_image(button_image_path);
	
	for (int i = 0; i < 6; i++) {
		fullres_button_images[i] = new Magick::Image(button_image);
		fullres_button_images[i]->crop(Magick::Geometry(MAXIMUM_LAUNCHPAD_BUTTON_SIZE, MAXIMUM_LAUNCHPAD_BUTTON_SIZE, MAXIMUM_LAUNCHPAD_BUTTON_SIZE * i, 0));
	}
	bzero(scaled_button_images, sizeof(scaled_button_images));
	
	scaled_base_image = NULL;
	lp_img = NULL;

	for (int i = 1; i < 99; i++) {
		if (i == 9 || i == 90) continue;
		button_colors[i] = 0;
	}
	paintNow();

	Bind(DISPLAY_REFRESH, &DisplayPanel::refreshNow, this, ID_TimelinePanel_CellSelect);
}

DisplayPanel::~DisplayPanel() {
	delete m_timer;
	delete fullres_base_image;
	for (int i = 0; i < 6; i++) delete fullres_button_images[i];
}

void DisplayPanel::paintEvent(wxPaintEvent &event) {
	// depending on your system you may need to look at double-buffered dcs
	wxPaintDC canvas(this);
	render(canvas);
}

void DisplayPanel::paintNow() {
	// depending on your system you may need to look at double-buffered dcs
	wxClientDC canvas(this);
	render(canvas);
}

void DisplayPanel::refreshNow() {
	Refresh();
	Update();
}

void DisplayPanel::refreshNow(wxCommandEvent &event) {
	refreshNow();
}

void DisplayPanel::onSize(wxSizeEvent &event) {
	// Re-render when resizing
	Refresh();
	event.Skip();
}

void DisplayPanel::startstop(wxCommandEvent &event) {
	if (m_timer->IsRunning()) m_timer->Stop();
	else m_timer->Start(1000 / 60);
}

void DisplayPanel::play_next_frame(wxTimerEvent &event) {
	frame += 1;
	std::cout << frame << std::endl;
	Refresh();
}

void DisplayPanel::resize_images(int new_size) {
	double ratio = new_size / (MAXIMUM_LAUNCHPAD_IMAGE_SIZE * 1.0);

	// Load the other resolutions of the image as needed
	if (new_size != image_size) {
		// int img_res = closest_two_power(new_size, MINIMUM_LAUNCHPAD_IMAGE_SIZE,
		// 										MAXIMUM_LAUNCHPAD_IMAGE_SIZE);
		// fullres_base_image->LoadFile(base_image_path, wxBITMAP_TYPE_PNG);
		// launchpad_button_image->LoadFile(button_image_path, wxBITMAP_TYPE_PNG);

		image_size = new_size;
		Magick::Geometry size(MAXIMUM_LAUNCHPAD_BUTTON_SIZE * ratio, MAXIMUM_LAUNCHPAD_BUTTON_SIZE * ratio);

		for (int i = 0; i < 6; i++) {
			if (scaled_button_images[i]) delete scaled_button_images[i];
			scaled_button_images[i] = new Magick::Image(*fullres_button_images[i]);
			scaled_button_images[i]->scale(size);
		}
		
		if (scaled_base_image) delete scaled_base_image;
		scaled_base_image = new Magick::Image(*fullres_base_image);
		scaled_base_image->scale(Magick::Geometry(new_size, new_size));

		if (lp_img) delete lp_img;
		lp_img = new Magick::Image(*scaled_base_image);
	}
}

void DisplayPanel::render_buttons() {
	// Draw the buttons on the screen (and set Launchpad colors)
	
	delete lp_img;
	lp_img = new Magick::Image(*scaled_base_image);

	launchpad->beginColorUpdate();

	for (int i = 1; i < 99; i++) {
		if (i == 9 || i == 90) continue;
		int btn_x = i % 10;
		int btn_y = 9 - (i / 10);
		int button_style = get_button_style(btn_x, btn_y);
		wxColor bcolor = velocitycolors[button_colors[i]];

		Magick::Image current_button(*scaled_button_images[button_style]);
		current_button.modulate(180.0, 0.0, 100.0);
		current_button.colorize(50, 50, 50, Magick::ColorRGB(bcolor.Red() / 255.0,
		                        bcolor.Green() / 255.0, bcolor.Blue() / 255.0));
		lp_img->composite(current_button, buttonIndexToPos(btn_x),
		                  buttonIndexToPos(btn_y),  Magick::OverCompositeOp);

		launchpad->setColor(i, button_colors[i]);
	}

	launchpad->endColorUpdate();
}

void DisplayPanel::render(wxDC &canvas) {
	int neww, newh, min_fit_size;
	Magick::Geometry size;

	canvas.GetSize(&neww, &newh);
	min_fit_size = std::max(std::min(neww, newh), MINIMUM_LAUNCHPAD_IMAGE_SIZE);

	if (neww != panel_width || newh != panel_height) {
		resize_images(min_fit_size);

		// Reposition as needed
		if ( neww > newh ) {
			image_xpos = (neww - min_fit_size) / 2;
			image_ypos = 0;
		} else {
			image_xpos = 0;
			image_ypos = (newh - min_fit_size) / 2;
		}

		panel_width = neww;
		panel_height = newh;
	}

	render_buttons();

	// Derived from http://stackoverflow.com/questions/28151240/get-rgb-color-with-magick-using-c
	int width = lp_img->columns();
	int height = lp_img->rows();
	Magick::PixelPacket *pixels = lp_img->getPixels(0, 0, width, height);
	Magick::ColorRGB color_sample;
	wxImage out(width, height);
	out.SetAlpha();

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			color_sample = pixels[width * y + x];
			out.SetRGB(x, y, color_sample.red() * 255, color_sample.green() * 255,
			            color_sample.blue() * 255);
			out.SetAlpha(x, y, (1 - color_sample.alpha()) * 255);
		}
	}

	canvas.DrawBitmap(wxBitmap(out), image_xpos, image_ypos);
}

int DisplayPanel::get_button_style(int btn_x, int btn_y) {
	// Choose button style
	if (btn_x == 0 || btn_x == 9 || btn_y == 0 || btn_y == 9) {
		return 0;
	} else {
		switch (btn_x + (btn_y * 10)) {
			case 44:
				return 2;
				break;
			case 45:
				return 3;
				break;
			case 54:
				return 4;
				break;
			case 55:
				return 5;
				break;
			default:
				return 1;
				break;
		}
	}
}

float DisplayPanel::buttonIndexToPos(int index) {
//	float pos = DISPLAY_LEFT_MARGIN + (index * DISPLAY_BUTTON_PADDING);
//	int padding = (index + 7) / 8;
//	return (pos + (padding * (DISPLAY_BUTTON_PADDING / 10))) * image_size;
	
	assert(index >= 0);
	assert(index < 10);
	return button_pos[index] * image_size;
}

void DisplayPanel::colorButton(int button, wxColor color) {
	if (button < 1 || button > 98 || button == 9 || button == 90) return;
	button_colors[button] = ColorConverter::get_closest_velocity(color);
}

void DisplayPanel::colorButton(wxColourPickerEvent &event) {
	colorButton(event.GetInt(), event.GetColour());
}

wxBEGIN_EVENT_TABLE(DisplayPanel, wxPanel)
	EVT_PAINT(DisplayPanel::paintEvent)
	EVT_SIZE(DisplayPanel::onSize)
	EVT_MENU(ID_Menu_PlayPause, DisplayPanel::startstop)
	EVT_TIMER(ID_DisplayPanel_Timer, DisplayPanel::play_next_frame)
	EVT_COLOURPICKER_CHANGED(ID_TimelinePanel_TimelineGrid, DisplayPanel::colorButton)
	EVT_COMMAND(ID_Panel_Display, DISPLAY_REFRESH, DisplayPanel::refreshNow)
wxEND_EVENT_TABLE()
