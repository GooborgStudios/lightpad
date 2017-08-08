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

#include "Project.h"
#include "MidiLayer.h"

class LightpadProject: public HOWL::Project {
	private:
		MidiFile *midifile;
		void pushButton(int tick, std::string button, unsigned char velocity);
		
	public:
		LightpadProject(int BPM = 120, int ticksPerBeat = 32, int beatsPerMeasure = 4);
		LightpadProject(std::string filePath);
		int save();
		int save(std::string filePath);
	
		MidiLayer *layer;
};

extern LightpadProject *activeProject;
