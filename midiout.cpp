//
// Lightpad - midiout.cpp
// Created by Vinyl Darkscratch, Light Apacha, and WhoovesPON3, ©2017 Nightwave Studios.
// Additional assistance by Eric Busch (Origami1105).
// http://www.nightwave.co/lightpad
//

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#include "RtMidi.h"
#include "MidiFile.h"

#include "Helpers.h"

#ifdef _WIN32
	//define something for Windows (32-bit and 64-bit, this part is common)
	#define LAUNCHPAD_IN "MIDIIN2 (Launchpad Pro)"
	#define LAUNCHPAD_OUT "MIDIOUT2 (Launchpad Pro)"

	#ifdef _WIN64
		//define something for Windows (64-bit only)
	#endif
#elif __APPLE__
	//define something for Mac
	#define LAUNCHPAD_IN "Launchpad Pro Standalone Port"
	#define LAUNCHPAD_OUT "Launchpad Pro Standalone Port"
#else
	#error "Unknown/unsupported compiler/operating system"
#endif

#define ABLETON_LIVE_MODE 4
#define MIDI_MESSAGE_SYSEX_BEGIN 240
#define MIDI_MESSAGE_SYSEX_END 247
#define LAUNCHPAD_SIDE_LED_ID 99

int BPM = 120;

bool done;
static void finish(int ignore){ done = true; std::cout << std::endl; }

class Launchpad {
	public:
		Launchpad();
		int connect();
		void disconnect();
		int getMidiPort(std::string name, RtMidi *ports);
		double getMessage(std::vector<unsigned char> *message_in);
		void setColor(unsigned char light, unsigned char color);
		void setColor(unsigned char light, unsigned char red, unsigned char green, unsigned char blue);
		void setFlash(unsigned char light, unsigned char color);
		void setPulse(unsigned char light, unsigned char color);
		void displayText(unsigned int color, unsigned int speed, std::string text);
	private:
		RtMidiIn *midiin;
		RtMidiOut *midiout;
		int inport;
		int outport;
		std::vector<unsigned char> message;

		void sendMessage(unsigned int first_byte, ...);
};

Launchpad::Launchpad() {
	midiin = new RtMidiIn();
	midiout = new RtMidiOut();
}

int Launchpad::connect() {
	inport = getMidiPort(LAUNCHPAD_IN, midiin);
	outport = getMidiPort(LAUNCHPAD_OUT, midiout);

	if ( (inport == -1) || (outport == -1) ) {
		std::cout << "Launchpad not found!  Is it plugged in and turned on?\n";
		delete midiin;
		delete midiout;
		return -1;
	}

	// Open Launchpad Standalone port.
	midiin->openPort( inport );
	midiout->openPort( outport );

	// Don't ignore sysex, timing, or active sensing messages.
	midiin->ignoreTypes( false, false, false );

	std::vector<unsigned char> device_info;

	// Inquiry Device
	sendMessage( 240, 126, 127, 6, 1, 247 );
	while (device_info.size() == 0) {
		getMessage(&device_info);
		usleep( 10000 ); // Sleep for 10 milliseconds ... platform-dependent.
	}

	sendMessage( 240, 0, 32, 41, 2, 16, 33, 1, 247 ); // Set to Standalone Mode
	sendMessage( 240, 0, 32, 41, 2, 16, 44, 3, 247 ); // Set to Programmer Mode

	return 0;
}

void Launchpad::disconnect() {
	sendMessage( 240, 0, 32, 41, 2, 16, 14, 0, 247 ); // Clear all LED colors
	sendMessage( 240, 0, 32, 41, 2, 16, 44, 0, 247 ); // Set to Note Mode

	delete midiin;
	delete midiout;
}

int Launchpad::getMidiPort(std::string name, RtMidi *ports) {
	for ( unsigned int i=0; i < ports->getPortCount(); i++ ) {
		try {
			if ( ports->getPortName(i).compare(0, name.length(), name) == 0 ) {
				return i;
			}
		}
		catch ( RtMidiError &error ) {
			error.printMessage();
			return -1;
		}
	}

	return -1;
}

double Launchpad::getMessage(std::vector<unsigned char> *message_in) {
	return midiin->getMessage(message_in);
}

void Launchpad::sendMessage(unsigned int first_byte, ...) {
	va_list vl;
	va_start(vl, first_byte);
	unsigned char byte = first_byte;
	while (byte != MIDI_MESSAGE_SYSEX_END || byte > 255) {
		message.push_back(byte);
		byte = va_arg(vl, unsigned int);
	}
	message.push_back(byte);
	va_end(vl);

	midiout->sendMessage(&message);
	message.erase(message.begin(), message.begin()+message.size());
}

void Launchpad::setColor(unsigned char light, unsigned char color) {
	sendMessage( 240, 0, 32, 41, 2, 16, 10, light, color, 247 );
}

void Launchpad::setColor(unsigned char light, unsigned char red, unsigned char green, unsigned char blue) {
	sendMessage( 240, 0, 32, 41, 2, 16, 11, light, red, green, blue, 247 );
}

void Launchpad::setFlash(unsigned char light, unsigned char color) {
	sendMessage( 240, 0, 32, 41, 2, 16, 35, light, color, 247 );
}

void Launchpad::setPulse(unsigned char light, unsigned char color) {
	sendMessage( 240, 0, 32, 41, 2, 16, 40, light, color, 247 );
}

void Launchpad::displayText(unsigned int color, unsigned int speed, std::string text) {
	// XXX Broken!

	// addToMessage(240, 0, 32, 41, 2, 16, 40, color, speed);
	// for (int i = 0; i < text.size(); ++i)
	// 	addToMessage(text[i]);
	// addToMessage(247);
	// sendMessage();
}

int main() {
	MidiFile midifile;
	MidiEvent* mev;
	std::vector<unsigned char> message_in;
	int nBytes, color, deltatick;
	double stamp;
	Launchpad *lp = new Launchpad();
	int lp_status = lp->connect();
	if (lp_status < 0) {
		return 1;
	}

	srand(time(NULL));

	/*
	// Scroll text
	message.push_back( 240 );
	message.push_back( 0 );
	message.push_back( 32 );
	message.push_back( 41 );
	message.push_back( 2 );
	message.push_back( 16 );
	message.push_back( 20 ); // Scroll Text
	message.push_back( 53 ); // Purple
	message.push_back( 1 ); // Loop Text
	message.push_back( 5 ); // Speed 5 of 7
	message.push_back( 'H' );
	message.push_back( 'e' );
	message.push_back( 'l' );
	message.push_back( 'l' );
	message.push_back( 'o' );
	message.push_back( ',' );
	message.push_back( ' ' );
	message.push_back( 6 ); // Speed 6 of 7
	message.push_back( 'w' );
	message.push_back( 'o' );
	message.push_back( 'r' );
	message.push_back( 'l' );
	message.push_back( 'd' );
	message.push_back( '!' );
	message.push_back( 247 );
	midiout->sendMessage( &message );
	message.erase(message.begin(),  message.begin()+message.size());
	*/

	// Pulse side LED
	lp->setPulse(LAUNCHPAD_SIDE_LED_ID, 53);

	/*
	// Program change: 192, 5
	message.push_back( 192 );
	message.push_back( 5 );
	midiout->sendMessage( &message );
	// Control Change: 176, 7, 100 (volume)
	message[0] = 176;
	message[1] = 7;
	message.push_back( 100 );
	midiout->sendMessage( &message );
	// Note On: 144, 64, 90
	message[0] = 144;
	message[1] = 64;
	message[2] = 90;
	midiout->sendMessage( &message );
	SLEEP( 500 ); // Platform-dependent ... see example in tests directory.
	// Note Off: 128, 64, 40
	message[0] = 128;
	message[1] = 64;
	message[2] = 40;
	midiout->sendMessage( &message );
	*/

	midifile.read("./Light_Output_Test.mid");
	midifile.joinTracks();

	std::cout << "Playing MIDI File..." << std::endl;
	
	for (int event=0; event < midifile[0].size(); event++) {
		mev = &midifile[0][event];
		if (event == 0) {
			deltatick = mev->tick;
		} else {
			deltatick = mev->tick - midifile[0][event-1].tick;
		}
		if ((int)(*mev)[0] == 128 || (int)(*mev)[0] == 144) {
			color = (int)(*mev)[2];
			if ((int)(*mev)[0] == 128) color = 0;
			if (deltatick > 0) {
				usleep(60000 * 1000 / (BPM * deltatick));
			}

			lp->setColor(note_to_button((int)(*mev)[1]), color);

		}

		// std::cout << dec << mev->tick;
		// std::cout << '\t' << deltatick;
		// std::cout << '\t' << mev->track;
		// std::cout << '\t' << std::hex;
		// for (int i=0; i < mev->size(); i++) {
		// 	std::cout << (int)(*mev)[i] << ' ';
		// }
		// std::cout << std::endl;
	}

	std::cout << "Completed MIDI file playback!" << std::endl << std::endl;

	// Install an interrupt handler function.
	done = false;
	(void) signal(SIGINT, finish);
	// Periodically check input queue.
	std::cout << "Reading MIDI from port ... quit with Ctrl-C." << std::endl;
	while (!done) {
		stamp = lp->getMessage(&message_in);
		nBytes = message_in.size();
		if (nBytes > 0) {
			if ((message_in[0] == 144) || (message_in[0] == 176)) {
				color = rand() % 126 + 1;
				if (message_in[2] > 0) {

					lp->setColor(message_in[1], color);
				} else {
					lp->setPulse(message_in[1], color);
				}
			}
		}

		if ( nBytes > 0 ) {
			std::cout << std::fixed << std::setprecision(7) << stamp << " |   ";
			for ( int i=0; i<nBytes; i++ )
				std::cout << std::setw(4) << (int)message_in[i];
			std::cout << std::endl;
		}
		
		usleep( 10000 ); // Sleep for 10 milliseconds ... platform-dependent.
	}

	// delete midiin;
	// delete midiout;
	lp->disconnect();
	return 0;
}
