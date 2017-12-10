//
// Lightpad - QuickFilePanel.h
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

#include <vector>
#include <map>

#include "ElementIDs.h"

/*
 * String vector of search paths
	 * Add and remove files/folders (to search paths)
	 * Drag-and-drop files/folders (add to search paths)
 * Dynamic refresh
 * Sticky search paths (retained after program quits)
 */

// Quick file access panel
class QuickFilePanel: public wxPanel {
	public:
		QuickFilePanel(wxPanel *parent);
		~QuickFilePanel();
		void RefreshFileList();
		void Update();
		void AddPath(std::string path);
		void BlacklistFileType(std::string type);
		void SetFileTypeIcon(std::string type, wxIcon icon);
		void SetFileTypeIcon(std::string type, std::string icon_path);
		wxString GetFilePath(wxDataViewItem item);
		void ChangeSelectedFile(wxDataViewEvent &event);
		//void RenameFile(wxDataViewEvent &event);
		
	private:
		void ListDirectory(wxString path, wxDataViewItem *files);
		
		wxPanel *m_parent;
		wxBoxSizer *sizer;
		wxDataViewTreeCtrl *filelistbox;
		std::vector<std::string> search_paths;
		wxImageList *icon_list;
		std::map<std::string, int> file_types;
		#ifdef LIB_MAGIC
			magic_t myt;
		#endif

	wxDECLARE_EVENT_TABLE();
};

wxDECLARE_EVENT(FILE_SELECT, wxCommandEvent);
