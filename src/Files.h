#pragma once
#include <string>
#include <GL/glew.h>

std::string getVertexShaderSrc();
std::string getFragmentShaderSrc();

class Texture {

public:

  int width, height, nrChannels;
  GLuint id;
  unsigned char *data;
  Texture(const char* fileName);
  void dispose();

};
