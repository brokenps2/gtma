#pragma once
#include <GL/glew.h>
#include <cglm/types.h>
#include <linmath.h>

typedef struct Texture {
    int w;
    int h;
    int channels;
    unsigned char* data;
    GLuint id;

} Texture;

typedef struct Vertex {
    vec3 position;
    vec3 normal;
    vec3 texCoords;
} Vertex;

typedef struct Mesh {
    Vertex* vertices;
    unsigned int* indices;
    Texture* textures;
    unsigned int VAO, VBO, EBO;
} Mesh;

void createMesh(Mesh* mesh, Vertex* vertices, unsigned int* indices, Texture* textures);

char* getVertexShaderSrc();
char* getFragmentShaderSrc();

long getVtShaderLen();
long getFrShaderLen();

void createTexture(Texture* tex, const char* path);
void disposeTexture(Texture* tex);

