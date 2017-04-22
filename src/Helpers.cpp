//
// Lightpad - Helpers.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "Helpers.h"
#include "Launchpad.h"

#include <algorithm>
#include <cmath>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#ifdef XCODE_BUNDLE
#include <CoreFoundation/CoreFoundation.h>

std::string getResourcePath(const char *resource_name) {
	CFStringRef resourceName = CFStringCreateWithCString(NULL, resource_name, kCFStringEncodingUTF8);
	CFURLRef appUrlRef = CFBundleCopyResourceURL(CFBundleGetMainBundle(), resourceName, NULL, NULL);
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

LaunchpadPro *launchpad = new LaunchpadPro();
