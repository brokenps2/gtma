#include <cglm/affine.h>
#include <cglm/affine-pre.h>
#define FAST_OBJ_IMPLEMENTATION
#include <stdlib.h>
#include <GL/glew.h>
#include <fast_obj.h>
#include "Models.h"

void createModel(Model* model, const char* path, Texture* texture) {
    
    fastObjMesh* mesh = fast_obj_read(path);

    if(!mesh) {
        printf("unable to find model %s\n", path);
        exit(1);
    }

    model->indexCount = mesh->index_count;
    model->vertxCount = mesh->position_count;
    model->texcoCount = mesh->texcoord_count;

    model->vertices = (Vertex*)malloc(((mesh->position_count * 3) + (mesh->texcoord_count * 2)) * sizeof(Vertex));
    model->indices = (unsigned int*)malloc(model->indexCount * sizeof(unsigned int));

    int* vertexMap = (int*)malloc((mesh->position_count * mesh->texcoord_count) * sizeof(int));
    for (int i = 0; i < mesh->position_count * mesh->texcoord_count; ++i) {
        vertexMap[i] = -1; // Initialize with -1 indicating an empty slot
    }

    int uniqueVertexCount = 0;

    for (int i = 0; i < model->indexCount; ++i) {
        unsigned int posIndex = mesh->indices[i].p;
        unsigned int texIndex = mesh->indices[i].t;
        int vertexKey = posIndex * mesh->texcoord_count + texIndex;

        if (vertexMap[vertexKey] == -1) {

            Vertex newVertex;
            newVertex.position[0] = mesh->positions[3 * posIndex + 0];
            newVertex.position[1] = mesh->positions[3 * posIndex + 1];
            newVertex.position[2] = mesh->positions[3 * posIndex + 2];
            newVertex.texCoord[0] = mesh->texcoords[2 * texIndex + 0];
            newVertex.texCoord[1] = mesh->texcoords[2 * texIndex + 1];

            model->vertices[uniqueVertexCount] = newVertex;
            vertexMap[vertexKey] = uniqueVertexCount;

            model->indices[i] = uniqueVertexCount;
            uniqueVertexCount++;
        } else {
            model->indices[i] = vertexMap[vertexKey];
        }
    }

    free(vertexMap);
    fast_obj_destroy(mesh);

    model->texture = *texture;

    glGenVertexArrays(1, &model->VAO);
    glGenBuffers(1, &model->VBO);
    glGenBuffers(1, &model->EBO);
}

void createObject(Object* object, Texture* texture, const char* mdlPath, float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz) {
    Model model; 
    createModel(&model, mdlPath, texture);

    object->model = model;

    object->position[0] = x;
    object->position[1] = y;
    object->position[2] = z;

    object->scale[0] = sx;
    object->scale[1] = sy;
    object->scale[2] = sz;

    object->pitch = rx;
    object->yaw = ry;
    object->roll = rz;
}

void createTransformationMatrix(mat4* matrix, Object* object) {
    vec3 x = { 1, 0, 0 };
    vec3 y = { 0, 1, 0 };
    vec3 z = { 0, 0, 1 };
    glm_mat4_identity(*matrix);
    glm_translate(*matrix, object->position);
    glm_rotate(*matrix, object->pitch, x);
    glm_rotate(*matrix, object->yaw, y);
    glm_rotate(*matrix, object->roll, z);
    glm_scale(*matrix, object->scale);
}
