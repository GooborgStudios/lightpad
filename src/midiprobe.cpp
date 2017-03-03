//
// Lightpad - midiprobe.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#ifdef _WIN32
	#define OPERATING_SYSTEM "Windows"
#elif __APPLE__
	#define OPERATING_SYSTEM "macOS"
#else
	#error "Unknown/unsupported compiler/operating system"
#endif

#include <iostream>
#include <cstdlib>

#include "RtMidi.h"

RtMidiIn  *midiin = 0;
RtMidiOut *midiout = 0;

void quit(int error_code) {
	delete midiin;
	delete midiout;
	exit(error_code);
}

void quit(int error_code, RtMidiError &error) {
	error.printMessage();
	quit(error_code);
}

void quit() {
	quit(0);
}

int main() {
	std::cout << "Operating System: " << OPERATING_SYSTEM << std::endl;
	// RtMidiIn constructor
	try midiin = new RtMidiIn();
	catch ( RtMidiError &error ) {
		error.printMessage();
		exit( EXIT_FAILURE );
	}
	// Check inputs.
	unsigned int nPorts = midiin->getPortCount();
	std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
	std::string portName;
	for ( unsigned int i=0; i<nPorts; i++ ) {
		try {
			portName = midiin->getPortName(i);
		}
		catch ( RtMidiError &error ) {
			quit(1, error);
		}
		std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
	}
	// RtMidiOut constructor
	try {
		midiout = new RtMidiOut();
	}
	catch ( RtMidiError &error ) {
		error.printMessage();
		exit( EXIT_FAILURE );
	}
	// Check outputs.
	nPorts = midiout->getPortCount();
	std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
	for ( unsigned int i=0; i<nPorts; i++ ) {
		try {
			portName = midiout->getPortName(i);
		}
		catch (RtMidiError &error) {
			error.printMessage();
			goto cleanup;
		}
		std::cout << "  Output Port #" << i+1 << ": " << portName << '\n';
	}
	std::cout << '\n';

 	std::cin.ignore();
	return quit(0);
}
