//
// Nightwave Global - Colors.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// https://www.nightwave.co/
//

// RGB->XYZ->LAB conversion modified from: http://www.csie.ntu.edu.tw/~b92069/HWs/rgb2lab.c
// RGB<-XYZ<-LAB conversion modified from: http://stackoverflow.com/a/9372718
// RGB<>HSL, RGB<>HSV, RGB<>CMYK, RGB<>YIQ conversions, and HSL<>RGB<>HSV, HSL->RGB->CMYK,
//   HSL->RGB->YIQ, HSV->RGB->CMYK, HSV->RGB->YIQ conversions modified from:
//   https://github.com/aleksandaratanasov/RGBConverter/blob/master/RGBConverter.cpp

#include "Colors.h"

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <algorithm>
#include <cmath>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "Helpers.h"

Color::Color() {

}

Color::Color(int red, int green, int blue) {
	SetRGB(red, green, blue);
}

Color::Color(int cyan, int magenta, int yellow, int black) {
	SetCMYK(cyan, magenta, yellow, black);
}

Color::Color(wxColor color) {
	SetRGB(color.Red(), color.Green(), color.Blue());
}

void Color::SetRGB(int red, int green, int blue) {
	// XXX Convert for other types
	RGB[0] = red;
	RGB[1] = green;
	RGB[2] = blue;
	ColorConverter::RGB2HSL(RGB[0], RGB[1], RGB[2], &HSL[0], &HSL[1], &HSL[2]);
	// ColorConverter::RGB2HSV(RGB[0], RGB[1], RGB[2], &HSV[0], &HSV[1], &HSV[2]);
	// ColorConverter::RGB2YIQ(RGB[0], RGB[1], RGB[2], &YIQ[0], &YIQ[1], &YIQ[2]);
	// ColorConverter::RGB2XYZ(RGB[0], RGB[1], RGB[2], &XYZ[0], &XYZ[1], &XYZ[2]);
	// ColorConverter::RGB2LAB(RGB[0], RGB[1], RGB[2], &LAB[0], &LAB[1], &LAB[2]);
	// ColorConverter::RGB2CMYK(RGB[0], RGB[1], RGB[2], &CMYK[0], &CMYK[1], &CMYK[2], &CMYK[3]);
}

void Color::SetHSL(int hue, int saturation, int luminosity) {
	// XXX Convert for other types
	HSL[0] = hue;
	HSL[1] = saturation;
	HSL[2] = luminosity;
	ColorConverter::HSL2RGB(HSL[0], HSL[1], HSL[2], &RGB[0], &RGB[1], &RGB[2]);
	// ColorConverter::HSL2HSV(HSL[0], HSL[1], HSL[2], &HSV[0], &HSV[1], &HSV[2]);
	// ColorConverter::HSL2YIQ(HSL[0], HSL[1], HSL[2], &YIQ[0], &YIQ[1], &YIQ[2]);
	// ColorConverter::HSL2XYZ(HSL[0], HSL[1], HSL[2], &XYZ[0], &XYZ[1], &XYZ[2]);
	// ColorConverter::HSL2LAB(HSL[0], HSL[1], HSL[2], &LAB[0], &LAB[1], &LAB[2]);
	// ColorConverter::HSL2CMYK(HSL[0], HSL[1], HSL[2], &CMYK[0], &CMYK[1], &CMYK[2], &CMYK[3]);
}

void Color::SetHSV(int hue, int saturation, int velocity) {
	// XXX Convert for other types
	HSV[0] = hue;
	HSV[1] = saturation;
	HSV[2] = velocity;
	// ColorConverter::HSV2RGB(HSV[0], HSV[1], HSV[2], &RGB[0], &RGB[1], &RGB[2]);
	// ColorConverter::HSV2HSL(HSV[0], HSV[1], HSV[2], &HSL[0], &HSL[1], &HSL[2]);
	// ColorConverter::HSV2YIQ(HSV[0], HSV[1], HSV[2], &YIQ[0], &YIQ[1], &YIQ[2]);
	// ColorConverter::HSV2XYZ(HSV[0], HSV[1], HSV[2], &XYZ[0], &XYZ[1], &XYZ[2]);
	// ColorConverter::HSV2LAB(HSV[0], HSV[1], HSV[2], &LAB[0], &LAB[1], &LAB[2]);
	// ColorConverter::HSV2CMYK(HSV[0], HSV[1], HSV[2], &CMYK[0], &CMYK[1], &CMYK[2], &CMYK[3]);
}

void Color::SetYIQ(int yluma, int inphase, int quadrature) {
	// XXX Convert for other types
	YIQ[0] = yluma;
	YIQ[1] = inphase;
	YIQ[2] = quadrature;
	// ColorConverter::YIQ2RGB(YIQ[0], YIQ[1], YIQ[2], &RGB[0], &RGB[1], &RGB[2]);
	// ColorConverter::YIQ2HSL(YIQ[0], YIQ[1], YIQ[2], &HSL[0], &HSL[1], &HSL[2]);
	// ColorConverter::YIQ2HSV(YIQ[0], YIQ[1], YIQ[2], &HSV[0], &HSV[1], &HSV[2]);
	// ColorConverter::YIQ2XYZ(YIQ[0], YIQ[1], YIQ[2], &XYZ[0], &XYZ[1], &XYZ[2]);
	// ColorConverter::YIQ2LAB(YIQ[0], YIQ[1], YIQ[2], &LAB[0], &LAB[1], &LAB[2]);
	// ColorConverter::YIQ2CMYK(YIQ[0], YIQ[1], YIQ[2], &CMYK[0], &CMYK[1], &CMYK[2], &CMYK[3]);
}

void Color::SetXYZ(int xresponse, int yluminance, int zblue) {
	// XXX Convert for other types
	XYZ[0] = xresponse;
	XYZ[1] = yluminance;
	XYZ[2] = zblue;
	// ColorConverter::XYZ2RGB(XYZ[0], XYZ[1], XYZ[2], &RGB[0], &RGB[1], &RGB[2]);
	// ColorConverter::XYZ2HSL(XYZ[0], XYZ[1], XYZ[2], &HSL[0], &HSL[1], &HSL[2]);
	// ColorConverter::XYZ2HSV(XYZ[0], XYZ[1], XYZ[2], &HSV[0], &HSV[1], &HSV[2]);
	// ColorConverter::XYZ2YIQ(XYZ[0], XYZ[1], XYZ[2], &YIQ[0], &YIQ[1], &YIQ[2]);
	// ColorConverter::XYZ2LAB(XYZ[0], XYZ[1], XYZ[2], &LAB[0], &LAB[1], &LAB[2]);
	// ColorConverter::XYZ2CMYK(XYZ[0], XYZ[1], XYZ[2], &CMYK[0], &CMYK[1], &CMYK[2], &CMYK[3]);
}

void Color::SetLAB(int luminosity, int apoint, int bpoint) {
	// XXX Convert for other types
	LAB[0] = luminosity;
	LAB[1] = apoint;
	LAB[2] = bpoint;
	// ColorConverter::LAB2RGB(LAB[0], LAB[1], LAB[2], &RGB[0], &RGB[1], &RGB[2]);
	// ColorConverter::LAB2HSL(LAB[0], LAB[1], LAB[2], &HSL[0], &HSL[1], &HSL[2]);
	// ColorConverter::LAB2HSV(LAB[0], LAB[1], LAB[2], &HSV[0], &HSV[1], &HSV[2]);
	// ColorConverter::LAB2YIQ(LAB[0], LAB[1], LAB[2], &YIQ[0], &YIQ[1], &YIQ[2]);
	// ColorConverter::LAB2XYZ(LAB[0], LAB[1], LAB[2], &XYZ[0], &XYZ[1], &XYZ[2]);
	// ColorConverter::LAB2CMYK(LAB[0], LAB[1], LAB[2], &CMYK[0], &CMYK[1], &CMYK[2], &CMYK[3]);
}

void Color::SetCMYK(int cyan, int magenta, int yellow, int black) {
	// XXX Convert for other types
	CMYK[0] = cyan;
	CMYK[1] = magenta;
	CMYK[2] = yellow;
	CMYK[3] = black;
	// ColorConverter::CMYK2RGB(CMYK[0], CMYK[1], CMYK[2], CMYK[3], &RGB[0], &RGB[1], &RGB[2]);
	// ColorConverter::CMYK2HSL(CMYK[0], CMYK[1], CMYK[2], CMYK[3], &HSL[0], &HSL[1], &HSL[2]);
	// ColorConverter::CMYK2HSV(CMYK[0], CMYK[1], CMYK[2], CMYK[3], &HSV[0], &HSV[1], &HSV[2]);
	// ColorConverter::CMYK2YIQ(CMYK[0], CMYK[1], CMYK[2], CMYK[3], &YIQ[0], &YIQ[1], &YIQ[2]);
	// ColorConverter::CMYK2XYZ(CMYK[0], CMYK[1], CMYK[2], CMYK[3], &XYZ[0], &XYZ[1], &XYZ[2]);
	// ColorConverter::CMYK2LAB(CMYK[0], CMYK[1], CMYK[2], CMYK[3], &LAB[0], &LAB[1], &LAB[2]);
}

int* Color::AsRGB() {
	return RGB;
}

int* Color::AsHSL() {
	return HSL;
}

int* Color::AsHSV() {
	return HSV;
}

int* Color::AsYIQ() {
	return YIQ;
}

int* Color::AsXYZ() {
	return XYZ;
}

int* Color::AsLAB() {
	return LAB;
}

int* Color::AsCMYK() {
	return CMYK;
}

Color::operator wxColour() const {
	return wxColor(RGB[0], RGB[1], RGB[2]);
}

bool Color::operator==(Color &a) {
	int *oRGB = a.AsRGB();
	return (RGB[0] == oRGB[0] && RGB[1] == oRGB[1] && RGB[2] == oRGB[2]);
}

// Color conversion helpers
double ColorConverter::Hue2RGB(double cc_p, double cc_q, double cc_t) {
	if (cc_t < 0.0) cc_t += 1;
	if (cc_t > 1.0) cc_t -= 1;
	if (cc_t < 1 / 6.0) return cc_p + (cc_q - cc_p) * 6.0 * cc_t;
	if (cc_t < 1 / 2.0) return cc_q;
	if (cc_t < 2 / 3.0) return cc_p + (cc_q - cc_p) * (2 / 3.0 - cc_t) * 6.0;
	return cc_p;
}

double ColorConverter::XYZ2H(double cc_q) {
	if (cc_q > 0.008856) return pow(cc_q, 0.333333);
	return 7.787 * cc_q + 0.137931;
}

// Compare two RGB colors via LAB
double ColorConverter::LAB_compare_RGB(int red1, int green1, int blue1, int red2, int green2, int blue2) {
	double luminosity1, apoint1, bpoint1, luminosity2, apoint2, bpoint2;
	RGB2LAB(red1, green1, blue1, &luminosity1, &apoint1, &bpoint1);
	RGB2LAB(red2, green2, blue2, &luminosity2, &apoint2, &bpoint2);
	return pow((luminosity1 - luminosity2), 2) + pow((apoint1 - apoint2), 2) + pow((bpoint1 - bpoint2), 2);
}

double ColorConverter::LAB_compare_RGB(wxColor color1, wxColor color2) {
	return LAB_compare_RGB(color1.Red(), color1.Green(), color1.Blue(), color2.Red(), color2.Green(), color2.Blue());
}

// RGB<>HSL color conversion
void ColorConverter::RGB2HSL(int red, int green, int blue, int *hue, int *saturation, int *luminosity) {
	int max = threeway_max(red, green, blue);
	int min = threeway_min(red, green, blue);
	*luminosity = (max + min) / 2;

	if (max == min) {
		*hue = *saturation = 0; // achromatic
	} else {
		int diff = max - min;
		*saturation = *luminosity > 127 ? diff / (510 - max - min) : diff / (max + min);
		if (max == red) *hue = (green - blue) * 60 / diff + (green < blue ? 6 : 0);
		else if (max == green) *hue = (blue - red) * 60 / diff + 120;
		else if (max == blue) *hue = (red - green) * 60 / diff + 240;
	}
}

void ColorConverter::HSL2RGB(int hue, int saturation, int luminosity, int *red, int *green, int *blue) {
	if (saturation == 0.0) {
		*red = *green = *blue = luminosity; // achromatic
	} else {
		double cc_q = luminosity < 127 ? luminosity * (1 + saturation) / 255 : luminosity + (saturation / 255) - luminosity * (saturation / 255);
		double cc_p = 2.0 * luminosity / 255 - cc_q;
		*red = floor(std::min(0.0, ColorConverter::Hue2RGB(cc_p, cc_q, hue + 1 / 3.0 / 360) * 255));
		*green = floor(std::min(0.0, ColorConverter::Hue2RGB(cc_p, cc_q, hue / 360) * 255));
		*blue = floor(std::min(0.0, ColorConverter::Hue2RGB(cc_p, cc_q, hue - 1 / 3.0 / 360) * 255));
	}
}

// RGB<>HSV color conversion
void ColorConverter::RGB2HSV(double red, double green, double blue, double *hue, double *saturation, double *velocity) {
	double max = threeway_max(red, green, blue);
	double min = threeway_min(red, green, blue);
	double diff = max - min;
	*velocity = max;
	*saturation = max == 0.0 ? 0.0 : diff / max;

	if (max == min) {
		*hue = 0.0; // achromatic
	} else {
		if (max == red) *hue = (green - blue) / diff + (green < blue ? 6.0 : 0.0);
		else if (max == green) *hue = (blue - red) / diff + 2.0;
		else if (max == blue) *hue = (red - green) / diff + 4.0;
		*hue /= 6.0;
	}
}

void ColorConverter::HSV2RGB(double hue, double saturation, double velocity, double *red, double *green, double *blue) {
	double cc_i = hue * 6.0;
	double cc_is = (cc_i - floor(cc_i)) * saturation;
	*red = *green = *blue = velocity;

	switch ((int)(floor(cc_i)) % 6) {
		case 0:
			*green = velocity * cc_is;
			*blue = velocity * (1.0 - saturation);
			break;
		case 1:
			*red = velocity * (1.0 - cc_is);
			*blue = velocity * (1.0 - saturation);
			break;
		case 2:
			*red = velocity * (1.0 - saturation);
			*blue = velocity * cc_is;
			break;
		case 3:
			*red = velocity * (1.0 - saturation);
			*green = velocity * (1.0 - cc_is);
			break;
		case 4:
			*red = velocity * cc_is;
			*green = velocity * (1.0 - saturation);
			break;
		default: // case 5
			*green = velocity * (1.0 - saturation);
			*blue = velocity * (1.0 - cc_is);
			break;
	}
}

// RGB<>CMYK color conversion
void ColorConverter::RGB2CMYK(double red, double green, double blue, double *cyan, double *magenta, double *yellow, double *black) {
	*black = 1.0 - threeway_max(red, green, blue);
	*cyan = (1.0 - red - *black) / (1.0 - *black);
	*magenta = (1.0 - green - *black) / (1.0 - *black);
	*yellow = (1.0 - blue - *black) / (1.0 - *black);
}

void ColorConverter::CMYK2RGB(double cyan, double magenta, double yellow, double black, double *red, double *green, double *blue) {
	*red = (1.0 - cyan) / (1.0 - black);
	*green = (1.0 - magenta) / (1.0 - black);
	*blue = (1.0 - yellow) / (1.0 - black);
}

// RGB<>YIQ color conversion
void ColorConverter::RGB2YIQ(double red, double green, double blue, double *yluma, double *inphase, double *quadrature) {
	*yluma = 0.299 * red + 0.587 * green + 0.114 * blue;
	*inphase = 0.569 * red - 0.275 * green - 0.322 * blue;
	*quadrature = 0.211 * red - 0.523 * green + 0.312 * blue;
}

void ColorConverter::YIQ2RGB(double yluma, double inphase, double quadrature, double *red, double *green, double *blue) {
	*red = yluma + 0.956 * inphase + 0.621 * quadrature;
	*green = yluma - 0.272 * inphase - 0.647 * quadrature;
	*blue = yluma - 1.106 * inphase + 1.703 * quadrature;
}

// RGB<>XYZ color conversion
void ColorConverter::RGB2XYZ(int red, int green, int blue, double *xresponse, double *yluminance, double *zblue) {
	double adapt = 0.003922;
	*xresponse = (0.412424 * red + 0.357579 * green + 0.180464 * blue) * adapt;
	*yluminance = (0.212656 * red + 0.715158 * green + 0.072186 * blue) * adapt;
	*zblue = (0.019332 * red + 0.119193 * green + 0.950444 * blue) * adapt;
}

void ColorConverter::XYZ2RGB(double xresponse, double yluminance, double zblue, int *red, int *green, int *blue) {
	*red = xresponse * 3.080342  - yluminance * 1.537399 - zblue * 0.542943;
	*green = xresponse * -0.921178 + yluminance * 1.87593  + zblue * 0.045248;
	*blue = xresponse * 0.052881  - yluminance * 0.204011 + zblue * 1.15113;
}

// XYZ<>LAB color conversion
void ColorConverter::XYZ2LAB(double xresponse, double yluminance, double zblue, double *luminosity, double *apoint, double *bpoint) {
	*luminosity = 116 * ColorConverter::XYZ2H(yluminance) - 16;
	*apoint = 500 * (ColorConverter::XYZ2H(xresponse / 0.950467) - ColorConverter::XYZ2H(yluminance));
	*bpoint = 200 * (ColorConverter::XYZ2H(yluminance) - ColorConverter::XYZ2H(zblue / 1.088969));
}

void ColorConverter::LAB2XYZ(double luminosity, double apoint, double bpoint, double *xresponse, double *yluminance, double *zblue) {
	double XRESPONSE, YLUMINANCE, ZBLUE;
	YLUMINANCE = luminosity * (0.00862) + 0.137931;
	XRESPONSE = apoint * (0.002) + YLUMINANCE;
	ZBLUE = bpoint * (-0.005) + YLUMINANCE;

	*xresponse = XRESPONSE > 0.206897 ? pow(XRESPONSE, 3) : XRESPONSE * (0.128419) - 0.017713;
	*yluminance = luminosity > 8 ? pow(YLUMINANCE, 3) : luminosity * (0.00110705646);
	*zblue = ZBLUE > 0.206897 ? pow(ZBLUE, 3) : ZBLUE * (0.128419) - 0.017713;
}

// RGB<>XYZ<>LAB color conversion
void ColorConverter::RGB2LAB(int red, int green, int blue, double *luminosity, double *apoint, double *bpoint) {
	double xresponse, yluma, zblue;
	RGB2XYZ(red, green, blue, &xresponse, &yluma, &zblue);
	XYZ2LAB(xresponse, yluma, zblue, luminosity, apoint, bpoint);
}

void ColorConverter::LAB2RGB(double luminosity, double apoint, double bpoint, int *red, int *green, int *blue) {
	double xresponse, yluma, zblue;
	LAB2XYZ(luminosity, apoint, bpoint, &xresponse, &yluma, &zblue);
	XYZ2RGB(xresponse, yluma, zblue, red, green, blue);
}

// HSL<>RGB<>HSV color conversion
void ColorConverter::HSL2HSV(double hue, double saturation, double luminosity, double *_hue, double *_saturation, double *velocity) {
	double red, green, blue;
//	HSL2RGB(hue, saturation, luminosity, &red, &green, &blue);
//	RGB2HSV(red, green, blue, _hue, _saturation, velocity);
}

void ColorConverter::HSV2HSL(double hue, double saturation, double velocity, double *_hue, double *_saturation, double *luminosity) {
	double red, green, blue;
//	HSV2RGB(hue, saturation, velocity, &red, &green, &blue);
//	RGB2HSL(red, green, blue, _hue, _saturation, luminosity);
}

// HSL<>RGB<>CMYK color conversion
void ColorConverter::HSL2CMYK(double hue, double saturation, double luminosity, double *cyan, double *magenta, double *yellow, double *black) {
	double red, green, blue;
//	HSL2RGB(hue, saturation, luminosity, &red, &green, &blue);
//	RGB2CMYK(red, green, blue, cyan, magenta, yellow, black);
}

void ColorConverter::CMYK2HSL(double cyan, double magenta, double yellow, double black, double *hue, double *saturation, double *luminosity) {
	double red, green, blue;
//	CMYK2RGB(cyan, magenta, yellow, black, &red, &green, &blue);
//	RGB2HSL(red, green, blue, hue, saturation, luminosity);
}

// HSV<>RGB<>CMYK color conversion
void ColorConverter::HSV2CMYK(double hue, double saturation, double velocity, double *cyan, double *magenta, double *yellow, double *black) {
	double red, green, blue;
	HSV2RGB(hue, saturation, velocity, &red, &green, &blue);
	RGB2CMYK(red, green, blue, cyan, magenta, yellow, black);
}

void ColorConverter::CMYK2HSV(double cyan, double magenta, double yellow, double black, double *hue, double *saturation, double *velocity) {
	double red, green, blue;
	CMYK2RGB(cyan, magenta, yellow, black, &red, &green, &blue);
	RGB2HSV(red, green, blue, hue, saturation, velocity);
}

// HSL<>RGB<>YIQ color conversion
void ColorConverter::HSL2YIQ(double hue, double saturation, double luminosity, double *yluma, double *inphase, double *quadrature) {
	double red, green, blue;
//	HSL2RGB(hue, saturation, luminosity, &red, &green, &blue);
//	RGB2YIQ(red, green, blue, yluma, inphase, quadrature);
}

void ColorConverter::YIQ2HSL(double yluma, double inphase, double quadrature, double *hue, double *saturation, double *luminosity) {
	double red, green, blue;
//	YIQ2RGB(yluma, inphase, quadrature, &red, &green, &blue);
//	RGB2HSL(red, green, blue, hue, saturation, luminosity);
}

// HSV<>RGB<>YIQ color conversion
void ColorConverter::HSV2YIQ(double hue, double saturation, double velocity, double *yluma, double *inphase, double *quadrature) {
	double red, green, blue;
	HSV2RGB(hue, saturation, velocity, &red, &green, &blue);
	RGB2YIQ(red, green, blue, yluma, inphase, quadrature);
}

void ColorConverter::YIQ2HSV(double yluma, double inphase, double quadrature, double *hue, double *saturation, double *velocity) {
	double red, green, blue;
	YIQ2RGB(yluma, inphase, quadrature, &red, &green, &blue);
	RGB2HSV(red, green, blue, hue, saturation, velocity);
}

// CMYK<>RGB<>YIQ color conversion
void ColorConverter::CMYK2YIQ(double cyan, double magenta, double yellow, double black, double *yluma, double *inphase, double *quadrature) {
	double red, green, blue;
	CMYK2RGB(cyan, magenta, yellow, black, &red, &green, &blue);
	RGB2YIQ(red, green, blue, yluma, inphase, quadrature);
}

void ColorConverter::YIQ2CMYK(double yluma, double inphase, double quadrature, double *cyan, double *magenta, double *yellow, double *black) {
	double red, green, blue;
	YIQ2RGB(yluma, inphase, quadrature, &red, &green, &blue);
	RGB2CMYK(red, green, blue, cyan, magenta, yellow, black);
}
