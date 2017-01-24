//
// Lightpad - HelpersTest.h
// Created by Vinyl Darkscratch, Light Apacha, and WhoovesPON3, ©2017 Nightwave Studios.
// Additional assistance by Eric Busch (Origami1105).
// http://www.nightwave.co/lightpad
//

#include <cxxtest/TestSuite.h>

#include "Helpers.h"

// int main() {
// 	for (int x=28; x<=123; x++) {
// 		std::cout << note_to_button();
// 	}
// }

class HelpersTest : public CxxTest::TestSuite {
public:
	void testRoundTrip(void) {
		for (int n=28; n<=123; n++) {
			TS_ASSERT_EQUALS(button_to_note(note_to_button(n)), n);
		}
	}

	void testNTBTop(void) {
		TS_ASSERT_EQUALS(note_to_button(28), 91);
		TS_ASSERT_EQUALS(note_to_button(35), 98);
	}

	void testNTBLeft(void) {
		TS_ASSERT_EQUALS(note_to_button(108), 80);
		TS_ASSERT_EQUALS(note_to_button(115), 10);
	}

	void testNTBBottom(void) {
		TS_ASSERT_EQUALS(note_to_button(116), 1);
		TS_ASSERT_EQUALS(note_to_button(123), 8);
	}

	void testNTBRight(void) {
		TS_ASSERT_EQUALS(note_to_button(100), 89);
		TS_ASSERT_EQUALS(note_to_button(107), 19);
	}

	void testTwoPower(void) {
		TS_ASSERT_EQUALS(closest_two_power(51, 0, 128), 64);
		TS_ASSERT_EQUALS(closest_two_power(0, 16, 128), 16);
		TS_ASSERT_EQUALS(closest_two_power(280, 0, 128), 128);
	}
};