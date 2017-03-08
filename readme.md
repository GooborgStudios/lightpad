# Lightpad
Software Designed, Developed, and Published by [Nightwave Studios](https://www.nightwave.co) (Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3), ©2017 all rights reserved.
Additional support from [LaunchpadFun](http://www.launchpadfun.com/en/).

# This software is in early development and is not ready for use.  Please check back later!

## Build and Installation
Lightpad has three executables: _main_, _midiprobe_, and _midiout_.  _Main_ is just what it sounds like, it's the main application, bringing together all of it's features.  _Midiprobe_ and _midiout_ focus specifically on MIDI connectivity, with the first providing a list of available MIDI I/O ports, and the second playing a test file to the Launchpad, while also telling a pressed button to pulse a random color.

```bash
mkdir build
cd build
cmake .. && make -j4
../bin/lightpad[.exe]
../bin/midiprobe[.exe]
../bin/midiout[.exe]
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
* Windows (broken at the moment)
* Linux/Unix (implementing)

## License
Lightpad, Copyright (c) 2016-2017 Nightwave Studios

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, and/or distribute the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following conditions:

- The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.
- Unless otherwise granted permission from the authors, any and all
software using pieces of code from this software must be under the
same licensing conditions.
- Any person wishing to distribute modifications to the Software is
asked to send the modifications to the original developer so that
they can be reviewed, and possibly incorporated, into the canonical
copy of the software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.