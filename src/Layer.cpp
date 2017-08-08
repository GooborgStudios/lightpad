//
// HOWL - Music-synced animation library
// File: Layer.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// https://www.nightwave.co
//

#include "Layer.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <sys/time.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>
#include <cassert>

#include "Helpers.h"
#include "Project.h"

using namespace HOWL;

Keyframe::Keyframe() {

}

Keyframe::Keyframe(std::string name, long time) {
	this->name = name;
	this->time = time;
	this->smoother = SMOOTH_LINEAR;
}

std::string Keyframe::serialize() {
	return "";
}

void Keyframe::toBuffer(char *outbuf, int len) {
//	snprintf(outbuf, len, "keyframe %s %f %s %s %d\n", this->parent->description.c_str(), this->time, this->name.c_str(), this->serialize().c_str(), this->smoother);
	snprintf(outbuf, len, "");
}

void Keyframe::render(wxDC &canvas, wxRect bounding_box) {
	
}

DoubleKeyframe::DoubleKeyframe(std::string name, long time, double value) : Keyframe(name, time) {
	this->value = value;
};

DoubleKeyframe::DoubleKeyframe(std::string name, long time, float value) : Keyframe(name, time) {
	this->value = (double)value;
};

DoubleKeyframe::DoubleKeyframe(std::string name, long time, int value) : Keyframe(name, time) {
	this->value = (double)value;
};

std::string DoubleKeyframe::serialize() {
	std::stringstream stream;
	stream << value;
	return stream.str();
}

StringKeyframe::StringKeyframe(std::string name, long time, std::string *value) : Keyframe(name, time) {
	this->value = value;
	this->smoother = SMOOTH_HOLD;
};

StringKeyframe::StringKeyframe(std::string name, long time, const char *value) : Keyframe(name, time) {
	this->value = new std::string(value);
	this->smoother = SMOOTH_HOLD;
};

std::string StringKeyframe::serialize() {
	return *value;
}

//KeyframeSet::KeyframeSet() {
//	this->parent = NULL;
//}

KeyframeSet::KeyframeSet(Layer *parent) {
	this->parent = parent;
//	seek();
}

void KeyframeSet::AddKeyframe(Keyframe *keyframe) {
	KeyframeIterator iter = keyframes.begin();
	
	for (; iter != keyframes.end(); ++iter) {
		if ( keyframe->time < (*iter)->time ) {
			keyframes.insert(iter, keyframe);
			break;
		}
	}
	
	if (iter == keyframes.end()) {
		keyframes.push_back(keyframe);
	}
}

void KeyframeSet::seek(long newTime) {
	currentTime = 0;
	prevKF = keyframes.begin();
	if (prevKF != keyframes.end()) {
		nextKF = std::next(prevKF, 1);
	} else {
		nextKF = keyframes.end();
	}
	advanceFrame(newTime);
}

void KeyframeSet::advanceFrame(long increment) {
	currentTime += increment;
	while (nextKF != keyframes.end() && currentTime >= (*nextKF)->time) {
		++prevKF;
		++nextKF;
	}
}

double KeyframeSet::smoother_fraction() {
	// XXX Only does Linear smoothing, add other smoother types
	if (nextKF == keyframes.end()) return 0.0;
	
	assert(currentTime >= (*prevKF)->time);
	assert((*nextKF)->time >= currentTime);
	
	double dur = (*nextKF)->time - (*prevKF)->time;
	if (dur == 0) return 0.0; // Avoid divide-by-zero crash
	return (currentTime - (*prevKF)->time) / dur;
}

Keyframe *KeyframeSet::getFirst() {
	if (prevKF == keyframes.end()) return NULL;
	return *prevKF;
}

Keyframe *KeyframeSet::getSecond() {
	if (nextKF == keyframes.end()) return NULL;
	return *nextKF;
}

Layer::Layer() {
	description = "";
};

Layer::Layer(std::string description) {
	this->description = description;
}

void Layer::AddKeyframe(Keyframe *keyframe) {
	std::string type = keyframe->name;
	if (keyframes.find(type) == keyframes.end()) keyframes[type] = new KeyframeSet(this);
	keyframes[type]->AddKeyframe(keyframe);
}

void Layer::seek(long newTime) {
	for (auto iter : keyframes) iter.second->seek(newTime);
}

void Layer::advanceFrame(long increment) {
	for (auto iter : keyframes) iter.second->advanceFrame(increment);
}

double Layer::getDouble(std::string type) {
	KeyframeSet *set = keyframes[type];
	DoubleKeyframe *KF1 = (DoubleKeyframe *)(set->getFirst());
	DoubleKeyframe *KF2 = (DoubleKeyframe *)(set->getSecond());
	
	if (KF1 == NULL) return 0.0;
	if (KF2 == NULL) return KF1->value;
	double pos = set->smoother_fraction();
	return KF1->value - (KF1->value * pos) + (KF2->value * pos); // XXX Simplify me (to get a good grade)!
}

std::string *Layer::getString(std::string type) {
	if (keyframes[type]->getFirst() == NULL) return new std::string("");
	return ((StringKeyframe *)(keyframes[type]->getFirst()))->value;
}

bool is_sooner(Keyframe *a, Keyframe *b) {
	return a->time < b->time;
}
