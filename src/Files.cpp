
#include <stb_image.h>
#include <string>
#include <fstream>
#include "Config.h"
#include "Files.h"

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

Texture::Texture(const char* fileName) {

}

void Texture::dispose() {

}
