//
// Nightwave Global - Helpers.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// https://www.nightwave.co/
//

#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>

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
#elif __linux__
	#define LINUX 1
	#define APP_ICON "icons/icon_64.png"
#else // Linux, Unix, POSIX, iOS, Android...
	#warning "Unknown/unsupported compiler/operating system"
	#define APP_ICON "icons/icon_64.png"
#endif

#define PI 3.14159265

std::string getResourcePath(const char *resource_name);
std::string to_padded_string(int value, int width);
std::string to_padded_string(double value, int width, int precision);

double threeway_max(double val_a, double val_b, double val_c);
int threeway_max(int val_a, int val_b, int val_c);
double threeway_min(double val_a, double val_b, double val_c);
int threeway_min(int val_a, int val_b, int val_c);
double clamp(double val, double min, double max);
int clamp(int val, int min, int max);
