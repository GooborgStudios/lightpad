//
// Lightpad - Helpers.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#ifdef _WIN32 // Windows
	#define WINDOWS 1
	#include <windows.h>
	#define APP_ICON "icons/icon.ico"
	#ifdef _WIN64 // Windows 64-bit
		#define WINDOWS_64
	#else // Windows 32-bit
		#define WINDOWS_32
	#endif
#elif __APPLE__ // macOS
	#define MACOS 1
	#define APP_ICON "icons/icon.icns"
#else // Linux, Unix, POSIX, iOS, Android...
	#warning "Unknown/unsupported compiler/operating system"
	#define APP_ICON "icons/icon_64.png"
#endif

#include "Launchpad.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#define PI 3.14159265
#define Color wxColor

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
	ID_Menu_PlayPause,
	ID_FilePanel_Tree,
	ID_PropertiesPanel_ColorSelector,
	ID_PropertiesPanel_ColorSelect,
	ID_TimelinePanel_TimelineGrid,
	ID_TimelinePanel_CellSelect,
	ID_DisplayPanel_Timer
};

std::string getResourcePath(const char *resource_name);

double threeway_max(double val_a, double val_b, double val_c);
double threeway_min(double val_a, double val_b, double val_c);
double val_in_range(double val, double min, double max);
int val_in_range(int val, int min, int max);
int closest_two_power(int current_size, int min_range, int max_range);

int note_to_button(int note);
int button_to_note(int button);

class Note {
	public:
		Note();
		Note(int pos, int col, int start);
		Note(int pos, int col, int start, int dur);
		int position; // Note position
		int color; // Velocity
		int time; // Note start time
		int duration; // Note duration
};

extern LaunchpadPro *launchpad;
