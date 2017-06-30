//
// Lightpad - MidiLayer.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "MidiLayer.h"

#include <string>

#include "Helpers.h"
#include "Layer.h"

NoteKeyframe::NoteKeyframe(int name, long time, unsigned char velocity) : Keyframe(to_padded_string(name, 2), time) {
	this->velocity = velocity;
}

std::string NoteKeyframe::serialize() {
	return std::string("");
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
