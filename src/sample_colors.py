# -*- coding: utf-8 -*-

# Lightpad - sample_colors.py
# ©2018 Gooborg Studios: Vinyl Darkscratch, Light Apacha.
# Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
# http://www.gooborg.com/lightpad

from PIL import Image

def do_it(img):
	o = []
	for y in range(2, img.height, 4):
		for x in range(2, img.width, 4):
			o.append("Color(%d, %d, %d)" %img.getpixel((x, y)))
	return o

print("Color velocitycolors[] = {\n\t" + ",\n\t".join(
	do_it(Image.open('graphics/rgbcolors.png').convert('RGB'))
) + "\n};\n")
