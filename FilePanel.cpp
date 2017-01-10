//
// Lightpad - FilePanel.cpp
// Created by Vinyl Darkscratch and Light Apacha, ©2017 Nightwave Studios.
//

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/file.h>
#include <wx/dir.h>
#include <wx/dataview.h>
#include <wx/imaglist.h>
#include <wx/artprov.h>

//#include <magic.h>

#include "FilePanel.h"

// Initialize the file panel and it's elements
FilePanel::FilePanel(wxPanel *parent)
	   : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;

	icon_list = new wxImageList();
	icon_list->Add(wxArtProvider::GetIcon(wxART_FOLDER, wxART_MENU));
	icon_list->Add(wxArtProvider::GetIcon(wxART_NORMAL_FILE, wxART_MENU));

	sizer = new wxBoxSizer(wxHORIZONTAL);
	filelistbox = new wxDataViewTreeCtrl(this, 5, wxPoint(-1, -1), wxSize(-1, -1), wxDV_SINGLE|wxDV_NO_HEADER);
	filelistbox->SetImageList(icon_list);
	parent_dvi = wxDataViewItem();
	RefreshFileList();
	sizer->Add(filelistbox, 1, wxEXPAND | wxALL, 0);

	Update();
}

// List all project directories
void FilePanel::RefreshFileList() {
	ListDirectory(wxString("/Users/vinyldarkscratch/Documents/Max 7/Library/MIDIext"), parent_dvi); // XXX Hard-coded full path to the folder, "~/Docume..." doesn't work.  // XXX Assuming that there's only one library folder, and that it's always in the user documents, which both are inaccurate -- should obtain paths from Max itself.
}

// Obtain all of the contents of a directory and add it to a specified file list
void FilePanel::ListDirectory(wxString path, wxDataViewItem files) {
	wxDir dir(path);
	wxString filename;
	//magic_t myt = magic_open(MAGIC_ERROR|MAGIC_MIME_TYPE);
	//magic_load(myt,NULL);

	if (!dir.IsOpened()) {
		// deal with the error here - wxDir would already log an error message
		// explaining the exact reason of the failure
		return;
	}

	// List files
	bool cont = dir.GetFirst(&filename, "", wxDIR_FILES);
	while (cont) {
		//std::string filetype(magic_file(myt, (path+"/"+filename).c_str()));
		//if (filetype == "audio/midi" || filetype == "text/plain") { // Only add if a MIDI or plain text file (animations and saves)
			filelistbox->AppendItem(files, filename, 1);
		//}
		cont = dir.GetNext(&filename);
	}

	// List directories
	cont = dir.GetFirst(&filename, "", wxDIR_DIRS);
	while (cont) {
		wxDataViewItem fl = filelistbox->AppendContainer(files, filename, 0);
		ListDirectory(path+"/"+filename, fl); // Obtain the contents of the directories by running the function in itself
		cont = dir.GetNext(&filename);
	}
}

void FilePanel::Update() {
	this->SetSizer(sizer);
	sizer->Layout();
}
