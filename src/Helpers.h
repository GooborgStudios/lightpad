//
// Lightpad - Helpers.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#ifdef _WIN32 // Windows
	#define WINDOWS
	#include <windows.h>
	#define APP_ICON "graphics/icons/icon.ico"
	#ifdef _WIN64 // Windows 64-bit
		#define WINDOWS_64
	#else // Windows 32-bit
		#define WINDOWS_32
	#endif
#elif __APPLE__ // macOS
	#define MACOS
	#define APP_ICON "graphics/icons/icon.icns"
#else // Linux, Unix, POSIX, iOS, Android...
	#warning "Unknown/unsupported compiler/operating system"
	#define APP_ICON "graphics/icons/icon_64.png"
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "RtMidi.h"

#define ABLETON_LIVE_MODE 4
#define MIDI_MESSAGE_SYSEX_BEGIN 240
#define MIDI_MESSAGE_SYSEX_END 247
#define LAUNCHPAD_PRO_SIDE_LED_ID 99

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
	ID_FilePanel_Tree,
	ID_PropertiesPanel_ColorSelector,
	ID_TimelinePanel_TimelineGrid
};

#ifdef WINDOWS
	void usleep(__int64 usec);
#endif

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
		int getMidiPort(std::string name, RtMidi *ports);
		double getMessage(std::vector<unsigned char> *message_in);
		virtual void setColor(unsigned char light, unsigned char color);
		virtual void setPulse(unsigned char light, unsigned char color);
		std::string INPORT_NAME;
		std::string OUTPORT_NAME;
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

class ColorConverter {
	// All converters take values from 0.0 to 1.0
	public:
		// Conversion helpers
		static double Hue2RGB(double p, double q, double t);
		static double XYZ2H(double q);
		static double LAB_compare_RGB(int red1, int grn1, int blu1, int red2, int grn2, int blu2);
		static double LAB_compare_RGB(wxColor clr1, wxColor clr2);

		// Base converters
		static void RGB2HSL(double red, double grn, double blu, double *hue, double *sat, double *lum); 						// XXX Convert to int (0-255)
		static void HSL2RGB(double hue, double sat, double lum, double *red, double *grn, double *blu); 						// XXX Convert to int (0-255)
		static void RGB2HSV(double red, double grn, double blu, double *hue, double *sat, double *vel); 						// XXX Convert to int (0-255)
		static void HSV2RGB(double hue, double sat, double vel, double *red, double *grn, double *blu); 						// XXX Convert to int (0-255)
		static void RGB2CMYK(double red, double grn, double blu, double *cyan, double *mgnta, double *ylw, double *blk); 		// XXX Convert to int (0-255)
		static void CMYK2RGB(double cyan, double mgnta, double ylw, double blk, double *red, double *grn, double *blu); 		// XXX Convert to int (0-255)
		static void RGB2YIQ(double red, double grn, double blu, double *ylum, double *iphs, double *quad); 						// XXX Convert to int (0-255)
		static void YIQ2RGB(double ylum, double iphs, double quad, double *red, double *grn, double *blu); 						// XXX Convert to int (0-255)
		static void RGB2XYZ(int red, int grn, int blu, double *xrsp, double *ylum, double *zblu);
		static void XYZ2RGB(double xrsp, double ylum, double zblu, int *red, int *grn, int *blu);
		static void XYZ2LAB(double xrsp, double ylum, double zblu, double *lum, double *apt, double *bpt);
		static void LAB2XYZ(double lum, double apt, double bpt, double *xrsp, double *ylum, double *zblu);

		// Two-step converters
		static void RGB2LAB(int red, int grn, int blu, double *lum, double *apt, double *bpt);
		static void LAB2RGB(double lum, double apt, double bpt, int *red, int *grn, int *blu);
		static void HSL2HSV(double hue, double sat, double lum, double *_hue, double *_sat, double *vel); 						// XXX Convert to int (0-255)
		static void HSV2HSL(double hue, double sat, double vel, double *_hue, double *_sat, double *lum); 						// XXX Convert to int (0-255)
		static void HSL2CMYK(double hue, double sat, double lum, double *cyan, double *mgnta, double *ylw, double *blk); 		// XXX Convert to int (0-255)
		static void CMYK2HSL(double cyan, double mgnta, double ylw, double blk, double *hue, double *sat, double *lum); 		// XXX Convert to int (0-255)
		static void HSV2CMYK(double hue, double sat, double vel, double *cyan, double *mgnta, double *ylw, double *blk); 		// XXX Convert to int (0-255)
		static void CMYK2HSV(double cyan, double mgnta, double ylw, double blk, double *hue, double *sat, double *vel); 		// XXX Convert to int (0-255)
		static void HSL2YIQ(double hue, double sat, double lum, double *ylum, double *iphs, double *quad); 						// XXX Convert to int (0-255)
		static void YIQ2HSL(double ylum, double iphs, double quad, double *hue, double *sat, double *lum); 						// XXX Convert to int (0-255)
		static void HSV2YIQ(double hue, double sat, double vel, double *ylum, double *iphs, double *quad); 						// XXX Convert to int (0-255)
		static void YIQ2HSV(double ylum, double iphs, double quad, double *hue, double *sat, double *vel); 						// XXX Convert to int (0-255)
		static void CMYK2YIQ(double cyan, double mgnta, double ylw, double blk, double *ylum, double *iphs, double *quad); 		// XXX Convert to int (0-255)
		static void YIQ2CMYK(double ylum, double iphs, double quad, double *cyan, double *mgnta, double *ylw, double *blk); 	// XXX Convert to int (0-255)
};
