//
// Lightpad - Project.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#include <vector>
#include <string>

#include "Layer.h"

class Project {
	protected:
		std::string filePath = "";
	
	public:
		int BPM = 120;
		int ticksPerBeat = 32;
		int beatsPerMeasure = 4;
		long currentTime = 0;
	
		std::vector<Layer *> layers;
	
		virtual int save()=0;
		virtual int save(std::string filePath)=0;
		void seek(long newTime);
		void advanceFrame(long increment);
};
