//
// Lightpad - FileMetadata.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "ElementIDs.h"
#include "NightwaveCore/Helpers.h"
#include "HOWL/Project.h"

class FileMetadata : public wxDialog {
	public:
	FileMetadata(const wxString& title, HOWL::Project *project);
};
