# Lightpad
Software Designed, Developed, and Published by [Nightwave Studios](https://www.nightwave.co) (Vinyl Darkscratch, Light Apacha), © 2017 under a GNU General Public License.
Additional support from [LaunchpadFun](http://www.launchpadfun.com/en/).

# This software is almost ready for use.  Please check back later!

## Dependencies
- A C++11 compiler
- CMake
- wxWidgets
- GraphicsMagick
- RtMidi
- LibMidifile (submodule)
- NightwaveCore (submodule)
- HOWL (submodule)
- LibMagic (optional)

## Build and Installation
Lightpad has four executables: _main_, _timeline_, _midiprobe_, and _midiout_.  _Main_ is just what it sounds like, it's the main application, bringing together all of it's features.  _Timeline_ is a teast program to demo the timeline on it's own.  _Midiprobe_ and _midiout_ focus specifically on MIDI connectivity, with the first providing a list of available MIDI I/O ports, and the second playing a test file to the Launchpad, while also telling a pressed button to pulse a random color.

Lightpad utilizes CMake as it's buildscript.  To compile and run Lightpad, type the following commands:

```bash
mkdir build
cd build
cmake .. && make -j8
./bin/lightpad[.exe]
./bin/timeline[.exe]
./bin/midiprobe[.exe]
./bin/midiout[.exe]
```

## Features
(Work In Progress List)

- Fast switching between projects and files
- Looping animation playback
- On-screen Launchpad to emulate animations
- Connectivity with a physical Launchpad
- RGB color to closest velocity match
- Automatic saving (not implemented yet)
- Integration with [Exige's MIDI Extension](http://forum.launchpad-pro.com/viewtopic.php?pid=35098)
- Export to a new Ableton Live project (not implemented yet)
- *More to come!*

## Compatibility
The software is mainly developed on a 2016 MacBook Pro (touch bar), 2.7GHz Intel i7 with Integrated Graphics and a Radeon Pro 460 (4GB VRAM).  The program is not using the GPU as of yet, which lets us emulate a less powerful machine using CPU rendering.

Supported Operating Systems:

* Mac OS [![Build Status](https://travis-ci.org/VaduNightwave/lightpad.svg?branch=master)](https://travis-ci.org/VaduNightwave/lightpad)
* Windows (implementing)
* Linux/Unix (implementing)
