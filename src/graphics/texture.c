#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_ONLY_GIF
#include <stb_image.h>
#include <GL/glew.h>
#include "texture.h"
#include "../util/files.h"


void gtmaCreateTexture(Texture* tex, const char* path) {
    memset(tex, 0, sizeof(Texture));

    stbi_set_flip_vertically_on_load(1);
    tex->data = stbi_load(res(path), &tex->w, &tex->h, &tex->channels, 0);

    tex->frames = 1;
    tex->currentFrame = 0;
    tex->ids = malloc(sizeof(unsigned int));
    tex->delays = malloc(sizeof(int));
    tex->delays[0] = 0;
    tex->frameTimer = 0;
    tex->gifAlpha = 1;
   
    glGenTextures(1, &tex->ids[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->ids[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if(tex->channels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->w, tex->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->w, tex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->data);
    }
}

void gtmaLoadGIF(Texture* tex, const char* path, float alpha) {

    char* rawGifData;
    int rawGifSize;

    rawGifSize = getFileSize(res(path));
    rawGifData = getFileSrc(res(path));

    tex->data = stbi_load_gif_from_memory((unsigned char*)rawGifData, rawGifSize, &tex->delays, &tex->w, &tex->h, &tex->frames, &tex->channels, 4);

    if(!tex->data) {
        printf("failed to load GIF animation: %s\n", path);
        printf("STB ERROR: %s\n", stbi_failure_reason());
        exit(1);
    }

    tex->currentFrame = 0;
    tex->frameTimer = 0;
    tex->gifAlpha = alpha;

    tex->ids = malloc(sizeof(unsigned int) * tex->frames);

    glGenTextures(tex->frames, tex->ids);

    size_t frameSize = tex->w * tex->h * 4;

    for(int i = 0; i < tex->frames; i++) {
        unsigned char* framePtr = tex->data + (i * frameSize);
        glBindTexture(GL_TEXTURE_2D, tex->ids[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->w, tex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, framePtr);
    }

    free(rawGifData);
}

int gtmaLoadTextureFromMemory(Texture* tex, const unsigned char* buffer, size_t size) {
    stbi_set_flip_vertically_on_load(0);
    tex->data = stbi_load_from_memory(buffer, size, &tex->w, &tex->h, &tex->channels, 0);
    if (!tex->data) {
        return 1;
    }
    tex->frames = 1;
    tex->ids = malloc(sizeof(unsigned int));
    tex->delays = malloc(sizeof(int));
    tex->delays[0] = 0;
    tex->currentFrame = 0;
    tex->frameTimer = 0;
    tex->gifAlpha = 1;

    glGenTextures(1, &tex->ids[0]);
    glBindTexture(GL_TEXTURE_2D, tex->ids[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, tex->channels == 4 ? GL_RGBA : GL_RGB, tex->w, tex->h, 0, tex->channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, tex->data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return 0;
}

void gtmaDeleteTexture(Texture* tex) {
    if (!tex) return;

    if (tex->data) {
        stbi_image_free(tex->data);
        tex->data = NULL;
    }

    if (tex->delays) {
        free(tex->delays);
        tex->delays = NULL;
    }

    if (tex->ids) {
        free(tex->ids);
        tex->ids = NULL;
    }
}
