//
// Lightpad - TestProject.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "TestProject.h"

//#include <wx/wxprec.h>
//#ifndef WX_PRECOMP
//	#include <wx/wx.h>
//#endif

#include "Colors.h"
#include "Helpers.h"
#include "Launchpad.h"
#include "LightpadProject.h"

const int COLS = 32;
const int ROWS = 96;

TestProject::TestProject() : LightpadProject(120, 32) {
	Color rainbow[18];
	for (int j = 0; j < 18; j++) {
		int red = 0, grn = 0, blu = 0;
		if (j >= 15) red = 255 * ((18 - j) / 3.0);
		else red = 255 * ((j - 9) / 3.0);
		if (j >= 12) grn = 255 * ((15 - j) / 3.0);
		else grn = 255 * ((j - 3) / 3.0);
		if (j >= 6) blu = 255 * ((9 - j) / 3.0);
		else blu = 255 * ((j) / 3.0);
		rainbow[j] = Color(clamp(red, 0, 255), clamp(grn, 0, 255), clamp(blu, 0, 255));
	}
	
	int btn_x = 0;
	int btn_y = 9;
	
	for (int row = 0; row < ROWS; row++) {
		btn_x++;
		if (row == 8 || row == 88) btn_x++;
		if (btn_x / 10 > 0) {
			btn_x = btn_x % 10;
			btn_y--;
		}
		
		for (int col = 0; col < COLS; col++) {
			layer->AddKeyframe(new NoteKeyframe(btn_x + (btn_y * 10), col*ticksPerBeat, get_closest_velocity(rainbow[(btn_x + btn_y + col) % 18])));
		}
	}
}
