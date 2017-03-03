//
// Lightpad - Helpers.h
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.nightwave.co/lightpad
//

#ifndef HELPERS_H
#define HELPERS_H

// Attempt to load precompiled, if compiler doesn't support then load normal
// Not needed in Helpers.cpp/.h
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

double threeway_max(double a, double b, double c);
double threeway_min(double a, double b, double c);
double val_in_range(double val, double min, double max);
int val_in_range(int val, int min, int max);
int closest_two_power(int current_size, int min_range, int max_range);

int note_to_button(int note);
int button_to_note(int button);

class Note {
	public:
		Note();
		Note(int pos, int col, int t);
		Note(int pos, int col, int t, int dur);
		int position; // Note position
		int color; // Velocity
		int time; // Note start time
		int duration; // Note duration
};

class Launchpad {
	public:
		Launchpad();
		int connect();
		void disconnect();
		bool isConnected();
		int getMidiPort(std::string name, RtMidi *ports);
		double getMessage(std::vector<unsigned char> *message_in);
		virtual void setColor(unsigned char msg_type, unsigned char light, unsigned char color);
		virtual void setPulse(unsigned char msg_type, unsigned char light, unsigned char color);
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

class LaunchpadPro : public Launchpad {
	public:
		LaunchpadPro();
		int connect();
		void disconnect();
		bool isConnected();
		void setColor(unsigned char msg_type, unsigned char light, unsigned char color);
		void setColor(unsigned char msg_type, unsigned char light, unsigned char red, unsigned char green, unsigned char blue);
		void setFlash(unsigned char msg_type, unsigned char light, unsigned char color);
		void setPulse(unsigned char msg_type, unsigned char light, unsigned char color);
		void displayText(unsigned char msg_type, unsigned int color, unsigned int speed, std::string text);
};

class LaunchpadS : public Launchpad {
	public:
		LaunchpadS();
		int connect();
		void disconnect();
		bool isConnected();
		void setColor(unsigned char msg_type, unsigned char light, unsigned char color);
		void setPulse(unsigned char msg_type, unsigned char light, unsigned char color);
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
		static double LAB_compare_RGB(int r1, int g1, int b1, int r2, int g2, int b2);

		// Base converters
		static void RGB2HSL(double r, double g, double b, double *h, double *s, double *l); // XXX Convert to int (0-255)
		static void HSL2RGB(double h, double s, double l, double *r, double *g, double *b); // XXX Convert to int (0-255)
		static void RGB2HSV(double r, double g, double b, double *h, double *s, double *v); // XXX Convert to int (0-255)
		static void HSV2RGB(double h, double s, double v, double *r, double *g, double *b); // XXX Convert to int (0-255)
		static void RGB2CMYK(double r, double g, double b, double *c, double *m, double *y, double *k); // XXX Convert to int (0-255)
		static void CMYK2RGB(double c, double m, double y, double k, double *r, double *g, double *b); // XXX Convert to int (0-255)
		static void RGB2YIQ(double r, double g, double b, double *y, double *i, double *q); // XXX Convert to int (0-255)
		static void YIQ2RGB(double y, double i, double q, double *r, double *g, double *b); // XXX Convert to int (0-255)
		static void RGB2XYZ(int r, int g, int b, double *x, double *y, double *z);
		static void XYZ2RGB(double x, double y, double z, int *r, int *g, int *b);
		static void XYZ2LAB(double x, double y, double z, double *l, double *a, double *b);
		static void LAB2XYZ(double l, double a, double b, double *x, double *y, double *z);

		// Two-step converters
		static void RGB2LAB(int r, int g, int b, double *l, double *a, double *_b);
		static void LAB2RGB(double l, double a, double b, int *r, int *g, int *_b);
		static void HSL2HSV(double h, double s, double l, double *_h, double *_s, double *v); // XXX Convert to int (0-255)
		static void HSV2HSL(double h, double s, double v, double *_h, double *_s, double *l); // XXX Convert to int (0-255)
		static void HSL2CMYK(double h, double s, double l, double *c, double *m, double *y, double *k); // XXX Convert to int (0-255)
		static void CMYK2HSL(double c, double m, double y, double k, double *h, double *s, double *l); // XXX Convert to int (0-255)
		static void HSV2CMYK(double h, double s, double v, double *c, double *m, double *y, double *k); // XXX Convert to int (0-255)
		static void CMYK2HSV(double c, double m, double y, double k, double *h, double *s, double *v); // XXX Convert to int (0-255)
		static void HSL2YIQ(double h, double s, double l, double *y, double *i, double *q); // XXX Convert to int (0-255)
		static void YIQ2HSL(double y, double i, double q, double *h, double *s, double *l); // XXX Convert to int (0-255)
		static void HSV2YIQ(double h, double s, double v, double *y, double *i, double *q); // XXX Convert to int (0-255)
		static void YIQ2HSV(double y, double i, double q, double *h, double *s, double *v); // XXX Convert to int (0-255)
		static void CMYK2YIQ(double c, double m, double y, double k, double *_y, double *i, double *q); // XXX Convert to int (0-255)
		static void YIQ2CMYK(double y, double i, double q, double *c, double *m, double *_y, double *k); // XXX Convert to int (0-255)
};

#endif
