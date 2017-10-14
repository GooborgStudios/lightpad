//
// Lightpad - MidiLayer.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "MidiLayer.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <string>

#include "NightwaveCore/Helpers.h"
#include "NightwaveCore/Colors.h"
#include "Launchpad.h"
#include "HOWL/Layer.h"

NoteKeyframe::NoteKeyframe(int name, long time, unsigned char velocity) : Keyframe(to_padded_string(name, 2), time) {
	this->velocity = velocity;
}

std::string NoteKeyframe::serialize() {
	return std::string("");
}

void NoteKeyframe::render(wxDC &canvas, wxRect bounding_box) {
	canvas.SetPen(*wxBLACK_PEN);
	canvas.DrawLine(bounding_box.GetLeft(), bounding_box.GetTop(), bounding_box.GetLeft(), bounding_box.GetHeight());
	canvas.SetPen(*wxTRANSPARENT_PEN);
	canvas.SetBrush(wxBrush(velocitycolors[this->velocity]));
	canvas.DrawRectangle(bounding_box.GetLeft(), bounding_box.GetTop(), bounding_box.GetWidth(), bounding_box.GetHeight());
}

unsigned char MidiLayer::getVelocity(int position) {
	return getVelocity(to_padded_string(position, 2));
}

unsigned char MidiLayer::getVelocity(std::string position) {
	if (keyframes[position]->getFirst() == NULL) return '\0';
	return ((NoteKeyframe *)(keyframes[position]->getFirst()))->velocity;
}

void MidiLayer::setVelocity(int position, unsigned char velocity) {
	std::string type = to_padded_string(position, 2);
	NoteKeyframe *keyframe = (NoteKeyframe *)(keyframes[type]->getFirst());
	if (keyframe == NULL) AddKeyframe(new NoteKeyframe(position, keyframes[type]->currentTime, velocity));
	else keyframe->velocity = velocity;
}

Color velocitycolors[] = {
	Color(0, 0, 0),       Color(28, 28, 28),    Color(124, 124, 124), Color(252, 252, 252),
	Color(255, 128, 128), Color(255, 0, 0),     Color(178, 0, 0),     Color(89, 0, 0),
	Color(255, 170, 128), Color(255, 85, 0),    Color(178, 59, 0),    Color(89, 29, 0),
	Color(255, 255, 128), Color(255, 255, 0),   Color(178, 178, 0),   Color(89, 89, 0),
	Color(160, 255, 128), Color(64, 255, 0),    Color(45, 178, 0),    Color(22, 89, 0),
	Color(149, 255, 128), Color(43, 255, 0),    Color(30, 178, 0),    Color(15, 89, 0),
	Color(128, 255, 128), Color(0, 255, 0),     Color(0, 178, 0),     Color(0, 89, 0),
	Color(128, 255, 160), Color(0, 255, 64),    Color(0, 178, 45),    Color(0, 89, 22),
	Color(128, 255, 202), Color(0, 255, 149),   Color(0, 178, 104),   Color(0, 89, 52),
	Color(128, 213, 255), Color(0, 170, 255),   Color(0, 119, 178),   Color(0, 59, 89),
	Color(128, 171, 255), Color(0, 86, 255),    Color(0, 60, 178),    Color(0, 30, 89),
	Color(128, 128, 255), Color(0, 0, 255),     Color(0, 0, 178),     Color(0, 0, 89),
	Color(170, 128, 255), Color(85, 0, 255),    Color(59, 0, 178),    Color(29, 0, 89),
	Color(255, 128, 255), Color(255, 0, 255),   Color(178, 0, 178),   Color(89, 0, 89),
	Color(255, 128, 170), Color(255, 0, 85),    Color(178, 0, 59),    Color(89, 0, 29),
	Color(255, 25, 1),    Color(154, 53, 0),    Color(122, 81, 1),    Color(58, 102, 0),
	Color(1, 57, 0),      Color(0, 84, 50),     Color(0, 83, 127),    Color(0, 0, 254),
	Color(0, 68, 77),     Color(26, 0, 209),    Color(124, 124, 124), Color(32, 32, 32),
	Color(251, 12, 2),    Color(186, 253, 0),   Color(172, 236, 0),   Color(86, 253, 0),
	Color(0, 136, 0),     Color(1, 252, 123),   Color(0, 167, 255),   Color(2, 26, 255),
	Color(53, 0, 255),    Color(120, 0, 255),   Color(180, 23, 126),  Color(65, 32, 0),
	Color(255, 74, 1),    Color(130, 225, 0),   Color(102, 253, 0),   Color(0, 254, 0),
	Color(0, 254, 0),     Color(69, 253, 97),   Color(1, 251, 203),   Color(80, 134, 255),
	Color(39, 77, 200),   Color(132, 122, 237), Color(211, 12, 255),  Color(255, 6, 90),
	Color(255, 125, 1),   Color(184, 177, 0),   Color(138, 253, 0),   Color(129, 93, 0),
	Color(58, 40, 2),     Color(13, 76, 5),     Color(0, 80, 55),     Color(19, 20, 41),
	Color(16, 31, 90),    Color(106, 60, 24),   Color(172, 4, 1),     Color(225, 81, 54),
	Color(220, 105, 0),   Color(254, 225, 0),   Color(153, 225, 1),   Color(96, 181, 0),
	Color(27, 28, 49),    Color(220, 253, 84),  Color(118, 251, 185), Color(150, 152, 255),
	Color(139, 98, 255),  Color(64, 64, 64),    Color(116, 116, 116), Color(222, 252, 252),
	Color(159, 6, 1),     Color(52, 1, 0),      Color(0, 210, 1),     Color(0, 65, 1),
	Color(184, 177, 0),   Color(60, 48, 0),     Color(180, 93, 0),    Color(74, 21, 4)
};

unsigned char get_closest_velocity(Color color) {
	/* Take a wxColor and find the closest match to a Launchpad Pro available color */
	int best_match_index = 0;
	int best_match_value = 1024;
	for (int i = 0; i < COLORCOUNT; i++) {
		int comparison = ColorConverter::LAB_compare_RGB(velocitycolors[i], color);
		if (comparison < best_match_value) {
			best_match_index = i;
			best_match_value = comparison;
		}
	}
	
	return best_match_index;
};

char get_color_velocity(Color color) {
	/* Take a wxColor and find the matching velocity of a Launchpad Pro available color */
	for (int i = 0; i < COLORCOUNT; i++) {
		if (color == velocitycolors[i]) return i;
	}
	
	return -1;
};
