//
// Lightpad - DisplayPanel.cpp
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.nightwave.co/lightpad
//

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/sizer.h>

#include <sstream>
#include <algorithm>

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

#include "DisplayPanel.h"
#include "Colors.h"
#include "Helpers.h"

// Initialize the file panel and it's elements
DisplayPanel::DisplayPanel(wxPanel *parent)
	   : wxPanel(parent, ID_Panel_Display, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;

	// Load the graphics
	// base_image_path = "graphics/launchpad_display/launchpad_display_4096.png";
	base_image_path = "graphics/launchpad_display/base/base_4096.png";
	button_image_path = "graphics/launchpad_display/buttons/buttons_4096.png";
	launchpad_base_image = new wxImage(base_image_path, wxBITMAP_TYPE_PNG);
	launchpad_button_image = new wxImage(button_image_path, wxBITMAP_TYPE_PNG);

	// Initialize size and position variables
	image_size = MAXIMUM_LAUNCHPAD_IMAGE_SIZE;
	panel_width = -1;
	panel_height = -1;
	xpos = 0;
	ypos = 0;

	paintNow();
}

void DisplayPanel::paintEvent(wxPaintEvent &evt) {
	// depending on your system you may need to look at double-buffered dcs
	wxPaintDC dc(this);
	render(dc);
}

void DisplayPanel::paintNow() {
	// depending on your system you may need to look at double-buffered dcs
	wxClientDC dc(this);
	render(dc);
}

void DisplayPanel::OnSize(wxSizeEvent& event) {
	// Re-render when resizing
	Refresh();
	event.Skip();
}

void DisplayPanel::render(wxDC &dc) {
	int neww;
	int newh;
	std::stringstream ss;
	dc.GetSize(&neww, &newh);

	if( neww != panel_width || newh != panel_height ) {
		int new_size = std::min(neww, newh);
		if (new_size == 0) {
			new_size = image_size; // When first launching the app, this will make sure it doesn't crash
		}
		int new_image_size = closest_two_power(new_size, 256, 4096);

		// Load the other resolutions of the image as needed
		if (new_image_size != image_size) {
			launchpad_base_image->LoadFile(base_image_path, wxBITMAP_TYPE_PNG);
			// launchpad_button_image->LoadFile(button_image_path, wxBITMAP_TYPE_PNG);
			image_size = new_image_size;
		}
		resized = wxBitmap(launchpad_base_image->Scale(new_size, new_size));
		// resized = wxBitmap(launchpad_button_image->Scale(new_size, new_size));

		// Reposition as needed
		if ( neww > newh ) {
			xpos = (neww - new_size)/2;
			ypos = 0;
		} else {
			xpos = 0;
			ypos = (newh - new_size)/2;
		}

		panel_width = neww;
		panel_height = newh;
	}

	dc.DrawBitmap(resized, xpos, ypos, false);
}

wxBEGIN_EVENT_TABLE(DisplayPanel, wxPanel)
	EVT_PAINT(DisplayPanel::paintEvent)
	EVT_SIZE(DisplayPanel::OnSize)
wxEND_EVENT_TABLE()
