//
// Lightpad - GenerateNoteButtonMap.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include <iostream>

int note_to_button(int note) {
	if (note < 28) { // Invalid notes
		return 0;
	} else if (note < 36) { // Top circle row
		return 63 + note;
	} else if (note <= 99) { // Main (square) buttons
		return (note - 36) % 4 + 1 + 10 * ((note - 36) % 32 / 4 + 1) + ((note - 36) / 32 * 4);
	} else if (note <= 115) { // Left and right circle rows
		return 89 - 10 * ((note - 100) % 8) - (note / 108 * 9);
	} else if (note <= 123) { // Bottom circle row
		return note - 115;
	}
	return 0; // More invalid notes
}

int main() {
	std::cout << "int note_button_map[] = {";
	for (int x = 28; x <= 123; x++) {
		std::cout << note_to_button(x);
		if (x < 123) std::cout << ", ";
	}
	std::cout << "};" << std::endl << "const int note_button_offset = 28;" << std::endl;
	std::cout << "const int note_button_size = sizeof(note_button_map)/sizeof(int);" << std::endl;
}
