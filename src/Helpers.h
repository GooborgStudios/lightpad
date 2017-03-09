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

#include <regex>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "RtMidi.h"

#define ABLETON_LIVE_MODE 4
#define MIDI_MESSAGE_SYSEX_BEGIN 240
#define MIDI_MESSAGE_SYSEX_END 247
#define LAUNCHPAD_PRO_SIDE_LED_ID 99

#define PI 3.14159265

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
	ID_TimelinePanel_TimelineGrid,
	ID_DisplayPanel_Timer
};

#ifdef WINDOWS
	void usleep(__int64 usec);
#endif

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

class LaunchpadBase {
	public:
		LaunchpadBase();
		int connect();
		void disconnect();
		bool isConnected();
		int getMidiPort(RtMidi *ports);
		double getMessage(std::vector<unsigned char> *message_in);
		virtual void setColor(unsigned char light, unsigned char color);
		virtual void setPulse(unsigned char light, unsigned char color);
		std::regex PORT_REGEX;
	protected:
		RtMidiIn *midiin;
		RtMidiOut *midiout;
		int inport;
		int outport;
		std::vector<unsigned char> message;
		void sendMessage(unsigned int first_byte, ...);
		bool connected;
};

class LaunchpadPro : public LaunchpadBase {
	public:
		LaunchpadPro();
		int connect();
		void disconnect();
		bool isConnected();
		void setColor(unsigned char light, unsigned char color);
		void setColor(unsigned char light, unsigned char red, unsigned char green, unsigned char blue);
		void setFlash(unsigned char light, unsigned char color);
		void setPulse(unsigned char light, unsigned char color);
		void displayText(unsigned int color, unsigned int speed, std::string text);
		void stopText();
};

class LaunchpadS : public LaunchpadBase {
	public:
		LaunchpadS();
		int connect();
		void disconnect();
		bool isConnected();
		void setColor(unsigned char light, unsigned char color);
		void setPulse(unsigned char light, unsigned char color);
		unsigned char pro_to_s_note(unsigned char pro_note, unsigned char msg_type);
		unsigned char pro_to_s_color(unsigned char pro_color);
};

extern LaunchpadPro *launchpad;
