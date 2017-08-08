//
// Lightpad - Project.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "Project.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <vector>
#include <string>

#include "Layer.h"

void Project::seek(long newTime) {
	currentTime = newTime;
	for (auto layer : layers) layer->seek(newTime);
}

void Project::advanceFrame(long increment) {
	currentTime += increment;
	for (auto layer : layers) layer->advanceFrame(increment);
}
