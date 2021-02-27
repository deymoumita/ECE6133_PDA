// MD
#ifndef READFILE_H
#define READFILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

using namespace std;

class Readfile {

	public:
		size_t getNumModules(string);	// return the total number of modules in benchmark
		size_t getNetSize(string);		// return total number of nets in the benchmark
		// loads all attributes such as width, height, area, etc for all the modules after reading the benchmark file
		void loadPEAttributes(string fname, size_t sizeofPE, string* PE, bool* Operand, float* Width, float* Height, bool* isSoft, float* Area, float* LAspect, float* HAspect);
		// loads all net attributes such as the net name and the modules present in the each net after readinf from the benchmark file
		vector<vector<string>> loadNetAttributes(string fname, vector<string>& AllNets_f);

	private:
      vector<string> module_list;
      vector<vector<string>> AllNetlists;
      vector<string> AllNets;

};

#endif
// DM
