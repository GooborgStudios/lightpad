//
// Lightpad - Helpers.cpp
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.nightwave.co/lightpad
//

//
// RGB->XYZ->LAB conversion modified from: http://www.csie.ntu.edu.tw/~b92069/HWs/rgb2lab.c
// RGB<-XYZ<-LAB conversion modified from: http://stackoverflow.com/a/9372718
// 3-way min/max, RGB<>HSL, RGB<>HSV, RGB<>CMYK, RGB<>YIQ conversions, and HSL<>RGB<>HSV,
//   HSL->RGB->CMYK, HSL->RGB->YIQ, HSV->RGB->CMYK, HSV->RGB->YIQ conversions modified from:
//   https://github.com/aleksandaratanasov/RGBConverter/blob/master/RGBConverter.cpp
//

#include <algorithm>
#include <cmath>

// Attempt to load precompiled, if compiler doesn't support then load normal
// Not needed in Helpers.cpp/.h
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "RtMidi.h"

#include "Helpers.h"

LaunchpadPro *launchpad = new LaunchpadPro();

// Generate with GenerateNoteButtonMap.cpp
const int note_button_map[] = {91, 92, 93, 94, 95, 96, 97, 98, 11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34, 41, 42, 43, 44, 51, 52, 53, 54, 61, 62, 63, 64, 71, 72, 73, 74, 81, 82, 83, 84, 15, 16, 17, 18, 25, 26, 27, 28, 35, 36, 37, 38, 45, 46, 47, 48, 55, 56, 57, 58, 65, 66, 67, 68, 75, 76, 77, 78, 85, 86, 87, 88, 89, 79, 69, 59, 49, 39, 29, 19, 80, 70, 60, 50, 40, 30, 20, 10, 1, 2, 3, 4, 5, 6, 7, 8};
const int note_button_offset = 28;
const int note_button_size = sizeof(note_button_map)/sizeof(int);
// End generate block

// Math helpers
double threeway_max(double a, double b, double c) {
	return std::max(a, std::max(b, c));
}

double threeway_min(double a, double b, double c) {
	return std::min(a, std::min(b, c));
}

double val_in_range(double val, double min, double max) {
	return std::max(min, std::min(val, max));
}

int val_in_range(int val, int min, int max) {
	return std::max(min, std::min(val, max));
}

// closest_two_power() written by Eric Busch (Origami1105) on 1/19/2017
int closest_two_power(int current_size, int min_range, int max_range) {
	int pow_two = 1, i = 1;

	while (true) {
		// increment the power of 2 if the value is still smaller
		// than current_size, if it is no longer smaller than
		// current_size, set success to true so that the loop ends
		if ((pow_two < min_range) || (pow_two < max_range && pow_two < current_size))
			pow_two = pow(2, i++);
		else
			return pow_two;
	}
}

// Note<>Button conversion
int note_to_button(int note) {
	if (note < note_button_offset || note > (note_button_size + note_button_offset)) return 0;
	return note_button_map[note-note_button_offset];
}

int button_to_note(int button) {
	for (int i=0; i < note_button_size + note_button_offset; i++)
		if (note_button_map[i] == button) return i + note_button_offset;
	return 0;
}

Note::Note() {
	int position = 0;
	int color = 0;
	int time = 0;
	int duration = 0;
}

Note::Note(int pos, int col, int t) {
	int position = pos;
	int color = col;
	int time = t;
	int duration = 0;
}

Note::Note(int pos, int col, int t, int dur) {
	int position = pos;
	int color = col;
	int time = t;
	int duration = dur;
}

Launchpad::Launchpad() {
	midiin = new RtMidiIn();
	midiout = new RtMidiOut();
}

int Launchpad::connect() {
	connected = false;

	inport = getMidiPort(INPORT_NAME, midiin);
	outport = getMidiPort(OUTPORT_NAME, midiout);

	if ( (inport == -1) || (outport == -1) ) {
		std::cout << "Launchpad not found!  Is it plugged in and turned on?\n";
		delete midiin;
		delete midiout;
		return -1;
	}

	// Open LaunchpadPro Standalone port.
	midiin->openPort( inport );
	midiout->openPort( outport );

	// Don't ignore sysex, timing, or active sensing messages.
	midiin->ignoreTypes( false, false, false );

	connected = true;
	return 0;
}

void Launchpad::disconnect() {
	if (connected) {
		connected = false;
		delete midiin;
		delete midiout;
	}
}

bool Launchpad::isConnected() {
	return connected;
}

int Launchpad::getMidiPort(std::string name, RtMidi *ports) {
	for ( unsigned int i=0; i < ports->getPortCount(); i++ ) {
		try {
			if ( ports->getPortName(i).compare(0, name.length(), name) == 0 ) {
				return i;
			}
		}
		catch ( RtMidiError &error ) {
			error.printMessage();
			return -1;
		}
	}

	return -1;
}

double Launchpad::getMessage(std::vector<unsigned char> *message_in) {
	if (isConnected() == false) return -1;
	return midiin->getMessage(message_in);
}

void Launchpad::sendMessage(unsigned int first_byte, ...) {
	if (isConnected() == false) return;
	va_list vl;
	va_start(vl, first_byte);
	unsigned char byte = first_byte;
	while (byte != MIDI_MESSAGE_SYSEX_END || byte < 0 || byte > 255) {
		message.push_back(byte);
		byte = va_arg(vl, unsigned int);
	}
	if (byte >= 0 && byte <= 255) message.push_back(byte);
	va_end(vl);

	midiout->sendMessage(&message);
	message.erase(message.begin(), message.begin()+message.size());
}

void Launchpad::setColor(unsigned char msg_type, unsigned char light, unsigned char color) {
	if (isConnected() == false) return;

}

void Launchpad::setPulse(unsigned char msg_type, unsigned char light, unsigned char color) {
	if (isConnected() == false) return;

}

LaunchpadPro::LaunchpadPro() {
	#ifdef _WIN32
		INPORT_NAME = "MIDIIN2 (Launchpad Pro)";
		OUTPORT_NAME = "MIDIOUT2 (Launchpad Pro)";
	#else
		INPORT_NAME = "Launchpad Pro Standalone Port";
		OUTPORT_NAME = "Launchpad Pro Standalone Port";
	#endif
}

int LaunchpadPro::connect() {
	int x = Launchpad::connect();

	if (x == 0) {
		std::vector<unsigned char> device_info;

		// Inquiry Device
		sendMessage( 240, 126, 127, 6, 1, 247 );
		while (device_info.size() == 0) {
			getMessage(&device_info);
			usleep( 10000 ); // Sleep for 10 milliseconds ... platform-dependent.
		}

		sendMessage( 240, 0, 32, 41, 2, 16, 33, 1, 247 ); // Set to Standalone Mode
		sendMessage( 240, 0, 32, 41, 2, 16, 44, 3, 247 ); // Set to Programmer Mode
	}
	return x;
}

void LaunchpadPro::disconnect() {
	sendMessage( 240, 0, 32, 41, 2, 16, 14, 0, 247 ); // Clear all LED colors
	sendMessage( 240, 0, 32, 41, 2, 16, 44, 0, 247 ); // Set to Note Mode

	Launchpad::disconnect();
}

bool LaunchpadPro::isConnected() {
	return Launchpad::isConnected();
}

void LaunchpadPro::setColor(unsigned char msg_type, unsigned char light, unsigned char color) {
	if (isConnected() == false) return;
	sendMessage( 240, 0, 32, 41, 2, 16, 10, light, color, 247 );
}

void LaunchpadPro::setColor(unsigned char msg_type, unsigned char light, unsigned char red, unsigned char green, unsigned char blue) {
	if (isConnected() == false) return;
	sendMessage( 240, 0, 32, 41, 2, 16, 11, light, red, green, blue, 247 );
}

void LaunchpadPro::setFlash(unsigned char msg_type, unsigned char light, unsigned char color) {
	if (isConnected() == false) return;
	sendMessage( 240, 0, 32, 41, 2, 16, 35, light, color, 247 );
}

void LaunchpadPro::setPulse(unsigned char msg_type, unsigned char light, unsigned char color) {
	if (isConnected() == false) return;
	sendMessage( 240, 0, 32, 41, 2, 16, 40, light, color, 247 );
}

void LaunchpadPro::displayText(unsigned char msg_type, unsigned int color, unsigned int speed, std::string text) {
	if (isConnected() == false) return;
	// XXX Broken!

	// addToMessage(240, 0, 32, 41, 2, 16, 40, color, speed);
	// for (int i = 0; i < text.size(); ++i)
	// 	addToMessage(text[i]);
	// addToMessage(247);
	// sendMessage();
}

LaunchpadS::LaunchpadS() {
	#ifdef _WIN32 // May not be accurate!
		INPORT_NAME = "Launchpad S";
		OUTPORT_NAME = "Launchpad S";
	#else
		INPORT_NAME = "Launchpad S";
		OUTPORT_NAME = "Launchpad S";
	#endif
}

int LaunchpadS::connect() {
	int x = Launchpad::connect();

	if (x == 0) {
		std::vector<unsigned char> device_info;

		// Inquiry Device
		sendMessage( 240, 126, 127, 6, 1, 247 );
		while (device_info.size() == 0) {
			getMessage(&device_info);
			usleep( 10000 ); // Sleep for 10 milliseconds ... platform-dependent.
		}

		sendMessage( 176, 0, 2, -1 ); // Set to Drum Rack Mode
	}
	return x;
}

void LaunchpadS::disconnect() {
	sendMessage( 176, 0, 0, -1 ); // Reset

	Launchpad::disconnect();
}

bool LaunchpadS::isConnected() {
	return Launchpad::isConnected();
}

unsigned char LaunchpadS::pro_to_s_note(unsigned char pro_note, unsigned char msg_type) {
	if (msg_type == 176) return pro_note;
	return pro_note;
}

unsigned char LaunchpadS::pro_to_s_color(unsigned char pro_color) {
	return pro_color;
}

void LaunchpadS::setColor(unsigned char msg_type, unsigned char light, unsigned char color) {
	if (isConnected() == false) return;
	message.push_back(144);
	message.push_back(pro_to_s_note(light, msg_type));
	message.push_back(color);

	midiout->sendMessage(&message);
	message.erase(message.begin(), message.begin()+message.size());
}

void LaunchpadS::setPulse(unsigned char msg_type, unsigned char light, unsigned char color) {
	if (isConnected() == false) return;
	// XXX Implement me!
}

// Conversion helpers
double ColorConverter::Hue2RGB(double p, double q, double t) {
  if (t < 0.0) t += 1;
  if (t > 1.0) t -= 1;
  if (t < 1/6.0) return p + (q - p) * 6.0 * t;
  if (t < 1/2.0) return q;
  if (t < 2/3.0) return p + (q - p) * (2/3.0 - t) * 6.0;
  return p;
}

double ColorConverter::XYZ2H(double q) {
	if (q > 0.008856) return pow(q, 0.333333);
	return 7.787 * q + 0.137931;
}

// Compare two RGB colors via LAB
double ColorConverter::LAB_compare_RGB(int r1, int g1, int b1, int r2, int g2, int b2) {
	double l1, a1, _b1, l2, a2, _b2;
	RGB2LAB(r1, g1, b1, &l1, &a1, &_b1);
	RGB2LAB(r2, g2, b2, &l2, &a2, &_b2);
	return pow((l1 - l2), 2) + pow((a1 - a2), 2) + pow((_b1 - _b2), 2);
}

// RGB<>HSL color conversion
void ColorConverter::RGB2HSL(double r, double g, double b, double *h, double *s, double *l) {
	double max = threeway_max(r, g, b);
	double min = threeway_min(r, g, b);
	*l = (max + min) / 2;

	if (max == min) {
		*h = *s = 0.0; // achromatic
	} else {
		double d = max - min;
		*s = *l > 0.5 ? d / (2.0 - max - min) : d / (max + min);
		if (max == r) *h = (g - b) / d + (g < b ? 6.0 : 0.0);
		else if (max == g) *h = (b - r) / d + 2.0;
		else if (max == b) *h = (r - g) / d + 4.0;

		*h /= 6;
	}
}

void ColorConverter::HSL2RGB(double h, double s, double l, double *r, double *g, double *b) {
	if (s == 0.0) {
		*r = *g = *b = l; // achromatic
	} else {
		double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
		double p = 2.0 * l - q;
		*r = ColorConverter::Hue2RGB(p, q, h + 1/3.0);
		*g = ColorConverter::Hue2RGB(p, q, h);
		*b = ColorConverter::Hue2RGB(p, q, h - 1/3.0);
	}
}

// RGB<>HSV color conversion
void ColorConverter::RGB2HSV(double r, double g, double b, double *h, double *s, double *v) {
	double max = threeway_max(r, g, b);
	double min = threeway_min(r, g, b);
	*v = max;

	double d = max - min;
	*s = max == 0.0 ? 0.0 : d / max;

	if (max == min) {
		*h = 0.0; // achromatic
	} else {
		if (max == r) *h = (g - b) / d + (g < b ? 6.0 : 0.0);
		else if (max == g) *h = (b - r) / d + 2.0;
		else if (max == b) *h = (r - g) / d + 4.0;

		*h /= 6.0;
	}
}

void ColorConverter::HSV2RGB(double h, double s, double v, double *r, double *g, double *b) {
	unsigned int i = (unsigned int)(h * 6.0);
	double f = h * 6.0 - i;
	double p = v * (1.0 - s);
	double q = v * (1.0 - f * s);
	double t = v * (1.0 - (1.0 - f) * s);

	switch (i % 6) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}
}

// RGB<>CMYK color conversion
void ColorConverter::RGB2CMYK(double r, double g, double b, double *c, double *m, double *y, double *k) {
	*k = 1.0 - threeway_max(r, g, b);
	*c = (1.0 - r - *k) / (1.0 - *k);
	*m = (1.0 - g - *k) / (1.0 - *k);
	*y = (1.0 - b - *k) / (1.0 - *k);
}

void ColorConverter::CMYK2RGB(double c, double m, double y, double k, double *r, double *g, double *b) {
	*r = (1.0 - c) / (1.0 - k);
	*g = (1.0 - m) / (1.0 - k);
	*b = (1.0 - y) / (1.0 - k);
}

// RGB<>YIQ color conversion
void ColorConverter::RGB2YIQ(double r, double g, double b, double *y, double *i, double *q) {
	*y = 0.299 * r + 0.587 * g + 0.114 * b;
	*i = 0.569 * r - 0.275 * g - 0.322 * b;
	*q = 0.211 * r - 0.523 * g + 0.312 * b;
}

void ColorConverter::YIQ2RGB(double y, double i, double q, double *r, double *g, double *b) {
	*r = y + 0.956 * i + 0.621 * q;
	*g = y - 0.272 * i - 0.647 * q;
	*b = y - 1.106 * i + 1.703 * q;
}

// RGB<>XYZ color conversion
void ColorConverter::RGB2XYZ(int r, int g, int b, double *x, double *y, double *z) {
	double R, G, B;
	double adapt = 0.003922;

	R = r * adapt;
	G = g * adapt;
	B = b * adapt;

	*x = 0.412424 * R + 0.357579 * G + 0.180464 * B;
	*y = 0.212656 * R + 0.715158 * G + 0.0721856 * B;
	*z = 0.0193324 * R + 0.119193 * G + 0.950444 * B;
}

void ColorConverter::XYZ2RGB(double x, double y, double z, int *r, int *g, int *b) {
	*r = x * (1219569 / 395920) + y * (-608687 / 395920) + z * (-107481 / 197960);
	*g = x * (-80960619 / 87888100) + y * (82435961 / 43944050) + z * (3976797 / 87888100);
	*b = x * (93813 / 1774030) + y * (-180961 / 887015) + z * (107481 / 93370);
}

// XYZ<>LAB color conversion
void ColorConverter::XYZ2LAB(double x, double y, double z, double *l, double *a, double *b) {
	*l = 116 * ColorConverter::XYZ2H(y / 1.0) - 16;
	*a = 500 * (ColorConverter::XYZ2H(x / 0.950467) - ColorConverter::XYZ2H (y / 1.0));
	*b = 200 * (ColorConverter::XYZ2H(y / 1.0) - ColorConverter::XYZ2H (z / 1.088969));
}

void ColorConverter::LAB2XYZ(double l, double a, double b, double *x, double *y, double *z) {
	double X, Y, Z;
	Y = l * (1 / 116) + 16 / 116;
	X = a * (1 / 500) + Y;
	Z = b * (-1 / 200) + Y;

	*x = X > 6 / 29 ? X * X * X : X * (108 / 841) - 432 / 24389;
	*y = l > 8 ? Y * Y * Y : l * (27 / 24389);
	*z = Z > 6 / 29 ? Z * Z * Z : Z * (108 / 841) - 432 / 24389;
}

// RGB<>XYZ<>LAB color conversion
void ColorConverter::RGB2LAB(int r, int g, int b, double *l, double *a, double *_b) {
	double x, y, z;
	RGB2XYZ(r, g, b, &x, &y, &z);
	XYZ2LAB(x, y, z, l, a, _b);
}

void ColorConverter::LAB2RGB(double l, double a, double b, int *r, int *g, int *_b) {
	double x, y, z;
	LAB2XYZ(l, a, b, &x, &y, &z);
	XYZ2RGB(x, y, z, r, g, _b);
}

// HSL<>RGB<>HSV color conversion
void ColorConverter::HSL2HSV(double h, double s, double l, double *_h, double *_s, double *v) {
	double r, g, b;
	HSL2RGB(h, s, l, &r, &g, &b);
	RGB2HSV(r, g, b, _h, _s, v);
}

void ColorConverter::HSV2HSL(double h, double s, double v, double *_h, double *_s, double *l) {
	double r, g, b;
	HSV2RGB(h, s, v, &r, &g, &b);
	RGB2HSL(r, g, b, _h, _s, l);
}

// HSL<>RGB<>CMYK color conversion
void ColorConverter::HSL2CMYK(double h, double s, double l, double *c, double *m, double *y, double *k) {
	double r, g, b;
	HSL2RGB(h, s, l, &r, &g, &b);
	RGB2CMYK(r, g, b, c, m, y, k);
}

void ColorConverter::CMYK2HSL(double c, double m, double y, double k, double *h, double *s, double *l) {
	double r, g, b;
	CMYK2RGB(c, m, y, k, &r, &g, &b);
	RGB2HSL(r, g, b, h, s, l);
}

// HSV<>RGB<>CMYK color conversion
void ColorConverter::HSV2CMYK(double h, double s, double v, double *c, double *m, double *y, double *k) {
	double r, g, b;
	HSV2RGB(h, s, v, &r, &g, &b);
	RGB2CMYK(r, g, b, c, m, y, k);
}

void ColorConverter::CMYK2HSV(double c, double m, double y, double k, double *h, double *s, double *v) {
	double r, g, b;
	CMYK2RGB(c, m, y, k, &r, &g, &b);
	RGB2HSV(r, g, b, h, s, v);
}

// HSL<>RGB<>YIQ color conversion
void ColorConverter::HSL2YIQ(double h, double s, double l, double *y, double *i, double *q) {
	double r, g, b;
	HSL2RGB(h, s, l, &r, &g, &b);
	RGB2YIQ(r, g, b, y, i, q);
}

void ColorConverter::YIQ2HSL(double y, double i, double q, double *h, double *s, double *l) {
	double r, g, b;
	YIQ2RGB(y, i, q, &r, &g, &b);
	RGB2HSL(r, g, b, h, s, l);
}

// HSV<>RGB<>YIQ color conversion
void ColorConverter::HSV2YIQ(double h, double s, double v, double *y, double *i, double *q) {
	double r, g, b;
	HSV2RGB(h, s, v, &r, &g, &b);
	RGB2YIQ(r, g, b, y, i, q);
}

void ColorConverter::YIQ2HSV(double y, double i, double q, double *h, double *s, double *v) {
	double r, g, b;
	YIQ2RGB(y, i, q, &r, &g, &b);
	RGB2HSV(r, g, b, h, s, v);
}

// CMYK<>RGB<>YIQ color conversion
void ColorConverter::CMYK2YIQ(double c, double m, double y, double k, double *_y, double *i, double *q) {
	double r, g, b;
	CMYK2RGB(c, m, y, k, &r, &g, &b);
	RGB2YIQ(r, g, b, _y, i, q);
}

void ColorConverter::YIQ2CMYK(double y, double i, double q, double *c, double *m, double *_y, double *k) {
	double r, g, b;
	YIQ2RGB(y, i, q, &r, &g, &b);
	RGB2CMYK(r, g, b, c, m, _y, k);
}
