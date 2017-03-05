# Lightpad
Software Designed, Developed, and Published by [Nightwave Studios](https://www.nightwave.co) (Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3), ©2017 all rights reserved.
Additional support from [LaunchpadFun](http://www.launchpadfun.com/en/).

# This software is in early development and is not ready for use.  Please check back later!

## Build and Installation
Lightpad has three executables: _main_, _midiprobe_, and _midiout_.  _Main_ is just what it sounds like, it's the main application, bringing together all of it's features.  _Midiprobe_ and _midiout_ focus specifically on MIDI connectivity, with the first providing a list of available MIDI I/O ports, and the second playing a test file to the Launchpad, while also telling a pressed button to pulse a random color.

```bash
cd [lightpad directory path]
cmake . && make -j4
./bin/lightpad[.exe]
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
- Automatic saving
- Integration with [Exige's MIDI Extension](http://forum.launchpad-pro.com/viewtopic.php?pid=35098)
- Export to a new Ableton Live project
- *More to come!*

## Compatibility
The software is mainly developed on a mid-2014 MacBook Pro, 2.5GHz Intel i7 with Integrated Graphics and a NVIDIA GeForce GT 750M.

Supported Operating Systems:

* Mac OS
* Windows (possibly broken at the moment)
* Linux/Unix (untested)
