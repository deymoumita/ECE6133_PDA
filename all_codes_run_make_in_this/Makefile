# Makefile for ECE4893/8893, Lab 3
# Fall 2010
# George F. Riley

CXXFLAGS = -std=c++0x -g
all:	Floorplan.o 
	$(CXX) -g -Wall -o Floorplan Floorplan.o -L/usr/lib64 -lglut -lGLU -lGL


Floorplan.o:	Floorplan.cc PE.o Algorithms.o

PE.o:	PE.h PE.cc Node.o

Node.o:	Node.h Node.cc

Readfile.o: Readfile.h Readfile.cc 

DrawFloorplan.o: DrawFloorplan.h DrawFloorplan.cc 

Algorithms.o: Algorithms.h Algorithms.cc Readfile.o DrawFloorplan.o PE.o

clean:
	@rm -r *.o Floorplan

