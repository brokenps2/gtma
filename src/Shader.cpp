//author: eli
//created: 1-10-2024
//Shader.cpp shader class

#include <GL/glew.h>
#include <string>
#include "FileLoader.h"
#include "Shader.h"

Shader::Shader(int type) {
  sType = type;
}

void Shader::initialize() { 
  vtShaderValue = getVertexShaderSrc(); vtShaderSrc = vtShaderValue.c_str();
  frShaderValue = getFragmentShaderSrc(); frShaderSrc = frShaderValue.c_str();

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vtShaderSrc, NULL);
  glCompileShader(vertexShader);

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &frShaderSrc, NULL);
  glCompileShader(fragmentShader);

  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::use() {
  glUseProgram(shaderProgram);
}
