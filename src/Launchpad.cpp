//
// Lightpad - Launchpad.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

// XXX Found a bunch of memory leaks in this file

#include "Launchpad.h"

#include <cstdarg>
#include <ctime>
#include <regex>

#include "RtMidi.h"

#ifdef WINDOWS
	#include <windows.h>
	void usleep(__int64 usec) {
		HANDLE timer;
		LARGE_INTEGER ft;

		// Convert to 100 nanosecond interval, negative value indicates relative time
		ft.QuadPart = -(10 * usec);

		timer = CreateWaitableTimer(NULL, TRUE, NULL);
		SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
		WaitForSingleObject(timer, INFINITE);
		CloseHandle(timer);
	}
#endif

Message::Message() {
	data = new std::vector<unsigned char>;
}

Message::Message(unsigned int first_byte, ...): Message::Message() {

	va_list varlist;
	va_start(varlist, first_byte);
	append(first_byte, varlist);
	va_end(varlist);
}

Message::~Message() {
	//delete data;
}

void Message::append(unsigned int first_byte, va_list varlist) {
	unsigned int byte = first_byte;
	while (byte != END_MSG) {
		data->push_back(byte);
		byte = va_arg(varlist, unsigned int);
	}
}

void Message::append(unsigned int first_byte, ...) {
	va_list varlist;
	va_start(varlist, first_byte);
	append(first_byte, varlist);
	va_end(varlist);
}

std::vector<unsigned char> *Message::serialize() {
	return data;
}

SysExMessage::SysExMessage(unsigned int msg_type) : Message() {
	append(240, 0, 32, 41, 2, 16, msg_type, END_MSG);
}

SysExMessage::SysExMessage(unsigned int msg_type, unsigned int first_byte, ...) : Message() {
	append(240, 0, 32, 41, 2, 16, msg_type, END_MSG);

	va_list varlist;
	va_start(varlist, first_byte);
	append(first_byte, varlist);
	va_end(varlist);
}

std::vector<unsigned char> *SysExMessage::serialize() {
	if (data->back() != 247) append(247, END_MSG);
	return data;
}

LaunchpadBase::LaunchpadBase() {
	PORT_REGEX = std::regex(".*");
	connected = true;
	inport = -1;
	outport = -1;
	midiin = new RtMidiIn();
	midiout = new RtMidiOut();
	color_update = NULL;
	// message = new std::vector<unsigned char>();
}

LaunchpadBase::~LaunchpadBase() {
	delete midiin;
	delete midiout;
}

int LaunchpadBase::preconnect() {
	connected = true;

	inport = getMidiPort(midiin);
	outport = getMidiPort(midiout);

	if ((inport == -1) || (outport == -1)) return -1;

	midiin->openPort(inport);
	midiout->openPort(outport);
	midiin->ignoreTypes(false, false, false); // Don't ignore sysex, timing, or active sensing messages.
	return 0;
}

bool LaunchpadBase::isConnected() {
	return connected;
}

void LaunchpadBase::inquiryDevice() {
	std::vector<unsigned char> device_info;

	// Inquiry Device
	Message message(240, 126, 127, 6, 1, 247, END_MSG);
	sendMessage(message);
	while (device_info.size() == 0) {
		getMessage(&device_info);
		//usleep( 10000 ); // Sleep for 10 milliseconds
	}
}

int LaunchpadBase::getMidiPort(RtMidi *ports) {
	for ( unsigned int i = 0; i < ports->getPortCount(); i++ ) {
		try {
			if (std::regex_match(ports->getPortName(i), PORT_REGEX)) {
				return i;
			}
		} catch (RtMidiError &error) {
			error.printMessage();
			return -1;
		}
	}

	return -1;
}

int LaunchpadBase::buttonToNote(int button) {
	return button;
}

double LaunchpadBase::getMessage(std::vector<unsigned char> *message_in) {
	if (!isConnected()) return -1;
	return midiin->getMessage(message_in);
}

void LaunchpadBase::sendMessage(Message message) {
	if (!isConnected()) return;
	midiout->sendMessage(message.serialize());
}

void LaunchpadBase::beginColorUpdate() {
	if (!isConnected()) return;
	color_update = new SysExMessage(10);
}

void LaunchpadBase::endColorUpdate() {
	if (!isConnected()) return;
	color_update->append(2471, END_MSG);
	sendMessage(*color_update);
	delete color_update;
}


LaunchpadPro::LaunchpadPro() {
	#ifdef WINDOWS
	PORT_REGEX = std::regex("MIDI(IN|OUT)2\\s\\((\\d+\\s-\\s)?Launchpad\\sPro(\\s\\d+)?\\)");
	#else
	PORT_REGEX = std::regex("Launchpad\\sPro(\\s\\d+)?\\sStandalone\\sPort");
	#endif
}

int LaunchpadPro::connect() {
	int status = preconnect();
	if (status == 0) {
		LaunchpadBase::inquiryDevice();

		SysExMessage msg1(33, 1, END_MSG);
		sendMessage(msg1); // Set to Standalone Mode
		SysExMessage msg2(44, 3, END_MSG);
		sendMessage(msg2); // Set to Programmer Mode
	}
	return status;
}

void LaunchpadPro::disconnect() {
	if (isConnected()) {
		SysExMessage msg1(14, 0, END_MSG);
		sendMessage(msg1); // Clear all LED colors
		SysExMessage msg2(44, 0, END_MSG);
		sendMessage(msg2); // Set to Note Mode
	}
}

void LaunchpadPro::setColor(unsigned char light, unsigned char color) {
	if (!isConnected()) return;

	if (color_update) {
		color_update->append(light, color, END_MSG);
	} else {
		SysExMessage msg(10, light, color, END_MSG);
		sendMessage(msg);
	}
}

void LaunchpadPro::setColor(unsigned char light,
							unsigned char red, unsigned char green, unsigned char blue) {
	if (!isConnected()) return;
	SysExMessage msg(11, light, red, green, blue, END_MSG);
	sendMessage(msg);
}

void LaunchpadPro::setFlash(unsigned char light, unsigned char color) {
	if (!isConnected()) return;
	SysExMessage msg(35, light, color, END_MSG);
	sendMessage(msg);
}

void LaunchpadPro::setPulse(unsigned char light, unsigned char color) {
	if (!isConnected()) return;
	SysExMessage msg(40, light, color, END_MSG);
	sendMessage(msg);
}

void LaunchpadPro::displayText(unsigned int color, std::string text) {
	if (!isConnected()) return;
	bool slash = false;

	SysExMessage msg(20);
	msg.append(color, END_MSG);
	char letter;

	for (int i = 0; i < text.size(); ++i) {
		letter = text[i];
		if (slash) {
			slash = false;
			if (letter == '/') msg.append('/', END_MSG);
			else if (letter >= '1' && letter <= '7') msg.append(letter-'1', END_MSG);
			else msg.append('/', letter, END_MSG);
		} else if (letter == '/') {
			slash = true;
		} else {
			msg.append(letter, END_MSG);
		}
	}

	sendMessage(msg);
}

void LaunchpadPro::displayText(unsigned int color, unsigned int speed,
							   std::string text) {
	displayText(color, "/" + std::to_string(speed) + text);
}

void LaunchpadPro::stopText() {
	if (!isConnected()) return;
	SysExMessage msg(16, 20, END_MSG);
	sendMessage(msg);
}

LaunchpadS::LaunchpadS() {
	PORT_REGEX = std::regex("Launchpad S");
}

int LaunchpadS::connect() {
	int status = preconnect();

	if (status == 0) {
		LaunchpadBase::inquiryDevice();
		Message msg(176, 0, 2, END_MSG);
		sendMessage(msg); // Set to Drum Rack Mode
	}
	return status;
}

void LaunchpadS::disconnect() {
	if (isConnected()) {
		Message msg(176, 0, 0, END_MSG);
		sendMessage(msg); // Reset
	}
}

int LaunchpadS::buttonToNote(int button) {
	return button; //XXX FIXME
}

void LaunchpadS::setColor(unsigned char light, unsigned char color) {
	if (!isConnected()) return;
	Message msg(144, buttonToNote(light), color, END_MSG);
	sendMessage(msg);
}

void LaunchpadS::setPulse(unsigned char light, unsigned char color) {
	if (!isConnected()) return;
	// XXX Implement me!
}

LaunchpadBase *launchpad = new LaunchpadPro();
