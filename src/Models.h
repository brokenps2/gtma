#pragma once
#include <cglm/cglm.h>
#include <uthash.h>
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
    int packID;
    bool inPack;
} Object;

typedef struct ObjectPack {
    Object** objects;
    int objectCount;
} ObjectPack;

typedef struct VertexMapEntry {
    int vertexKey;
    int vertexIndex;
    UT_hash_handle hh;
} VertexMapEntry;

void createModel(Model* model, const char* path, Texture* texture);
void createObject(Object* object, Texture* texture, const char* mdlPath, float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz);
void createObjectPack(ObjectPack* pack);
void loadTransformationMatrix(mat4* matrix, Object* object);
void deleteModel(Model* model);
