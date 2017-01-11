CXX = `wx-config --cxx`
CXXFLAGS = `wx-config --cxxflags` -O3
LIBS = `wx-config --libs` -lmagic

lightpad: main.o FilePanel.o Colors.o
	$(CXX) $(CXXFLAGS) -o lightpad main.o FilePanel.o Colors.o $(LIBS)

FilePanel.o: FilePanel.cpp FilePanel.h
Colors.cpp: Colors.cpp.template graphics/references/rgbcolors1.png graphics/references/rgbcolors2.png sample_colors.py
	python sample_colors.py > Colors.cpp
Colors.o: Colors.cpp Colors.h

colors: Colors.o
	$(CXX) $(CXXFLAGS) -o colors Colors.o $(LIBS)

clean:
	rm *.o
	rm lightpad