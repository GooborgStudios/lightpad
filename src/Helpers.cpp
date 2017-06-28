//
// Nightwave Global - Helpers.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch and Light Apacha.
// https://www.nightwave.co/
//

#include "Helpers.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>

// Mac app bundle vs. unbundled executable resource obtainer
#ifdef XCODE_BUNDLE
	#include <CoreFoundation/CoreFoundation.h>
	
	std::string getResourcePath(const char *resource_name) {
		CFStringRef resourceName = CFStringCreateWithCString(NULL, resource_name, kCFStringEncodingUTF8);
		CFURLRef appUrlRef = CFBundleCopyResourceURL(CFBundleGetMainBundle(), resourceName, NULL, NULL);
		CFRelease(resourceName);
		if (appUrlRef == NULL) return std::string(resource_name);
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

// Variable conversion
std::string to_padded_string(int value, int width) {
	std::ostringstream out;
	out << std::setfill('0') << std::setw(width) << value;
	return out.str();
}

std::string to_padded_string(double value, int width, int precision) {
	std::ostringstream out;
	out << std::setfill('0') << std::setw(width+precision) << std::setprecision(precision) << value;
	return out.str();
}

// Math helpers
double threeway_max(double val_a, double val_b, double val_c) {
	return std::max(val_a, std::max(val_b, val_c));
}

int threeway_max(int val_a, int val_b, int val_c) {
	return std::max(val_a, std::max(val_b, val_c));
}

double threeway_min(double val_a, double val_b, double val_c) {
	return std::min(val_a, std::min(val_b, val_c));
}

int threeway_min(int val_a, int val_b, int val_c) {
	return std::min(val_a, std::min(val_b, val_c));
}

double clamp(double val, double min, double max) {
	return std::max(min, std::min(val, max));
}

int clamp(int val, int min, int max) {
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
