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

hsTexture::hsTexture(const char* fileName) {

  hsTexture::data = stbi_load(fileName, &width, &height, &nrChannels, 0);
  printf(fileName);

}

void hsTexture::dispose() {
  stbi_image_free(data);
}
