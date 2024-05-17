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

    //for every vertex (3 position)
    for(int i = 0; i <= mesh->position_count; i++) {
        int posArrIdx = i * 3;
        float tempPos[3];
        tempPos[0] = mesh->positions[posArrIdx];
        tempPos[1] = mesh->positions[posArrIdx+1];
        tempPos[2] = mesh->positions[posArrIdx+2];
        model->vertices[i].position[0] = tempPos[0];
        model->vertices[i].position[1] = tempPos[1];
        model->vertices[i].position[2] = tempPos[2];
    }


    model->indices = (unsigned int*)malloc(mesh->index_count * sizeof(unsigned int));

    for(int i = 0; i <= mesh->index_count; i++) {
        model->indices[i] = mesh->indices[i].p;
    }

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
