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

void Shader::setBool(const std::string &name, bool value) const {         
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value); 
}
void Shader::setInt(const std::string &name, int value) const { 
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value); 
}
void Shader::setFloat(const std::string &name, float value) const { 
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value); 
}

void Shader::use() {
  glUseProgram(shaderProgram);
}
