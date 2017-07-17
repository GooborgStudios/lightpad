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
#include <wx/debugrpt.h>
#include <wx/splash.h>
#include <wx/app.h>

#include "Magick++.h"
#include "RtMidi.h"

#include "ElementIDs.h"
#include "Helpers.h"
#include "Project.h"
#include "TestProject.h"
#include "FilePanel.h"
#include "DisplayPanel.h"
#include "TimelinePanel.h"
#include "PropertiesPanel.h"
#include "Launchpad.h"
#include "SplashScreen.h"

#define wxUSE_ON_FATAL_EXCEPTION 1
#define PADDING 0

Project *activeProject = new TestProject();

class MainFrame;

// Main app
class MainApp: public wxApp {
	public:
		virtual bool OnInit();
		virtual int OnExit();
		virtual void OnFatalException();
		MainFrame *frame;
};

// Main window frame
class MainFrame: public wxFrame {
	public:
		MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
		void ShowSplash();
		wxMenuBar *menuBar;
		wxMenu *menuFile;
		wxMenu *menuHelp;
		wxToolBar *toolbar;
		wxPanel *m_parent;
		FilePanel *m_fp;
		PropertiesPanel *m_pp;
		DisplayPanel *m_dp;
		TimelinePanel *m_tlp;
		wxBoxSizer *top_half;
		wxBoxSizer *sizer;
	private:
		void SaveAs();
	
		void OnHello(wxCommandEvent &event);
		void OnExit(wxCommandEvent &event);
		void OnAbout(wxCommandEvent &event);
		void OnSaveRequest(wxCommandEvent &event);
		void OnSaveAsRequest(wxCommandEvent &event);
		void OnSelectFile(wxCommandEvent &event);
		void OnStartStop(wxCommandEvent &event);
		void OnRestart(wxCommandEvent &event);

		wxDECLARE_EVENT_TABLE(); // Initialize event listener
};

wxIMPLEMENT_APP(MainApp); // Tell wxWidgets to commence our app

// Construct and display main window frame
bool MainApp::OnInit() {
	wxHandleFatalExceptions(true); // Enable error handler

	Magick::InitializeMagick(NULL);
	wxImage::AddHandler(new wxPNGHandler); // Enable PNG support(?)

	if (launchpad->connect() < 0) launchpad->disconnect();
	else launchpad->setPulse(LAUNCHPAD_PRO_SIDE_LED_ID, 53);

	SetAppName("Lightpad");
	SetAppDisplayName("Lightpad");
	SetClassName("co.nightwave.launchpad");
	SetVendorName("Nightwave Studios");
	SetVendorDisplayName("Nightwave Studios");

	frame = new MainFrame("Lightpad", wxPoint(50, 50), wxSize(1280, 720));
	frame->SetMinSize(wxSize(800, 600));
	frame->Show(true);
	return true;
}

int MainApp::OnExit() {
	launchpad->disconnect();

	return 0;
}

void MainApp::OnFatalException() {
	wxDebugReport report;
	wxDebugReportPreviewStd preview;
	report.AddAll();
	if (preview.Show(report)) report.Process();
}

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size): wxFrame(NULL, ID_Frame_Main, title, pos, size) {
	ShowSplash();

	wxBitmap lightpad_icon(getResourcePath("icons/icon_24.png"), wxBITMAP_TYPE_PNG);
	SetIcon(wxIcon(getResourcePath(APP_ICON)));

	// Initialize the menubar and attach keyboard shortcuts
	// wxWidgets automatically maps Ctrl to Cmd for us to enable cross-platform compatibility
	menuBar = new wxMenuBar;
	menuFile = new wxMenu;
	menuHelp = new wxMenu;
	menuFile->Append(ID_Menu_Hello, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
	menuFile->Append(ID_Menu_Save, "&Save\tCtrl-S", "Saves the file");
	menuFile->Append(ID_Menu_SaveAs, "&Save As...\tCtrl-Shift-S", "Saves the file to a specified location");
	menuFile->Append(ID_Menu_PlayPause, "&Play/Pause\tSpace", "Plays/pauses the animation");
	menuFile->Append(ID_Menu_Restart, "&Return to Start\tEnter", "Returns the playhead to the beginning of the timeline");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	menuHelp->Append(wxID_ABOUT);
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);

	// Bottom status bar
	CreateStatusBar();
	SetStatusText("Lightpad - Nightwave Studios");

	// Toolbar
	toolbar = CreateToolBar(wxTB_FLAT);
	toolbar->AddTool(ID_Menu_About, wxT("About"), lightpad_icon);
	toolbar->Realize();

	Centre(); // Center the window

	// Main window elements
	m_parent = new wxPanel(this, ID_Panel_Main);
	m_fp = new FilePanel(m_parent);
	m_pp = new PropertiesPanel(m_parent);
	m_dp = new DisplayPanel(m_parent);
	m_tlp = new TimelinePanel(m_parent);

	sizer = new wxBoxSizer(wxVERTICAL);
	top_half = new wxBoxSizer(wxHORIZONTAL);

	top_half->Add(m_fp, 1, wxEXPAND | wxLEFT, PADDING);
	top_half->Add(m_dp, 4, wxEXPAND | wxALL, PADDING);
	top_half->Add(m_pp, 0, wxEXPAND | wxRIGHT, PADDING);
	sizer->Add(top_half, 7, wxEXPAND | wxALL, PADDING);
	sizer->Add(m_tlp, 1, wxEXPAND | wxALL, PADDING);

	m_parent->SetSizer(sizer);

	Bind(FILE_SELECT, &MainFrame::OnSelectFile, this, ID_Frame_Main);
}

void MainFrame::OnExit(wxCommandEvent &event) {
	if (Close()) {
		delete m_fp;
		delete m_pp;
		delete m_dp;
		delete m_tlp;
		delete m_parent;
		delete toolbar;
	}
}

void MainFrame::ShowSplash() {
	std::string copyright = "© 2017 Nightwave Studios, GNU General Public License v3.0.  Programming by Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.  App icon and display panel graphic by Vinyl Darkscratch.  Splash screen by Vinyl Darkscratch, drawing by Yogfan14, with brushes by Alberto Seveso.  Big thanks to the Launchpad community for making this program possible.";
	wxBitmap splash_image(getResourcePath("splash.png"), wxBITMAP_TYPE_PNG);
	splash_image.UseAlpha();
	SplashScreen *splash = new SplashScreen(this, ID_Frame_Splash, splash_image, copyright, wxRect(760, 340, 1088, 478), wxFont(wxFontInfo(32).Family(wxFONTFAMILY_SWISS).FaceName("Helvetica Neue")), *wxWHITE);
	wxYield();
}

void MainFrame::OnAbout(wxCommandEvent &event) {
	ShowSplash();
}

void MainFrame::OnSelectFile(wxCommandEvent &event) {
	activeProject = new Project(event.GetString().ToStdString());
	m_tlp->movePlayhead(0);
}

void MainFrame::SaveAs() {
	wxFileDialog saveFileDialog(this, "Save file", "", "", "MIDI file (*.mid)|*.mid", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL) return;
	activeProject->save(saveFileDialog.GetPath().ToStdString());
}

void MainFrame::OnSaveRequest(wxCommandEvent &event) {
	if (activeProject->save() == -1) SaveAs();
}

void MainFrame::OnSaveAsRequest(wxCommandEvent &event) {
	SaveAs();
}

void MainFrame::OnHello(wxCommandEvent &event) {
	wxLogMessage("Testing the menu bars! -Vadu");
	m_tlp->Update();
}

void MainFrame::OnStartStop(wxCommandEvent &event) {
	m_tlp->nextBeat();
}

void MainFrame::OnRestart(wxCommandEvent &event) {
	m_tlp->movePlayhead(0);
}

// Initialize event listeners
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(ID_Menu_Hello, MainFrame::OnHello)
	EVT_MENU(ID_Menu_Save, MainFrame::OnSaveRequest)
	EVT_MENU(ID_Menu_SaveAs, MainFrame::OnSaveAsRequest)
	EVT_MENU(ID_Menu_About, MainFrame::OnAbout)
	EVT_MENU(ID_Menu_PlayPause, MainFrame::OnStartStop)
	EVT_MENU(ID_Menu_Restart, MainFrame::OnRestart)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_COMMAND(ID_Frame_Main, FILE_SELECT, MainFrame::OnSelectFile)
wxEND_EVENT_TABLE()
