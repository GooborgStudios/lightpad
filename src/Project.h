//
// HOWL - Music-synced animation library
// File: Project.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// https://www.nightwave.co
//

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <vector>
#include <string>

#include "Layer.h"

namespace HOWL {
	class Project {
		protected:
			std::string filePath = "";
		
		public:
			int BPM = 120;
			int ticksPerBeat = 32;
			int beatsPerMeasure = 4;
			long currentTime = 0;
		
			std::vector<Layer *> layers;
		
			virtual int save()=0;
			virtual int save(std::string filePath)=0;
			void seek(long newTime);
			void advanceFrame(long increment);
	};
}
