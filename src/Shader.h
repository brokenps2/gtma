#pragma once
#include <GL/glew.h>
#include <cglm/cglm.h>
#include "Files.h"

typedef struct Shader {

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint id;

    int sType;

    char* vtShaderSrc;
    char* frShaderSrc;

} Shader;

void createShader(Shader* shader);
void useShader(Shader* shader);

void setBool(Shader* shader, const char* name, bool value);
void setInt(Shader* shader, const char* name, int value);
void setFloat(Shader* shader, const char* name, float value);
void setMatrix(Shader* shader, const char* name, mat4 value);
void setVec3(Shader* shader, const char* name, vec3 value);

GLint getBool(Shader* shader, const char* name);
GLint getInt(Shader* shader, const char* name);
GLfloat getFloat(Shader* shader, const char* name);

