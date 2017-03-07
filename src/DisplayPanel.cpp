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

#include "Magick++.h"

#include "Colors.h"
#include "Helpers.h"

// Initialize the file panel and it's elements
DisplayPanel::DisplayPanel(wxPanel *parent)
	: wxPanel(parent, ID_Panel_Display, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;

	m_timer = new wxTimer(this, ID_DisplayPanel_Timer);

	// Initialize variables
	base_image_path = "graphics/launchpad_display/base/base_4096.png";
	button_image_path = "graphics/launchpad_display/buttons/buttons_4096.png";
	image_size = MAXIMUM_LAUNCHPAD_IMAGE_SIZE;
	button_size = image_size * 0.06982421875;
	button_radius = image_size / 512;
	panel_width = -1;
	panel_height = -1;
	image_xpos = 0;
	image_ypos = 0;
	frame = 0;

	#ifdef wxDRAW_BUTTONS
#warning "wxDrawing not recommended!"
	launchpad_base_image = new wxImage(base_image_path, wxBITMAP_TYPE_PNG);
	launchpad_button_image = new wxImage(button_image_path, wxBITMAP_TYPE_PNG);
	for (int i = 0; i < 6; i++)
		launchpad_button_images[i] = new wxImage(launchpad_button_image->Size(wxSize(286, 286),
		        wxPoint(-286 * i, 0)));
	#else
	launchpad_base_image.read(base_image_path);
	launchpad_button_image.read(button_image_path);
	for (int i = 0; i < 6; i++) {
		launchpad_button_images[i] = new Magick::Image(launchpad_button_image);
		launchpad_button_images[i]->crop(Magick::Geometry(286, 286, 286 * i, 0));
	}
	#endif

	// wxColor rainbow[18];
	// for (int j = 0; j < 18; j++) {
	// 	int red = 0, grn = 0, blu = 0;
	// 	if (j >= 15) red = 255 * ((18 - j) / 3.0);
	// 	else red = 255 * ((j - 9) / 3.0);
	// 	if (j >= 12) grn = 255 * ((15 - j) / 3.0);
	// 	else grn = 255 * ((j - 3) / 3.0);
	// 	if (j >= 6) blu = 255 * ((9 - j) / 3.0);
	// 	else blu = 255 * ((j) / 3.0);
	// 	rainbow[j] = wxColor(val_in_range(red, 0, 255), val_in_range(grn, 0, 255), val_in_range(blu, 0,
	// 	                     255));
	// }
	// for (int i = 1; i < 99; i++) {
	// 	if (i == 9 || i == 90) continue;
	// 	int j = (i + / 10) + (9 - (i % 10));
	// 	button_colors[i] = get_closest_velocity(rainbow[j]);
	// }

	paintNow();
}

void DisplayPanel::paintEvent(wxPaintEvent &event) {
	// depending on your system you may need to look at double-buffered dcs
	wxPaintDC dc(this);
	render(dc);
}

void DisplayPanel::paintNow() {
	// depending on your system you may need to look at double-buffered dcs
	wxClientDC dc(this);
	render(dc);
}

void DisplayPanel::OnSize(wxSizeEvent &event) {
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

void DisplayPanel::render(wxDC &dc) {
	int neww, newh, ratio, new_image_size, min_fit_size;
	wxColor bcolor;
	#ifdef wxDRAW_BUTTONS
	wxMemoryDC bdc;
	wxSize size;
	wxPoint pos;
	wxCoord width, height;
	wxColor pixel;
	double hue, sat, lum, hue2, sat2, lum2, newr, newg, newb;
	#else
	Magick::Image *current_button;
	Magick::Geometry size;
	#endif

	for (int i = 1; i < 99; i++) {
		if (i == 9 || i == 90) continue;
		int x = i % 10;
		int y = 9 - (i / 10);
		int offset = std::abs(sin(frame * PI / 6) * 3);

		if (x == 0 || y == 0 || x == 9 || y == 9) {
			button_colors[i] = 0;
		} else if (x == 3 || y == 3 || x == 6 || y == 6) {
			if ((x == 3 || x == 6) && (y == 3 || y == 6)) {
				button_colors[i] = 81;
			} else {
				// std::cout << x << "," << y << ": " << offset;
				if (x < 3) offset += std::abs(sin(x + 1 * PI / 6) * 3);
				if (x > 6) offset += std::abs(sin(x * PI / 6) * 3);
				if (y < 3) offset += std::abs(sin(y + 1 * PI / 6) * 3);
				if (y > 6) offset += std::abs(sin(y * PI / 6) * 3);
				// std::cout << " - " << offset << std::endl;
				button_colors[i] = 45 + offset;
			}
		} else {
			button_colors[i] = 0;
		}
	}

	dc.GetSize(&neww, &newh);
	min_fit_size = std::min(neww, newh);

	if ( neww != panel_width || newh != panel_height ) {
		// When first launching the app, the following will make sure it doesn't crash
		if (min_fit_size == 0) min_fit_size = 256;

		new_image_size = closest_two_power(min_fit_size, 256, 4096);
		ratio = 4096 / new_image_size;

		// Load the other resolutions of the image as needed
		if (new_image_size != image_size) {
			//launchpad_base_image->LoadFile(base_image_path, wxBITMAP_TYPE_PNG);
			//launchpad_button_image->LoadFile(button_image_path, wxBITMAP_TYPE_PNG);
			image_size = new_image_size;

			button_size = image_size * 0.06982421875;
			button_radius = image_size / 512;

			#ifdef wxDRAW_BUTTONS
			resized = wxBitmap(launchpad_button_image->Scale(button_size * 6, button_size));
			for (int i = 0; i < 6; i++) {
				size = wxSize(button_size, button_size);
				pos = wxPoint(button_size * i * -1, 0);
				launchpad_button_images[i] = new wxImage(resized.ConvertToImage().Size(size, pos));
			}
			resized = wxBitmap(launchpad_base_image->Scale(min_fit_size, min_fit_size));
			#else
			resized = Magick::Image(launchpad_button_image);
			size = resized.size();
			size.width(size.width() / ratio);
			size.height(size.height() / ratio);
			resized.scale(size);
			for (int i = 0; i < 6; i++) {
				launchpad_button_images[i] = new Magick::Image(resized);
				launchpad_button_images[i]->crop(Magick::Geometry(button_size, button_size, button_size * i, 0));
			}
			resized = Magick::Image(launchpad_base_image);
			size = resized.size();
			size.width(size.width() / ratio);
			size.height(size.height() / ratio);
			resized.scale(size);
			#endif
		}

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

	#ifdef wxDRAW_BUTTONS
	dc.DrawBitmap(resized, image_xpos, image_ypos);
	dc.SetPen(*(wxTRANSPARENT_PEN));
	#endif

	// convert button number to coordinates
	for (int i = 1; i < 99; i++) {
		if (i == 9 || i == 90) continue;
		int x = i % 10;
		int y = 9 - (i / 10);
		int button_style;

		bcolor = wxColor(velocitycolors[button_colors[i]]);
		#ifdef wxDRAW_BUTTONS
		wxPoint bpos(image_xpos + (min_fit_size * getButtonPosition(x)),
		             image_ypos + (min_fit_size * getButtonPosition(y)));
		ColorConverter::RGB2HSL(bcolor.Red() / 255.0, bcolor.Green() / 255.0, bcolor.Blue() / 255.0, &hue2,
		                        &sat2, &lum2);
		// if (x == 1 && y == 0) std::cout << "Buttn - " << (int)(bcolor.Red()) << " " << (int)(bcolor.Green()) << " " << (int)(bcolor.Blue()) << " - " << hue2 << " " << sat2 << " " << lum2 << std::endl;
		dc.SetBrush(wxBrush(bcolor));
		#endif

		// Choose button style
		if (x == 0 || x == 9 || y == 0 || y == 9) {
			button_style = 0;
		} else {
			switch (i) {
				case 44:
					button_style = 4;
					break;
				case 45:
					button_style = 5;
					break;
				case 54:
					button_style = 2;
					break;
				case 55:
					button_style = 3;
					break;
				default:
					button_style = 1;
					break;
			}
		}

		#ifdef wxDRAW_BUTTONS
		if (button_style == 1) dc.DrawEllipse(bpos, wxSize(button_size, button_size));
		else dc.DrawRoundedRectangle(bpos, wxSize(button_size, button_size), button_radius);

		wxBitmap bimg = wxBitmap(*(launchpad_button_images[button_style]));
		bdc.SelectObject(bimg);
		bdc.GetSize(&width, &height);
		for (int bmp_x = 0; bmp_x < width; bmp_x++) {
			for (int bmp_y = 0; bmp_y < width; bmp_y++) {
				continue;

				bdc.GetPixel(bmp_x, bmp_y, &pixel);
				// if (x == 1 && y == 0 && bmp_x == 0 && bmp_y == 0) std::cout << x << "," << y << "_" << bmp_x << "," << bmp_y << "_" << width << "," << height << ":" << std::endl;
				float r_percent = pixel.Red() / 255.0, g_percent = pixel.Green() / 255.0,
				      b_percent = pixel.Blue() / 255.0;
				ColorConverter::RGB2HSL(r_percent, g_percent, b_percent, &hue, &sat, &lum);
				// if (x == 1 && y == 0 && bmp_x == 0 && bmp_y == 0) std::cout << "Pixel - " << std::setw(10) << r_percent << " " << std::setw(10) << g_percent << " " << std::setw(10) << b_percent << " " << std::setw(10) << pixel.Alpha()/255.0 << " - " << std::setw(10) << hue << " " << std::setw(10) << sat << " " << std::setw(10) << lum << std::endl;
				ColorConverter::HSL2RGB(hue2, sat2, lum, &newr, &newg, &newb);
				// if (x == 1 && y == 0 && bmp_x == 0 && bmp_y == 0) std::cout << "Merge - " << (int)(newr*255) << " " << (int)(newg*255) << " " << (int)(newb*255) << " " << (int)(pixel.Alpha()) << std::endl;
				pixel.Set((int)(newr * 255), (int)(newg * 255), (int)(newb * 255), (int)(pixel.Alpha()));
				bdc.SetPen(wxPen(pixel));
				bdc.DrawPoint(bmp_x, bmp_y);
			}
		}
		dc.Blit(bpos.x, bpos.y, button_size, button_size, &bdc, 0, 0);
		bdc.SelectObject(wxNullBitmap);
		#else
		current_button = new Magick::Image(*launchpad_button_images[button_style]);
		current_button->modulate(180.0, 0.0, 100.0);
		current_button->colorize(50, 50, 50, Magick::ColorRGB(bcolor.Red() / 255.0, bcolor.Green() / 255.0,
		                         bcolor.Blue() / 255.0));

		resized.composite(*current_button, getButtonPosition(x)*image_size, getButtonPosition(y)*image_size,
		                  Magick::OverCompositeOp);
		#endif

		if (launchpad->isConnected()) {
			// std::cout << i << " " << button_colors[i] << std::endl;
			if (button_style == 1) launchpad->setColor(i, button_colors[i]);
			else launchpad->setColor(i, button_colors[i]);
		}
	}

	#ifndef wxDRAW_BUTTONS
	// Obtained and modified from: http://stackoverflow.com/questions/28151240/get-rgb-color-with-magick-using-c
	int width = resized.columns();
	int height = resized.rows();
	int range = std::pow(2, resized.modulusDepth());
	assert(range > 0); // Better do some assertion/error checking here
	Magick::PixelPacket *pixels = resized.getPixels(0, 0, width, height);
	Magick::ColorRGB color_sample;
	wxImage out(width, height);
	out.SetAlpha();

	for (int x = 0; x < width - 1; x++) {
		for (int y = 0; y < height - 1; y++) {
			color_sample = pixels[width * y + x];
			out.SetRGB(x, y, color_sample.red() * 255, color_sample.green() * 255, color_sample.blue() * 255);
			out.SetAlpha(x, y, (1 - color_sample.alpha()) * 255);
		}
	}

	dc.DrawBitmap(wxBitmap(out.Scale(min_fit_size, min_fit_size)), image_xpos, image_ypos);
	#endif
}

float DisplayPanel::getButtonPosition(int digit) {
	float x = 0.113525390625 + (digit * 0.078125);
	int y = 0;
	if (digit > 0) y = 1;
	if (digit == 9) y = 2;
	x = x + (y * 0.0078125);

	return x;
}

void DisplayPanel::colorButton(int button, wxColor color) {
	button_colors[button] = get_closest_velocity(color);
	// This should check for invalid buttons (0, 9, 99)
}

wxBEGIN_EVENT_TABLE(DisplayPanel, wxPanel)
	EVT_PAINT(DisplayPanel::paintEvent)
	EVT_SIZE(DisplayPanel::OnSize)
	EVT_MENU(ID_Menu_PlayPause, DisplayPanel::startstop)
	EVT_TIMER(ID_DisplayPanel_Timer, DisplayPanel::play_next_frame)
wxEND_EVENT_TABLE()
