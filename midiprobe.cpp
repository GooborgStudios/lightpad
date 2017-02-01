//
// Lightpad - midiprobe.cpp
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.nightwave.co/lightpad
//

#include <iostream>
#include <cstdlib>
#include "RtMidi.h"

#ifdef _WIN32
	//define something for Windows (32-bit and 64-bit, this part is common)
	#define OPERATING_SYSTEM "Windows"
	#ifdef _WIN64
		//define something for Windows (64-bit only)
	#endif
#elif __APPLE__
	//define something for Mac (and Linux(?))
	#define OPERATING_SYSTEM "macOS"
#else
	#error "Unknown/unsupported compiler/operating system"
#endif

int main() {
	std::cout << "Operating System: " << OPERATING_SYSTEM << std::endl;
	RtMidiIn  *midiin = 0;
	RtMidiOut *midiout = 0;
	// RtMidiIn constructor
	try {
		midiin = new RtMidiIn();
	}
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
			error.printMessage();
			goto cleanup;
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
	// Clean up
 cleanup:
 	std::cin.ignore();
	delete midiin;
	delete midiout;
	return 0;
}
