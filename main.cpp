// 
// Lightpad - main.cpp
// Created by Vinyl Darkscratch and Light Apacha, ©2017 Nightwave Studios.
// 

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/gdicmn.h>

#include "FilePanel.h"

const int PADDING = 0;

// Element IDs
enum {
	ID_About,
	ID_Menu_Hello,
	ID_Menu_Save
};

// Main app
class MainApp: public wxApp {
	public:
		virtual bool OnInit();
};

class PropertiesPanel: public wxPanel {
	public:
		PropertiesPanel(wxPanel *parent);
	private:
		wxPanel *m_parent;
};

class TimelinePanel: public wxPanel {
	public:
		TimelinePanel(wxPanel *parent);
	private:
		wxPanel *m_parent;
};

class DisplayPanel: public wxPanel {
	public:
		DisplayPanel(wxPanel *parent);
	private:
		wxPanel *m_parent;
};

PropertiesPanel::PropertiesPanel(wxPanel *parent)
	   : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;
}

TimelinePanel::TimelinePanel(wxPanel *parent)
	   : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;
}

DisplayPanel::DisplayPanel(wxPanel *parent)
	   : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;
}

class TopPanel: public wxPanel {
	public:
		TopPanel(wxPanel *parent);
		FilePanel *m_fp;
		PropertiesPanel *m_pp;
		DisplayPanel *m_dp;
	private:
		wxPanel *m_parent;
};

TopPanel::TopPanel(wxPanel *parent)
	   : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;

	// Top panel elements
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

	m_fp = new FilePanel(this);
	m_pp = new PropertiesPanel(this);
	m_dp = new DisplayPanel(this);

	hbox->Add(m_fp, 1, wxEXPAND | wxALL, PADDING);
	hbox->Add(m_dp, 3, wxEXPAND | wxALL, PADDING);
	hbox->Add(m_pp, 1, wxEXPAND | wxALL, PADDING);

	this->SetSizer(hbox);
}

// Main frame
class MainFrame: public wxFrame {
	public:
		MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
		wxPanel *m_parent;
		TopPanel *m_tp;
		TimelinePanel *m_tlp;

	private:
		void OnHello(wxCommandEvent& event);
		void OnExit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		void OnSaveRequest(wxCommandEvent& event);
		wxDECLARE_EVENT_TABLE();
};

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {
	MainFrame *frame = new MainFrame("Lightpad", wxPoint(50, 50), wxSize(800,600));
	frame->SetMinSize(wxSize(800,600));
	frame->Show(true);
	return true;
}

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
		: wxFrame(NULL, wxID_ANY, title, pos, size) {

	//SetIcon(wxIcon(wxT("icons/lightpad.ico")));

	// Menubar
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_Menu_Hello, "&Hello...\tCtrl-H",
					 "Help string shown in status bar for this menu item");
	menuFile->Append(ID_Menu_Save, "&Save\tCtrl-S",
					 "Saves the file");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);

	// Statusbar
	CreateStatusBar();
	SetStatusText("Lightpad - Nightwave Studios");

	// Toolbar

	wxImage::AddHandler(new wxPNGHandler);
  
	wxBitmap exit(wxT("icons/exit.png"), wxBITMAP_TYPE_PNG);
	wxBitmap lightpad(wxT("icons/lightpad.png"), wxBITMAP_TYPE_PNG);
	wxBitmap add(wxT("icons/add.png"), wxBITMAP_TYPE_PNG);

	wxToolBar *toolbar = CreateToolBar();

	toolbar->AddTool(ID_About, wxT("About"), lightpad);
	toolbar->AddTool(wxID_EXIT, wxT("Exit Application"), exit);
	toolbar->Realize();

	Connect(wxID_EXIT, wxEVT_COMMAND_TOOL_CLICKED, 
		wxCommandEventHandler(MainFrame::OnExit));
	Connect(ID_About, wxEVT_COMMAND_TOOL_CLICKED, 
		wxCommandEventHandler(MainFrame::OnAbout));
	Connect(ID_Menu_Save, wxEVT_COMMAND_TOOL_CLICKED, 
		wxCommandEventHandler(MainFrame::OnSaveRequest));

	Centre();

	// Main window elements
	m_parent = new wxPanel(this, wxID_ANY);
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

	m_tp = new TopPanel(m_parent);
	m_tlp = new TimelinePanel(m_parent);

	vbox->Add(m_tp, 3, wxEXPAND | wxALL, PADDING);
	vbox->Add(m_tlp, 1, wxEXPAND | wxALL, PADDING);

	m_parent->SetSizer(vbox);

	// wxBitmap TempBitmap;
	// TempBitmap.LoadFile("background.png",wxBITMAP_TYPE_PNG);
	// wxBackgroundBitmap * Background = new wxBackgroundBitmap(TempBitmap);
	// m_parent->PushEventHandler(Background);
}

void MainFrame::OnExit(wxCommandEvent& event) {
	Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event) {
	wxMessageBox("Lightpad Copyright 2017 Nightwave Studios, all rights reserved.  The application is coded by Vinyl Darkscratch and Light Apacha.  Big thanks to the support from the Launchpad Lightshow Community.",
				"About Lightpad", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnSaveRequest(wxCommandEvent& event) {
	//activeProject.save("./foo.lightsave");
}

void MainFrame::OnHello(wxCommandEvent& event) {
	wxLogMessage("Testing the menu bars! -Vadu");
	m_tlp->Update();
}

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(ID_Menu_Hello,   MainFrame::OnHello)
	EVT_MENU(ID_Menu_Save,   MainFrame::OnSaveRequest)
	EVT_MENU(wxID_EXIT,  MainFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()
