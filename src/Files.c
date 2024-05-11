#define STB_IMAGE_IMPLEMENTATION
#define FAST_OBJ_IMPLEMENTATION
#include <stb_image.h>
#include <fast_obj.h>
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

int allocateForObjData(FILE* file, int type) {

    int i = 0;
    while(1) {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if(res == EOF) {
            break;
        } else if(type == 0) {
            if(strcmp(lineHeader, "v") == 0) {
                i++;
            }
        } else if(type == 1) {
            if(strcmp(lineHeader, "vt") == 0) {
                i++;
            }
        } else if(type == 2) {
            if(strcmp(lineHeader, "vn") == 0) {
                i++;
            }
        } else if(type == 3) {
            if(strcmp(lineHeader, "f") == 0) {
                i++;
            }
        }
    }
            
    return i;
}


void createModel(Model* model, const char* path) {
    unsigned int *vertexIndices, *uvIndices, *normalIndices;
    int vtindidx = 0, uvindidx = 0, nmindidx = 0;

    vec3* tempVertices;
    int vtindex = 0;
    vec2* tempUVs;
    int uvindex = 0;
    vec3* tempNormals;
    int nmindex = 0;

    FILE* file = fopen(path, "r");
    if(file == NULL) {
        printf("unable to open file %s\n", path);
        exit(1);
    }

    int vtcount = allocateForObjData(file, 0);
    int uvcount = allocateForObjData(file, 1);
    int nmcount = allocateForObjData(file, 2);

    int idcount = allocateForObjData(file, 3);

    tempVertices = (vec3*)malloc(vtcount * sizeof(vec3));
    tempUVs = (vec2*)malloc(uvcount * sizeof(vec2));
    tempNormals = (vec3*)malloc(nmcount * sizeof(vec3));

    vertexIndices = (unsigned int*)malloc(idcount * sizeof(unsigned int));
    uvIndices = (unsigned int*)malloc(idcount * sizeof(unsigned int));
    normalIndices = (unsigned int*)malloc(idcount * sizeof(unsigned int));

    rewind(file);

    while(1) {

        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if(res == EOF) {
            break;
        } else if(strcmp(lineHeader, "v") == 0) {
            vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
            tempVertices[vtindex][0] = vertex[0];
            tempVertices[vtindex][1] = vertex[1];
            tempVertices[vtindex][2] = vertex[2];
            vtindex++;
        } else if(strcmp(lineHeader, "vt") == 0) {
            vec3 uv;
            fscanf(file, "%f %f\n", &uv[0], &uv[1]);
            tempUVs[uvindex][0] = uv[0];
            tempUVs[uvindex][1] = uv[1];
            uvindex++;
        } else if(strcmp(lineHeader, "vn") == 0) {
            vec3 normal;
            fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
            tempNormals[nmindex][0] = normal[0];
            tempNormals[nmindex][1] = normal[1];
            tempNormals[nmindex][2] = normal[2];
            nmindex++;
        } else if(strcmp(lineHeader, "f") == 0) {
            unsigned int vertIdx[3], uvIdx[3], nmIdx[3];
            fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertIdx[0], &uvIdx[0], &nmIdx[0], 
                                 &vertIdx[1], &uvIdx[1], &nmIdx[1], 
                                 &vertIdx[2], &uvIdx[2], &nmIdx[2]);
            vertexIndices[vtindidx] = vertIdx[0];
            vtindidx++;
            vertexIndices[vtindidx] = vertIdx[1];
            vtindidx++;
            vertexIndices[vtindidx] = vertIdx[2];
            vtindidx++;
            uvIndices[uvindidx] = uvIdx[0];
            uvindidx++;
            uvIndices[uvindidx] = uvIdx[1];
            uvindidx++;
            uvIndices[uvindidx] = uvIdx[2];
            normalIndices[nmindidx] = nmIdx[0];
            nmindidx++;
            normalIndices[nmindidx] = nmIdx[1];
            nmindidx++;
            normalIndices[nmindidx] = nmIdx[2];
            nmindidx++;
        }
    }

    int finalvtindex = 0;
    int finaluvindex = 0;
    int finalnmindex = 0;

    for(unsigned int i = 0; i < (sizeof(&vertexIndices) / sizeof(unsigned int)); i++) {
        unsigned int vertexIndex = vertexIndices[i];

        printf("%i\n\n\n", vertexIndices[0]);

        if (vertexIndex > vtcount || vertexIndex <= 0) {
            printf("Error: Vertex index out of bounds: %d\n", vertexIndex);
        }

        vec3 vtx;
        vtx[0] = tempVertices[vertexIndex - 1][0];
        vtx[1] = tempVertices[vertexIndex - 1][1];
        vtx[2] = tempVertices[vertexIndex - 1][2];

        model->vertices = (vec3*)malloc(vtcount * sizeof(vec3));

        model->vertices[finalvtindex][0] = vtx[0];
        model->vertices[finalvtindex][1] = vtx[1];
        model->vertices[finalvtindex][2] = vtx[2];
        finalvtindex++;
    
        unsigned int uvIndex = uvIndices[i];
        vec3 uvx;
        uvx[0] = tempUVs[uvIndex - 1][0];
        uvx[1] = tempUVs[uvIndex - 1][1];

        model->texCoords = (vec2*)malloc(vtcount * sizeof(vec2));

        model->texCoords[finaluvindex][0] = uvx[0];
        model->texCoords[finaluvindex][1] = uvx[1];
        finaluvindex++;

        unsigned int normalIndex = normalIndices[i];
        vec3 nmx;
        nmx[0] = tempNormals[normalIndex - 1][0];
        nmx[1] = tempNormals[normalIndex - 1][1];
        nmx[2] = tempNormals[normalIndex - 1][2];

        model->normals = (vec3*)malloc(vtcount * sizeof(vec3));

        model->normals[finalnmindex][0] = nmx[0];
        model->normals[finalnmindex][1] = nmx[1];
        model->normals[finalnmindex][2] = nmx[2];
        finalnmindex++;
    }

    free(tempVertices);
    free(tempUVs);
    free(tempNormals);

}

void createTexture(Texture* tex, const char* path) {
    stbi_set_flip_vertically_on_load(1);
    tex->data = stbi_load(path, &tex->w, &tex->h, &tex->channels, 0);
}

void disposeTexture(Texture* tex) {
    stbi_image_free(tex->data);
}
