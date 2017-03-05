//
// Lightpad - Colors.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

const int COLORCOUNT = 128;
extern wxColor velocitycolors[COLORCOUNT];

unsigned char get_closest_velocity(wxColor c);
char get_color_velocity(wxColor c);
