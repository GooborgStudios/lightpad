//
// Lightpad - FilePanel.h
// Created by Vinyl Darkscratch, Light Apacha, Eric Busch (Origami1105), and WhoovesPON3, ©2017 Nightwave Studios.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.nightwave.co/lightpad
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

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <windows.h>
	#include <shlobj.h>
	#pragma comment(lib, "shell32.lib")

	char my_documents[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

	const wxString max_user_library_path = wxString(my_documents) + wxString("\\MIDIext");
	const wxString max_shared_library_path = wxString(my_documents) + wxString("\\MIDIext"); // XXX Inaccurate
#else
	const wxString max_user_library_path = wxString(getenv("HOME")) + wxString("/Documents/Max 7/Library/MIDIext");
	const wxString max_shared_library_path = wxString("/Users/Shared/Max 7/Library/MIDIext");
#endif

// Graphical interface panel
class FilePanel: public wxPanel {
	public:
		FilePanel(wxPanel *parent);
		void RefreshFileList();
		void Update();
		wxString GetFilePath(wxDataViewItem item);
		void ChangeSelectedFile(wxDataViewEvent& event);
		//void RenameFile(wxDataViewEvent &event);
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
