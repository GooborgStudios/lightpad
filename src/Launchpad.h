//
// Lightpad - Launchpad.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
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
		std::vector<unsigned char> *serialize();
};

class LaunchpadBase {
	public:
		LaunchpadBase();
		~LaunchpadBase();
		virtual int connect() = 0;
		virtual void disconnect() = 0;
		bool isConnected();
		void inquiryDevice();
		int getMidiPort(RtMidi *ports);
		double getMessage(std::vector<unsigned char> *message_in);
		void beginColorUpdate();
		void endColorUpdate();
		virtual void setColor(unsigned char light, unsigned char color) = 0;
		virtual void setPulse(unsigned char light, unsigned char color) = 0;

		std::regex PORT_REGEX;

	protected:
		void sendMessage(Message message);
		int preconnect();
		
		RtMidiIn *midiin;
		RtMidiOut *midiout;
		int inport;
		int outport;
		bool connected;
		SysExMessage *color_update;

	private:
		virtual int buttonToNote(int button);
};

class LaunchpadPro: public LaunchpadBase {
	public:
		LaunchpadPro();
		int connect();
		void disconnect();
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
		void setColor(unsigned char light, unsigned char color);
		void setPulse(unsigned char light, unsigned char color);
	
	private:
		int buttonToNote(int button);
};

extern LaunchpadBase *launchpad;
