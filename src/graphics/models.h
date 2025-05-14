#pragma once
#include <cglm/cglm.h>
#include "graphics/texture.h"

typedef struct AABB {
    vec3 min;
    vec3 max;
} AABB;

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
    int totalNormalCount;
    AABB aabb;
} Model;

typedef struct GameObject {
    Model model;
    const char* name;
    vec3 position;
    vec3 lastFramePosition;
    vec3 lastFrameScale;
    vec3 rotation;
    vec3 scale;
    int packID;
    bool inPack;
} GameObject;

typedef struct GameObjectPack {
    GameObject** objects;
    int objectCount;
} GameObjectPack;

void gtmaCreateModel(Model* model, const char* path);
void gtmaCreateGameObject(GameObject* object, const char* mdlPath, const char* name, float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz);
void gtmaCreateGameObjectPack(GameObjectPack* pack);
void gtmaDeleteGameObject(GameObject* object);
void gtmaLoadTransformationMatrix(mat4* matrix, GameObject* object);
void gtmaDeleteModel(Model* model);
