//
// Lightpad - Colors.cpp
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.nightwave.co/lightpad
//

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "Colors.h"
#include "Helpers.h"

#include <stdio.h>
#include <math.h>

!XXX!

unsigned char get_closest_velocity(wxColor c) {
	/* Take a wxColor and find the closest match to a Launchpad Pro available color */
	int best_match_index = 0;
	int best_match_value = 1024;
	for (int i = 0; i < COLORCOUNT; i++) {
		int d = ColorConverter::LAB_compare_RGB(velocitycolors[i].Red(), velocitycolors[i].Green(), velocitycolors[i].Blue(), c.Red(), c.Green(), c.Blue());
		if (d < best_match_value) {
			best_match_index = i;
			best_match_value = d;
		}
		//std::cout << i << " - " << d << std::endl;
	}

	return best_match_index;
};

char get_color_velocity(wxColor c) {
	/* Take a wxColor and find the matching velocity of a Launchpad Pro available color */
	for (int i = 0; i < COLORCOUNT; i++) {
		if (c == velocitycolors[i]) return i;
	}

	return -1;
};