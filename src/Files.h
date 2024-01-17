#pragma once
#include <string>

std::string getVertexShaderSrc();
std::string getFragmentShaderSrc();

class hsTexture {

public:

  hsTexture(const char* fileName);

  int width, height, nrChannels;
  unsigned char *data;
  printf(fileName.c_str());



};
