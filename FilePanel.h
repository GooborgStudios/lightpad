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

#include <wx/dataview.h>
#include <wx/imaglist.h>

const wxString maxpath = wxString("/Users/vinyldarkscratch/Documents/Max 7/Library/MIDIext");

// Graphical interface panel
class FilePanel: public wxPanel {
	public:
		FilePanel(wxPanel *parent);
		void RefreshFileList();
		void Update();
		void ChangeSelectedFile(wxDataViewEvent& event);
	private:
		void ListDirectory(wxString path, wxDataViewItem files);
		wxPanel *m_parent;
		wxBoxSizer *sizer;
		wxDataViewTreeCtrl *filelistbox;
		wxDataViewItem parent_dvi;
		wxImageList *icon_list;

		wxDECLARE_EVENT_TABLE();
};

wxDECLARE_EVENT(FILE_SELECT, wxCommandEvent);

#endif