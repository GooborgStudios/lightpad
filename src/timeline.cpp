//
// Lightpad - timeline.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <stdio.h>
#include <sys/time.h>
#include <string>
#include <algorithm>
#include <vector>
#include <map>

#include "ElementIDs.h"
#include "Helpers.h"
#include "Project.h"
#include "TestProject.h"
#include "TimelinePanel.h"

#define wxUSE_ON_FATAL_EXCEPTION 1
#define PADDING 0

Project *activeProject = new TestProject();

// Main app
class MainApp: public wxApp {
	public:
		virtual bool OnInit();
};

// Main frame
class MainFrame: public wxFrame {
	public:
		MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
		wxPanel *m_parent;
		TimelinePanel *m_tlp;
		wxToolBar *toolbar;
	
	private:
		void OnExit(wxCommandEvent &event);
		wxDECLARE_EVENT_TABLE();
};

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {
	MainFrame *frame = new MainFrame("Lightpad", wxPoint(50, 50), wxSize(800,600));
	frame->SetMinSize(wxSize(800,600));
	frame->Show(true);
 
	return true;
}

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
: wxFrame(NULL, wxID_ANY, title, pos, size) {
 
	SetIcon(wxIcon(getResourcePath(APP_ICON)));
 
	// Menubar
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(wxID_EXIT);
	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);
 
	// Statusbar
	CreateStatusBar();
	SetStatusText("Lightpad (timeline.cpp) - Nightwave Studios");
 
	// Toolbar
 
	wxImage::AddHandler(new wxPNGHandler);
	
	wxBitmap exit(getResourcePath("icons/exit.png"), wxBITMAP_TYPE_PNG);
 
	toolbar = CreateToolBar();
 
	toolbar->AddTool(wxID_EXIT, wxT("Exit Application"), exit);
	toolbar->Realize();
 
	Connect(wxID_EXIT, wxEVT_COMMAND_TOOL_CLICKED,
			wxCommandEventHandler(MainFrame::OnExit));
 
	Centre();
 
	// Main window elements
	m_parent = new wxPanel(this, wxID_ANY);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	m_tlp = new TimelinePanel(m_parent);
	sizer->Add(m_tlp, 1, wxEXPAND | wxALL, PADDING);
	m_parent->SetSizer(sizer);
}

void MainFrame::OnExit(wxCommandEvent &WXUNUSED(event)) {
	if (Close()) {
		delete m_tlp;
		delete m_parent;
		delete toolbar;
	}
}

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
wxEND_EVENT_TABLE()
