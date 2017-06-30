//
// Lightpad - Project.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#include <vector>
#include <string>

#include "MidiFile.h"

#include "MidiLayer.h"

class Project {
	private:
		MidiFile *midifile;
	
	public:
		Project();
		Project(int BPM);
		Project(int BPM, int ticksPerBeat);
		Project(std::string filePath);
	
		int BPM;
		int ticksPerBeat;
		MidiLayer *layer;
		long currentTime;
	
		void pushButton(int tick, std::string button, unsigned char velocity);
		int save(std::string filePath);
		void seek(long newTime);
		void advanceFrame(long increment);
};

extern Project *activeProject;
