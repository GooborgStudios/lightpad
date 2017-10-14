//
// Lightpad - FileMetadata.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "FileMetadata.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "ElementIDs.h"
#include "NightwaveCore/Helpers.h"
#include "HOWL/Project.h"

FileMetadata::FileMetadata(const wxString & title, HOWL::Project *project) : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230)) {
	wxPanel *panel = new wxPanel(this, -1);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	
	char buf[16];
	snprintf(buf, 16, "%d", project->BPM);
	wxTextCtrl *bpm = new wxTextCtrl(panel, -1, wxString(buf), wxPoint(15, 15));
	//wxTextCtrl *ticksPerBeat = new wxTextCtrl(panel, -1, wxString(project->ticksPerBeat), wxPoint(15, 45));
	
	wxButton *okButton = new wxButton(this, -1, wxT("Ok"), wxDefaultPosition, wxSize(70, 30));
	wxButton *cancelButton = new wxButton(this, -1, wxT("Cancel"), wxDefaultPosition, wxSize(70, 30));
	
	hbox->Add(okButton, 1);
	hbox->Add(cancelButton, 1, wxLEFT, 5);
	
	vbox->Add(panel, 1);
	vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
	
	SetSizer(vbox);
	
	Centre();
	ShowModal();
	
	project->BPM = std::atoi(bpm->GetLineText(0));
	
	Destroy();
}
