//
// Lightpad - Helpers.h
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.nightwave.co/lightpad
//

#ifndef HELPERS_H
#define HELPERS_H

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

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

class ColorConverter {
	// All converters take values from 0.0 to 1.0
	public:
		// Conversion helpers
		static double Hue2RGB(double p, double q, double t);
		static double XYZ2H(double q);
		static double LAB_compare_RGB(double r1, double g1, double b1, double r2, double g2, double b2);
		static double LAB_compare_RGB(int r1, int g1, int b1, int r2, int g2, int b2);

		// Base converters
		static void RGB2HSL(double r, double g, double b, double *h, double *s, double *l);
		static void HSL2RGB(double h, double s, double l, double *r, double *g, double *b);
		static void RGB2HSV(double r, double g, double b, double *h, double *s, double *v);
		static void HSV2RGB(double h, double s, double v, double *r, double *g, double *b);
		static void RGB2CMYK(double r, double g, double b, double *c, double *m, double *y, double *k);
		static void CMYK2RGB(double c, double m, double y, double k, double *r, double *g, double *b);
		static void RGB2YIQ(double r, double g, double b, double *y, double *i, double *q);
		static void YIQ2RGB(double y, double i, double q, double *r, double *g, double *b);
		static void RGB2XYZ(double r, double g, double b, double *x, double *y, double *z);
		static void XYZ2RGB(double x, double y, double z, double *r, double *g, double *b);
		static void XYZ2LAB(double x, double y, double z, double *l, double *a, double *b);
		static void LAB2XYZ(double l, double a, double b, double *x, double *y, double *z);

		// Two-step converters
		static void RGB2LAB(double r, double g, double b, double *l, double *a, double *_b);
		static void LAB2RGB(double l, double a, double b, double *r, double *g, double *_b);
		static void HSL2HSV(double h, double s, double l, double *_h, double *_s, double *v);
		static void HSV2HSL(double h, double s, double v, double *_h, double *_s, double *l);
		static void HSL2CMYK(double h, double s, double l, double *c, double *m, double *y, double *k);
		static void CMYK2HSL(double c, double m, double y, double k, double *h, double *s, double *l);
		static void HSV2CMYK(double h, double s, double v, double *c, double *m, double *y, double *k);
		static void CMYK2HSV(double c, double m, double y, double k, double *h, double *s, double *v);
		static void HSL2YIQ(double h, double s, double l, double *y, double *i, double *q);
		static void YIQ2HSL(double y, double i, double q, double *h, double *s, double *l);
		static void HSV2YIQ(double h, double s, double v, double *y, double *i, double *q);
		static void YIQ2HSV(double y, double i, double q, double *h, double *s, double *v);
		static void CMYK2YIQ(double c, double m, double y, double k, double *_y, double *i, double *q);
		static void YIQ2CMYK(double y, double i, double q, double *c, double *m, double *_y, double *k);
};

#endif
