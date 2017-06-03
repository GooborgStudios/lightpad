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
//		int BPM;
		MidiLayer *layer;
//		int save(std::string fileLocation);
};

extern Project *activeProject;
