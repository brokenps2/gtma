#define STB_IMAGE_IMPLEMENTATION
#define FAST_OBJ_IMPLEMENTATION
#include <stb_image.h>
#include <fast_obj.h>
#include <GL/glew.h>
#include "Config.h"
#include "Files.h"

long vtShaderLength;
long frShaderLength;

char* getVertexShaderSrc() {

    char* buffer = 0;
    long length;
    FILE* fptr = fopen (cfgGetVertexShaderPath(), "r");

    if(fptr) {
        fseek(fptr, 0, SEEK_END);
        length = ftell(fptr);
        fseek(fptr, 0, SEEK_SET);
        buffer = malloc(length);
        if(buffer) {
            fread(buffer, 1, length, fptr);
        }
        fclose(fptr);
    }

    return buffer;
}

char* getFragmentShaderSrc() {

    char* buffer = 0;
    long length;
    FILE* fptr = fopen (cfgGetFragmentShaderPath(), "r");

    if(fptr) {
        fseek(fptr, 0, SEEK_END);
        length = ftell(fptr);
        fseek(fptr, 0, SEEK_SET);
        buffer = malloc(length);
        if(buffer) {
            fread(buffer, 1, length, fptr);
        }
        fclose(fptr);
    }

    return buffer;

}


void createModel(Model* model, const char* path, Texture* texture) {
    
    fastObjMesh* mesh = fast_obj_read(path);

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

    model->texture = *texture;

    glGenVertexArrays(1, &model->VAO);
    glGenBuffers(1, &model->VBO);
    glGenBuffers(1, &model->EBO);
}

void createTexture(Texture* tex, const char* path) {
    stbi_set_flip_vertically_on_load(1);
    tex->data = stbi_load(path, &tex->w, &tex->h, &tex->channels, 0);
}

void disposeTexture(Texture* tex) {
    stbi_image_free(tex->data);
}
