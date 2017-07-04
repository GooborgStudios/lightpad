//
// Nightwave Global - Colors.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// https://www.nightwave.co/
//

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#define Color wxColour

namespace ColorConverter {
	// Color conversion helpers
	double Hue2RGB(double cc_p, double cc_q, double cc_t);
	double XYZ2H(double cc_q);
	double LAB_compare_RGB(int red1, int green1, int blue1, int red2, int green2, int blue2);
	double LAB_compare_RGB(wxColor color1, wxColor color2);

	// Base converters
	void RGB2HSL(double red, double green, double blue, double *hue, double *saturation, double *luminosity); // XXX Convert to int (0-255)
	void HSL2RGB(double hue, double saturation, double luminosity, double *red, double *green, double *blue); // XXX Convert to int (0-255)
	void RGB2HSV(double red, double green, double blue, double *hue, double *saturation, double *velocity); // XXX Convert to int (0-255)
	void HSV2RGB(double hue, double saturation, double velocity, double *red, double *green, double *blue); // XXX Convert to int (0-255)
	void RGB2CMYK(double red, double green, double blue, double *cyan, double *magenta, double *yellow, double *black); // XXX Convert to int (0-255)
	void CMYK2RGB(double cyan, double magenta, double yellow, double black, double *red, double *green, double *blue); // XXX Convert to int (0-255)
	void RGB2YIQ(double red, double green, double blue, double *yluma, double *inphase, double *quadrature); // XXX Convert to int (0-255)
	void YIQ2RGB(double yluma, double inphase, double quadrature, double *red, double *green, double *blue); // XXX Convert to int (0-255)
	void RGB2XYZ(int red, int green, int blue, double *xresponse, double *yluminance, double *zblue); // XXX Convert to int (0-255)
	void XYZ2RGB(double xresponse, double yluminance, double zblue, int *red, int *green, int *blue); // XXX Convert to int (0-255)
	void XYZ2LAB(double xresponse, double yluminance, double zblue, double *luminosity, double *apoint, double *bpoint); // XXX Convert to int (0-255)
	void LAB2XYZ(double luminosity, double apoint, double bpoint, double *xresponse, double *yluminance, double *zblue); // XXX Convert to int (0-255)

	// Two-step converters
	void RGB2LAB(int red, int green, int blue, double *luminosity, double *apoint, double *bpoint); // XXX Convert to int (0-255)
	void LAB2RGB(double luminosity, double apoint, double bpoint, int *red, int *green, int *blue); // XXX Convert to int (0-255)
	void HSL2HSV(double hue, double saturation, double luminosity, double *_hue, double *_saturation, double *velocity); // XXX Convert to int (0-255)
	void HSV2HSL(double hue, double saturation, double velocity, double *_hue, double *_saturation, double *luminosity); // XXX Convert to int (0-255)
	void HSL2CMYK(double hue, double saturation, double luminosity, double *cyan, double *magenta, double *yellow, double *black); // XXX Convert to int (0-255)
	void CMYK2HSL(double cyan, double magenta, double yellow, double black, double *hue, double *saturation, double *luminosity); // XXX Convert to int (0-255)
	void HSV2CMYK(double hue, double saturation, double velocity, double *cyan, double *magenta, double *yellow, double *black); // XXX Convert to int (0-255)
	void CMYK2HSV(double cyan, double magenta, double yellow, double black, double *hue, double *saturation, double *velocity); // XXX Convert to int (0-255)
	void HSL2YIQ(double hue, double saturation, double luminosity, double *yluma, double *inphase, double *quadrature); // XXX Convert to int (0-255)
	void YIQ2HSL(double yluma, double inphase, double quadrature, double *hue, double *saturation, double *luminosity); // XXX Convert to int (0-255)
	void HSV2YIQ(double hue, double saturation, double velocity, double *yluma, double *inphase, double *quadrature); // XXX Convert to int (0-255)
	void YIQ2HSV(double yluma, double inphase, double quadrature, double *hue, double *saturation, double *velocity); // XXX Convert to int (0-255)
	void CMYK2YIQ(double cyan, double magenta, double yellow, double black, double *yluma, double *inphase, double *quadrature); // XXX Convert to int (0-255)
	void YIQ2CMYK(double yluma, double inphase, double quadrature, double *cyan, double *magenta, double *yellow, double *black); // XXX Convert to int (0-255)
};
