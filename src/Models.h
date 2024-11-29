#pragma once
#include <cglm/cglm.h>
#include "Texture.h"

typedef struct Vertex {
    float position[3];
    float normal[3];
    float color[3];
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

void gtmaCreateModel(Model* model, const char* path);
void gtmaCreateObject(Object* object, const char* mdlPath, float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz);
void gtmaCreateObjectPack(ObjectPack* pack);
void gtmaDeleteObject(Object* object);
void gtmaLoadTransformationMatrix(mat4* matrix, Object* object);
void gtmaDeleteModel(Model* model);
