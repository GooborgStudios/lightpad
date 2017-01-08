// 
// Lightpad - Project.cpp
// Created by Vinyl Darkscratch and Light Apacha, ©2017 Nightwave Studios.
// 

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/file.h>

#include <algorithm>

#include "Project.h"
#include "Layer.h"

using namespace std;

bool is_sooner(Keyframe *a, Keyframe *b) {
	return a->time < b->time;
}

int Project::save(string fileLocation) {
	char buffer[1024];
	wxFile* file = new wxFile();
	vector<KeyframeIterator *> keyframes;

	file->Create(fileLocation, true);
	if (!file->IsOpened()) return -1; // Panic if we don't have an opened file

	file->Write("# Fura Project\n");
	file->Write("\n# Begin Metadata\n");
	file->Write("meta version 0.0.1\n");

	sprintf(buffer, "meta bpm %d\n", this->BPM);
	file->Write(buffer, strlen(buffer));

	sprintf(buffer, "meta fps %.02f\n", this->frameRate);
	file->Write(buffer, strlen(buffer));

	file->Write("\n# Begin Layers\n");
	for (int i = 1; i <= this->layers->size(); i++) {
		Layer *l = this->layers->at(i-1);
		sprintf(buffer, "layer %d %s %s\n", i, l->type.c_str(), l->description.c_str());
		file->Write(buffer, strlen(buffer));
		//keyframes.push_back(l->begin());
	}

	file->Write("\n# Begin Keyframes\n");

	// vector of next keyframes
	std::vector<KeyframeIterator> nextframes;
	std::vector<KeyframeIterator>::iterator lowKF;

	for ( int i = 0; i< this->layers->size(); i++ ) nextframes.push_back(this->layers->at(i)->begin());

	while ( nextframes.size() > 0 ) {
		lowKF = nextframes.begin();
		for ( std::vector<KeyframeIterator>::iterator it = nextframes.begin(); it != nextframes.end(); ++it ) {
			if ( (**it)->time < (**lowKF)->time ) lowKF = it;
		}

        (**lowKF)->toBuffer(buffer, sizeof(buffer));
		file->Write(buffer, strlen(buffer));

		if ( (*lowKF+1) == (**lowKF)->parent->end() ) nextframes.erase(lowKF);
		++(*lowKF);
	}

	file->Write("\n# End Fura Project\n");

	file->Close();
	return 0;
}