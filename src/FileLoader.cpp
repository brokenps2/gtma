//author: eli
//created: 1-8-2024
//fileloader.cpp file loader

#include <string>
#include <fstream>
#include "Config.h"

using namespace std;

string vtShaderPath;

string getVertexShaderSrc() {

  vtShaderPath = cfgGetVertexShaderPath();
  
  ifstream in(vtShaderPath);
  string contents((istreambuf_iterator<char>(in)), 
    istreambuf_iterator<char>());

  return contents;

}

string getFragmentShaderSrc() {

  vtShaderPath = cfgGetFragmentShaderPath();
  
  ifstream in(vtShaderPath);
  string contents((istreambuf_iterator<char>(in)), 
    istreambuf_iterator<char>());

  return contents;

}

