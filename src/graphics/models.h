#pragma once
#include <cglm/cglm.h>
#include "graphics/texture.h"
#include "physics/physics.h"

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
    AABB aabb;
    bool collisionEnabled;
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

void gtmaCreateModel(Model* model, const char* path);
void gtmaDeleteModel(Model* model);
