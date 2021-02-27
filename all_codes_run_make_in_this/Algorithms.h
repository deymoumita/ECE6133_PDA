// MD
#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>

#include "PE.h"
#include "PE.cc"
#include "Readfile.h"
#include "DrawFloorplan.h"

using namespace std;

// Class containing function calls to the main program
class Algorithms {

	public:
	// runs polish expression
        void SA_Call(string fname, int argc, char**& argv, int option);

	private:
		
};

#endif
// DM
