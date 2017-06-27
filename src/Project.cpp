//
// Lightpad - Project.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "Project.h"

#include <vector>
#include <string>

#include "MidiFile.h"

#include "MidiLayer.h"
#include "NoteHelpers.h"

Project::Project() : Project::Project(120) {

}

Project::Project(int BPM) : Project::Project(BPM, 32) {
	
}

Project::Project(int BPM, int ticksPerBeat) {
	currentTime = 0;
	this->BPM = BPM;
	this->ticksPerBeat = ticksPerBeat;
	
	layer = new MidiLayer();
	
	int btn_x = 0;
	int btn_y = 9;
	
	for (int row = 0; row < 96; row++) {
		btn_x++;
		if (row == 8 || row == 88) btn_x++;
		if (btn_x / 10 > 0) {
			btn_x = btn_x % 10;
			btn_y--;
		}
		
		layer->AddKeyframe(new NoteKeyframe(btn_x + (btn_y * 10), 0, 0));
	}
}

Project::Project(std::string filePath) : Project::Project() {
	MidiFile *midifile = new MidiFile(filePath);
	MidiEvent mev;
	int color;
	
	midifile->joinTracks();
	this->ticksPerBeat = midifile->getTicksPerQuarterNote();
	
	for (int event = 0; event < (*midifile)[0].size(); event++) {
		mev = (*midifile)[0][event];
		switch ((int)mev[0]) {
			case 128:
				color = 0;
				break;
			case 144:
				color = (int)mev[2];
				break;
			default:
				continue;
		}
		
		layer->AddKeyframe(new NoteKeyframe(note_to_button((int)mev[1]), mev.tick, color));
	}
	
	delete midifile;
}

int Project::save(std::string filePath) {
	return 0;
}

void Project::seek(long newTime) {
	currentTime = newTime;
	layer->seek(newTime);
}

void Project::advanceFrame(long increment) {
	currentTime += increment;
	layer->advanceFrame(increment);
}
