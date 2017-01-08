// 
// Lightpad - FilePanel.h
// Created by Vinyl Darkscratch and Light Apacha, ©2017 Nightwave Studios.
// 

#ifndef FILEPANEL_H
#define FILEPANEL_H

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <string>

class FileEntry; // Must be initialized before the vector, but the class references the vector, so it's contents and methods cannot be defined here

typedef std::vector<FileEntry *> FileList;

// File entry
class FileEntry {
	public:
		FileEntry();
		FileEntry(wxString n, wxString t);
		wxString name;
		wxString type;
		FileList *contents;
};

// Folder entry
class FolderEntry: public FileEntry {
	public:
		FolderEntry();
		FolderEntry(wxString n, wxString t, FileList *c);
};

// Graphical interface panel
class FilePanel: public wxPanel {
	public:
		FilePanel(wxPanel *parent);
		void RefreshFileList();
		void Update();
	private:
		void ListDirectory(wxString path, FileList &files);
		wxPanel *m_parent;
		wxBoxSizer *sizer;
		wxListBox *listbox;
		FileList fileList;
};

#endif