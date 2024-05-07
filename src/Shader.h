#pragma once
#include <GL/glew.h>
#include "Files.h"

typedef struct Shader {

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint id;

    int sType;

    const char* vtShaderSrc;
    const char* frShaderSrc;

} Shader;

void createShader(Shader* shader);
void useShader(Shader* shader);
void setBool(Shader* shader, const char* name, bool value);
void setInt(Shader* shader, const char* name, int value);
void setFloat(Shader* shader, const char* name, float value);
GLint getBool(Shader* shader, const char* name);
GLint getInt(Shader* shader, const char* name);
GLfloat getFloat(Shader* shader, const char* name);

