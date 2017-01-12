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
#include <wx/treebase.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #define WINDOWS
#elif defined(__APPLE__)
    #define MAC
#else
    #define LINUX
#endif

#ifndef WINDOWS
    #include <magic.h>
#endif

#include "FilePanel.h"

enum {
	ID_FilePanel_Tree = 5
};

wxDEFINE_EVENT(FILE_SELECT, wxCommandEvent);

// Initialize the file panel and it's elements
FilePanel::FilePanel(wxPanel *parent)
	   : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;

	icon_list = new wxImageList();
	icon_list->Add(wxArtProvider::GetIcon(wxART_FOLDER, wxART_MENU));
	icon_list->Add(wxArtProvider::GetIcon(wxART_FOLDER_OPEN, wxART_MENU));
	icon_list->Add(wxArtProvider::GetIcon(wxART_NORMAL_FILE, wxART_MENU));
	icon_list->Add(wxArtProvider::GetIcon(wxART_NORMAL_FILE, wxART_MENU));

	sizer = new wxBoxSizer(wxHORIZONTAL);
	filelistbox = new wxDataViewTreeCtrl(this, ID_FilePanel_Tree, wxPoint(-1, -1), wxSize(-1, -1), wxDV_SINGLE|wxDV_NO_HEADER);
	filelistbox->SetImageList(icon_list);
	parent_dvi = wxDataViewItem();
	RefreshFileList();
	sizer->Add(filelistbox, 1, wxEXPAND | wxALL, 0);

	Update();
}

// List all project directories
void FilePanel::RefreshFileList() {
	ListDirectory(maxpath, parent_dvi); // XXX Hard-coded full path to the folder, "~/Docume..." doesn't work.  // XXX Assuming that there's only one library folder, and that it's always in the user documents, which both are inaccurate -- should obtain paths from Max itself.
}

// Obtain all of the contents of a directory and add it to a specified file list
void FilePanel::ListDirectory(wxString path, wxDataViewItem files) {
	wxDir dir(path);
	wxString filename;
	#ifndef WINDOWS
        magic_t myt = magic_open(MAGIC_ERROR|MAGIC_MIME_TYPE);
        magic_load(myt,NULL);
	#endif

	if (!dir.IsOpened()) {
		// deal with the error here - wxDir would already log an error message
		// explaining the exact reason of the failure
		return;
	}

	// List files
	bool cont = dir.GetFirst(&filename, "", wxDIR_FILES);
	while (cont) {
        #ifndef WINDOWS
		std::string filetype(magic_file(myt, (path+"/"+filename).c_str()));
		if (filetype == "audio/midi" || filetype == "text/plain") { // Only add if a MIDI or plain text file (animations and saves)
        #endif
			int i = 2;
			#ifndef WINDOWS
                if (filetype == "text/plain") i = 3;
			#endif
			filelistbox->AppendItem(files, filename, i);
        #ifndef WINDOWS
		}
        #endif
		cont = dir.GetNext(&filename);
	}

	// List directories
	cont = dir.GetFirst(&filename, "", wxDIR_DIRS);
	while (cont) {
		wxDataViewItem cf = filelistbox->AppendContainer(files, filename, 0, 1);
		ListDirectory(path+"/"+filename, cf); // Obtain the contents of the directories by running the function in itself
		cont = dir.GetNext(&filename);
	}
}

void FilePanel::Update() {
	this->SetSizer(sizer);
	sizer->Layout();
}

void FilePanel::ChangeSelectedFile(wxDataViewEvent& event) {
	wxDataViewTreeStore *store = filelistbox->GetStore();
	wxDataViewItem item(event.GetItem());
	wxDataViewItem parent(store->GetParent(item));
	if (!store->IsContainer(item)) {
		wxCommandEvent evt(FILE_SELECT);
		evt.SetEventObject(this);
		evt.SetString(maxpath+"/"+filelistbox->GetItemText(parent)+"/"+filelistbox->GetItemText(item));
		ProcessEvent(evt);
	}
}

wxBEGIN_EVENT_TABLE(FilePanel, wxPanel)
	EVT_DATAVIEW_SELECTION_CHANGED(ID_FilePanel_Tree, FilePanel::ChangeSelectedFile)
wxEND_EVENT_TABLE()
