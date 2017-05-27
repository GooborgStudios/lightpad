//
// Lightpad - Launchpad.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#include <cstdarg>
#include <ctime>
#include <regex>

#include "RtMidi.h"

#define ABLETON_LIVE_MODE 4
#define MIDI_MESSAGE_SYSEX_BEGIN 240
#define MIDI_MESSAGE_SYSEX_END 247
#define LAUNCHPAD_PRO_SIDE_LED_ID 99
#define END_MSG 256

#ifdef WINDOWS
	void usleep(__int64 usec);
#endif

class Message {
	public:
		Message();
		Message(unsigned int first_byte, ...);
		~Message();
		void append(unsigned int first_byte, va_list varlist);
		void append(unsigned int first_byte, ...);
		std::vector<unsigned char> *serialize();

	protected:
		std::vector<unsigned char> *data;
};

class SysExMessage: public Message {
	public:
		SysExMessage(unsigned int msg_type);
		SysExMessage(unsigned int msg_type, unsigned int first_byte, ...);
		~SysExMessage();
		std::vector<unsigned char> *serialize();
};

class LaunchpadBase {
	public:
		LaunchpadBase();
		~LaunchpadBase();
		int connect();
		virtual void disconnect();
		bool isConnected();
		void inquiryDevice();
		int getMidiPort(RtMidi *ports);
		double getMessage(std::vector<unsigned char> *message_in);
		void beginColorUpdate();
		void endColorUpdate();
		virtual void setColor(unsigned char light, unsigned char color);
		virtual void setPulse(unsigned char light, unsigned char color);
		std::regex PORT_REGEX;
	protected:
		RtMidiIn *midiin;
		RtMidiOut *midiout;
		int inport;
		int outport;
		void sendMessage(Message message);
		bool connected;
		SysExMessage *color_update;
};

class LaunchpadPro: public LaunchpadBase {
	public:
		LaunchpadPro();
		int connect();
		void disconnect();
		bool isConnected();
		void setColor(unsigned char light, unsigned char color);
		void setColor(unsigned char light, unsigned char red, unsigned char green, unsigned char blue);
		void setFlash(unsigned char light, unsigned char color);
		void setPulse(unsigned char light, unsigned char color);
		void displayText(unsigned int color, std::string text);
		void displayText(unsigned int color, unsigned int speed, std::string text);
		void stopText();
};

class LaunchpadS: public LaunchpadBase {
	public:
		LaunchpadS();
		int connect();
		void disconnect();
		bool isConnected();
		void setColor(unsigned char light, unsigned char color);
		void setPulse(unsigned char light, unsigned char color);
		unsigned char pro_to_s_note(unsigned char pro_note, unsigned char msg_type);
		unsigned char pro_to_s_color(unsigned char pro_color);
};

extern LaunchpadPro *launchpad;
