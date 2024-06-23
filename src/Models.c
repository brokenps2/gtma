#include <cglm/util.h>
#define FAST_OBJ_IMPLEMENTATION
#include <cglm/affine.h>
#include <cglm/affine-pre.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <fast_obj.h>
#include <uthash.h>
#include "Models.h"

void createModel(Model* model, const char* path, Texture* texture) {
    
    fastObjMesh* mesh = fast_obj_read(path);    

    if(!mesh) {
        printf("unable to find model %s\n", path);
        exit(1);
    }

    model->indexCount = mesh->index_count;
    model->postnCount = mesh->position_count;
    model->texcoCount = mesh->texcoord_count;
    model->normlCount = mesh->normal_count;

    model->vertices = (Vertex*)malloc((mesh->index_count * sizeof(Vertex) * 3));
    model->indices = (unsigned int*)malloc(model->indexCount * 3 * sizeof(unsigned int));

    VertexMapEntry* vertexMap = NULL;
    int uniqueVertexCount = 0;

    for (int i = 0; i < model->indexCount; i++) {
        unsigned int posIndex = mesh->indices[i].p;
        unsigned int texIndex = mesh->indices[i].t;
        unsigned int norIndex = mesh->indices[i].n;
        int vertexKey = posIndex * (mesh->texcoord_count + mesh->normal_count) + texIndex;

        VertexMapEntry* entry;
        HASH_FIND_INT(vertexMap, &vertexKey, entry);

        if (!entry) {
            Vertex newVertex;
            newVertex.position[0] = mesh->positions[3 * posIndex + 0];
            newVertex.position[1] = mesh->positions[3 * posIndex + 1];
            newVertex.position[2] = mesh->positions[3 * posIndex + 2];
            newVertex.texCoord[0] = mesh->texcoords[2 * texIndex + 0];
            newVertex.texCoord[1] = mesh->texcoords[2 * texIndex + 1];
            newVertex.normal[0] = mesh->normals[3 * norIndex + 0];
            newVertex.normal[1] = mesh->normals[3 * norIndex + 1];
            newVertex.normal[2] = mesh->normals[3 * norIndex + 2];

            model->vertices[uniqueVertexCount] = newVertex;

            entry = (VertexMapEntry*)malloc(sizeof(VertexMapEntry));
            entry->vertexKey = vertexKey;
            entry->vertexIndex = uniqueVertexCount;
            HASH_ADD_INT(vertexMap, vertexKey, entry);

            model->indices[i] = uniqueVertexCount;
            uniqueVertexCount++;
        } else {
            model->indices[i] = entry->vertexIndex;
        }
    }

    // Free the hash map
    VertexMapEntry *currentEntry, *tmp;
    HASH_ITER(hh, vertexMap, currentEntry, tmp) {
        HASH_DEL(vertexMap, currentEntry);
        free(currentEntry);
    }

    fast_obj_destroy(mesh);
    model->texture = *texture;
    model->lit = true;

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

    object->rotation[0] = rx;
    object->rotation[1] = ry;
    object->rotation[2] = rz;
}

void loadTransformationMatrix(mat4* matrix, Object* object) {

    if(object->rotation[0] < 0) object->rotation[0] = 360 + object->rotation[0];
    if(object->rotation[1] < 0) object->rotation[1] = 360 + object->rotation[1];
    if(object->rotation[2] < 0) object->rotation[2] = 360 + object->rotation[2];

    if(object->rotation[0] >= 360) object->rotation[0] = object->rotation[0] - 360;
    if(object->rotation[0] <= -360) object->rotation[0] = object->rotation[0] + 360;

    if(object->rotation[1] >= 360) object->rotation[1] = object->rotation[1] - 360;
    if(object->rotation[1] <= -360) object->rotation[1] = object->rotation[1] + 360;

    if(object->rotation[2] >= 360) object->rotation[2] = object->rotation[2] - 360;
    if(object->rotation[2] <= -360) object->rotation[2] = object->rotation[2] + 360;

    glm_mat4_identity(*matrix);
    glm_translate(*matrix, object->position);
    glm_rotate_x(*matrix, glm_rad(object->rotation[0]), *matrix);
    glm_rotate_y(*matrix, glm_rad(object->rotation[1]), *matrix);
    glm_rotate_z(*matrix, glm_rad(object->rotation[2]), *matrix);

    glm_scale(*matrix, object->scale);
}
