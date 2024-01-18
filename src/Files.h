#pragma once
#include <string>
#include <GL/glew.h>

std::string getVertexShaderSrc();
std::string getFragmentShaderSrc();

class hsTexture {

public:

  int width, height, nrChannels;
  GLuint id;
  unsigned char *data;
  hsTexture(const char* fileName);
  void dispose();

};
