//
// Lightpad - FilePanel.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "FilePanel.h"

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
#include <wx/validate.h>
#include <wx/file.h>

#ifdef LIB_MAGIC
	#include <magic.h>
#endif

#include "ElementIDs.h"
#include "Helpers.h"

wxDEFINE_EVENT(FILE_SELECT, wxCommandEvent);

// Initialize the file panel and it's elements
FilePanel::FilePanel(wxPanel *parent)
	: wxPanel(parent, ID_Panel_File, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;

	#ifdef LIB_MAGIC
	myt = magic_open(MAGIC_ERROR | MAGIC_MIME_TYPE);
	magic_load(myt, NULL);
	#endif

	// Initialize icons
	icon_list = new wxImageList();
	icon_list->Add(wxArtProvider::GetIcon(wxART_FOLDER, wxART_MENU));
	icon_list->Add(wxArtProvider::GetIcon(wxART_FOLDER_OPEN, wxART_MENU));
	icon_list->Add(wxArtProvider::GetIcon(wxART_NORMAL_FILE, wxART_MENU));
	icon_list->Add(wxArtProvider::GetIcon(wxART_NORMAL_FILE, wxART_MENU));

	sizer = new wxBoxSizer(wxHORIZONTAL);

	// Set up the file list
	filelistbox = new wxDataViewTreeCtrl(this, ID_FilePanel_Tree, wxPoint(-1, -1),
	                                     wxSize(-1, -1), wxDV_SINGLE | wxDV_NO_HEADER);
	filelistbox->SetImageList(icon_list);
	parent_dvi = new wxDataViewItem();
	RefreshFileList();

	sizer->Add(filelistbox, 1, wxEXPAND | wxALL, 0);
	this->SetSizer(sizer);
	Update();
}

FilePanel::~FilePanel() {
	delete filelistbox;
	delete parent_dvi;
	delete icon_list;
}

// List all project directories
void FilePanel::RefreshFileList() {
	ListDirectory(max_user_library_path, parent_dvi);
	ListDirectory(max_shared_library_path, parent_dvi);
	// XXX Should also obtain user search paths.
}

// Obtain all of the contents of a directory and add it to a specified file list
void FilePanel::ListDirectory(wxString path, wxDataViewItem *files) {
	wxDataViewItem *current_file;
	wxDir dir(path);
	wxString filename;
	std::string filetype;
	bool cont;
	int icon_type;

	if (!dir.IsOpened()) {
		// deal with the error here - wxDir would already log an error message
		// explaining the exact reason of the failure
		return;
	}

	// List files
	cont = dir.GetFirst(&filename, "", wxDIR_FILES);
	while (cont) {
		#ifdef LIB_MAGIC
		filetype = magic_file(myt, (path + "/" + filename).c_str());
		#else
		filetype = "audio/midi"; // XXX Use file extension instead
		#endif
		if (filetype == "audio/midi") { // Only add if a MIDI file
			icon_type = 2;
			if (filetype == "text/plain") icon_type = 3;
			filelistbox->AppendItem(*files, filename, icon_type);
		}
		cont = dir.GetNext(&filename);
	}

	// List directories
	cont = dir.GetFirst(&filename, "", wxDIR_DIRS);
	while (cont) {
		current_file = new wxDataViewItem(filelistbox->AppendContainer(*files, filename, 0, 1));
		ListDirectory(path + "/" + filename, current_file);
		// Obtain the contents of the directories by running the function in itself
		cont = dir.GetNext(&filename);
		delete current_file;
	}
}

void FilePanel::Update() {
	this->SetSizer(sizer);
	sizer->Layout();
}

wxString FilePanel::GetFilePath(wxDataViewItem item) {
	wxDataViewTreeStore *store = filelistbox->GetStore();
	wxString path;

	for (int i = 0; i <= 1; i++) { // XXX Improve me!
		if (i == 0) path = max_user_library_path + "/";
		else path = max_shared_library_path + "/";

		if (!store->IsContainer(item))
			path += filelistbox->GetItemText(store->GetParent(item)) + "/";
		path += filelistbox->GetItemText(item);
		if (wxFile::Exists(path))
			break;
	}
	delete store;
	return path;
}

void FilePanel::ChangeSelectedFile(wxDataViewEvent &event) {
	wxCommandEvent evt(FILE_SELECT);
	evt.SetEventObject(this);
	evt.SetString(GetFilePath(event.GetItem()));
	if (!filelistbox->GetStore()->IsContainer(event.GetItem())) ProcessEvent(evt);
}

// void FilePanel::RenameFile(wxDataViewEvent &event) {
// 	std::cout << filelistbox->GetItemText(event.GetItem());
//  std::cout << " > " << event.GetValue().GetType() << std::endl;
// }

wxBEGIN_EVENT_TABLE(FilePanel, wxPanel)
	EVT_DATAVIEW_SELECTION_CHANGED(ID_FilePanel_Tree, FilePanel::ChangeSelectedFile)
	//EVT_DATAVIEW_ITEM_VALUE_CHANGED(ID_FilePanel_Tree, FilePanel::RenameFile)
wxEND_EVENT_TABLE()
