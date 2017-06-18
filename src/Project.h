//
// Lightpad - Project.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#include <vector>

#include "MidiLayer.h"

class Project {
	public:
		Project();
		Project(int BPM);
		Project(int BPM, int ticksPerBeat);
	
		int BPM;
		int ticksPerBeat;
		MidiLayer *layer;
		long currentTime;
//		int save(std::string fileLocation);
	
		void seek(long newTime);
		void advanceFrame(long increment);
};

extern Project *activeProject;
