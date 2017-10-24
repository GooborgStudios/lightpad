//
// Lightpad - LightpadProject.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "LightpadProject.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <string>
#include <cassert>

#include "MidiFile.h"

#include "MidiLayer.h"

// Generated with GenerateNoteButtonMap.cpp
const int note_button_map[] = {
	91, 92, 93, 94, 95, 96, 97, 98, 11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34, 41, 42, 43, 44,
	51, 52, 53, 54, 61, 62, 63, 64, 71, 72, 73, 74, 81, 82, 83, 84, 15, 16, 17, 18, 25, 26, 27, 28,
	35, 36, 37, 38, 45, 46, 47, 48, 55, 56, 57, 58, 65, 66, 67, 68, 75, 76, 77, 78, 85, 86, 87, 88,
	89, 79, 69, 59, 49, 39, 29, 19, 80, 70, 60, 50, 40, 30, 20, 10, 1, 2, 3, 4, 5, 6, 7, 8
};
const int note_button_offset = 28;
const int note_button_size = sizeof(note_button_map) / sizeof(int);
// End generated block

LightpadProject::LightpadProject(int BPM, int ticksPerBeat, int beatsPerMeasure) {
	this->BPM = BPM;
	this->ticksPerBeat = ticksPerBeat;
	this->beatsPerMeasure = beatsPerMeasure;
	
	midifile = new MidiFile();
	layer = new MidiLayer();
	
	int btn_x = 0;
	int btn_y = 0;
	
	for (int row = 0; row < 96; row++) {
		btn_x++;
		if (row == 8 || row == 88) btn_x++;
		if (btn_x / 10 > 0) {
			btn_x = btn_x % 10;
			btn_y++;
		}
		
		layer->AddKeyframe(new NoteKeyframe(btn_x + (btn_y * 10), 0, 0));
	}
	
	layers.push_back(layer);
	
	seek(0);
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
		layer->AddKeyframe(new NoteKeyframe(noteToButton((int)mev[1]), mev.tick, color));
	}
	
	seek(0);
	
	delete midifile;
}

int LightpadProject::noteToButton(int note) {
	if (note < note_button_offset || note > (note_button_size + note_button_offset)) return 0;
	return note_button_map[note - note_button_offset];
}

int LightpadProject::buttonToNote(int button) {
	for (int i = 0; i < note_button_size + note_button_offset; i++)
		if (note_button_map[i] == button) return i + note_button_offset;
	return 0;
}



void LightpadProject::pushButton(int tick, std::string button, unsigned char velocity) {
	vector<unsigned char> data;
	
	data.push_back(velocity == 0 ? 128 : 144);
	data.push_back(buttonToNote(stoi(button)));
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
		for (auto keyframe: button->keyframes) {
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
