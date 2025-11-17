#pragma once
#include <GL/glew.h>
#include <cglm/cglm.h>

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
    unsigned int flags;
    int packID;
    bool inPack;
    float range;
} PointLight;

typedef struct PointLightPack {
    PointLight** lights;
    int lightCount;
} PointLightPack;

void gtmaCreateShader(Shader* shader);
void gtmaCreateScreenShader(Shader* shader);
void gtmaUseShader(Shader* shader);

void gtmaSetBool(Shader* shader, const char* name, bool value);
void gtmaSetInt(Shader* shader, const char* name, int value);
void gtmaSetFloat(Shader* shader, const char* name, float value);
void gtmaSetMatrix(Shader* shader, const char* name, mat4 value);
void gtmaSetVec3(Shader* shader, const char* name, vec3 value);
void gtmaSetVec2(Shader* shader, const char* name, vec2 value);

void gtmaCreatePointLight(PointLight* light, float x, float y, float z, float r, float g, float b, unsigned int flags);

void gtmaRemoveLightID(PointLightPack* lightPack, int id);
void gtmaAddLight(PointLight* light, PointLightPack* lightPack);

GLint gtmaGetBool(Shader* shader, const char* name);
GLint gtmaGetInt(Shader* shader, const char* name);
GLfloat gtmaGetFloat(Shader* shader, const char* name);

