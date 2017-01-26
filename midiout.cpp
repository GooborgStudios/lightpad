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

	#ifdef _WIN64
		//define something for Windows (64-bit only)
	#endif
#else//#elif __APPLE__
	//define something for Mac (and Linux(?))
//#else
	//#error "Unknown/unsupported compiler/operating system"
#endif

#define ABLETON_LIVE_MODE 4
#define MIDI_MESSAGE_SYSEX_BEGIN 240
#define MIDI_MESSAGE_SYSEX_END 247
#define LAUNCHPAD_PRO_SIDE_LED_ID 99

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
		virtual void setColor(unsigned char msg_type, unsigned char light, unsigned char color);
		virtual void setPulse(unsigned char msg_type, unsigned char light, unsigned char color);
		std::string INPORT_NAME;
		std::string OUTPORT_NAME;
	protected:
		RtMidiIn *midiin;
		RtMidiOut *midiout;
		int inport;
		int outport;
		std::vector<unsigned char> message;
		void sendMessage(unsigned int first_byte, ...);
		
};

class LaunchpadPro : public Launchpad {
	public:
		LaunchpadPro();
		int connect();
		void disconnect();
		void setColor(unsigned char msg_type, unsigned char light, unsigned char color);
		void setColor(unsigned char msg_type, unsigned char light, unsigned char red, unsigned char green, unsigned char blue);
		void setFlash(unsigned char msg_type, unsigned char light, unsigned char color);
		void setPulse(unsigned char msg_type, unsigned char light, unsigned char color);
		void displayText(unsigned char msg_type, unsigned int color, unsigned int speed, std::string text);
};

class LaunchpadS : public Launchpad {
	public:
		LaunchpadS();
		int connect();
		void disconnect();
		void setColor(unsigned char msg_type, unsigned char light, unsigned char color);
		void setPulse(unsigned char msg_type, unsigned char light, unsigned char color);
		unsigned char pro_to_s_note(unsigned char pro_note, unsigned char msg_type);
		unsigned char pro_to_s_color(unsigned char pro_color);
};

Launchpad::Launchpad() {
	midiin = new RtMidiIn();
	midiout = new RtMidiOut();
}

int Launchpad::connect() {
	inport = getMidiPort(INPORT_NAME, midiin);
	outport = getMidiPort(OUTPORT_NAME, midiout);

	if ( (inport == -1) || (outport == -1) ) {
		std::cout << "Launchpad not found!  Is it plugged in and turned on?\n";
		delete midiin;
		delete midiout;
		return -1;
	}

	// Open LaunchpadPro Standalone port.
	midiin->openPort( inport );
	midiout->openPort( outport );

	// Don't ignore sysex, timing, or active sensing messages.
	midiin->ignoreTypes( false, false, false );

	return 0;

}

void Launchpad::disconnect() {
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
	while (byte != MIDI_MESSAGE_SYSEX_END || byte < 0 || byte > 255) {
		message.push_back(byte);
		byte = va_arg(vl, unsigned int);
	}
	if (byte >= 0 && byte <= 255) message.push_back(byte);
	va_end(vl);

	midiout->sendMessage(&message);
	message.erase(message.begin(), message.begin()+message.size());
}

void Launchpad::setColor(unsigned char msg_type, unsigned char light, unsigned char color) {

}

void Launchpad::setPulse(unsigned char msg_type, unsigned char light, unsigned char color) {

}

LaunchpadPro::LaunchpadPro() {
	#ifdef _WIN32
		INPORT_NAME = "MIDIIN2 (Launchpad Pro)";
		OUTPORT_NAME = "MIDIOUT2 (Launchpad Pro)";
	#else
		INPORT_NAME = "Launchpad Pro Standalone Port";
		OUTPORT_NAME = "Launchpad Pro Standalone Port";
	#endif
}

int LaunchpadPro::connect() {
	int x = Launchpad::connect();

	if (x == 0) {
		std::vector<unsigned char> device_info;

		// Inquiry Device
		sendMessage( 240, 126, 127, 6, 1, 247 );
		while (device_info.size() == 0) {
			getMessage(&device_info);
			usleep( 10000 ); // Sleep for 10 milliseconds ... platform-dependent.
		}

		sendMessage( 240, 0, 32, 41, 2, 16, 33, 1, 247 ); // Set to Standalone Mode
		sendMessage( 240, 0, 32, 41, 2, 16, 44, 3, 247 ); // Set to Programmer Mode
	}
	return x;
}

void LaunchpadPro::disconnect() {
	sendMessage( 240, 0, 32, 41, 2, 16, 14, 0, 247 ); // Clear all LED colors
	sendMessage( 240, 0, 32, 41, 2, 16, 44, 0, 247 ); // Set to Note Mode

	Launchpad::disconnect();
}

void LaunchpadPro::setColor(unsigned char msg_type, unsigned char light, unsigned char color) {
	sendMessage( 240, 0, 32, 41, 2, 16, 10, light, color, 247 );
}

void LaunchpadPro::setColor(unsigned char msg_type, unsigned char light, unsigned char red, unsigned char green, unsigned char blue) {
	sendMessage( 240, 0, 32, 41, 2, 16, 11, light, red, green, blue, 247 );
}

void LaunchpadPro::setFlash(unsigned char msg_type, unsigned char light, unsigned char color) {
	sendMessage( 240, 0, 32, 41, 2, 16, 35, light, color, 247 );
}

void LaunchpadPro::setPulse(unsigned char msg_type, unsigned char light, unsigned char color) {
	sendMessage( 240, 0, 32, 41, 2, 16, 40, light, color, 247 );
}

void LaunchpadPro::displayText(unsigned char msg_type, unsigned int color, unsigned int speed, std::string text) {
	// XXX Broken!

	// addToMessage(240, 0, 32, 41, 2, 16, 40, color, speed);
	// for (int i = 0; i < text.size(); ++i)
	// 	addToMessage(text[i]);
	// addToMessage(247);
	// sendMessage();
}

LaunchpadS::LaunchpadS() {
	#ifdef _WIN32 // May not be accurate!
		INPORT_NAME = "Launchpad S";
		OUTPORT_NAME = "Launchpad S";
	#else
		INPORT_NAME = "Launchpad S";
		OUTPORT_NAME = "Launchpad S";
	#endif
}

int LaunchpadS::connect() {
	int x = Launchpad::connect();

	if (x == 0) {
		std::vector<unsigned char> device_info;

		// Inquiry Device
		sendMessage( 240, 126, 127, 6, 1, 247 );
		while (device_info.size() == 0) {
			getMessage(&device_info);
			usleep( 10000 ); // Sleep for 10 milliseconds ... platform-dependent.
		}

		sendMessage( 176, 0, 2, -1 ); // Set to Drum Rack Mode
	}
	return x;
}

void LaunchpadS::disconnect() {
	sendMessage( 176, 0, 0, -1 ); // Reset

	Launchpad::disconnect();
}

unsigned char LaunchpadS::pro_to_s_note(unsigned char pro_note, unsigned char msg_type) {
	if (msg_type == 176) return pro_note;
	return pro_note;
}

unsigned char LaunchpadS::pro_to_s_color(unsigned char pro_color) {
	return pro_color;
}

void LaunchpadS::setColor(unsigned char msg_type, unsigned char light, unsigned char color) {
	message.push_back(144);
	message.push_back(pro_to_s_note(light, msg_type));
	message.push_back(color);

	midiout->sendMessage(&message);
	message.erase(message.begin(), message.begin()+message.size());
}

void LaunchpadS::setPulse(unsigned char msg_type, unsigned char light, unsigned char color) {

}

int main() {
	MidiFile midifile;
	MidiEvent* mev;
	std::vector<unsigned char> message_in;
	int nBytes, color, deltatick;
	double stamp;
	LaunchpadS *lp = new LaunchpadS();
	int lp_status = lp->connect();
	if (lp_status < 0) {
		return 1;
	}

	srand(time(NULL));

	// Pulse side LED
	//lp->setPulse(176, LAUNCHPAD_PRO_SIDE_LED_ID, 53);

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

			lp->setColor((int)(*mev)[0], note_to_button((int)(*mev)[1]), color);

		}
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

					lp->setColor(message_in[0], message_in[1], color);
				} else {
					lp->setPulse(message_in[0], message_in[1], color);
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
