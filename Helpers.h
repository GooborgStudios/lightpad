//
// Lightpad - Helpers.h
// Created by Vinyl Darkscratch, Light Apacha, and WhoovesPON3, ©2017 Nightwave Studios.
// Additional assistance by Eric Busch (Origami1105).
// http://www.nightwave.co/lightpad
//

#ifndef HELPERS_H
#define HELPERS_H

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

// Element IDs
enum {
	ID_Frame_Main,
	ID_Panel_Main,
	ID_Panel_Properties,
	ID_Panel_Timeline,
	ID_Panel_Display,
	ID_Panel_File,
	ID_Menu_About,
	ID_Menu_Hello,
	ID_Menu_Save,
	ID_FilePanel_Tree
};

int closest_two_power(int current_size, int min_range, int max_range);

#endif