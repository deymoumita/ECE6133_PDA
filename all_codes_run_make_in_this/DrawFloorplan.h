// MD
#ifndef DRAWFLOORPLAN_H
#define DRAWFLOORPLAN_H

#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <vector>

using namespace std;

// GUI class
class DrawFloorplan {

	public:
		void drawFloorplan(string title, size_t Num_Modules, int argc, char**& argv);	// main GUI function call
		void timer(int);	// timer function of opengl
		void reshape(int w, int h);	// reshape of opengl for scaling 
		void display(void);	// main display function of opengl which has draw objects function calls and any text
		void drawRect();	// draws all modules
		void drawGrid();	// draws the background grid
		void init();	//  initialization of widndow function of opengl
		void keyPressed(unsigned char key, int x, int y);	// keypress of opengl
		void loadPolishAttributes(unsigned char key);	// reads modules details from the output file

	private:
		int win_id;
		int num_modules;
		vector<string> value;
		vector<float> width;
		vector<float> height;
		vector<float> x;
		vector<float> y;
		vector<float> cx;
		vector<float> cy;
		// determines colors for the modules	
		vector<float> color1;
		vector<float> color2;
		vector<float> color3;

		int winW;
		int winH; 
		float aspect;
		float max_width;
		float max_height;
		float max_scale;



};

#endif
// DM
