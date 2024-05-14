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

    model->vertices = mesh->positions;

    model->indices = (unsigned int*)malloc(mesh->index_count * sizeof(unsigned int));

    for(int i = 0; i <= mesh->index_count; i++) {
        model->indices[i] = mesh->indices[i].p;
    }

    model->texCoords = mesh->texcoords;

    model->texture = *texture;

    glGenVertexArrays(1, &model->VAO);
    glGenBuffers(1, &model->VBO);
    glGenBuffers(1, &model->EBO);
}

void deleteModel(Model* model) {
    free(model->indices);
    model->indices = NULL;
}

void createTexture(Texture* tex, const char* path) {
    stbi_set_flip_vertically_on_load(1);
    tex->data = stbi_load(path, &tex->w, &tex->h, &tex->channels, 0);
}

void disposeTexture(Texture* tex) {
    stbi_image_free(tex->data);
}
