#pragma once
#include <GL/glew.h>

typedef struct Texture {
    int w;
    int h;
    int channels;
    unsigned char* data;
    GLuint id;

} Texture;

char* getVertexShaderSrc();
char* getFragmentShaderSrc();

long getVtShaderLen();
long getFrShaderLen();

void createTexture(Texture* tex, const char* path);
void disposeTexture(Texture* tex);


