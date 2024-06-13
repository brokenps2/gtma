#pragma once
#include <cglm/cglm.h>
#include "Texture.h"

typedef struct Vertex {
    float position[3];
    float normal[3];
    float texCoord[2];
} Vertex;

typedef struct Model {
    Vertex* vertices;
    unsigned int* indices;
    int indexCount;
    int postnCount;
    int texcoCount;
    int normlCount;
    Texture texture;
    unsigned int VAO, VBO, EBO;
    bool lit;
} Model;

typedef struct Object {
    Model model;
    vec3 position;
    vec3 rotation;
    vec3 scale;
} Object;

void createModel(Model* model, const char* path, Texture* texture);
void createObject(Object* object, Texture* texture, const char* mdlPath, float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz);
void createTransformationMatrix(mat4* matrix, Object* object);
void deleteModel(Model* model);
