//
// Lightpad - DisplayPanel.cpp
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.nightwave.co/lightpad
//

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/dcbuffer.h>

#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

#ifdef _WIN32
	//define something for Windows (32-bit and 64-bit, this part is common)
	#define WINDOWS
	#ifdef _WIN64
		//define something for Windows (64-bit only)
		#define WINDOWS_64
	#endif
#elif __APPLE__
	//define something for Mac
	#define MACOS
#else
	//#error "Unknown/unsupported compiler/operating system"
#endif

#include "DisplayPanel.h"
#include "Colors.h"
#include "Helpers.h"

// Initialize the file panel and it's elements
DisplayPanel::DisplayPanel(wxPanel *parent)
	   : wxPanel(parent, ID_Panel_Display, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;

	// Load the graphics
	// base_image_path = "graphics/launchpad_display/launchpad_display_4096.png";
	base_image_path = "graphics/launchpad_display/base/base_4096.png";
	button_image_path = "graphics/launchpad_display/buttons/buttons_4096.png";
	launchpad_base_image = new wxImage(base_image_path, wxBITMAP_TYPE_PNG);
	launchpad_button_image = new wxImage(button_image_path, wxBITMAP_TYPE_PNG);
	for (int i = 0; i < 6; i++) launchpad_button_images[i] = new wxImage(launchpad_button_image->Size(wxSize(286, 286), wxPoint(-286*i, 0)));

	// Initialize size and position variables
	image_size = MAXIMUM_LAUNCHPAD_IMAGE_SIZE;
	panel_width = -1;
	panel_height = -1;
	image_xpos = 0;
	image_ypos = 0;

	wxColor rainbow[18];
	for (int j = 0; j < 18; j++) {
		int r = 0, g = 0, b = 0;
		if (j >= 15) r = 255*((18-j)/3.0);
		else r = 255*((j-9)/3.0);
		if (j >= 12) g = 255*((15-j)/3.0);
		else g = 255*((j-3)/3.0);
		if (j >= 6) b = 255*((9-j)/3.0);
		else b = 255*((j)/3.0);

		r = val_in_range(r, 0, 255);
		g = val_in_range(g, 0, 255);
		b = val_in_range(b, 0, 255);

		// std::cout << std::setw(2) << std::right << j << ": R = " << std::setw(3) << std::left << r << "  G = " << std::setw(3) << g << "  B = " << std::setw(3) << b << std::endl;
		rainbow[j] = wxColor(r,g,b);
	}

	for (int i = 1; i < 99; i++) {
		if (i == 9 || i == 90) continue;
		int j = (i / 10) + (i % 10);
		button_colors[i] = wxColor(rainbow[j]);
	}

	paintNow();
}

void DisplayPanel::paintEvent(wxPaintEvent &evt) {
	// depending on your system you may need to look at double-buffered dcs
	wxPaintDC dc(this);
	render(dc);
}

void DisplayPanel::paintNow() {
	// depending on your system you may need to look at double-buffered dcs
	wxClientDC dc(this);
	render(dc);
}

void DisplayPanel::OnSize(wxSizeEvent& event) {
	// Re-render when resizing
	Refresh();
	event.Skip();
}

void DisplayPanel::render(wxDC &dc) {
	int neww, newh;
	wxMemoryDC bdc;
	wxCoord width, height;
	wxColor pixel, bcolor;
	double hue, sat, lum, hue2, sat2, lum2, newr, newg, newb;

	dc.GetSize(&neww, &newh);
	int min_fit_size = std::min(neww, newh);

	if ( neww != panel_width || newh != panel_height ) {
		if (min_fit_size == 0) {
			min_fit_size = 256;
		} // When first launching the app, this will make sure it doesn't crash

		int new_image_size = closest_two_power(min_fit_size, 256, 4096);

		// Load the other resolutions of the image as needed
		if (new_image_size != image_size) {
			//launchpad_base_image->LoadFile(base_image_path, wxBITMAP_TYPE_PNG);
			//launchpad_button_image->LoadFile(button_image_path, wxBITMAP_TYPE_PNG);
			image_size = new_image_size;
		}
		button_size = min_fit_size * 0.06982421875;
		button_radius = image_size / 512;

		resized = wxBitmap(launchpad_button_image->Scale(button_size * 6, button_size));
		for (int i = 0; i < 6; i++) {
			wxSize s = wxSize(button_size, button_size);
			wxPoint p = wxPoint(button_size*i*-1, 0);
			launchpad_button_images[i] = new wxImage(resized.ConvertToImage().Size(s, p));
		}
		resized = wxBitmap(launchpad_base_image->Scale(min_fit_size, min_fit_size));

		// Reposition as needed
		if ( neww > newh ) {
			image_xpos = (neww - min_fit_size)/2;
			image_ypos = 0;
		} else {
			image_xpos = 0;
			image_ypos = (newh - min_fit_size)/2;
		}

		panel_width = neww;
		panel_height = newh;
	}

	dc.DrawBitmap(resized, image_xpos, image_ypos);
	dc.SetPen(*(wxTRANSPARENT_PEN));

	// convert button number to coordinates
	for (int i = 1; i < 99; i++) {
		if (i == 9 || i == 90) continue;
		int x = i % 10;
		int y = 9 - (i / 10);
		int z = 1;
		wxPoint bpos(image_xpos+(min_fit_size*getButtonPosition(9-x)), image_ypos+(min_fit_size*getButtonPosition(y)));
		bcolor = button_colors[i];
		ColorConverter::RGB2HSL(bcolor.Red()/255.0, bcolor.Green()/255.0, bcolor.Blue()/255.0, &hue2, &sat2, &lum2);
		// if (x == 1 && y == 0) std::cout << "Buttn - " << (int)(bcolor.Red()) << " " << (int)(bcolor.Green()) << " " << (int)(bcolor.Blue()) << " - " << hue2 << " " << sat2 << " " << lum2 << std::endl;
		dc.SetBrush(wxBrush(bcolor));

		if (x == 0 || x == 9 || y == 0 || y == 9) z = 0;
		else if ((x == 4 || x == 5) && (y == 4 || y == 5)) {
			z = 3;
			if (x == 5) z -= 1;
			if (y == 5) z += 2;
		}

		if (x == 0 || x == 9 || y == 0 || y == 9) dc.DrawEllipse(bpos, wxSize(button_size, button_size));
		else dc.DrawRoundedRectangle(bpos, wxSize(button_size, button_size), button_radius);

		wxBitmap bimg = wxBitmap(*(launchpad_button_images[z]));
		bdc.SelectObject(bimg);
		bdc.GetSize(&width, &height);
		for (int bmp_x = 0; bmp_x < width; bmp_x++) {
			for (int bmp_y = 0; bmp_y < width; bmp_y++) {
				continue;
				
				bdc.GetPixel(bmp_x, bmp_y, &pixel);
				// if (x == 1 && y == 0 && bmp_x == 0 && bmp_y == 0) std::cout << x << "," << y << "_" << bmp_x << "," << bmp_y << "_" << width << "," << height << ":" << std::endl;
				ColorConverter::RGB2HSL(pixel.Red()/255.0, pixel.Green()/255.0, pixel.Blue()/255.0, &hue, &sat, &lum);
				// if (x == 1 && y == 0 && bmp_x == 0 && bmp_y == 0) std::cout << "Pixel - " << (int)(pixel.Red()) << " " << (int)(pixel.Green()) << " " << (int)(pixel.Blue()) << " " << (int)(pixel.Alpha()) << " - " << hue << " " << sat << " " << lum << std::endl;
				ColorConverter::HSL2RGB(hue2, sat2, lum, &newr, &newg, &newb);
				// if (x == 1 && y == 0 && bmp_x == 0 && bmp_y == 0) std::cout << "Merge - " << (int)(newr*255) << " " << (int)(newg*255) << " " << (int)(newb*255) << " " << (int)(pixel.Alpha()) << std::endl;
				pixel.Set((int)(newr*255), (int)(newg*255), (int)(newb*255), (int)(pixel.Alpha()));
				bdc.SetPen(wxPen(pixel));
				bdc.DrawPoint(bmp_x, bmp_y);
			}
		}
		dc.Blit(bpos.x, bpos.y, button_size, button_size, &bdc, 0, 0);
		// bdc.SelectObject(wxNullBitmap);
	}
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
	button_colors[button] = color;
	// This should check for invalid buttons (0, 9, 99)
}

wxBEGIN_EVENT_TABLE(DisplayPanel, wxPanel)
	EVT_PAINT(DisplayPanel::paintEvent)
	EVT_SIZE(DisplayPanel::OnSize)
wxEND_EVENT_TABLE()
