#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
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

void createTexture(Texture* tex, const char* path) {
    stbi_set_flip_vertically_on_load(1);
    tex->data = stbi_load(path, &tex->w, &tex->h, &tex->channels, 0);
}

void disposeTexture(Texture* tex) {
    stbi_image_free(tex->data);
}
