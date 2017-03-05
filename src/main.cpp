//
// Lightpad - main.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/gdicmn.h>
#include <wx/artprov.h>

#include "Magick++.h"
#include "RtMidi.h"

#include "Helpers.h"
#include "FilePanel.h"
#include "DisplayPanel.h"
#include "TimelinePanel.h"
#include "PropertiesPanel.h"

const int PADDING = 0;

// Main app
class MainApp: public wxApp {
	public:
		virtual bool OnInit();
};

// Main window frame
class MainFrame: public wxFrame {
	public:
		MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
		wxPanel *m_parent;
		FilePanel *m_fp;
		PropertiesPanel *m_pp;
		DisplayPanel *m_dp;
		TimelinePanel *m_tlp;

	private:
		void OnHello(wxCommandEvent& event);
		void OnExit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		void OnSaveRequest(wxCommandEvent& event);
		void OnSelectFile(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE(); // Initialize event listener
};

wxIMPLEMENT_APP(MainApp); // Tell wxWidgets to commence our app

// Construct and display main window frame
bool MainApp::OnInit() {
	Magick::InitializeMagick(NULL);

	if (launchpad->connect() < 0) launchpad->disconnect();
	else launchpad->setPulse(LAUNCHPAD_PRO_SIDE_LED_ID, 53);

	MainFrame *frame = new MainFrame("Lightpad", wxPoint(50, 50), wxSize(800,600));
	frame->SetMinSize(wxSize(800,600));
	frame->Show(true);
	return true;
}

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
		: wxFrame(NULL, ID_Frame_Main, title, pos, size) {

	wxImage::AddHandler(new wxPNGHandler); // Enable PNG support(?)

	#ifdef WINDOWS
        SetIcon(wxIcon(wxT("graphics/icons/icon.ico")));
    #else
        #ifdef MACOS
            SetIcon(wxIcon(wxT("graphics/icons/icon.icns")));
        #else
            SetIcon(wxIcon(wxT("graphics/icons/icon_64.png")));
        #endif
	#endif

	// Initialize the menubar and attach keyboard shortcuts
	// wxWidgets automatically maps Ctrl to Cmd for us to enable cross-platform compatibility
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

	// Bottom status bar
	CreateStatusBar();
	SetStatusText("Lightpad - Nightwave Studios");

	// Toolbar
	wxBitmap lightpad_icon(wxT("graphics/icons/icon_24.png"), wxBITMAP_TYPE_PNG);

	wxToolBar *toolbar = CreateToolBar(wxTB_FLAT);

	toolbar->AddTool(ID_Menu_About, wxT("About"), lightpad_icon);
	toolbar->Realize();

	Centre(); // Center the window

	// Main window elements
	m_parent = new wxPanel(this, ID_Panel_Main);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

	m_fp = new FilePanel(m_parent);
	m_pp = new PropertiesPanel(m_parent);
	m_dp = new DisplayPanel(m_parent);

	hbox->Add(m_fp, 1, wxEXPAND | wxLEFT, PADDING);
	hbox->Add(m_dp, 4, wxEXPAND | wxALL, PADDING);
	hbox->Add(m_pp, 0, wxEXPAND | wxRIGHT, PADDING);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

	m_tlp = new TimelinePanel(m_parent);

	vbox->Add(hbox, 3, wxEXPAND | wxALL, PADDING);
	vbox->Add(m_tlp, 1, wxEXPAND | wxALL, PADDING);

	m_parent->SetSizer(vbox);

	// wxBitmap TempBitmap;
	// TempBitmap.LoadFile("background.png",wxBITMAP_TYPE_PNG);
	// wxBackgroundBitmap * Background = new wxBackgroundBitmap(TempBitmap);
	// m_parent->PushEventHandler(Background);

	Bind(FILE_SELECT, &MainFrame::OnSelectFile, this, ID_Frame_Main);
}

void MainFrame::OnExit(wxCommandEvent& event) {
	launchpad->disconnect();
	Close(true); // Make sure that we safely quit the program
}

void MainFrame::OnAbout(wxCommandEvent& event) {
	wxMessageBox("Lightpad Copyright 2017 Nightwave Studios, all rights reserved.  The application is coded by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3.  Some features based upon those found in LightshowCreator by LaunchpadFun with exclusive permission.  Big thanks to the support from the Launchpad Lightshow Community.",
				"About Lightpad", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnSelectFile(wxCommandEvent& event) {
	wxMessageBox(event.GetString(), "File Selected", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnSaveRequest(wxCommandEvent& event) {
	//activeProject.save("./foo.lightsave");
}

void MainFrame::OnHello(wxCommandEvent& event) {
	wxLogMessage("Testing the menu bars! -Vadu");
	m_tlp->Update();
}

// Initialize event listeners
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(ID_Menu_Hello,   MainFrame::OnHello)
	EVT_MENU(ID_Menu_Save,   MainFrame::OnSaveRequest)
	EVT_MENU(ID_Menu_About, MainFrame::OnAbout)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
	EVT_MENU(wxID_EXIT,  MainFrame::OnExit)
	EVT_COMMAND(ID_Frame_Main, FILE_SELECT, MainFrame::OnSelectFile)
wxEND_EVENT_TABLE()
