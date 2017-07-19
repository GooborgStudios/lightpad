//
// Lightpad - Layer.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <map>

#define PLAYBACK_DEBUG 0

enum SmootherType {
	SMOOTH_HOLD = 0,
	SMOOTH_LINEAR = 1,
	SMOOTH_BEZIER = 2,
	SMOOTH_CONT_BEZIER = 3
};

class Layer;

class Keyframe {
	public:
		std::string name;
		SmootherType smoother;
		long time;

		Keyframe();
		Keyframe(std::string name, long time);
	
		virtual std::string serialize();
		void toBuffer(char *outbuf, int len);
};

typedef std::vector<Keyframe *>::iterator KeyframeIterator;

class DoubleKeyframe: public Keyframe {
	public:
		double value;
		DoubleKeyframe(std::string name, long time, double value);
		DoubleKeyframe(std::string name, long time, float value);
		DoubleKeyframe(std::string name, long time, int value);
		std::string serialize();
};

class StringKeyframe: public Keyframe {
	public:
		std::string *value;
		StringKeyframe(std::string name, long time, std::string *value);
		StringKeyframe(std::string name, long time, const char *value);
		std::string serialize();
};

class KeyframeSet {
	protected:
		KeyframeIterator prevKF;
		KeyframeIterator nextKF;
		Layer *parent;
	public:
		std::vector<Keyframe *> keyframes;
		long currentTime;
		
//		KeyframeSet();
		KeyframeSet(Layer *parent);
	
		void AddKeyframe(Keyframe *f);
		void seek(long newTime);
		void advanceFrame(long increment);
		double smoother_fraction();
	
		Keyframe *getFirst();
		Keyframe *getSecond();
};

// XXX Turn this into an empty base class, and move basically everything over to a new, vector-like "KeyframeData" class
class Layer {
	public:
		Layer();
		Layer(std::string d);

		std::map<std::string, KeyframeSet *> keyframes;
		std::string description;
		std::string type;
		void AddKeyframe(Keyframe *f);
		void seek(long newTime);
		void advanceFrame(long increment);
	
		double getDouble(std::string type);
		std::string *getString(std::string type);
};

typedef std::vector<Layer *>::iterator LayerIterator;
