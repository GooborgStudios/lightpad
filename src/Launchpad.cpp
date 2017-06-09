//
// Lightpad - Launchpad.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "Launchpad.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <cstdarg>
#include <ctime>
#include <regex>

#include "RtMidi.h"

#include "Colors.h"

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

Color velocitycolors[] = {
	Color(0, 0, 0),       Color(28, 28, 28),    Color(124, 124, 124), Color(252, 252, 252),
	Color(255, 128, 128), Color(255, 0, 0),     Color(178, 0, 0),     Color(89, 0, 0),
	Color(255, 170, 128), Color(255, 85, 0),    Color(178, 59, 0),    Color(89, 29, 0),
	Color(255, 255, 128), Color(255, 255, 0),   Color(178, 178, 0),   Color(89, 89, 0),
	Color(160, 255, 128), Color(64, 255, 0),    Color(45, 178, 0),    Color(22, 89, 0),
	Color(149, 255, 128), Color(43, 255, 0),    Color(30, 178, 0),    Color(15, 89, 0),
	Color(128, 255, 128), Color(0, 255, 0),     Color(0, 178, 0),     Color(0, 89, 0),
	Color(128, 255, 160), Color(0, 255, 64),    Color(0, 178, 45),    Color(0, 89, 22),
	Color(128, 255, 202), Color(0, 255, 149),   Color(0, 178, 104),   Color(0, 89, 52),
	Color(128, 213, 255), Color(0, 170, 255),   Color(0, 119, 178),   Color(0, 59, 89),
	Color(128, 171, 255), Color(0, 86, 255),    Color(0, 60, 178),    Color(0, 30, 89),
	Color(128, 128, 255), Color(0, 0, 255),     Color(0, 0, 178),     Color(0, 0, 89),
	Color(170, 128, 255), Color(85, 0, 255),    Color(59, 0, 178),    Color(29, 0, 89),
	Color(255, 128, 255), Color(255, 0, 255),   Color(178, 0, 178),   Color(89, 0, 89),
	Color(255, 128, 170), Color(255, 0, 85),    Color(178, 0, 59),    Color(89, 0, 29),
	Color(255, 25, 1),    Color(154, 53, 0),    Color(122, 81, 1),    Color(58, 102, 0),
	Color(1, 57, 0),      Color(0, 84, 50),     Color(0, 83, 127),    Color(0, 0, 254),
	Color(0, 68, 77),     Color(26, 0, 209),    Color(124, 124, 124), Color(32, 32, 32),
	Color(251, 12, 2),    Color(186, 253, 0),   Color(172, 236, 0),   Color(86, 253, 0),
	Color(0, 136, 0),     Color(1, 252, 123),   Color(0, 167, 255),   Color(2, 26, 255),
	Color(53, 0, 255),    Color(120, 0, 255),   Color(180, 23, 126),  Color(65, 32, 0),
	Color(255, 74, 1),    Color(130, 225, 0),   Color(102, 253, 0),   Color(0, 254, 0),
	Color(0, 254, 0),     Color(69, 253, 97),   Color(1, 251, 203),   Color(80, 134, 255),
	Color(39, 77, 200),   Color(132, 122, 237), Color(211, 12, 255),  Color(255, 6, 90),
	Color(255, 125, 1),   Color(184, 177, 0),   Color(138, 253, 0),   Color(129, 93, 0),
	Color(58, 40, 2),     Color(13, 76, 5),     Color(0, 80, 55),     Color(19, 20, 41),
	Color(16, 31, 90),    Color(106, 60, 24),   Color(172, 4, 1),     Color(225, 81, 54),
	Color(220, 105, 0),   Color(254, 225, 0),   Color(153, 225, 1),   Color(96, 181, 0),
	Color(27, 28, 49),    Color(220, 253, 84),  Color(118, 251, 185), Color(150, 152, 255),
	Color(139, 98, 255),  Color(64, 64, 64),    Color(116, 116, 116), Color(222, 252, 252),
	Color(159, 6, 1),     Color(52, 1, 0),      Color(0, 210, 1),     Color(0, 65, 1),
	Color(184, 177, 0),   Color(60, 48, 0),     Color(180, 93, 0),    Color(74, 21, 4)
};

unsigned char get_closest_velocity(wxColor color) {
	/* Take a wxColor and find the closest match to a Launchpad Pro available color */
	int best_match_index = 0;
	int best_match_value = 1024;
	for (int i = 0; i < COLORCOUNT; i++) {
		int comparison = ColorConverter::LAB_compare_RGB(velocitycolors[i], color);
		if (comparison < best_match_value) {
			best_match_index = i;
			best_match_value = comparison;
		}
	}
	
	return best_match_index;
};

char get_color_velocity(wxColor color) {
	/* Take a wxColor and find the matching velocity of a Launchpad Pro available color */
	for (int i = 0; i < COLORCOUNT; i++) {
		if (color == velocitycolors[i]) return i;
	}
	
	return -1;
};

LaunchpadPro *launchpad = new LaunchpadPro();
