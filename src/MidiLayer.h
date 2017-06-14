//
// Lightpad - MidiLayer.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <string>

#include "Layer.h"

template <typename T> std::string to_string_with_precision(const T a_value, const int n = 6);
std::string lp_note_to_string(int note);

class NoteKeyframe: public Keyframe {
	public:
		unsigned char velocity;
		NoteKeyframe(int name, double time, unsigned char velocity);
		std::string serialize();
};

class MidiLayer: public Layer {
	public:
		unsigned char getVelocity(int position);
		void setVelocity(int position, unsigned char velocity);
};
