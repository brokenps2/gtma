//author: eli
//created: 1-8-2024
//fileloader.cpp file loader

#define STB_IMAGE_IMPLEMENTATION
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

unsigned char getBasicImageTexture(string fileName) {
  int width, height, nrChannels;
  unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
  printf(fileName.c_str());
  char* attribs[sizeof(data)][4];
  attribs[0][0] = (char*)fileName.c_str();
  sprintf(attribs[1][0], "%i", width);
  sprintf(attribs[2][0], "%i", height);
  sprintf(attribs[3][0], "%i", nrChannels);
}


hsTexture::hsTexture(const char* fileName) {

}
