//
// Lightpad - FilePanel.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/dataview.h>
#include <wx/imaglist.h>

#ifdef LIB_MAGIC
	#include <magic.h>
#endif

#include "ElementIDs.h"

#ifdef WINDOWS
	#include <windows.h>
	#include <shlobj.h>
	#pragma comment(lib, "shell32.lib")

	//extern char documents_path[MAX_PATH];
	//SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

	const wxString max_user_library_path = /*wxString(documents_path) +*/ wxString("\\MIDIext");
	const wxString max_shared_library_path = /*wxString(documents_path) +*/
	wxString("\\MIDIext"); // XXX Inaccurate
#else
	const wxString max_user_library_path = wxString(getenv("HOME")) + wxString("/Documents/Max 7/Library/MIDIext");
	const wxString max_shared_library_path = wxString("/Users/Shared/Max 7/Library/MIDIext");
#endif

// Graphical interface panel
class QuickFilePanel: public wxPanel {
	public:
		QuickFilePanel(wxPanel *parent);
		~QuickFilePanel();
		void RefreshFileList();
		void Update();
		wxString GetFilePath(wxDataViewItem item);
		void ChangeSelectedFile(wxDataViewEvent &event);
		//void RenameFile(wxDataViewEvent &event);
		
	private:
		void ListDirectory(wxString path, wxDataViewItem *files);
		
		wxPanel *m_parent;
		wxBoxSizer *sizer;
		wxDataViewTreeCtrl *filelistbox;
		wxDataViewItem *parent_dvi;
		wxImageList *icon_list;
		#ifdef LIB_MAGIC
			magic_t myt;
		#endif

	wxDECLARE_EVENT_TABLE();
};

wxDECLARE_EVENT(FILE_SELECT, wxCommandEvent);
