//
// Lightpad - LightpadProject.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "LightpadProject.h"

#include <string>
#include <cassert>

#include "MidiFile.h"

#include "MidiLayer.h"
#include "NoteHelpers.h"


LightpadProject::LightpadProject(int BPM, int ticksPerBeat, int beatsPerMeasure) {
	this->BPM = BPM;
	this->ticksPerBeat = ticksPerBeat;
	this->beatsPerMeasure = beatsPerMeasure;
	
	midifile = new MidiFile();
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
	
	layers.push_back(layer);
}

LightpadProject::LightpadProject(std::string filePath) : LightpadProject::LightpadProject() {
	this->filePath = filePath;
	midifile = new MidiFile(filePath);
	MidiEvent mev;
	int color;
	
	midifile->sortTracks();
	midifile->joinTracks();
	this->ticksPerBeat = midifile->getTicksPerQuarterNote();
	// XXX Get time signature here
	
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

void LightpadProject::pushButton(int tick, std::string button, unsigned char velocity) {
	vector<unsigned char> data;
	
	data.push_back(velocity == 0 ? 128 : 144);
	data.push_back(button_to_note(stoi(button)));
	data.push_back(velocity);
	assert(midifile != NULL);
	midifile->addEvent(0, tick, data);
}

int LightpadProject::save() {
	return save(this->filePath);
}

int LightpadProject::save(std::string filePath) {
	if (filePath == "") return -1;
	this->filePath = filePath;
	
	midifile->setTicksPerQuarterNote(this->ticksPerBeat);
	
	unsigned char velocity = 0;
	for (auto button: layer->keyframes) {
		unsigned char last_command = 128;
		for (auto keyframe: button.second->keyframes) {
			velocity = ((NoteKeyframe *)(keyframe))->velocity;
			
			if (velocity > 0 && last_command != 128) pushButton(keyframe->time, keyframe->name, 0);
			pushButton(keyframe->time, keyframe->name, velocity);
			
			last_command = velocity == 0 ? 128 : 144;
		}
	}
	
	midifile->sortTracks();
	midifile->write(this->filePath);
	
	return 0;
}
