#pragma once

#include <GL/glew.h>
#include <string>
#include "Files.h"

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

    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;

    GLint getBool(const std::string &name) const;
    GLint getInt(const std::string &name) const;
    GLfloat getFloat(const std::string &name) const;

};
