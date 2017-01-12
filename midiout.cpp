// midiout.cpp

#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "RtMidi.h"

#ifdef _WIN32
   //define something for Windows (32-bit and 64-bit, this part is common)
   #ifdef _WIN64
      //define something for Windows (64-bit only)
   #endif
#elif __APPLE__
    //define something for Mac
#else
	#error "Unknown/unsupported compiler/operating system"
#endif

bool done;
static void finish(int ignore){ done = true; std::cout << std::endl; }

int main() {
	RtMidiIn *midiin = new RtMidiIn();
	RtMidiOut *midiout = new RtMidiOut();
	int inport, outport = -1;
	std::vector<unsigned char> message_out;
	std::vector<unsigned char> message_in;
	std::string t("Launchpad Pro Standalone Port");
	int nBytes, i;
	double stamp;
	// Check available ports.
	for ( unsigned int i=0; i < midiin->getPortCount(); i++ ) {
		try {
		    std::string u("MIDIIN2 (Launchpad Pro)");
			if ( midiin->getPortName(i).compare(0, t.length(), t) == 0 || midiin->getPortName(i).compare(0, u.length(), u) == 0 ) {
				inport = i;
				std::cout << "In:  " << i << std::endl;
				break;
			}
		}
		catch ( RtMidiError &error ) {
			error.printMessage();
			return 1;
		}
	}
	for ( unsigned int i=0; i < midiout->getPortCount(); i++ ) {
		try {
		    std::string u("MIDIOUT2 (Launchpad Pro)");
			if ( midiout->getPortName(i).compare(0, t.length(), t) == 0 || midiout->getPortName(i).compare(0, u.length(), u) == 0 ) {
				outport = i;
				std::cout << "Out: " << i << std::endl;
				break;
			}
		}
		catch ( RtMidiError &error ) {
			error.printMessage();
			return 1;
		}
	}
	if ( (inport == -1) || (outport == -1) ) {
		std::cout << "Launchpad not found!  Is it plugged in and turned on?\n";
		delete midiin;
		delete midiout;
		return 2;
	}
	// Open Launchpad Standalone port.
	midiin->openPort( inport );
	midiout->openPort( outport );
	// Send out a series of MIDI messages.
	// Set to Programmer Mode
	message_out.push_back( 240 );
	message_out.push_back( 0 );
	message_out.push_back( 32 );
	message_out.push_back( 41 );
	message_out.push_back( 2 );
	message_out.push_back( 16 );
	message_out.push_back( 44 ); // Mode Switch
	message_out.push_back( 3 ); // Programmer Mode
	message_out.push_back( 247 );
	midiout->sendMessage( &message_out );
	message_out.erase(message_out.begin(),  message_out.begin()+message_out.size());

	/*
	// Scroll text
	message_out.push_back( 240 );
	message_out.push_back( 0 );
	message_out.push_back( 32 );
	message_out.push_back( 41 );
	message_out.push_back( 2 );
	message_out.push_back( 16 );
	message_out.push_back( 20 ); // Scroll Text
	message_out.push_back( 53 ); // Purple
	message_out.push_back( 1 ); // Loop Text
	message_out.push_back( 5 ); // Speed 5 of 7
	message_out.push_back( 'H' );
	message_out.push_back( 'e' );
	message_out.push_back( 'l' );
	message_out.push_back( 'l' );
	message_out.push_back( 'o' );
	message_out.push_back( ',' );
	message_out.push_back( ' ' );
	message_out.push_back( 6 ); // Speed 6 of 7
	message_out.push_back( 'w' );
	message_out.push_back( 'o' );
	message_out.push_back( 'r' );
	message_out.push_back( 'l' );
	message_out.push_back( 'd' );
	message_out.push_back( '!' );
	message_out.push_back( 247 );
	midiout->sendMessage( &message_out );
	message_out.erase(message_out.begin(),  message_out.begin()+message_out.size());
	*/

	// Pulse side LED
	message_out.push_back( 240 );
	message_out.push_back( 0 );
	message_out.push_back( 32 );
	message_out.push_back( 41 );
	message_out.push_back( 2 );
	message_out.push_back( 16 );
	message_out.push_back( 40 ); // Pulse
	message_out.push_back( 99 ); // Side LED
	message_out.push_back( 53 ); // Purple
	message_out.push_back( 247 );
	midiout->sendMessage( &message_out );
	message_out.erase(message_out.begin(),  message_out.begin()+message_out.size());

	/*
	// Program change: 192, 5
	message_out.push_back( 192 );
	message_out.push_back( 5 );
	midiout->sendMessage( &message_out );
	// Control Change: 176, 7, 100 (volume)
	message_out[0] = 176;
	message_out[1] = 7;
	message_out.push_back( 100 );
	midiout->sendMessage( &message_out );
	// Note On: 144, 64, 90
	message_out[0] = 144;
	message_out[1] = 64;
	message_out[2] = 90;
	midiout->sendMessage( &message_out );
	SLEEP( 500 ); // Platform-dependent ... see example in tests directory.
	// Note Off: 128, 64, 40
	message_out[0] = 128;
	message_out[1] = 64;
	message_out[2] = 40;
	midiout->sendMessage( &message_out );
	*/

	// Don't ignore sysex, timing, or active sensing messages.
	midiin->ignoreTypes( false, false, false );

	// Install an interrupt handler function.
	done = false;
	(void) signal(SIGINT, finish);
	// Periodically check input queue.
	std::cout << "Reading MIDI from port ... quit with Ctrl-C.\n";
	while ( !done ) {
		stamp = midiin->getMessage( &message_in );
		nBytes = message_in.size();
		if (nBytes > 0) {
			if ((message_in[0] == 144) || (message_in[0] == 176)) {
				message_out.push_back( 240 );
				message_out.push_back( 0 );
				message_out.push_back( 32 );
				message_out.push_back( 41 );
				message_out.push_back( 2 );
				message_out.push_back( 16 );
				if ((int)message_in[2] > 0) message_out.push_back( 10 ); // Lit While Held
				else message_out.push_back( 40 ); // Pulse When Released
				message_out.push_back( (int)message_in[1] ); // Pressed Button
				message_out.push_back( rand() % 127 + 0 ); // Random color
				message_out.push_back( 247 );
				midiout->sendMessage( &message_out );
				message_out.erase(message_out.begin(),  message_out.begin()+message_out.size());
			}
		}
		for ( int i=0; i<nBytes; i++ )
			std::cout << "Byte " << i << " = " << (int)message_in[i] << ", ";
		if ( nBytes > 0 )
			std::cout << "stamp = " << stamp << std::endl;
		// Sleep for 10 milliseconds ... platform-dependent.
		usleep( 10000 );
	}

	// Clear colors
	message_out.push_back( 240 );
	message_out.push_back( 0 );
	message_out.push_back( 32 );
	message_out.push_back( 41 );
	message_out.push_back( 2 );
	message_out.push_back( 16 );
	message_out.push_back( 14 ); // Select All LEDS
	message_out.push_back( 0 ); // Turn off
	message_out.push_back( 247 );
	midiout->sendMessage( &message_out );
	message_out.erase(message_out.begin(),  message_out.begin()+message_out.size());

	// Set to Note Mode
	message_out.push_back( 240 );
	message_out.push_back( 0 );
	message_out.push_back( 32 );
	message_out.push_back( 41 );
	message_out.push_back( 2 );
	message_out.push_back( 16 );
	message_out.push_back( 44 ); // Mode Switch
	message_out.push_back( 0 ); // Note Mode
	message_out.push_back( 247 );
	midiout->sendMessage( &message_out );
	message_out.erase(message_out.begin(),  message_out.begin()+message_out.size());

	delete midiin;
	delete midiout;
	return 0;
}
