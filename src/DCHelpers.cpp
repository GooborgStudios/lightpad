//
// Nightwave Global - DCHelpers.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.
// https://www.nightwave.co/
//

#include "DCHelpers.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <sstream>
#include <queue>
#include <vector>

void DrawWrappedText(std::string text, wxDC &canvas, wxRect box) {
	std::queue<std::string> words;
	std::vector<std::string> *lines = new std::vector<std::string>();
	
	std::stringstream ssin(text);
	std::string word;
	while (ssin.good()) {
		ssin >> word;
		words.push(word);
	}
	
	std::string currentLine = words.front();
	words.pop();
	while (!words.empty()) {
		std::string newline = currentLine + " " + words.front();
		wxSize size = canvas.GetTextExtent(newline);
		if (size.GetWidth() > box.GetWidth()) {
			lines->push_back(currentLine);
			currentLine = words.front();
		} else {
			currentLine = newline;
		}
		
		words.pop();
	}
	
	lines->push_back(currentLine);
	
	for (std::string line : *lines) {
		canvas.DrawLabel(line, box, wxALIGN_CENTER|wxALIGN_TOP);
		box.SetTop(box.GetTop() + canvas.GetTextExtent(line).GetHeight() + 4);
	}
	
	delete lines;
}
