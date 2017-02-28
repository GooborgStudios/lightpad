//
// Lightpad - Colors.h
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.nightwave.co/lightpad
//

#ifndef COLORS_H
#define COLORS_H

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

const int COLORCOUNT = 128;
extern wxColor velocitycolors[COLORCOUNT];

unsigned char get_closest_velocity(wxColor c);
char get_color_velocity(wxColor c);

#endif