//
// Lightpad - MidiLayer.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "MidiLayer.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <string>

#include "Layer.h"

#include <sstream>
#include <iomanip>

template <typename T> std::string to_string_with_precision(const T value, const int width) {
	std::ostringstream out;
	out << std::setfill('0') << std::setw(width) << value;
	return out.str();
}

std::string lp_note_to_string(int note) {
	note++;
	if (note >= 9) note++;
	if (note >= 89) note++;
	return to_string_with_precision(note, 2);
}

NoteKeyframe::NoteKeyframe(int name, double time, unsigned char velocity) : Keyframe(lp_note_to_string(name), time) {
	this->velocity = velocity;
}

std::string NoteKeyframe::serialize() {
	return std::string("");
}

unsigned char MidiLayer::getVelocity(int position) {
	std::string type = std::to_string(position);
	if (keyframes[type]->getFirst() == NULL) return '\0';
	return ((NoteKeyframe *)(keyframes[type]->getFirst()))->velocity;
}

void MidiLayer::setVelocity(int position, unsigned char velocity) {
	std::string type = std::to_string(position);
	NoteKeyframe *keyframe = (NoteKeyframe *)(keyframes[type]->getFirst());
	if (keyframe == NULL) AddKeyframe(new NoteKeyframe(position, currentTime, velocity));
	else keyframe->velocity = velocity;
}
