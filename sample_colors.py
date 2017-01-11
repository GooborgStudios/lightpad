# -*- coding: utf-8 -*-
from PIL import Image

output = "\n\nwxColor velocitycolors[] = {\n\t"

def do_it(img):
	o = []
	for y in range(36, img.height, 74):
		for x in range(36, img.width, 74):
			o.append("wxColor(%d, %d, %d)" %img.getpixel((x, y)))
	return o

output += ",\n\t".join(
	do_it(Image.open('graphics/references/rgbcolors1.png').convert('RGB')) +
	do_it(Image.open('graphics/references/rgbcolors2.png').convert('RGB'))
) + "\n};\n"

cpp_file = open("Colors.cpp", "w")
cpp_tmpl = open("Colors.cpp.template", "r")

for line in cpp_tmpl:
	if line.strip() == "!XXX!":
		cpp_file.write(output)
	else:
		cpp_file.write(line)

cpp_file.close()
cpp_tmpl.close()