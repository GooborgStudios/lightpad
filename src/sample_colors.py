# -*- coding: utf-8 -*-

# Lightpad - sample_colors.py
# ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
# Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
# https://www.nightwave.co/lightpad

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
