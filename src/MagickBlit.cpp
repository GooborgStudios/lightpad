//
// Lightpad - MagickBlit.cpp
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.nightwave.co/lightpad
//

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <iostream>
#include "Magick++.h"

#include "Helpers.h"

#define DEBUG_MESSAGES
#define BUTTON_SIZE 285

#ifdef DEBUG_MESSAGES
	#define debug std::cout
#else
	class NullBuffer: public std::streambuf {
		int overflow(int c) {return c;}
	};
	NullBuffer debug_buffer;
	std::ostream debug(&debug_buffer);
#endif

#define newline std::endl

int position(int digit) {
	float pos = 0.113525390625 + (digit * 0.078125);
	int circle_distance = 0;
	if (digit > 0) circle_distance = 1;
	if (digit == 9) circle_distance = 2;
	pos = pos + (circle_distance * 0.0078125);

	return (int)(pos * 4096);
}

int main() {
	wxColor rainbow[18];
	wxColor button_colors[100];
	wxColor current_color;
	int r, g, b;
	int x, y, z;

	std::cout << "Running program..." << newline;
	debug << "Initializing..." << newline;

	Magick::InitializeMagick(NULL);

	Magick::Image button_img;
	Magick::Image bg_img;
	Magick::Image *button[6];
	Magick::Image *current_button;
	button_img.read("./graphics/launchpad_display/buttons/buttons_4096.png");
	bg_img.read("./graphics/launchpad_display/base/base_4096.png");

	for (int i = 0; i < 6; i++) {
		button[i] = new Magick::Image(button_img);
		button[i]->crop(Magick::Geometry(BUTTON_SIZE, BUTTON_SIZE, BUTTON_SIZE*i, 0));
	}

	debug << "Initialized.  Commencing rainbow preparation..." << newline;

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

		rainbow[j] = wxColor(r,g,b);
	}

	for (int i = 1; i < 99; i++) {
		if (i == 9 || i == 90) continue;
		int j = (i / 10) + (i % 10);
		button_colors[i] = wxColor(rainbow[j]);
	}

	debug << "Rainbow prepared.  Commencing image composition..." << newline;

	for (int i = 1; i < 99; i++) {
		if (i == 9 || i == 90) continue;
		x = i % 10;
		y = 9 - (i / 10);
		debug << "\rProcessing...  " << i << " (" << x << ", " << y << ")" << std::flush;

		// Choose button style
		if (x == 0 || x == 9 || y == 0 || y == 9) {
			z = 0;
		} else {
			switch (i) {
				case 44:
					z = 5;
					break;
				case 45:
					z = 4;
					break;
				case 54:
					z = 3;
					break;
				case 55:
					z = 2;
					break;
				default:
					z = 1;
			}
		}

		current_color = button_colors[i];
		current_button = new Magick::Image(*button[z]);
		current_button->modulate(180.0, 0.0, 100.0);
		current_button->colorize(50, 50, 50, Magick::ColorRGB(current_color.Red()/255.0, current_color.Green()/255.0, current_color.Blue()/255.0));

		bg_img.composite(*current_button, position(9-x), position(y), Magick::OverCompositeOp);
	}

	debug << "\rCompositing process complete!" << newline;

	debug << "Saving file..." << newline;
	bg_img.write("./MagickBlit.png");
	debug << "File saved!" << newline;

	std::cout << "Complete!" << newline;
}