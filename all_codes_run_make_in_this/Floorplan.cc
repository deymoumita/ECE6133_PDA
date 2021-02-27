//Created by Anirudha Kurhade [anirudha@gatech.edu]
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <math.h>

// Include our PE class definition and implementation

#include "Algorithms.h"
#include "Algorithms.cc"

using namespace std;

int main(int argc, char *argv[]) 
{
    // MD
    
    // determine benchmark file
    string fname0 = "benchmarks/test.fp";
    string fname1 = "benchmarks/fft.fp";
    string fname2 = "benchmarks/industry.fp";
    string fname3 = "benchmarks/mult.fp";
    string fname4 = "benchmarks/processor.fp";
    string fname5 = "benchmarks/rca.fp";
    string fname6;
    string fname;
    
    if(argc < 2)
    {
        cout<<"Usage: ./Floorplan algorithm filenumber"<<endl;
        exit(1);
    }
    string algo = argv[1];

    switch (atoi(argv[2]))
    {
        case 0:
            fname = fname0;
            break;
        case 1: 
            fname = fname1;
            break;
        case 2:
            fname = fname2;
            break;
        case 3:
            fname = fname3;
            break;
        case 4:
            fname = fname4;
            break;
        case 5:
            fname = fname5;
            break;
        case 6:
            fname = argv[3];
            break;
        default:
            cout<<"Test number should be between 0-5"<<endl;
            exit(1); 
    }
    
    // Algorithms object
    Algorithms Al;
    
    // call specific function depending upon the options
    if (algo == "SA")
    {
        Al.SA_Call(fname, argc, argv, 0);
    }
    else if(algo == "SM")
    {
        Al.SA_Call(fname, argc, argv, 1);
    }
    else if(algo == "SASM")
    {
        Al.SA_Call(fname, argc, argv, 2);
    }
        

   //DM
    return 0;
}



