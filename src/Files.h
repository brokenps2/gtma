#pragma once
#include <cglm/cglm.h>

typedef struct Texture {
    int w;
    int h;
    int channels;
    unsigned char* data;
    unsigned int id;

} Texture;

typedef struct Vertex {
    float position[3];
    float texCoords[2];
} Vertex;

typedef struct Model {
    Vertex* vertices;
    unsigned int* indices;
    int indexCount;
    int vertxCount;
    int texcoCount;
    Texture texture;
    unsigned int VAO, VBO, EBO;
} Model;


char* getVertexShaderSrc();
char* getFragmentShaderSrc();

long getVtShaderLen();
long getFrShaderLen();

void createModel(Model* model, const char* path, Texture* texture);
void deleteModel(Model* model);

void createTexture(Texture* tex, const char* path);
void disposeTexture(Texture* tex);

