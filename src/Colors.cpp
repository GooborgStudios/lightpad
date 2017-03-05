//
// Lightpad - Colors.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "Colors.h"

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "Helpers.h"

// !!! Auto-generated by sample_colors.py from graphics/rgbcolors1/2.png !!!
// !!! MAKE CHANGES TO Colors.cpp.in INSTEAD !!!

wxColor velocitycolors[] = {
	wxColor(0, 0, 0),
	wxColor(28, 28, 28),
	wxColor(124, 124, 124),
	wxColor(252, 252, 252),
	wxColor(255, 78, 72),
	wxColor(253, 10, 2),
	wxColor(90, 0, 0),
	wxColor(27, 0, 0),
	wxColor(255, 188, 97),
	wxColor(255, 87, 0),
	wxColor(90, 29, 0),
	wxColor(36, 24, 2),
	wxColor(253, 253, 33),
	wxColor(251, 254, 1),
	wxColor(88, 88, 0),
	wxColor(22, 25, 0),
	wxColor(129, 253, 43),
	wxColor(64, 253, 1),
	wxColor(22, 88, 0),
	wxColor(19, 40, 1),
	wxColor(53, 253, 43),
	wxColor(7, 250, 0),
	wxColor(0, 88, 1),
	wxColor(0, 25, 0),
	wxColor(53, 252, 71),
	wxColor(0, 254, 0),
	wxColor(0, 88, 1),
	wxColor(0, 24, 0),
	wxColor(50, 253, 127),
	wxColor(9, 251, 43),
	wxColor(1, 88, 20),
	wxColor(0, 29, 16),
	wxColor(47, 252, 177),
	wxColor(0, 251, 145),
	wxColor(1, 87, 50),
	wxColor(1, 24, 16),
	wxColor(57, 190, 255),
	wxColor(1, 166, 255),
	wxColor(1, 64, 81),
	wxColor(0, 16, 26),
	wxColor(65, 134, 255),
	wxColor(0, 80, 255),
	wxColor(1, 26, 90),
	wxColor(1, 6, 25),
	wxColor(71, 71, 255),
	wxColor(0, 1, 252),
	wxColor(0, 0, 90),
	wxColor(2, 0, 28),
	wxColor(131, 71, 255),
	wxColor(80, 0, 255),
	wxColor(22, 0, 103),
	wxColor(10, 0, 50),
	wxColor(255, 72, 254),
	wxColor(247, 4, 252),
	wxColor(90, 0, 90),
	wxColor(28, 0, 24),
	wxColor(248, 80, 129),
	wxColor(255, 2, 83),
	wxColor(88, 2, 27),
	wxColor(33, 1, 16),
	wxColor(255, 20, 9),
	wxColor(156, 52, 1),
	wxColor(122, 79, 11),
	wxColor(57, 104, 0),
	wxColor(1, 56, 1),
	wxColor(0, 84, 50),
	wxColor(0, 83, 127),
	wxColor(0, 0, 254),
	wxColor(0, 68, 81),
	wxColor(26, 0, 209),
	wxColor(124, 124, 124),
	wxColor(32, 32, 32),
	wxColor(253, 10, 4),
	wxColor(186, 253, 0),
	wxColor(172, 236, 0),
	wxColor(86, 253, 0),
	wxColor(0, 137, 0),
	wxColor(1, 252, 123),
	wxColor(2, 166, 255),
	wxColor(2, 26, 255),
	wxColor(53, 0, 255),
	wxColor(120, 0, 255),
	wxColor(180, 23, 126),
	wxColor(65, 32, 0),
	wxColor(255, 73, 0),
	wxColor(130, 225, 1),
	wxColor(105, 252, 0),
	wxColor(0, 254, 0),
	wxColor(0, 254, 0),
	wxColor(69, 253, 97),
	wxColor(1, 251, 203),
	wxColor(80, 134, 255),
	wxColor(39, 77, 202),
	wxColor(132, 122, 237),
	wxColor(213, 9, 255),
	wxColor(255, 6, 90),
	wxColor(255, 125, 1),
	wxColor(184, 177, 0),
	wxColor(138, 253, 0),
	wxColor(129, 93, 0),
	wxColor(55, 42, 0),
	wxColor(13, 76, 5),
	wxColor(0, 80, 55),
	wxColor(19, 20, 41),
	wxColor(18, 31, 83),
	wxColor(104, 61, 27),
	wxColor(175, 2, 8),
	wxColor(226, 80, 57),
	wxColor(233, 100, 0),
	wxColor(250, 229, 0),
	wxColor(150, 227, 1),
	wxColor(100, 179, 0),
	wxColor(27, 28, 49),
	wxColor(220, 253, 84),
	wxColor(118, 251, 186),
	wxColor(150, 152, 255),
	wxColor(139, 98, 254),
	wxColor(64, 64, 64),
	wxColor(116, 116, 116),
	wxColor(222, 252, 252),
	wxColor(162, 5, 0),
	wxColor(54, 0, 0),
	wxColor(2, 207, 6),
	wxColor(0, 65, 1),
	wxColor(183, 178, 0),
	wxColor(60, 48, 0),
	wxColor(177, 91, 14),
	wxColor(76, 19, 0)
};

unsigned char get_closest_velocity(wxColor color) {
	/* Take a wxColor and find the closest match to a Launchpad Pro available color */
	int best_match_index = 0;
	int best_match_value = 1024;
	for (int i = 0; i < COLORCOUNT; i++) {
		int comparison = ColorConverter::LAB_compare_RGB(velocitycolors[i], color);
		if (comparison < best_match_value) {
			best_match_index = i;
			best_match_value = comparison;
		}
	}

	return best_match_index;
};

char get_color_velocity(wxColor color) {
	/* Take a wxColor and find the matching velocity of a Launchpad Pro available color */
	for (int i = 0; i < COLORCOUNT; i++) {
		if (color == velocitycolors[i]) return i;
	}

	return -1;
};
