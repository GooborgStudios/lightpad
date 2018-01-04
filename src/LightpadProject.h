//
// Lightpad - LightpadProject.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <string>

#include "MidiFile.h"

#include "HOWL/Project.h"
#include "MidiLayer.h"

class LightpadProject: public HOWL::Project {
	public:
		LightpadProject(int BPM = 120, int ticksPerBeat = 32, int beatsPerMeasure = 4);
		LightpadProject(std::string filePath);
		int save();
		int save(std::string filePath);
	
		MidiLayer *layer;
	
	private:
		void pushButton(int tick, std::string button, unsigned char velocity);
		int noteToButton(int note);
		int buttonToNote(int button);
		
		MidiFile *midifile;
};
