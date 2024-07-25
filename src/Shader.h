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

typedef struct PointLight {    
    vec3 position;
    vec3 color;
    bool active;
    bool sunMode;
    int packID;
    bool inPack;
} PointLight;

typedef struct PointLightPack {
    PointLight** lights;
    int lightCount;
} PointLightPack;

void createShader(Shader* shader);
void useShader(Shader* shader);

void setBool(Shader* shader, const char* name, bool value);
void setInt(Shader* shader, const char* name, int value);
void setFloat(Shader* shader, const char* name, float value);
void setMatrix(Shader* shader, const char* name, mat4 value);
void setVec3(Shader* shader, const char* name, vec3 value);

void createPointLight(PointLight* light, float x, float y, float z, float r, float g, float b);

GLint getBool(Shader* shader, const char* name);
GLint getInt(Shader* shader, const char* name);
GLfloat getFloat(Shader* shader, const char* name);

