// MD
// #ifndef READFILE_CC
// #define READFILE_CC

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>

#include "Readfile.h"

using namespace std;

size_t Readfile::getNumModules(string fname) 
{
  string line;
  size_t n = 0;
  ifstream myfile (fname.c_str());
  if (myfile.is_open())
  {
    while (getline (myfile,line))
    {
      if (line == "*PREPLACED")
      {
        getline (myfile,line);
        while(line != "*END")
        {
          n++;
          getline (myfile,line);
        }
      }      
      if (line == "*SOFTBLOCKS")
      {
        getline (myfile,line);
        while(line != "*END")
        {
          n++;
          getline (myfile,line);
        }
      }

      if (line == "*HARDBLOCKS")
      {
        getline (myfile,line);
        while(line != "*END")
        {
          n++;
          getline (myfile,line);
        }
      }
    }
  }
  return n;
}

size_t Readfile::getNetSize(string fname) 
{
  int num_nets = 0;
	
  string line = "";
  int count = 0;
  string str = "";
  bool isValidNet = false;
  bool readModule = false;

    
  AllNetlists.clear();
  module_list.clear();
  AllNets.clear();
  
  
  string net_name = "";
  int net_num_modules = 100;
  int net_num_modules_count = 0;
  vector<string> temp_module_list;
  
  ifstream myfile (fname.c_str());
  if (myfile.is_open())
  {
    while (getline (myfile,line))
    {
      if (line == "*NETS")
      {
        getline (myfile,line);
        while(line != "*END")
        {
          count = 0;
          str = "";
          for(int i = 0; i < line.size() + 1; i++)
          {
            if(line[0] == '-')
            {
              int module_list_size = module_list.size();
              if(module_list_size != 0)
              {
                AllNetlists.push_back(module_list);
                module_list.clear();  
              }
              if(net_num_modules_count == net_num_modules)
              {
              	//cout<<"Moumita"<<endl;
              	temp_module_list.push_back("m");
              	AllNetlists.push_back(temp_module_list);
              	temp_module_list.clear();
              	net_num_modules = 100;
              }

              if(((int)line[i] == 32) || (i == line.size()))
              {
                if (count == 0)
                {
                  // read '-', do nothing
                  count++;
                  str = "";
                }
                else if (count == 1)
                {
                  if(line[2] == 'n')
                  {
                  	// read net name
                  	net_name = str;
                    AllNets.push_back(str);
                    
                    //cout<<"Net name: "<<str<<endl;
                    isValidNet = true;
                  }
                  else
                  {
                    isValidNet = false;
                  }
                  count++;
                  str = "";
                }
                else if (count == 2)
                {
                  // read number of modules in that net, do nothing
                  count = 0;
                  net_num_modules_count = 0;
                  net_num_modules = atoi(str.c_str());
                  str = "";
                }
                else
                {
                  break;
                }
              }
              else
              {
                str += line[i];
              }
            }
          }
          if((line[0] == 'm') && (isValidNet == true))
          {
            for(int i = 0; i < line.size() + 1; i++)
            {
              if(((int)line[i] == 32) || (i == line.size()))
              {
                readModule = true;
              }
              else
              {
                str += line[i];
              }
            }
            if (readModule == true)
            {	
              module_list.push_back(str);

              
              str = "";
              readModule = false;              
            }
          }
            if((line[0] == 'p'))
          	{
          		net_num_modules_count++;
          		//cout<<"NNM: "<<net_num_modules<<endl;
          	}
          
          getline (myfile,line);
        }
        
          if(net_num_modules_count == net_num_modules)
          {
          	//cout<<"Moumita"<<endl;
          	temp_module_list.push_back("m");
          	AllNetlists.push_back(temp_module_list);
          	temp_module_list.clear();
          	net_num_modules = 100;
          }      
        
	    AllNetlists.push_back(module_list);
      }
    }
  }
  
  if(net_num_modules_count == net_num_modules)
  {
    //cout<<"Moumita"<<endl;

  	temp_module_list.push_back("m");
  	AllNetlists.push_back(temp_module_list);
  	temp_module_list.clear();
  }
  AllNetlists.push_back(module_list);
  //AllNetlists.erase(AllNetlists.begin());
  //cout<<"outside n1652 module size: "<<AllNetlists[1176].size()<<" "<<AllNets[1176]<<endl;
  

  //vector<vector<string>>::iterator iter1;
  vector<string>::iterator iter2;
  size_t j = 0;
  
  for (size_t i = 0; i < AllNets.size(); i++) 
  {
      	
	
      if(AllNetlists[i].size() > 1)
      {
          num_nets++;
          
          
          
      }
      
      
  }
    
  return num_nets;      
}

void Readfile::loadPEAttributes(string fname, size_t sizeofPE, string* PE, bool* Operand, float* Width, float* Height, bool* isSoft, float* Area, float* LAspect, float* HAspect) 
{
  string line = "";
  string str = "";
  int count = 0;
  int num_lines = 0;
  
  ifstream myfile (fname.c_str());
  if (myfile.is_open())
  {
    while (getline (myfile,line))
    {
      if (line == "*PREPLACED")
      {
        getline (myfile, line);
        while(line != "*END")
        {
          count = 0;
          for(int i = 0; i < line.size() + 1; i++)
          {
            if(((int)line[i] == 32) || (i == line.size()))
            {
              if(count == 0)
              {
                PE[num_lines] = str;
                Operand[num_lines] = 1;
                isSoft[num_lines] = 0;                
                //cout<<"Module name: "<<str<<endl;
                count++;
                str = "";
              }
              else if(count == 1)
              {
                Width[num_lines] = atof(str.c_str());                    
                //cout<<"Width: "<<str<<endl;
                count++;
                str = "";
              }
              else if(count == 2)
              {
                Height[num_lines] = atof(str.c_str());
                //cout<<"Height: "<<str<<endl;
                count++;
                str = "";
                break;
              }
              else
              {                
                break;
              }
            }
            else
            {
              str += line[i];
            }
          }
          getline (myfile,line);
          num_lines++;
        }
      }
      else if (line == "*SOFTBLOCKS")
      {
        getline (myfile, line);
        while(line != "*END")
        {
          //cout<<line.size()<<endl;
          count = 0;
          for(int i = 0; i < line.size() + 1; i++)
          {
            if(((int)line[i] == 32) || (i == line.size()))
            {
              if(count == 0)
              {
                PE[num_lines] = str;
                Operand[num_lines] = 1;
                Width[num_lines] = 0;
                Height[num_lines] = 0;
                isSoft[num_lines] = 1;                
                //cout<<"Module name: "<<str<<endl;
                count++;
                str = "";
              }
              else if(count == 1)
              {
                Area[num_lines] = atof(str.c_str());                    
                //cout<<"Area: "<<str<<endl;
                count++;
                str = "";
              }
              else if(count == 2)
              {
                LAspect[num_lines] = atof(str.c_str());
                //cout<<"LAspect: "<<str<<endl;
                count++;
                str = "";
              }
              else if(count == 3)
              {
                HAspect[num_lines] = atof(str.c_str());
                //cout<<"HAspect: "<<str<<endl;
                count = 0;
                str = "";
                break;
              }
              else
              {                
                break;
              }
            }
            else
            {
              str += line[i];
            }
          }
          getline (myfile,line);
          num_lines++;
        }
      }
      else if (line == "*HARDBLOCKS")
      {
        getline (myfile,line);
        while(line != "*END")
        {
          //cout<<line.size()<<endl;
          count = 0;
          for(int i = 0; i < line.size() + 1; i++)
          {
            if(((int)line[i] == 32) || (i == line.size()))
            {
              if(count == 0)
              {
                PE[num_lines] = str;
                Operand[num_lines] = 1;
                isSoft[num_lines] = 0;
                Area[num_lines] = 0;
                LAspect[num_lines] = 0;
                HAspect[num_lines] = 0;                
                //cout<<"Module name: "<<str<<endl;
                count++;
                str = "";
              }
              else if(count == 1)
              {
                Width[num_lines] = atof(str.c_str());                    
                //cout<<"Area: "<<str<<endl;
                count++;
                str = "";
              }
              else if(count == 2)
              {
                Height[num_lines] = atof(str.c_str());
                //cout<<"LAspect: "<<str<<endl;
                count = 0;
                str = "";
                break;
              }
              else
              {                
                break;
              }
            }
            else
            {
              str += line[i];
            }
          }
          getline (myfile,line);
          num_lines++;
        }
      }
    }
    myfile.close();
  }
  else cout << "Unable to open file"; 
}

vector<vector<string>> Readfile::loadNetAttributes(string fname, vector<string>& AllNets_f) 
{
  string line = "";
  int count = 0;
  string str = "";
  bool isValidNet = false;
  bool readModule = false;

  AllNets_f = AllNets;  
        
  return AllNetlists;
}



// #endif
// // DM
