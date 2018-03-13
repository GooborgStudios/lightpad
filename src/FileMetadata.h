//
// Lightpad - FileMetadata.h
// ©2018 Gooborg Studios: Vinyl Darkscratch, Light Apacha.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// http://www.gooborg.com/lightpad
//

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "ElementIDs.h"
#include "GooCore/GooCore.h"
#include "HOWL/Project.h"

class FileMetadata : public wxDialog {
	public:
		FileMetadata(const wxString& title, HOWL::Project *project);
};
