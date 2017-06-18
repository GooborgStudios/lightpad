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

class NoteKeyframe: public Keyframe {
	public:
		unsigned char velocity;
		NoteKeyframe(int name, long time, unsigned char velocity);
		std::string serialize();
};

class MidiLayer: public Layer {
	public:
		unsigned char getVelocity(int position);
		void setVelocity(int position, unsigned char velocity);
};
