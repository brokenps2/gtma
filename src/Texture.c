#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <GL/glew.h>
#include "Texture.h"
#include "Files.h"


void createTexture(Texture* tex, const char* path) {

    stbi_set_flip_vertically_on_load(1);
    tex->data = stbi_load(res(path), &tex->w, &tex->h, &tex->channels, 0);   
   
    glGenTextures(1, &tex->id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->id);

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

    stbi_image_free(tex->data);

}

void createAnimatedTexture(Texture** frames, const char* path) {
}

void disposeTexture(Texture* tex) {
    stbi_image_free(tex->data);
}
