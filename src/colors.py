import math

# RGB<>HSL color conversion
def RGB2HSL(red, green, blue):
	max_val = max(red, green, blue)
	min_val = min(red, green, blue)
	luminosity = (max_val + min_val) / 2

	if max_val == min_val:
		hue = 0
		saturation = 0
	else:
		diff = max_val - min_val
		if luminosity > 127: saturation = diff * 255 / (510 - max_val - min_val) # Weird line...
		else: saturation = diff * 255 / (max_val + min_val) # Weird line...
		if max_val == red:
			hue = (green - blue) * 60 / diff
			if green < blue: hue += 360
		elif max_val == green: hue = (blue - red) * 60 / diff + 120
		elif max_val == blue: hue = (red - green) * 60 / diff + 240
	return [hue, saturation, luminosity]
	
def Hue2RGB(cc_p, cc_q, cc_t):
	if (cc_t < 0.0): cc_t += 1
	if (cc_t > 1.0): cc_t -= 1
	if (cc_t < 1 / 6.0): return cc_p + (cc_q - cc_p) * 6.0 * cc_t
	if (cc_t < 1 / 2.0): return cc_q
	if (cc_t < 2 / 3.0): return cc_p + (cc_q - cc_p) * (2 / 3.0 - cc_t) * 6.0
	return cc_p

def HSL2RGB(hue, saturation, luminosity):
	hue = hue / 360.0
	if (saturation == 0.0): red = green = blue = luminosity
	else:
		if luminosity < 127: cc_q = luminosity * (1 + saturation) / 255
		else: cc_q = luminosity + (saturation / 255) - luminosity * (saturation / 255)
		cc_p = 2.0 * luminosity / 255 - cc_q
		red = round(max(0, Hue2RGB(cc_p, cc_q, hue + 1 / 3.0) * 255))
		green = round(max(0, Hue2RGB(cc_p, cc_q, hue) * 255))
		blue = round(max(0, Hue2RGB(cc_p, cc_q, hue - 1 / 3.0) * 255))
	return [int(red), int(green), int(blue)]

# RGB<>HSV color conversion
def RGB2HSV(red, green, blue):
	max_val = max(red, green, blue)
	min_val = min(red, green, blue)
	diff = max_val - min_val
	velocity = max_val
	if max_val == 0: saturation = 0
	else: saturation = diff / max_val * 255

	if max_val == min_val:
		hue = 0
	else:
		if max_val == red:
			hue = (green - blue) * 60 / diff
			if green < blue: hue += 360
		elif max_val == green: hue = (blue - red) * 60 / diff + 120
		elif max_val == blue: hue = (red - green) * 60 / diff + 240
	return [hue, saturation, velocity]

def HSV2RGB(hue, saturation, velocity):
	cc_i = hue / 6.0
	cc_ir = int((cc_i - math.floor(cc_i)))
	p = int(velocity * (255 - saturation) / 255)
	q = int(velocity * (255 - cc_ir * saturation) / 255)
	t = int(velocity * (255 - (1.0 - cc_ir) * saturation) / 255)
	red = green = blue = velocity
	
	rem = int(math.floor(cc_i)) % 6
	if rem == 0:
		green = t
		blue = p
	elif rem == 1:
		red = q
		blue = p
	elif rem == 2:
		red = p
		blue = t
	elif rem == 3:
		red = p
		green = q
	elif rem == 4:
		red = t
		green = p
	elif rem == 5:
		green = p
		blue = q
	return [red, green, blue]



# RGB<>CMYK color conversion
def RGB2CMYK(red, green, blue):
	black = 1.0 - _max(red, green, blue)
	cyan = (1.0 - red - black) / (1.0 - black)
	magenta = (1.0 - green - black) / (1.0 - black)
	yellow = (1.0 - blue - black) / (1.0 - black)
	return [cyan, magenta, yellow, black]

def CMYK2RGB(cyan, magenta, yellow, black):
	red = (1.0 - cyan) / (1.0 - black)
	green = (1.0 - magenta) / (1.0 - black)
	blue = (1.0 - yellow) / (1.0 - black)
	return [red, green, blue]

# RGB<>YIQ color conversion
def RGB2YIQ(red, green, blue):
	yluma = 0.299 * red + 0.587 * green + 0.114 * blue
	inphase = 0.569 * red - 0.275 * green - 0.322 * blue
	quadrature = 0.211 * red - 0.523 * green + 0.312 * blue
	return [yluma, inphase, quadrature]

def YIQ2RGB(yluma, inphase, quadrature):
	red = yluma + 0.956 * inphase + 0.621 * quadrature
	green = yluma - 0.272 * inphase - 0.647 * quadrature
	blue = yluma - 1.106 * inphase + 1.703 * quadrature
	return [red, green, blue]

def XYZ2H(cc_q):
	if cc_q > 0.008856: return cc_q**0.333333
	return 7.787 * cc_q + 0.137931

# RGB<>XYZ color conversion
def RGB2XYZ(red, green, blue):
	adapt = 0.003922
	xresponse = (0.412424 * red + 0.357579 * green + 0.180464 * blue) * adapt
	yluminance = (0.212656 * red + 0.715158 * green + 0.072186 * blue) * adapt
	zblue = (0.019332 * red + 0.119193 * green + 0.950444 * blue) * adapt
	return [xresponse, yluminance, zblue]

def XYZ2RGB(xresponse, yluminance, zblue):
	red = xresponse * 3.080342 - yluminance * 1.537399 - zblue * 0.542943
	green = xresponse * -0.921178 + yluminance * 1.87593 + zblue * 0.045248
	blue = xresponse * 0.052881 - yluminance * 0.204011 + zblue * 1.15113
	return [red, green, blue]

# XYZ<>LAB color conversion
def XYZ2LAB(xresponse, yluminance, zblue):
	luminosity = 116 * XYZ2H(yluminance) - 16
	apoint = 500 * (XYZ2H(xresponse / 0.950467) - XYZ2H(yluminance))
	bpoint = 200 * (XYZ2H(yluminance) - XYZ2H(zblue / 1.088969))
	return [luminosity, apoint, bpoint]

def LAB2XYZ(luminosity, apoint, bpoint):
	YLUMINANCE = luminosity * (0.00862) + 0.137931
	XRESPONSE = apoint * (0.002) + YLUMINANCE
	ZBLUE = bpoint * (-0.005) + YLUMINANCE

	if XRESPONSE > 0.206897: xresponse = pow(XRESPONSE, 3)
	else: xresponse = XRESPONSE * (0.128419) - 0.017713
	if luminosity > 8: yluminance = pow(YLUMINANCE, 3)
	else: yluminance = luminosity * (0.00110705646)
	if ZBLUE > 0.206897: zblue = pow(ZBLUE, 3)
	else: zblue = ZBLUE * (0.128419) - 0.017713
	return [xresponse, yluminance, zblue]



# # RGB<>XYZ<>LAB color conversion
# void ColorConverter::RGB2LAB(int red, int green, int blue, double *luminosity, double *apoint, double *bpoint) {
# 	double xresponse, yluma, zblue;
# 	RGB2XYZ(red, green, blue, &xresponse, &yluma, &zblue);
# 	XYZ2LAB(xresponse, yluma, zblue, luminosity, apoint, bpoint);
# }

# void ColorConverter::LAB2RGB(double luminosity, double apoint, double bpoint, int *red, int *green, int *blue) {
# 	double xresponse, yluma, zblue;
# 	LAB2XYZ(luminosity, apoint, bpoint, &xresponse, &yluma, &zblue);
# 	XYZ2RGB(xresponse, yluma, zblue, red, green, blue);
# }

# # HSL<>RGB<>HSV color conversion
# void ColorConverter::HSL2HSV(double hue, double saturation, double luminosity, double *_hue, double *_saturation, double *velocity) {
# 	double red, green, blue;
# 	HSL2RGB(hue, saturation, luminosity, &red, &green, &blue);
# 	RGB2HSV(red, green, blue, _hue, _saturation, velocity);
# }

# void ColorConverter::HSV2HSL(double hue, double saturation, double velocity, double *_hue, double *_saturation, double *luminosity) {
# 	double red, green, blue;
# 	HSV2RGB(hue, saturation, velocity, &red, &green, &blue);
# 	RGB2HSL(red, green, blue, _hue, _saturation, luminosity);
# }

# # HSL<>RGB<>CMYK color conversion
# void ColorConverter::HSL2CMYK(double hue, double saturation, double luminosity, double *cyan, double *magenta, double *yellow, double *black) {
# 	double red, green, blue;
# 	HSL2RGB(hue, saturation, luminosity, &red, &green, &blue);
# 	RGB2CMYK(red, green, blue, cyan, magenta, yellow, black);
# }

# void ColorConverter::CMYK2HSL(double cyan, double magenta, double yellow, double black, double *hue, double *saturation, double *luminosity) {
# 	double red, green, blue;
# 	CMYK2RGB(cyan, magenta, yellow, black, &red, &green, &blue);
# 	RGB2HSL(red, green, blue, hue, saturation, luminosity);
# }

# # HSV<>RGB<>CMYK color conversion
# void ColorConverter::HSV2CMYK(double hue, double saturation, double velocity, double *cyan, double *magenta, double *yellow, double *black) {
# 	double red, green, blue;
# 	HSV2RGB(hue, saturation, velocity, &red, &green, &blue);
# 	RGB2CMYK(red, green, blue, cyan, magenta, yellow, black);
# }

# void ColorConverter::CMYK2HSV(double cyan, double magenta, double yellow, double black, double *hue, double *saturation, double *velocity) {
# 	double red, green, blue;
# 	CMYK2RGB(cyan, magenta, yellow, black, &red, &green, &blue);
# 	RGB2HSV(red, green, blue, hue, saturation, velocity);
# }

# # HSL<>RGB<>YIQ color conversion
# void ColorConverter::HSL2YIQ(double hue, double saturation, double luminosity, double *yluma, double *inphase, double *quadrature) {
# 	double red, green, blue;
# 	HSL2RGB(hue, saturation, luminosity, &red, &green, &blue);
# 	RGB2YIQ(red, green, blue, yluma, inphase, quadrature);
# }

# void ColorConverter::YIQ2HSL(double yluma, double inphase, double quadrature, double *hue, double *saturation, double *luminosity) {
# 	double red, green, blue;
# 	YIQ2RGB(yluma, inphase, quadrature, &red, &green, &blue);
# 	RGB2HSL(red, green, blue, hue, saturation, luminosity);
# }

# # HSV<>RGB<>YIQ color conversion
# void ColorConverter::HSV2YIQ(double hue, double saturation, double velocity, double *yluma, double *inphase, double *quadrature) {
# 	double red, green, blue;
# 	HSV2RGB(hue, saturation, velocity, &red, &green, &blue);
# 	RGB2YIQ(red, green, blue, yluma, inphase, quadrature);
# }

# void ColorConverter::YIQ2HSV(double yluma, double inphase, double quadrature, double *hue, double *saturation, double *velocity) {
# 	double red, green, blue;
# 	YIQ2RGB(yluma, inphase, quadrature, &red, &green, &blue);
# 	RGB2HSV(red, green, blue, hue, saturation, velocity);
# }

# # CMYK<>RGB<>YIQ color conversion
# void ColorConverter::CMYK2YIQ(double cyan, double magenta, double yellow, double black, double *yluma, double *inphase, double *quadrature) {
# 	double red, green, blue;
# 	CMYK2RGB(cyan, magenta, yellow, black, &red, &green, &blue);
# 	RGB2YIQ(red, green, blue, yluma, inphase, quadrature);
# }

# void ColorConverter::YIQ2CMYK(double yluma, double inphase, double quadrature, double *cyan, double *magenta, double *yellow, double *black) {
# 	double red, green, blue;
# 	YIQ2RGB(yluma, inphase, quadrature, &red, &green, &blue);
# 	RGB2CMYK(red, green, blue, cyan, magenta, yellow, black);
# }



def test_converter(converter1, converter2, tests):
	print "Testing {0} -> {1} -> {0}...".format(converter1.__name__, converter2.__name__)
	for t in tests:
		comment = "(%s)" %t[3] if len(t) >= 4 else ""
		result1 = converter1(*t[0:3])
		result2 = converter2(*result1)
		print "Pass:" if t[0] == result2[0] and t[1] == result2[1] and t[2] == result2[2] else "FAIL:", t[0:3], "-> %s() ->" %converter1.__name__, result1, "-> %s() ->" %converter2.__name__, result2, comment
	print ""

test_converter(RGB2HSL, HSL2RGB, [[255, 255, 255], [0, 0, 0], [255, 0, 0], [255, 255, 0], [0, 255, 0], [0, 255, 255], [0, 0, 255], [255, 0, 255]])
test_converter(HSL2RGB, RGB2HSL, [[0, 255, 255, "Expected to fail"], [0, 0, 0], [60, 255, 127], [300, 255, 127], [40, 255, 127]])
test_converter(RGB2HSV, HSV2RGB, [[255, 255, 255], [0, 0, 0], [255, 0, 0], [255, 255, 0], [0, 255, 0], [0, 255, 255], [0, 0, 255], [255, 0, 255]])
test_converter(HSV2RGB, RGB2HSV, [[0, 255, 255], [0, 0, 0], [60, 255, 255], [300, 255, 255], [40, 0, 255, "Expected to fail"]])

# Reasons behind failure expectations: there's not enough data retained in RGB to know the original hue if saturation is 0 and either velocity is 0 or luminosity is 0 or 255.  These are actually passing checks.
