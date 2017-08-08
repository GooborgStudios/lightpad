//
// Lightpad - main.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
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
#include "LightpadProject.h"
#include "TestProject.h"
#include "FilePanel.h"
#include "DisplayPanel.h"
#include "TimelinePanel.h"
#include "PropertiesPanel.h"
#include "Launchpad.h"
#include "SplashScreen.h"

#define wxUSE_ON_FATAL_EXCEPTION 1
#define PADDING 0

LightpadProject *activeProject = new TestProject();

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
		void ShowSplash(bool loading = false);
		SplashScreen *splash;
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
		void OnNextBeat(wxCommandEvent &event);
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

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(NULL, ID_Frame_Main, title, pos, size) {
	ShowSplash(true);
	
	splash->SetProgress(0, "Initializing icon...");
	wxBitmap lightpad_icon(getResourcePath("icons/icon_24.png"), wxBITMAP_TYPE_PNG);
	SetIcon(wxIcon(getResourcePath(APP_ICON)));

	splash->SetProgress(1, "Initializing menu...");
	// Initialize the menubar and attach keyboard shortcuts
	// wxWidgets automatically maps Ctrl to Cmd for us to enable cross-platform compatibility
	menuBar = new wxMenuBar;
	menuFile = new wxMenu;
	menuHelp = new wxMenu;
	menuFile->Append(ID_Menu_Hello, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
	menuFile->Append(ID_Menu_Save, "&Save\tCtrl-S", "Saves the file");
	menuFile->Append(ID_Menu_SaveAs, "&Save As...\tCtrl-Shift-S", "Saves the file to a specified location");
	//menuFile->Append(ID_Menu_PlayPause, "&Play/Pause\tSpace", "Plays/pauses the animation");
	menuFile->Append(ID_Menu_PlayPause, "&Next Quarter Beat\tSpace", "Moves the playhead a quarter beat ahead");
	menuFile->Append(ID_Menu_ShiftPlayPause, "&Next Full Beat\tShift-Space", "Moves the playhead a full beat ahead");
	menuFile->Append(ID_Menu_Restart, "&Return to Start\tEnter", "Returns the playhead to the beginning of the timeline");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	menuHelp->Append(wxID_ABOUT);
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);

	splash->SetProgress(8, "Initializing status bar...");
	// Bottom status bar
	CreateStatusBar();
	SetStatusText("Lightpad - Nightwave Studios");

	splash->SetProgress(9, "Initializing toolbar...");
	// Toolbar
	toolbar = CreateToolBar(wxTB_FLAT);
	toolbar->AddTool(ID_Menu_About, wxT("About"), lightpad_icon);
	toolbar->Realize();

	Centre(); // Center the window

	splash->SetProgress(10, "Initializing panels...");
	// Main window elements
	m_parent = new wxPanel(this, ID_Panel_Main);
	splash->SetProgress(20, "Loading File Panel...");
	m_fp = new FilePanel(m_parent);
	splash->SetProgress(30, "Loading Properties Panel...");
	m_pp = new PropertiesPanel(m_parent);
	splash->SetProgress(40, "Loading Display Panel...");
	m_dp = new DisplayPanel(m_parent, splash);
	splash->SetProgress(90, "Loading Timeline Panel...");
	m_tlp = new TimelinePanel(m_parent);

	sizer = new wxBoxSizer(wxVERTICAL);
	top_half = new wxBoxSizer(wxHORIZONTAL);

	top_half->Add(m_fp, 1, wxEXPAND | wxLEFT, PADDING);
	top_half->Add(m_dp, 4, wxEXPAND | wxALL, PADDING);
	top_half->Add(m_pp, 0, wxEXPAND | wxRIGHT, PADDING);
	sizer->Add(top_half, 7, wxEXPAND | wxALL, PADDING);
	sizer->Add(m_tlp, 1, wxEXPAND | wxALL, PADDING);

	m_parent->SetSizer(sizer);
	
	splash->Close();

	Bind(FILE_SELECT, &MainFrame::OnSelectFile, this, ID_Frame_Main);
}

void MainFrame::OnExit(wxCommandEvent &WXUNUSED(event)) {
	if (Close()) {
		delete m_fp;
		delete m_pp;
		delete m_dp;
		delete m_tlp;
		delete m_parent;
		delete toolbar;
	}
}

void MainFrame::ShowSplash(bool loading) {
	std::string copyright = "© 2017 Nightwave Studios, GNU General Public License v3.0.  Programming by Vinyl Darkscratch and Light Apacha.  App icon and display panel graphic by Vinyl Darkscratch.  Splash screen by Vinyl Darkscratch, drawing by Yogfan14, with brushes by Alberto Seveso.  Big thanks to the Launchpad community for making this program possible.";
	wxBitmap splash_image(getResourcePath("splash.png"), wxBITMAP_TYPE_PNG);
	splash_image.UseAlpha();
	wxRect loadingbox = loading ? wxRect(760, 860, 1088, 0) : wxRect(0, 0, 0, 0);
	wxRect loadingbar = loading ? wxRect(810, 900, 988, 40) : wxRect(0, 0, 0, 0);
	splash = new SplashScreen(this, ID_Frame_Splash, splash_image, copyright, wxRect(760, 540, 1088, 0), *wxWHITE, wxFont(wxFontInfo(32).Family(wxFONTFAMILY_SWISS).FaceName("Helvetica Neue")), loadingbox, loadingbar);
	wxYield();
}

void MainFrame::OnAbout(wxCommandEvent &WXUNUSED(event)) {
	ShowSplash();
}

void MainFrame::OnSelectFile(wxCommandEvent &event) {
	activeProject = new LightpadProject(event.GetString().ToStdString());
	m_tlp->movePlayhead(0);
}

void MainFrame::SaveAs() {
	wxFileDialog saveFileDialog(this, "Save file", "", "", "MIDI file (*.mid)|*.mid", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL) return;
	activeProject->save(saveFileDialog.GetPath().ToStdString());
}

void MainFrame::OnSaveRequest(wxCommandEvent &WXUNUSED(event)) {
	if (activeProject->save() == -1) SaveAs();
}

void MainFrame::OnSaveAsRequest(wxCommandEvent &WXUNUSED(event)) {
	SaveAs();
}

void MainFrame::OnHello(wxCommandEvent &WXUNUSED(event)) {
	wxLogMessage("Testing the menu bars! -Vadu");
	m_tlp->Update();
}

void MainFrame::OnStartStop(wxCommandEvent &WXUNUSED(event)) {
	m_tlp->nextQuarterBeat();
}

void MainFrame::OnNextBeat(wxCommandEvent &WXUNUSED(event)) {
	m_tlp->nextBeat();
}

void MainFrame::OnRestart(wxCommandEvent &WXUNUSED(event)) {
	m_tlp->movePlayhead(0);
}

// Initialize event listeners
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(ID_Menu_Hello, MainFrame::OnHello)
	EVT_MENU(ID_Menu_Save, MainFrame::OnSaveRequest)
	EVT_MENU(ID_Menu_SaveAs, MainFrame::OnSaveAsRequest)
	EVT_MENU(ID_Menu_About, MainFrame::OnAbout)
	EVT_MENU(ID_Menu_PlayPause, MainFrame::OnStartStop)
	EVT_MENU(ID_Menu_ShiftPlayPause, MainFrame::OnNextBeat)
	EVT_MENU(ID_Menu_Restart, MainFrame::OnRestart)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_COMMAND(ID_Frame_Main, FILE_SELECT, MainFrame::OnSelectFile)
wxEND_EVENT_TABLE()
