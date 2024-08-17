#pragma once
#include <cglm/cglm.h>
#include <uthash.h>
#include "Texture.h"

typedef struct Vertex {
    float position[3];
    float normal[3];
    float color[4];
    float texCoord[2];
    bool colored;
} Vertex;

typedef struct Mesh {
    Vertex* vertices;
    unsigned int* indices;
    int indexCount;
    int postnCount;
    int texcoCount;
    int normlCount;
    Texture texture;
    bool lit;
    unsigned int VAO, VBO, EBO;
} Mesh;

typedef struct Model {
    Mesh* meshes;
    int meshCount;
    int totalIndexCount;
    int totalPosCount;
    int totalUVCount;
    int totalNormalCound;
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

void createModel(Model* model, const char* path);
float getPositionHeight(Model* model, float x, float z);
void createObject(Object* object, const char* mdlPath, float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz);
void createObjectPack(ObjectPack* pack);
void destroyObject(Object* object);
void loadTransformationMatrix(mat4* matrix, Object* object);
void transformVertex(mat4* matrix, vec3 vertex);
void deleteModel(Model* model);
