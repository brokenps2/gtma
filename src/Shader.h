#pragma once

#include <GL/glew.h>
#include <string>
#include "FileLoader.h"

class Shader {

public:

  GLuint vertexShader;
  GLuint fragmentShader;

  GLuint shaderProgram;

  int sType;

  std::string vtShaderValue;
  const char* vtShaderSrc;
  
  std::string frShaderValue;
  const char* frShaderSrc;

  Shader(int type);

  void initialize();

  void use();

};
