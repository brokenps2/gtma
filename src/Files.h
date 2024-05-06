#pragma once
#include <GL/glew.h>

typedef struct Texture {
    int w;
    int h;
    int channels;
    unsigned char* data;
    GLuint id;

} Texture;

const char* getVertexShaderSrc();
const char* getFragmentShaderSrc();

void createTexture(Texture* tex, const char* path);
void disposeTexture(Texture* tex);


