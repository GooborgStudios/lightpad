//
// Lightpad - DisplayPanel.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "DisplayPanel.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/dcbuffer.h>
#include <wx/clrpicker.h>

#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

#include "Magick++.h"

#include "ElementIDs.h"
#include "NightwaveCore/Helpers.h"
#include "Launchpad.h"
#include "NightwaveCore/Colors.h"
#include "LightpadProject.h"
#include "HOWL/TimelinePanel.h"
#include "NightwaveCore/SplashScreen.h"

const float button_pos[10] = {0.113525390625, 0.199462890625, 0.277587890625, 0.355712890625, 0.433837890625, 0.511962890625, 0.590087890625, 0.668212890625, 0.746337890625, 0.832275390625};
const float button_size = 0.06982421875;

// Initialize the file panel and it's elements
DisplayPanel::DisplayPanel(wxPanel *parent, SplashScreen *splash): wxPanel(parent, ID_Panel_Display, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;

	// Initialize variables
	base_image_path = getResourcePath("launchpad_display/base/base_4096.png");
	button_image_path = getResourcePath("launchpad_display/buttons/buttons_4096.png");
	button_halo_image_path = getResourcePath("launchpad_display/buttons/halo/buttons_halo_4096.png");

	fullres_base_image = new Magick::Image(base_image_path);
	Magick::Image button_image(button_image_path);
	Magick::Image button_halo_image(button_halo_image_path);
		
	bzero(scaled_button_images, sizeof(scaled_button_images));
	bzero(scaled_button_halo_images, sizeof(scaled_button_halo_images));
	scaled_base_image = NULL;
	lp_img = NULL;
	
	scale = 1.0;
	
	splash->SetProgress(45, "Loading buttons...");
	
	for (int j = 0; j < 6; j++) {
		Magick::Image *fullres_button_image = new Magick::Image(button_image);
		fullres_button_image->crop(Magick::Geometry(MAXIMUM_LAUNCHPAD_BUTTON_SIZE, MAXIMUM_LAUNCHPAD_BUTTON_SIZE, MAXIMUM_LAUNCHPAD_BUTTON_SIZE * j, 0));
		
		for (int i = 0; i < 128; i++) {
			wxColor bcolor = velocitycolors[i];
			int arraypos = i + (128 * j);
			
			fullres_button_images[arraypos] = new Magick::Image(*fullres_button_image);
			fullres_button_images[arraypos]->modulate(180.0, 0.0, 100.0);
			fullres_button_images[arraypos]->colorize(50, 50, 50, Magick::ColorRGB(bcolor.Red() / 255.0,
														bcolor.Green() / 255.0, bcolor.Blue() / 255.0));
		}
		
		delete fullres_button_image;
	}
	
	splash->SetProgress(80, "Loading button halos...");
		
	for (int i = 0; i < 6; i++) {
		fullres_button_halo_images[i] = new Magick::Image(button_halo_image);
		fullres_button_halo_images[i]->crop(Magick::Geometry(MAXIMUM_LAUNCHPAD_BUTTON_SIZE, MAXIMUM_LAUNCHPAD_BUTTON_SIZE, MAXIMUM_LAUNCHPAD_BUTTON_SIZE * i, 0));
	}

	paintNow();

	Bind(HOWL::DISPLAY_REFRESH, &DisplayPanel::refreshNow, this, ID_TimelinePanel_CellSelect);
}

DisplayPanel::~DisplayPanel() {
	delete fullres_base_image;
	for (int i = 0; i < 6; i++) delete fullres_button_images[i];
}

void DisplayPanel::paintEvent(wxPaintEvent &WXUNUSED(event)) {
	// depending on your system you may need to look at double-buffered dcs
	wxPaintDC canvas(this);
	render(canvas);
}

void DisplayPanel::paintNow() {
	// depending on your system you may need to look at double-buffered dcs
	wxClientDC canvas(this);
	render(canvas);
}

wxRealPoint DisplayPanel::buttonAtCoords(wxPoint coords) {
	return wxRealPoint(buttonPosToIndex((coords.x - image_xpos) * 1.0 / image_size), buttonPosToIndex((image_size - coords.y + image_ypos) * 1.0 / image_size));
}

void DisplayPanel::onLeftDown(wxMouseEvent &event) {
	wxPoint mousepos = event.GetLogicalPosition(wxClientDC(this));
	wxRealPoint btn = buttonAtCoords(mousepos);
	
	if (!event.ControlDown()) {
		for (int i = 0; i < 100; i++) {
			selectButton(i, false);
		}
	}
	
	if (event.ShiftDown()) {
		wxRealPoint first_btn = buttonAtCoords(clickpos);
		for (int x = ceil(std::min(btn.x, first_btn.x)); x <= floor(std::max(btn.x, first_btn.x)); x++) {
			for (int y = ceil(std::min(btn.y, first_btn.y)); y <= floor(std::max(btn.y, first_btn.y)); y++) {
				selectButton(x, y, true);
			}
		}
	} else {
		if (btn.x == floor(btn.x) && btn.y == floor(btn.y)) {
			selectButton(btn.x, btn.y, !selected_buttons[(int)(btn.x + (btn.y * 10))]);
		}
		clickpos = mousepos;
	}
	
	refreshNow();
}

void DisplayPanel::onMouseMove(wxMouseEvent &event) {
	if (!event.LeftIsDown()) return;
	
	for (int i = 0; i < 100; i++) {
		if (selected_buttons_box[i]) changed_buttons[i] = true;
		selected_buttons_box[i] = false;
	}
	
	wxPoint mousepos = event.GetLogicalPosition(wxClientDC(this));
	wxRealPoint btn = buttonAtCoords(mousepos);
	wxRealPoint first_btn = buttonAtCoords(clickpos);
	
	if (btn.x == first_btn.x && btn.y == first_btn.y) return;
	
	for (int x = ceil(std::min(btn.x, first_btn.x)); x <= floor(std::max(btn.x, first_btn.x)); x++) {
		for (int y = ceil(std::min(btn.y, first_btn.y)); y <= floor(std::max(btn.y, first_btn.y)); y++) {
			selected_buttons_box[x + (y * 10)] = true;
			changed_buttons[x + (y * 10)] = true;
		}
	}
	
	refreshNow();
}

void DisplayPanel::onLeftUp(wxMouseEvent &WXUNUSED(event)) {
//	wxRealPoint btn = buttonAtCoords(mousepos);
	
	for (int i = 0; i < 100; i++) {
		if (selected_buttons_box[i]) {
			selectButton(i, true);
			selected_buttons_box[i] = false;
		}
	}
	
	refreshNow();
}

void DisplayPanel::refreshNow() {
	Refresh();
	Update();
}

void DisplayPanel::refreshNow(wxCommandEvent &WXUNUSED(event)) {
	refreshNow();
}

void DisplayPanel::onSize(wxSizeEvent &event) {
	// Re-render when resizing
	Refresh();
	event.Skip();
}

void DisplayPanel::resize_images(int new_size) {
	if (new_size == image_size) return;
	
	scale = new_size / (MAXIMUM_LAUNCHPAD_IMAGE_SIZE * 1.0);
	int new_button_size = MAXIMUM_LAUNCHPAD_BUTTON_SIZE * scale;
	Magick::Geometry size(new_size, new_size);
	Magick::Geometry button_size(new_button_size, new_button_size);

	for (int i = 0; i < 768; i++) {
		if (new_size > image_size) {
			delete scaled_button_images[i];
			scaled_button_images[i] = new Magick::Image(*fullres_button_images[i]);
		}
		scaled_button_images[i]->scale(button_size);
	}
	
	for (int i = 0; i < 6; i++) {
		if (new_size > image_size) {
			delete scaled_button_halo_images[i];
			scaled_button_halo_images[i] = new Magick::Image(*fullres_button_halo_images[i]);
		}
		scaled_button_halo_images[i]->scale(button_size);
	}
	
	if (new_size > image_size) {
		delete scaled_base_image;
		scaled_base_image = new Magick::Image(*fullres_base_image);
	}
	scaled_base_image->scale(size);

	delete lp_img;
	lp_img = new wxImage(new_size, new_size);
	lp_img->SetAlpha();
	MagickToWx(lp_img, scaled_base_image);
	
	for (int i = 0; i < 100; i++) changed_buttons[i] = true;
	
	image_size = new_size;
}

void DisplayPanel::render_buttons() {
	// Draw the buttons on the screen (and set Launchpad colors)

	launchpad->beginColorUpdate();

	for (int i = 1; i < 99; i++) {
		if (i == 9 || i == 90) continue;
		int btn_x = i % 10;
		int btn_y = 9 - (i / 10);
		int button_style = get_button_style(btn_x, btn_y);
		
		if (changed_buttons[i]) {
			Magick::Image base_crop(*scaled_base_image);
			base_crop.crop(Magick::Geometry(MAXIMUM_LAUNCHPAD_BUTTON_SIZE * scale, MAXIMUM_LAUNCHPAD_BUTTON_SIZE * scale, buttonIndexToPos(btn_x), buttonIndexToPos(btn_y)));
			
			if (selected_buttons[i] || selected_buttons_box[i])
				base_crop.composite(*scaled_button_halo_images[button_style], 0, 0, Magick::OverCompositeOp);
		
			Magick::Image current_button(*scaled_button_images[button_colors[i] + (128 * button_style)]);
			base_crop.composite(current_button, 0, 0, Magick::OverCompositeOp);
			
			launchpad->setColor(i, button_colors[i]);
			MagickToWx(lp_img, &base_crop, buttonIndexToPos(btn_x), buttonIndexToPos(btn_y));
			
			changed_buttons[i] = false;
		}
	}

	launchpad->endColorUpdate();
}

void DisplayPanel::render(wxDC &canvas) {
	int neww, newh, min_fit_size;

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

	canvas.DrawBitmap(wxBitmap(*lp_img), image_xpos, image_ypos);
}

int DisplayPanel::get_button_style(int btn_x, int btn_y) {
	// Choose button style
	if (btn_x == 0 || btn_x == 9 || btn_y == 0 || btn_y == 9) return 0;
	if ((btn_x == 4 || btn_x == 5) && (btn_y == 4 || btn_y == 5)) return 2 + (btn_x - 4) + ((btn_y - 4) * 2);
	return 1;
}

float DisplayPanel::buttonIndexToPos(int index) {
	assert(index >= 0);
	assert(index < 10);
	return button_pos[index] * image_size;
}

float DisplayPanel::buttonPosToIndex(float pos) {
	int idx = 0;
	while (button_pos[idx] < pos && idx < 10) idx++;
	idx--;
	if (idx == -1) return -0.5;
	else if (pos - button_pos[idx] < button_size) return idx * 1.0;
	return idx + 0.5;
}

void DisplayPanel::colorButton(int button, int color) {
	if (button < 1 || button > 98 || button == 9 || button == 90) return;
	button_colors[button] = color;
	changed_buttons[button] = true;
}

void DisplayPanel::colorButtons(wxCommandEvent &event) {
	for (auto iter : activeProject->layer->keyframes) {
		colorButton(std::stoi(iter->name), activeProject->layer->getVelocity(iter->name));
	}
	
	refreshNow();
}

void DisplayPanel::selectButton(int button, bool state) {
	if (selected_buttons[button] != state) {
		changed_buttons[button] = true;
		selected_buttons[button] = state;
	}
}

void DisplayPanel::selectButton(int x, int y, bool state) {
	selectButton(x + (y * 10), state);
}

void DisplayPanel::MagickToWx(wxImage *out, Magick::Image *image, const int offset_x, const int offset_y) {
	int width = image->columns();
	int height = image->rows();
	Magick::PixelPacket *pixels = image->getPixels(0, 0, width, height);
	Magick::PixelPacket p;
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			p = *pixels;
            out->SetRGB(x+offset_x, y+offset_y, p.red / 256, p.green / 256, p.blue / 256);
            out->SetAlpha(x+offset_x, y+offset_y, 255 - (p.opacity / 256));
            pixels++;
        }
	}
}

wxBEGIN_EVENT_TABLE(DisplayPanel, wxPanel)
	EVT_LEFT_DOWN(DisplayPanel::onLeftDown)
	EVT_MOTION(DisplayPanel::onMouseMove)
	EVT_LEFT_UP(DisplayPanel::onLeftUp)
	EVT_PAINT(DisplayPanel::paintEvent)
	EVT_SIZE(DisplayPanel::onSize)
	EVT_COMMAND(ID_Panel_Display, HOWL::PLAYHEAD_MOVED, DisplayPanel::colorButtons)
	EVT_COMMAND(ID_Panel_Display, HOWL::DISPLAY_REFRESH, DisplayPanel::refreshNow)
wxEND_EVENT_TABLE()
