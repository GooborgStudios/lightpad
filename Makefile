CXX = `wx-config --cxx`
CXXFLAGS = `wx-config --cxxflags`
LIBS = `wx-config --libs` -lmagic

lightpad: main.o FilePanel.o
	$(CXX) $(CXXFLAGS) -o lightpad main.o FilePanel.o $(LIBS)

FilePanel.o: FilePanel.cpp FilePanel.h

clean:
	rm *.o
	rm lightpad