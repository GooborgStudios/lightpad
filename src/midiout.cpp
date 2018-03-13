//
// Lightpad - midiout.cpp
// ©2018 Gooborg Studios: Vinyl Darkscratch, Light Apacha.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.gooborg.com/lightpad
//

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdarg>
#include <csignal>
#include <ctime>
#include <unistd.h>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "RtMidi.h"
#include "MidiFile.h"

#include "GooCore/GooCore.h"
#include "NoteHelpers.h"
#include "Launchpad.h"

int BPM = 120;

void finish(int code) {
	int err_code = 0;
	if (code == -1) err_code = 1;
	launchpad->disconnect();
	std::cout << std::endl;
	exit(err_code);
}

void launchpad_not_found_quit() {
	std::cout << "Launchpad not found!  Is it plugged in and turned on?" << std::endl;
	finish(-1);
}

void playback_file(const char *file) {
	MidiFile *midifile = new MidiFile(file);
	MidiEvent mev;
	int color, deltatick;

	midifile->joinTracks();

	for (int event = 0; event < (*midifile)[0].size(); event++) {
		mev = (*midifile)[0][event];
		if (event == 0) deltatick = mev.tick;
		else deltatick = mev.tick - (*midifile)[0][event - 1].tick;
		if ((int)mev[0] == 128 || (int)mev[0] == 144) {
			color = (int)mev[2];
			if ((int)mev[0] == 128) color = 0;
			if (deltatick > 0) usleep(60000 * 1000 / (BPM * deltatick));
			launchpad->setColor(note_to_button((int)mev[1]), color);
		}
	}

	delete midifile;
}

int main() {
	std::vector<unsigned char> message_in;
	int nBytes, color;
	double stamp;
	srand(time(NULL));

	if (launchpad->connect() < 0) launchpad_not_found_quit();
	launchpad->setPulse(LAUNCHPAD_PRO_SIDE_LED_ID, 53); // Pulse side LED

	std::cout << "Playing MIDI File..." << std::endl;
	playback_file("../references/Light_Output_Test.mid");
	std::cout << "Completed MIDI file playback!" << std::endl << std::endl;

	(void)signal(SIGINT, finish); // Install an interrupt handler function.
	std::cout << "Reading MIDI from port ... quit with Ctrl-C." << std::endl;
	while (true) {
		stamp = launchpad->getMessage(&message_in);
		nBytes = message_in.size();
		if (nBytes > 0) {
			if ((message_in[0] == 144) || (message_in[0] == 176)) {
				color = rand() % 126 + 1;
				if (message_in[2] > 0) launchpad->setColor(message_in[1], color);
				else launchpad->setPulse(message_in[1], color);
			}
			std::cout << std::fixed << std::setprecision(7) << stamp << " |   ";
			for (int i = 0; i < nBytes; i++) std::cout << std::setw(4) << (int)message_in[i];
			std::cout << std::endl;
		}

		usleep( 10000 ); // Sleep for 10 milliseconds ... platform-dependent.
	}

	return 0;
}
