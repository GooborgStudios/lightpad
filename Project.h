// 
// Lightpad - Project.h
// Created by Vinyl Darkscratch and Light Apacha, ©2017 Nightwave Studios.
// 

#ifndef PROJECT_H
#define PROJECT_H

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "Layer.h"

class Project {
	public:
		int BPM;
		double frameRate;
		std::vector<Layer *> *layers;
		int save(std::string fileLocation);
};

extern Project activeProject;

#endif