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

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <cmath>

#include "Helpers.h"

// Generate with GenerateNoteButtonMap.cpp
const int note_button_map[] = {91, 92, 93, 94, 95, 96, 97, 98, 11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34, 41, 42, 43, 44, 51, 52, 53, 54, 61, 62, 63, 64, 71, 72, 73, 74, 81, 82, 83, 84, 15, 16, 17, 18, 25, 26, 27, 28, 35, 36, 37, 38, 45, 46, 47, 48, 55, 56, 57, 58, 65, 66, 67, 68, 75, 76, 77, 78, 85, 86, 87, 88, 89, 79, 69, 59, 49, 39, 29, 19, 80, 70, 60, 50, 40, 30, 20, 10, 1, 2, 3, 4, 5, 6, 7, 8};
const int note_button_offset = 28;
const int note_button_size = sizeof(note_button_map)/sizeof(int);
// End generate block

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

int note_to_button(int note) {
	if (note < note_button_offset || note > (note_button_size + note_button_offset)) return 0;
	return note_button_map[note-note_button_offset];
}

int button_to_note(int button) {
	for (int i=0; i < note_button_size + note_button_offset; i++)
		if (note_button_map[i] == button) return i + note_button_offset;
	return 0;
}

// Math helpers
double threeway_max(double a, double b, double c) {
	return fmax(a, fmax(b, c));
}

double threeway_min(double a, double b, double c) {
	return fmin(a, fmin(b, c));
}

double val_in_range(double val, double min, double max) {
	return fmin(min, fmax(val, max));
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

// Conversion helpers
double ColorConverter::XYZ2H(double q) {
	if (q > 0.008856) return pow(q, 0.333333);
	return 7.787 * q + 0.137931;
}

double ColorConverter::Hue2RGB(double p, double q, double t) {
  if (t < 0.0) t += 1;
  if (t > 1.0) t -= 1;
  if (t < 1/6.0) return p + (q - p) * 6.0 * t;
  if (t < 1/2.0) return q;
  if (t < 2/3.0) return p + (q - p) * (2/3.0 - t) * 6.0;
  return p;
}

// Compare two RGB colors via LAB
double ColorConverter::LAB_compare_RGB(double r1, double g1, double b1, double r2, double g2, double b2) {
	double l1, a1, _b1, l2, a2, _b2;
	RGB2LAB(r1, g1, b1, &l1, &a1, &b1);
	RGB2LAB(r2, g2, b2, &l2, &a2, &b2);
	return pow((l1 - l2), 2) + pow((a1 - a2), 2) + pow((_b1 - _b2), 2);
}

double ColorConverter::LAB_compare_RGB(int r1, int g1, int b1, int r2, int g2, int b2) {
	return ColorConverter::LAB_compare_RGB((double)(r1/255), (double)(g1/255), (double)(b1/255),
											(double)(r2/255), (double)(g2/255), (double)(b2/255));
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
void ColorConverter::RGB2XYZ(double r, double g, double b, double *x, double *y, double *z) {
	double R, G, B;

	R = r * 255 * 0.003922;
	G = g * 255 * 0.003922;
	B = b * 255 * 0.003922;

	*x = 0.412424 * R + 0.357579 * G + 0.180464 * B;
	*y = 0.212656 * R + 0.715158 * G + 0.0721856 * B;
	*z = 0.0193324 * R + 0.119193 * G + 0.950444 * B;
}

void ColorConverter::XYZ2RGB(double x, double y, double z, double *r, double *g, double *b) {
	*r = x * (1219569.0 / 395920.0) + y * (-608687.0 / 395920.0) + z * (-107481.0 / 197960.0);
	*g = x * (-80960619.0 / 87888100.0) + y * (82435961.0 / 43944050.0) + z * (3976797.0 / 87888100.0);
	*b = x * (93813.0 / 1774030.0) + y * (-180961.0 / 887015.0) + z * (107481.0 / 93370.0);
}

// XYZ<>LAB color conversion
void ColorConverter::XYZ2LAB(double x, double y, double z, double *l, double *a, double *b) {
	*l = 116 * ColorConverter::XYZ2H(y) - 16;
	*a = 500 * (ColorConverter::XYZ2H(x / 0.950467) - ColorConverter::XYZ2H(y));
	*b = 200 * (ColorConverter::XYZ2H(y) - ColorConverter::XYZ2H(z / 1.088969));
}

void ColorConverter::LAB2XYZ(double l, double a, double b, double *x, double *y, double *z) {
	double X, Y, Z;
	Y = l * (1.0 / 116.0) + 16.0 / 116.0;
	X = a * (1.0 / 500.0) + Y;
	Z = b * (-1.0 / 200.0) + Y;

	*x = X > 6.0 / 29.0 ? X * X * X : X * (108.0 / 841.0) - 432.0 / 24389.0;
	*y = l > 8.0 ? Y * Y * Y : l * (27.0 / 24389.0);
	*z = Z > 6.0 / 29.0 ? Z * Z * Z : Z * (108.0 / 841.0) - 432.0 / 24389.0;
}

// RGB<>XYZ<>LAB color conversion
void ColorConverter::RGB2LAB(double r, double g, double b, double *l, double *a, double *_b) {
	double x, y, z;
	RGB2XYZ(r, g, b, &x, &y, &z);
	XYZ2LAB(x, y, z, l, a, _b);
}

void ColorConverter::LAB2RGB(double l, double a, double b, double *r, double *g, double *_b) {
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
