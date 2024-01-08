//author: eli
//created: 1-8-2024
//shaderprogram.cpp shader program handler and loader

#include <string>
#include <fstream>
#include "config.h"

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
