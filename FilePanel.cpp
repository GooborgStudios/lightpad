//
// Lightpad - FilePanel.cpp
// Created by Vinyl Darkscratch, Light Apacha, and WhoovesPON3, ©2017 Nightwave Studios.
// Additional assistance by Eric Busch (Origami1105).
// http://www.nightwave.co/lightpad
//

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#ifdef _WIN32
	//define something for Windows (32-bit and 64-bit, this part is common)
	#define WINDOWS
	#ifdef _WIN64
		//define something for Windows (64-bit only)
		#define WINDOWS_64
	#endif
#elif __APPLE__
	//define something for Mac
	#define MACOS
#else
	//#error "Unknown/unsupported compiler/operating system"
#endif

#include <wx/file.h>
#include <wx/dir.h>
#include <wx/dataview.h>
#include <wx/imaglist.h>
#include <wx/artprov.h>
#include <wx/treebase.h>
#include <wx/validate.h>
#include <wx/file.h>

#ifdef MACOS
	#include <magic.h>
#endif

#include "FilePanel.h"
#include "Helpers.h"

wxDEFINE_EVENT(FILE_SELECT, wxCommandEvent);

// Initialize the file panel and it's elements
FilePanel::FilePanel(wxPanel *parent)
	   : wxPanel(parent, ID_Panel_File, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
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
	ListDirectory(max_user_library_path, parent_dvi);
	ListDirectory(max_shared_library_path, parent_dvi);
	// XXX Should also obtain user search paths.
}

// Obtain all of the contents of a directory and add it to a specified file list
void FilePanel::ListDirectory(wxString path, wxDataViewItem files) {
	wxDir dir(path);
	wxString filename;
	#ifdef MACOS
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
		#ifdef MACOS
			std::string filetype(magic_file(myt, (path+"/"+filename).c_str()));
		#else
			std::string filetype("audio/midi");
		#endif
		if (filetype == "audio/midi" /*|| filetype == "text/plain"*/) { // Only add if a MIDI or plain text file (animations and saves)
			int i = 2;
			if (filetype == "text/plain") i = 3;
			filelistbox->AppendItem(files, filename, i);
		}
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

wxString FilePanel::GetFilePath(wxDataViewItem item) {
	wxDataViewTreeStore *store = filelistbox->GetStore();
	wxDataViewItem parent(store->GetParent(item));
	wxString path;

	for (int i = 0; i <= 1; i++) {
		if (i == 0)
			path = max_user_library_path+"/";
		else
			path = max_shared_library_path+"/";

		if (!store->IsContainer(item))
			path += filelistbox->GetItemText(parent)+"/";
		path += filelistbox->GetItemText(item);
		if (wxFile::Exists(path))
			break;
	}
	return path;
}

void FilePanel::ChangeSelectedFile(wxDataViewEvent &event) {
	wxCommandEvent evt(FILE_SELECT);
	evt.SetEventObject(this);
	evt.SetString(GetFilePath(event.GetItem()));
	if (!filelistbox->GetStore()->IsContainer(event.GetItem())) ProcessEvent(evt);
}

// void FilePanel::RenameFile(wxDataViewEvent &event) {
// 	std::cout << filelistbox->GetItemText(event.GetItem()) << " > " << event.GetValue().GetType() << std::endl;
// }

wxBEGIN_EVENT_TABLE(FilePanel, wxPanel)
	EVT_DATAVIEW_SELECTION_CHANGED(ID_FilePanel_Tree, FilePanel::ChangeSelectedFile)
	//EVT_DATAVIEW_ITEM_VALUE_CHANGED(ID_FilePanel_Tree, FilePanel::RenameFile)
wxEND_EVENT_TABLE()
