#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Config.h"
#include "Files.h"


const char* getVertexShaderSrc() {
  
    FILE* fptr;
    fptr = fopen(cfgGetVertexShaderPath(), "r");
    fseek(fptr, 0L, SEEK_END);
    int length = ftell(fptr);
    rewind(fptr);
    char* contents;
    fgets(contents, length, fptr);
    fclose(fptr);

    return contents;

}

const char* getFragmentShaderSrc() {

    FILE* fptr;
    fptr = fopen(cfgGetFragmentShaderPath(), "r");
    fseek(fptr, 0L, SEEK_END);
    int length = ftell(fptr);
    rewind(fptr);
    char* contents;
    fgets(contents, length, fptr);
    fclose(fptr);

    return contents;

}

void createTexture(Texture* tex, const char* path) {
    stbi_set_flip_vertically_on_load(1);
    tex->data = stbi_load(path, &tex->w, &tex->h, &tex->channels, 0);
}

void disposeTexture(Texture* tex) {
    stbi_image_free(&tex->data);
}
