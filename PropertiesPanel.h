//
// Lightpad - PropertiesPanel.h
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.nightwave.co/lightpad
//

#ifndef PROPERTIESPANEL_H
#define PROPERTIESPANEL_H

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/sizer.h>

#include "Colors.h"
#include "Helpers.h"

// Timeline panel
class PropertiesPanel: public wxPanel {
	public:
		PropertiesPanel(wxPanel *parent);
		void Update();
	private:
		wxPanel *m_parent;
		wxBoxSizer *sizer;
		wxGridSizer *gs;
};

#endif
