#pragma once

typedef struct Texture {
    int w;
    int h;
    int channels;
    unsigned char* data;
    unsigned int id;

} Texture;

void createTexture(Texture* tex, const char* path);
void disposeTexture(Texture* tex);
