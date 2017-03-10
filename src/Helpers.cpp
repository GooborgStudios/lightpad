//
// Lightpad - Helpers.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "Helpers.h"

#include <algorithm>
#include <cmath>
#include <regex>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "RtMidi.h"

#ifdef WINDOWS
#include <windows.h>

void usleep(__int64 usec) {
	HANDLE timer;
	LARGE_INTEGER ft;

	// Convert to 100 nanosecond interval, negative value indicates relative time
	ft.QuadPart = -(10 * usec);

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}
#endif

#ifdef XCODE_BUNDLE
#include <CoreFoundation/CoreFoundation.h>

std::string getResourcePath(const char *resource_name) {
	CFURLRef appUrlRef = CFBundleCopyResourceURL(CFBundleGetMainBundle(),
	                     CFStringCreateWithCString(NULL, resource_name, kCFStringEncodingUTF8),
	                     NULL, NULL);
	CFStringRef filePathRef = CFURLCopyPath(appUrlRef);
	std::string filePath(CFStringGetCStringPtr(filePathRef, kCFStringEncodingUTF8));

	// Release references
	CFRelease(filePathRef);
	CFRelease(appUrlRef);

	return filePath;
}
#else
std::string getResourcePath(const char *resource_name) {
	return std::string(RESOURCE_DIR) + "/" + std::string(resource_name);
}
#endif

// Generate with GenerateNoteButtonMap.cpp
const int note_button_map[] = {
	91, 92, 93, 94, 95, 96, 97, 98, 11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34, 41, 42, 43, 44,
	51, 52, 53, 54, 61, 62, 63, 64, 71, 72, 73, 74, 81, 82, 83, 84, 15, 16, 17, 18, 25, 26, 27, 28,
	35, 36, 37, 38, 45, 46, 47, 48, 55, 56, 57, 58, 65, 66, 67, 68, 75, 76, 77, 78, 85, 86, 87, 88,
	89, 79, 69, 59, 49, 39, 29, 19, 80, 70, 60, 50, 40, 30, 20, 10, 1, 2, 3, 4, 5, 6, 7, 8
};
const int note_button_offset = 28;
const int note_button_size = sizeof(note_button_map) / sizeof(int);
// End generate block

// Math helpers
double threeway_max(double val_a, double val_b, double val_c) {
	return std::max(val_a, std::max(val_b, val_c));
}

double threeway_min(double val_a, double val_b, double val_c) {
	return std::min(val_a, std::min(val_b, val_c));
}

double val_in_range(double val, double min, double max) {
	return std::max(min, std::min(val, max));
}

int val_in_range(int val, int min, int max) {
	return std::max(min, std::min(val, max));
}

// closest_two_power() written by Eric Busch (Origami1105) on 1/19/2017
int closest_two_power(int current_size, int min_range, int max_range) {
	int pow_two = 1, power = 1;

	while (true) {
		// increment the power of 2 if the value is still smaller
		// than current_size, if it is no longer smaller than
		// current_size, set success to true so that the loop ends
		if ((pow_two < min_range) || (pow_two < max_range && pow_two < current_size))
			pow_two = pow(2, power++);
		else
			return pow_two;
	}
}

// Note<>Button conversion
int note_to_button(int note) {
	if (note < note_button_offset || note > (note_button_size + note_button_offset)) return 0;
	return note_button_map[note - note_button_offset];
}

int button_to_note(int button) {
	for (int i = 0; i < note_button_size + note_button_offset; i++)
		if (note_button_map[i] == button) return i + note_button_offset;
	return 0;
}

Note::Note() {
	position = 0;
	color = 0;
	time = 0;
	duration = 0;
}

Note::Note(int pos, int col, int start) {
	position = pos;
	color = col;
	time = start;
	duration = 0;
}

Note::Note(int pos, int col, int start, int dur) {
	position = pos;
	color = col;
	time = start;
	duration = dur;
}

LaunchpadBase::LaunchpadBase() {
	midiin = new RtMidiIn();
	midiout = new RtMidiOut();
	message = new std::vector<unsigned char>();
}

LaunchpadBase::~LaunchpadBase() {
	delete midiin;
	delete midiout;
	delete message;
}

int LaunchpadBase::connect() {
	connected = true;

	inport = getMidiPort(midiin);
	outport = getMidiPort(midiout);

	if ((inport == -1) || (outport == -1)) return -1;

	midiin->openPort(inport);
	midiout->openPort(outport);
	midiin->ignoreTypes(false, false, false); // Don't ignore sysex, timing, or active sensing messages.
	return 0;
}

void LaunchpadBase::disconnect() {

}

bool LaunchpadBase::isConnected() {
	return connected;
}

int LaunchpadBase::getMidiPort(RtMidi *ports) {
	for ( unsigned int i = 0; i < ports->getPortCount(); i++ ) {
		try {
			if (std::regex_match(ports->getPortName(i), PORT_REGEX)) {
				return i;
			}
		} catch (RtMidiError &error) {
			error.printMessage();
			return -1;
		}
	}

	return -1;
}

double LaunchpadBase::getMessage(std::vector<unsigned char> *message_in) {
	if (!isConnected()) return -1;
	return midiin->getMessage(message_in);
}

void LaunchpadBase::sendMessage() {
	midiout->sendMessage(message);
	message->clear();
}

void LaunchpadBase::sendMessage(unsigned int first_byte, ...) {
	if (!isConnected()) return;
	va_list varlist;
	va_start(varlist, first_byte);
	unsigned int byte = first_byte;
	while (byte != MIDI_MESSAGE_SYSEX_END && byte <= 255) {
		message->push_back(byte);
		byte = va_arg(varlist, unsigned int);
	}
	if (byte <= 255) message->push_back(byte);
	va_end(varlist);

	sendMessage();
}

void LaunchpadBase::setColor(unsigned char light, unsigned char color) {
	if (!isConnected()) return;
}

void LaunchpadBase::setPulse(unsigned char light, unsigned char color) {
	if (!isConnected()) return;
}

LaunchpadPro::LaunchpadPro() {
	#ifdef WINDOWS
	PORT_REGEX = std::regex("MIDI(IN|OUT)2\\s\\((\\d+\\s-\\s)?Launchpad\\sPro(\\s\\d+)?\\)");
	#else
	PORT_REGEX = std::regex("Launchpad\\sPro(\\s\\d+)?\\sStandalone\\sPort");
	#endif
}

int LaunchpadPro::connect() {
	int status = LaunchpadBase::connect();
	if (status == 0) {
		std::vector<unsigned char> device_info;

		// Inquiry Device
		sendMessage(240, 126, 127, 6, 1, 247);
		while (device_info.size() == 0) {
			getMessage(&device_info);
			usleep(10000); // Sleep for 10 milliseconds ... platform-dependent.
		}

		sendMessage(240, 0, 32, 41, 2, 16, 33, 1, 247); // Set to Standalone Mode
		sendMessage(240, 0, 32, 41, 2, 16, 44, 3, 247); // Set to Programmer Mode
	}
	return status;
}

void LaunchpadPro::disconnect() {
	if (isConnected()) {
		sendMessage(240, 0, 32, 41, 2, 16, 14, 0, 247); // Clear all LED colors
		sendMessage(240, 0, 32, 41, 2, 16, 44, 0, 247); // Set to Note Mode
		LaunchpadBase::disconnect();
	}
}

bool LaunchpadPro::isConnected() {
	return LaunchpadBase::isConnected();
}

void LaunchpadPro::setColor(unsigned char light, unsigned char color) {
	if (!isConnected()) return;
	sendMessage(240, 0, 32, 41, 2, 16, 10, light, color, 247);
}

void LaunchpadPro::setColor(unsigned char light,
                            unsigned char red, unsigned char green, unsigned char blue) {
	if (!isConnected()) return;
	sendMessage(240, 0, 32, 41, 2, 16, 11, light, red, green, blue, 247);
}

void LaunchpadPro::setFlash(unsigned char light, unsigned char color) {
	if (!isConnected()) return;
	sendMessage(240, 0, 32, 41, 2, 16, 35, light, color, 247);
}

void LaunchpadPro::setPulse(unsigned char light, unsigned char color) {
	if (!isConnected()) return;
	sendMessage(240, 0, 32, 41, 2, 16, 40, light, color, 247);
}

void LaunchpadPro::displayText(unsigned int color, std::string text) {
	if (!isConnected()) return;

	message->push_back(240);
	message->push_back(0);
	message->push_back(32);
	message->push_back(41);
	message->push_back(2);
	message->push_back(16);
	message->push_back(20);
	message->push_back(color);

	bool slash = false;
	for (int i = 0; i < text.size(); ++i) {
		if (slash) {
			slash = false;
			switch (text[i]) { // XXX There's a better way to do this
				case '/':
					message->push_back('/');
					break;
				case '1':
					message->push_back(1);
					break;
				case '2':
					message->push_back(2);
					break;
				case '3':
					message->push_back(3);
					break;
				case '4':
					message->push_back(4);
					break;
				case '5':
					message->push_back(5);
					break;
				case '6':
					message->push_back(6);
					break;
				case '7':
					message->push_back(7);
					break;
				default:
					message->push_back(text[i]);
					break;
			}
		} else if (text[i] == '/') {
			slash = true;
		} else {
			message->push_back(text[i]);
		}
	}
	message->push_back(247);

	sendMessage();
}

void LaunchpadPro::displayText(unsigned int color, unsigned int speed,
                               std::string text) {
	displayText(color, "/" + std::to_string(speed) + text);
}

void LaunchpadPro::stopText() {
	if (!isConnected()) return;
	sendMessage(240, 0, 32, 41, 2, 16, 20, 247);
}

LaunchpadS::LaunchpadS() {
	PORT_REGEX = std::regex("Launchpad S");
}

int LaunchpadS::connect() {
	int status = LaunchpadBase::connect();

	if (status == 0) {
		std::vector<unsigned char> device_info;

		// Inquiry Device
		sendMessage( 240, 126, 127, 6, 1, 247 );
		while (device_info.size() == 0) {
			getMessage(&device_info);
			usleep( 10000 ); // Sleep for 10 milliseconds ... platform-dependent.
		}

		sendMessage( 176, 0, 2, -1 ); // Set to Drum Rack Mode
	}
	return status;
}

void LaunchpadS::disconnect() {
	if (isConnected()) {
		sendMessage( 176, 0, 0, -1 ); // Reset
		LaunchpadBase::disconnect();
	}
}

bool LaunchpadS::isConnected() {
	return LaunchpadBase::isConnected();
}

unsigned char LaunchpadS::pro_to_s_note(unsigned char pro_note, unsigned char msg_type) {
	if (msg_type == 176) return pro_note;
	return pro_note;
}

unsigned char LaunchpadS::pro_to_s_color(unsigned char pro_color) {
	return pro_color;
}

void LaunchpadS::setColor(unsigned char light, unsigned char color) {
	if (!isConnected()) return;
	message->push_back(144);
	message->push_back(pro_to_s_note(light, 144 /*msg_type*/));
	message->push_back(color);

	sendMessage();
}

void LaunchpadS::setPulse(unsigned char light, unsigned char color) {
	if (!isConnected()) return;
	// XXX Implement me!
}

LaunchpadPro *launchpad = new LaunchpadPro();
