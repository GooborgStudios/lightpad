//
// Lightpad - midiprobe.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#ifdef _WIN32
	#define OPERATING_SYSTEM "Windows"
#elif __APPLE__
	#define OPERATING_SYSTEM "macOS"
#else
	#warning "Unknown/unsupported compiler/operating system"
	#define OPERATING_SYSTEM "UNKNOWN"
#endif

#include <iostream>
#include <cstdlib>

#include "RtMidi.h"

RtMidiIn *midiin = 0;
RtMidiOut *midiout = 0;

void quit(int code) {
	delete midiin;
	delete midiout;
	exit(code);
}
void quit(RtMidiError &error) {
	error.printMessage();
	quit(1);
}
void quit() {
	quit(0);
}

int main() {
	int nPorts;

	std::cout << "Operating System: " << OPERATING_SYSTEM << std::endl << std::endl;

	try {
		midiin = new RtMidiIn();
		midiout = new RtMidiOut();
	} catch (RtMidiError &error) {
		quit(error);
	}

	// Check inputs.
	nPorts = midiin->getPortCount();
	std::cout << "There are " << nPorts << " MIDI input ports available:" << std::endl;
	std::string portName;
	for (int i = 0; i < nPorts; i++) {
		try {
			portName = midiin->getPortName(i);
		} catch (RtMidiError &error) {
			quit(error);
		}
		std::cout << " - Input Port #" << i + 1 << ": " << portName << std::endl;
	}
	std::cout << std::endl;

	// Check outputs.
	nPorts = midiout->getPortCount();
	std::cout << "There are " << nPorts << " MIDI output ports available:" << std::endl;
	for (int i = 0; i < nPorts; i++) {
		try {
			portName = midiout->getPortName(i);
		} catch (RtMidiError &error) {
			quit(error);
		}
		std::cout << " - Output Port #" << i + 1 << ": " << portName << std::endl;
	}
	std::cout << std::endl;

	std::cin.ignore(); // Wait for keypress before quitting
	quit(0);
}
