CXX = `wx-config --cxx`
CXXFLAGS = `wx-config --cxxflags` `GraphicsMagick++-config --cppflags --cxxflags` -std=c++11 -O3 -I. -I./include -I./include/midifile -I./include/rtmidi
LIBS = `wx-config --libs` `GraphicsMagick++-config --ldflags --libs` -lmagic -lrtmidi -lmidifile -L./lib

ifeq ($(OS),Windows_NT)
	# XXX We're using Code::Blocks for Windows compiling

	#CCFLAGS += -D WIN32
	CXXFLAGS += -D__NO_INLINE__
	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
		#CCFLAGS += -D AMD64
	else
		ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
			#CCFLAGS += -D AMD64
		endif
		ifeq ($(PROCESSOR_ARCHITECTURE),x86)
			#CCFLAGS += -D IA32
		endif
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		# CCFLAGS += -D LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		# CCFLAGS += -D OSX
		CXXFLAGS += -D__MACOSX_CORE__
		LIBS += -framework CoreMIDI -framework CoreAudio -framework CoreFoundation
	endif
	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		# CCFLAGS += -D AMD64
	endif
	ifneq ($(filter %86,$(UNAME_P)),)
		# CCFLAGS += -D IA32
	endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		# CCFLAGS += -D ARM
	endif
endif

lightpad: main.o FilePanel.o DisplayPanel.o TimelinePanel.o PropertiesPanel.o GridRenderer.o Colors.o Helpers.o
	$(CXX) $(CXXFLAGS) -o lightpad $^ $(LIBS)

midiprobe: midiprobe.o Helpers.o
	$(CXX) $(CXXFLAGS) -o midiprobe $^ $(LIBS)

midiout: midiout.o Helpers.o
	$(CXX) $(CXXFLAGS) -o midiout $^ $(LIBS)

main.o: main.cpp Helpers.h
midiprobe.o: midiprobe.cpp Helpers.h
midiout.o: midiout.cpp Helpers.h
Helpers.o: Helpers.cpp Helpers.h
DisplayPanel.o: DisplayPanel.cpp DisplayPanel.h Colors.h Helpers.h
GridRenderer.o: GridRenderer.cpp GridRenderer.h
TimelinePanel.o: TimelinePanel.cpp TimelinePanel.h GridRenderer.h Colors.h Helpers.h
PropertiesPanel.o: PropertiesPanel.cpp PropertiesPanel.h GridRenderer.h Colors.h Helpers.h
FilePanel.o: FilePanel.cpp FilePanel.h Helpers.h
Colors.o: Colors.cpp Colors.h Helpers.h

Colors.cpp: Colors_template.cpp references/rgbcolors1.png references/rgbcolors2.png sample_colors.py
	python sample_colors.py > Colors.cpp

.PHONY: tests
tests: tests/RunTests
	tests/RunTests

tests/RunTests: tests/HelpersTest.h Helpers.o
	cxxtestgen --error-printer -o tests/RunTests.cpp tests/HelpersTest.h
	$(CXX) $(CXXFLAGS) -o tests/RunTests tests/RunTests.cpp Helpers.o $(LIBS)

clean:
	-rm *.o
	-rm -r bin
	-rm -r obj
	-rm -r CMakeFiles
	-rm CMakeCache.txt
	-rm cmake_install.cmake
	-rm *.exe
	-rm a.out
	-rm lightpad
	-rm colors
	-rm midiprobe
	-rm midiout
	-rm tests/RunTests.cpp
	-rm tests/RunTests