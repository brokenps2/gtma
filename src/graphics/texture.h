#include <stddef.h>
#include <stdbool.h>
#pragma once

typedef struct Texture {
    int w;
    int h;
    int channels;
    bool isGIF;
    int frames;
    int* delays;
    float frameTimer;
    int currentFrame;
    float gifAlpha;
    unsigned char* data;
    unsigned int* ids;
} Texture;

void gtmaLoadGIF(Texture* tex, const char* path, float alpha);
void gtmaCreateTexture(Texture* tex, const char* path);
void gtmaLoadTextureFromMemory(Texture* texture, const unsigned char* buffer, size_t size);
void gtmaDeleteTexture(Texture* tex);
