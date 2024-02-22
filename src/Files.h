#pragma once
#include <string>
#include <GL/glew.h>

std::string getVertexShaderSrc();
std::string getFragmentShaderSrc();

class Texture {

  public:
    
  int w;
  int h;
  int channels;
  u_char* data;
  GLuint id;

  Texture(char* path);
  void dispose();

};
