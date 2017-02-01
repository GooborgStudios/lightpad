//
// Lightpad - PropertiesPanel.cpp
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
#include <wx/tglbtn.h>

#include <string>

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

#include "PropertiesPanel.h"
#include "Colors.h"
#include "Helpers.h"

// Initialize the properties panel and it's elements
PropertiesPanel::PropertiesPanel(wxPanel *parent)
	   : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;

	sizer = new wxBoxSizer(wxVERTICAL);
	gs = new wxGridSizer(16, 8, 0, 0);
	// wxColorDialog colorpicker(this);

	for (int i = 0; i < COLORCOUNT; i++) {
		gs->Add(new wxToggleButton(this, wxID_ANY, std::to_string(i), wxPoint(-1, -1), wxSize(32, 32))); //velocitycolors[i];
	}

	sizer->Add(gs, 1, wxSHRINK);
	this->SetSizer(sizer);
	Update();
}

void PropertiesPanel::Update() {

}