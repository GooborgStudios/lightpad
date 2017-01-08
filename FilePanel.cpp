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

#include <magic.h>

#include <iostream>
#include <string>
#include <vector>

#include "FilePanel.h"

FileEntry::FileEntry() {

}

FileEntry::FileEntry(wxString n, wxString t) {
	name = n;
	type = t;
}

FolderEntry::FolderEntry() {

}

FolderEntry::FolderEntry(wxString n, wxString t, FileList *c) {
	name = n;
	type = t;
	contents = c;
}

FilePanel::FilePanel(wxPanel *parent)
	   : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;

	RefreshFileList();

	sizer = new wxBoxSizer(wxHORIZONTAL);
	listbox = new wxListBox(this, 5, wxPoint(-1, -1), wxSize(-1, -1));
	for (FileList::iterator it = fileList.begin(); it != fileList.end(); ++it) {
		FileEntry *currentfile = fileList.at(it - fileList.begin());
		listbox->Append(currentfile->name);
		if (currentfile->type == "folder") {
			for (FileList::iterator iter = currentfile->contents->begin(); iter != currentfile->contents->end(); ++iter) {
				FileEntry *currentsubfile = currentfile->contents->at(iter - currentfile->contents->begin());
				listbox->Append(" » "+currentsubfile->name);
			}
		}
	}
	sizer->Add(listbox, 1, wxEXPAND | wxALL, 0);

	Update();
}

void FilePanel::RefreshFileList() {
	ListDirectory(wxString("/Users/vinyldarkscratch/Documents/Max 7/Library/MIDIext"), fileList);
}

void FilePanel::ListDirectory(wxString path, FileList &files) {
	wxDir dir(path);
	wxString filename;
	magic_t myt = magic_open(MAGIC_ERROR|MAGIC_MIME_TYPE);
	magic_load(myt,NULL);

	if (!dir.IsOpened()) {
		// deal with the error here - wxDir would already log an error message
		// explaining the exact reason of the failure
		return;
	}
	
	bool cont = dir.GetFirst(&filename, "", wxDIR_FILES);
	while (cont) {
		std::string filetype(magic_file(myt, (path+"/"+filename).c_str()));
		if (filetype == "audio/midi" || filetype == "text/plain") {
			files.push_back(new FileEntry(filename, "file"));
		}
		cont = dir.GetNext(&filename);
	}

	cont = dir.GetFirst(&filename, "", wxDIR_DIRS);
	while (cont) {
		FileList *fl = new FileList;
		ListDirectory(path+"/"+filename, *fl);
		files.push_back(new FolderEntry(filename, "folder", fl));
		cont = dir.GetNext(&filename);
	}
}

void FilePanel::Update() {
	this->SetSizer(sizer);
	sizer->Layout();
}
