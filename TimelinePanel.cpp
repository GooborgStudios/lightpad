//
// Lightpad - TimelinePanel.cpp
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.nightwave.co/lightpad
//

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/grid.h>

#ifdef _WIN32
	//define something for Windows (32-bit and 64-bit, this part is common)
	#define WINDOWS
	#ifdef _WIN64
		//define something for Windows (64-bit only)
		#define WINDOWS_64
	#endif
#elif __APPLE__
	//define something for Mac
	#define MACOS
#else
	//#error "Unknown/unsupported compiler/operating system"
#endif

#include "TimelinePanel.h"
#include "Colors.h"
#include "Helpers.h"

// Initialize the timeline panel and it's elements
const int COLS = 16;
const int ROWS = 16;

TimelinePanel::TimelinePanel(wxPanel *parent)
	   : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;

	sizer = new wxBoxSizer(wxHORIZONTAL);

	Update();
}

void TimelinePanel::Update() {
	char buf[8];

	this->DestroyChildren();

	// Create a wxGrid object
	grid = new wxGrid( this,
						-1,
						wxPoint( 0, 0 ),
						wxSize( -1, -1 ));
	// Then we call CreateGrid to set the dimensions of the grid
	// (100 rows and 10 columns in this example)

	grid->CreateGrid( ROWS, COLS );
	// We can set the sizes of individual rows and columns
	// in pixels
	grid->SetDefaultColSize(150, true);
	for (int c = 0; c < COLS; c++) {
		for (int r = 0; r < ROWS; r++) {
			
		}
	}

	// for (int c = 0; c < COLS; c++) {
	// 	snprintf(buf, 7, "%d.%d", c/4+1, c%4+1);
	// 	grid->SetColLabelValue(c, buf);
	// 	for (int r = 0; r < activeProject.layers->size(); r++) {
	// 		grid->SetCellValue(r,c,activeProject.layers->at(r)->description); // XXX This is assuming every layer is a TextElement!
	// 		grid->SetReadOnly(r,c);
	// 	}
	// }

	// std::string ln = "Layer";
	// for (int i=0; i<activeProject.layers->size(); i++) {
	// 	grid->SetRowLabelValue(i, ln/*activeProject.layers->at(i)->name*/);
	// }

	// //grid->SetRowSize( 0, 150 );
	// //grid->SetColSize( 0, 120 );
	// // And set grid cell contents as strings
	// grid->SetCellValue( 0, 0, "wxGrid is good" );
	// // We can specify that some cells are read->only
	// grid->SetCellValue( 0, 3, "This is read->only" );
	// grid->SetReadOnly( 0, 3 );
	// // Colours can be specified for grid cell contents
	// grid->SetCellValue(3, 3, "green on grey");
	// grid->SetCellTextColour(3, 3, *wxGREEN);
	// grid->SetCellBackgroundColour(3, 3, *wxLIGHT_GREY);
	// // We can specify the some cells will store numeric
	// // values rather than strings. Here we set grid column 5
	// // to hold floating point values displayed with width of 6
	// // and precision of 2
	// grid->SetColFormatFloat(5, 6, 2);
	// grid->SetCellValue(0, 6, "3.1415");

	sizer->Add(grid, 1, wxEXPAND);
	this->SetSizer(sizer);
	sizer->Layout();
}