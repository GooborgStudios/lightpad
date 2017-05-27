//
// Lightpad - Colors.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
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
double ColorConverter::LAB_compare_RGB(int red1, int grn1, int blu1,
                                       int red2, int grn2, int blu2) {
	double lum1, apt1, bpt1, lum2, apt2, bpt2;
	RGB2LAB(red1, grn1, blu1, &lum1, &apt1, &bpt1);
	RGB2LAB(red2, grn2, blu2, &lum2, &apt2, &bpt2);
	return pow((lum1 - lum2), 2) + pow((apt1 - apt2), 2) + pow((bpt1 - bpt2), 2);
}

double ColorConverter::LAB_compare_RGB(wxColor clr1, wxColor clr2) {
	return LAB_compare_RGB(clr1.Red(), clr1.Green(), clr1.Blue(),
	                       clr2.Red(), clr2.Green(), clr2.Blue());
}

// RGB<>HSL color conversion
void ColorConverter::RGB2HSL(double red, double grn, double blu,
                             double *hue, double *sat, double *lum) {
	double max = threeway_max(red, grn, blu);
	double min = threeway_min(red, grn, blu);
	*lum = (max + min) / 2;

	if (max == min) {
		*hue = *sat = 0.0; // achromatic
	} else {
		double diff = max - min;
		*sat = *lum > 0.5 ? diff / (2.0 - max - min) : diff / (max + min);
		if (max == red) *hue = (grn - blu) / diff + (grn < blu ? 6.0 : 0.0);
		else if (max == grn) *hue = (blu - red) / diff + 2.0;
		else if (max == blu) *hue = (red - grn) / diff + 4.0;
		*hue /= 6.0;
	}
}

void ColorConverter::HSL2RGB(double hue, double sat, double lum,
                             double *red, double *grn, double *blu) {
	if (sat == 0.0) {
		*red = *grn = *blu = lum; // achromatic
	} else {
		double cc_q = lum < 0.5 ? lum * (1 + sat) : lum + sat - lum * sat;
		double cc_p = 2.0 * lum - cc_q;
		*red = ColorConverter::Hue2RGB(cc_p, cc_q, hue + 1 / 3.0);
		*grn = ColorConverter::Hue2RGB(cc_p, cc_q, hue);
		*blu = ColorConverter::Hue2RGB(cc_p, cc_q, hue - 1 / 3.0);
	}
}

// RGB<>HSV color conversion
void ColorConverter::RGB2HSV(double red, double grn, double blu,
                             double *hue, double *sat, double *vel) {
	double max = threeway_max(red, grn, blu);
	double min = threeway_min(red, grn, blu);
	double diff = max - min;
	*vel = max;
	*sat = max == 0.0 ? 0.0 : diff / max;

	if (max == min) {
		*hue = 0.0; // achromatic
	} else {
		if (max == red) *hue = (grn - blu) / diff + (grn < blu ? 6.0 : 0.0);
		else if (max == grn) *hue = (blu - red) / diff + 2.0;
		else if (max == blu) *hue = (red - grn) / diff + 4.0;
		*hue /= 6.0;
	}
}

void ColorConverter::HSV2RGB(double hue, double sat, double vel,
                             double *red, double *grn, double *blu) {
	unsigned int cc_i = (unsigned int)(hue * 6.0);
	double cc_f = hue * 6.0 - cc_i;
	double cc_p = vel * (1.0 - sat);
	double cc_q = vel * (1.0 - cc_f * sat);
	double cc_t = vel * (1.0 - (1.0 - cc_f) * sat);

	switch (cc_i % 6) {
		case 0:
			*red = vel; *grn = cc_t; *blu = cc_p; break;
		case 1:
			*red = cc_q; *grn = vel; *blu = cc_p; break;
		case 2:
			*red = cc_p; *grn = vel; *blu = cc_t; break;
		case 3:
			*red = cc_p; *grn = cc_q; *blu = vel; break;
		case 4:
			*red = cc_t; *grn = cc_p; *blu = vel; break;
		default: // case 5
			*red = vel; *grn = cc_p; *blu = cc_q; break;
	}
}

// RGB<>CMYK color conversion
void ColorConverter::RGB2CMYK(double red, double grn, double blu, double *cyan,
                              double *mgnta, double *ylw, double *blk) {
	*blk   =  1.0 - threeway_max(red, grn, blu);
	*cyan  = (1.0 - red - *blk) / (1.0 - *blk);
	*mgnta = (1.0 - grn - *blk) / (1.0 - *blk);
	*ylw   = (1.0 - blu - *blk) / (1.0 - *blk);
}

void ColorConverter::CMYK2RGB(double cyan, double mgnta, double ylw, double blk,
                              double *red, double *grn, double *blu) {
	*red = (1.0 - cyan)  / (1.0 - blk);
	*grn = (1.0 - mgnta) / (1.0 - blk);
	*blu = (1.0 - ylw)   / (1.0 - blk);
}

// RGB<>YIQ color conversion
void ColorConverter::RGB2YIQ(double red, double grn, double blu,
                             double *ylum, double *iphs, double *quad) {
	*ylum = 0.299 * red + 0.587 * grn + 0.114 * blu;
	*iphs = 0.569 * red - 0.275 * grn - 0.322 * blu;
	*quad = 0.211 * red - 0.523 * grn + 0.312 * blu;
}

void ColorConverter::YIQ2RGB(double ylum, double iphs, double quad,
                             double *red, double *grn, double *blu) {
	*red = ylum + 0.956 * iphs + 0.621 * quad;
	*grn = ylum - 0.272 * iphs - 0.647 * quad;
	*blu = ylum - 1.106 * iphs + 1.703 * quad;
}

// RGB<>XYZ color conversion
void ColorConverter::RGB2XYZ(int red, int grn, int blu, double *xrsp, double *ylum, double *zblu) {
	double adapt = 0.003922;
	*xrsp = (0.412424 * red + 0.357579 * grn + 0.180464 * blu) * adapt;
	*ylum = (0.212656 * red + 0.715158 * grn + 0.072186 * blu) * adapt;
	*zblu = (0.019332 * red + 0.119193 * grn + 0.950444 * blu) * adapt;
}

void ColorConverter::XYZ2RGB(double xrsp, double ylum, double zblu, int *red, int *grn, int *blu) {
	*red = xrsp * 3.080342  - ylum * 1.537399 - zblu * 0.542943;
	*grn = xrsp * -0.921178 + ylum * 1.87593  + zblu * 0.045248;
	*blu = xrsp * 0.052881  - ylum * 0.204011 + zblu * 1.15113;
}

// XYZ<>LAB color conversion
void ColorConverter::XYZ2LAB(double xrsp, double ylum, double zblu,
                             double *lum, double *apt, double *bpt) {
	*lum = 116 * ColorConverter::XYZ2H(ylum) - 16;
	*apt = 500 * (ColorConverter::XYZ2H(xrsp / 0.950467) - ColorConverter::XYZ2H (ylum));
	*bpt = 200 * (ColorConverter::XYZ2H(ylum) - ColorConverter::XYZ2H (zblu / 1.088969));
}

void ColorConverter::LAB2XYZ(double lum, double apt, double bpt,
                             double *xrsp, double *ylum, double *zblu) {
	double XRSP, YLUM, ZBLU;
	YLUM = lum * (0.00862) + 0.137931;
	XRSP = apt * (0.002) + YLUM;
	ZBLU = bpt * (-0.005) + YLUM;

	*xrsp = XRSP > 0.206897 ? pow(XRSP, 3) : XRSP * (0.128419) - 0.017713;
	*ylum = lum > 8 ? pow(YLUM, 3) : lum * (0.00110705646);
	*zblu = ZBLU > 0.206897 ? pow(ZBLU, 3) : ZBLU * (0.128419) - 0.017713;
}

// RGB<>XYZ<>LAB color conversion
void ColorConverter::RGB2LAB(int red, int grn, int blu, double *lum, double *apt, double *bpt) {
	double xrsp, ylum, zblu;
	RGB2XYZ(red, grn, blu, &xrsp, &ylum, &zblu);
	XYZ2LAB(xrsp, ylum, zblu, lum, apt, bpt);
}

void ColorConverter::LAB2RGB(double lum, double apt, double bpt, int *red, int *grn, int *blu) {
	double xrsp, ylum, zblu;
	LAB2XYZ(lum, apt, bpt, &xrsp, &ylum, &zblu);
	XYZ2RGB(xrsp, ylum, zblu, red, grn, blu);
}

// HSL<>RGB<>HSV color conversion
void ColorConverter::HSL2HSV(double hue, double sat, double lum,
                             double *_hue, double *_sat, double *vel) {
	double red, grn, blu;
	HSL2RGB(hue, sat, lum, &red, &grn, &blu);
	RGB2HSV(red, grn, blu, _hue, _sat, vel);
}

void ColorConverter::HSV2HSL(double hue, double sat, double vel,
                             double *_hue, double *_sat, double *lum) {
	double red, grn, blu;
	HSV2RGB(hue, sat, vel, &red, &grn, &blu);
	RGB2HSL(red, grn, blu, _hue, _sat, lum);
}

// HSL<>RGB<>CMYK color conversion
void ColorConverter::HSL2CMYK(double hue, double sat, double lum,
                              double *cyan, double *mgnta, double *ylw, double *blk) {
	double red, grn, blu;
	HSL2RGB(hue, sat, lum, &red, &grn, &blu);
	RGB2CMYK(red, grn, blu, cyan, mgnta, ylw, blk);
}

void ColorConverter::CMYK2HSL(double cyan, double mgnta, double ylw, double blk,
                              double *hue, double *sat, double *lum) {
	double red, grn, blu;
	CMYK2RGB(cyan, mgnta, ylw, blk, &red, &grn, &blu);
	RGB2HSL(red, grn, blu, hue, sat, lum);
}

// HSV<>RGB<>CMYK color conversion
void ColorConverter::HSV2CMYK(double hue, double sat, double vel,
                              double *cyan, double *mgnta, double *ylw, double *blk) {
	double red, grn, blu;
	HSV2RGB(hue, sat, vel, &red, &grn, &blu);
	RGB2CMYK(red, grn, blu, cyan, mgnta, ylw, blk);
}

void ColorConverter::CMYK2HSV(double cyan, double mgnta, double ylw, double blk,
                              double *hue, double *sat, double *vel) {
	double red, grn, blu;
	CMYK2RGB(cyan, mgnta, ylw, blk, &red, &grn, &blu);
	RGB2HSV(red, grn, blu, hue, sat, vel);
}

// HSL<>RGB<>YIQ color conversion
void ColorConverter::HSL2YIQ(double hue, double sat, double lum,
                             double *ylum, double *iphs, double *quad) {
	double red, grn, blu;
	HSL2RGB(hue, sat, lum, &red, &grn, &blu);
	RGB2YIQ(red, grn, blu, ylum, iphs, quad);
}

void ColorConverter::YIQ2HSL(double ylum, double iphs, double quad,
                             double *hue, double *sat, double *lum) {
	double red, grn, blu;
	YIQ2RGB(ylum, iphs, quad, &red, &grn, &blu);
	RGB2HSL(red, grn, blu, hue, sat, lum);
}

// HSV<>RGB<>YIQ color conversion
void ColorConverter::HSV2YIQ(double hue, double sat, double vel,
                             double *ylum, double *iphs, double *quad) {
	double red, grn, blu;
	HSV2RGB(hue, sat, vel, &red, &grn, &blu);
	RGB2YIQ(red, grn, blu, ylum, iphs, quad);
}

void ColorConverter::YIQ2HSV(double ylum, double iphs, double quad,
                             double *hue, double *sat, double *vel) {
	double red, grn, blu;
	YIQ2RGB(ylum, iphs, quad, &red, &grn, &blu);
	RGB2HSV(red, grn, blu, hue, sat, vel);
}

// CMYK<>RGB<>YIQ color conversion
void ColorConverter::CMYK2YIQ(double cyan, double mgnta, double ylw, double blk,
                              double *ylum, double *iphs, double *quad) {
	double red, grn, blu;
	CMYK2RGB(cyan, mgnta, ylw, blk, &red, &grn, &blu);
	RGB2YIQ(red, grn, blu, ylum, iphs, quad);
}

void ColorConverter::YIQ2CMYK(double ylum, double iphs, double quad,
                              double *cyan, double *mgnta, double *ylw, double *blk) {
	double red, grn, blu;
	YIQ2RGB(ylum, iphs, quad, &red, &grn, &blu);
	RGB2CMYK(red, grn, blu, cyan, mgnta, ylw, blk);
}
