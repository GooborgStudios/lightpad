//
// Lightpad - Project.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "Project.h"

#include <vector>

#include "MidiLayer.h"

Project::Project() : Project::Project(120) {

}

Project::Project(int BPM) : Project::Project(BPM, 32) {
	
}

Project::Project(int BPM, int ticksPerBeat) {
	currentTime = 0;
	this->BPM = BPM;
	this->ticksPerBeat = ticksPerBeat;
}

//int Project::save(std::string fileLocation) {
//	return 0;
//}

void Project::seek(long newTime) {
	currentTime = newTime;
	layer->seek(newTime);
}

void Project::advanceFrame(long increment) {
	currentTime += increment;
	layer->advanceFrame(increment);
}
