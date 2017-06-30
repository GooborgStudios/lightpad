//
// Lightpad - NoteHelpers.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

int note_to_button(int note);
int button_to_note(int button);

class Note {
	public:
		Note();
		Note(int pos, int col, int start);
		Note(int pos, int col, int start, int dur);
		int position; // Note position
		int color; // Velocity
		int time; // Note start time
		int duration; // Note duration
};
