//
// Lightpad - PropertiesPanel.h
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/sizer.h>
#include <wx/grid.h>
#include <wx/clrpicker.h>

#include "Colors.h"
#include "Helpers.h"
#include "GridRenderer.h"

// Properties panel
class PropertiesPanel: public wxPanel {
	public:
		PropertiesPanel(wxPanel *parent);
		void Update();
		void OnSelectCell(wxGridEvent& event);
		void SelectColor(wxColourPickerEvent& event);
	private:
		wxPanel *m_parent;
		wxBoxSizer *sizer;
		wxGrid *grid;

		wxDECLARE_EVENT_TABLE();
};
