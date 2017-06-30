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
}

Project::Project(std::string filePath) : Project::Project() {
	midifile = new MidiFile(filePath);
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

void Project::pushButton(int tick, std::string button, unsigned char velocity) {
	vector<unsigned char> data;
	
	data.push_back(velocity == 0 ? 128 : 144);
	data.push_back(button_to_note(stoi(button)));
	data.push_back(velocity);
	midifile->addEvent(0, tick, data);
}

int Project::save(std::string filePath) {
//	MidiFile *midifile = new MidiFile();
	unsigned char velocity = 0;
	
	midifile->setTicksPerQuarterNote(this->ticksPerBeat);
	
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
	midifile->write(filePath);
	
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
