#pragma once
#include <cglm/cglm.h>

typedef struct Texture {
    int w;
    int h;
    int channels;
    unsigned char* data;
    unsigned int id;

} Texture;

typedef struct Model {
    vec3* vertices;
    vec2* texCoords;
    vec3* normals;
    unsigned int VAO, VBO, EBO;
} Model;


char* getVertexShaderSrc();
char* getFragmentShaderSrc();

long getVtShaderLen();
long getFrShaderLen();

void createModel(Model* model, const char* path);

void createTexture(Texture* tex, const char* path);
void disposeTexture(Texture* tex);

