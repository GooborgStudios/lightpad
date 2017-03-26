//
// Lightpad - Launchpad.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

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

Message::Message(unsigned int first_byte, ...) {
	Message::Message();

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

SysExMessage::SysExMessage(unsigned int msg_type) {
	Message::Message();
	append(240, 0, 32, 41, 2, 16, msg_type, END_MSG);
}

SysExMessage::SysExMessage(unsigned int msg_type, unsigned int first_byte, ...) {
	Message::Message();
	append(240, 0, 32, 41, 2, 16, msg_type, END_MSG);

	va_list varlist;
	va_start(varlist, first_byte);
	append(first_byte, varlist);
	va_end(varlist);
}

SysExMessage::~SysExMessage() {
	Message::~Message();
}

std::vector<unsigned char> *SysExMessage::serialize() {
	if (data->back() != 247) append(247, END_MSG);
	return data;
}

LaunchpadBase::LaunchpadBase() {
	midiin = new RtMidiIn();
	midiout = new RtMidiOut();
	color_update = NULL;
	// message = new std::vector<unsigned char>();
}

LaunchpadBase::~LaunchpadBase() {
	delete midiin;
	delete midiout;
}

int LaunchpadBase::connect() {
	connected = true;

	inport = getMidiPort(midiin);
	outport = getMidiPort(midiout);

	if ((inport == -1) || (outport == -1)) return -1;

	midiin->openPort(inport);
	midiout->openPort(outport);
	midiin->ignoreTypes(false, false, false); // Don't ignore sysex, timing, or active sensing messages.
	return 0;
}

void LaunchpadBase::disconnect() {

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
	sendMessage(*color_update);
	delete color_update;
}

void LaunchpadBase::setColor(unsigned char light, unsigned char color) {
	if (!isConnected()) return;
}

void LaunchpadBase::setPulse(unsigned char light, unsigned char color) {
	if (!isConnected()) return;
}

LaunchpadPro::LaunchpadPro() {
	#ifdef WINDOWS
	PORT_REGEX = std::regex("MIDI(IN|OUT)2\\s\\((\\d+\\s-\\s)?Launchpad\\sPro(\\s\\d+)?\\)");
	#else
	PORT_REGEX = std::regex("Launchpad\\sPro(\\s\\d+)?\\sStandalone\\sPort");
	#endif
}

int LaunchpadPro::connect() {
	int status = LaunchpadBase::connect();
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
		LaunchpadBase::disconnect();
	}
}

bool LaunchpadPro::isConnected() {
	return LaunchpadBase::isConnected();
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

	for (int i = 0; i < text.size(); ++i) {
		if (slash) {
			slash = false;
			switch (text[i]) { // XXX There's a better way to do this
				case '/':
					msg.append('/', END_MSG);
					break;
				case '1':
					msg.append(1, END_MSG);
					break;
				case '2':
					msg.append(2, END_MSG);
					break;
				case '3':
					msg.append(3, END_MSG);
					break;
				case '4':
					msg.append(4, END_MSG);
					break;
				case '5':
					msg.append(5, END_MSG);
					break;
				case '6':
					msg.append(6, END_MSG);
					break;
				case '7':
					msg.append(7, END_MSG);
					break;
				default:
					msg.append('/', text[i], END_MSG);
					break;
			}
		} else if (text[i] == '/') {
			slash = true;
		} else {
			msg.append(text[i], END_MSG);
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
	int status = LaunchpadBase::connect();

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
		LaunchpadBase::disconnect();
	}
}

bool LaunchpadS::isConnected() {
	return LaunchpadBase::isConnected();
}

unsigned char LaunchpadS::pro_to_s_note(unsigned char pro_note, unsigned char msg_type) {
	if (msg_type == 176) return pro_note;
	return pro_note;
}

unsigned char LaunchpadS::pro_to_s_color(unsigned char pro_color) {
	return pro_color;
}

void LaunchpadS::setColor(unsigned char light, unsigned char color) {
	if (!isConnected()) return;
	Message msg(144, pro_to_s_note(light, 144 /*msg_type*/), color, END_MSG);
	sendMessage(msg);
}

void LaunchpadS::setPulse(unsigned char light, unsigned char color) {
	if (!isConnected()) return;
	// XXX Implement me!
}
