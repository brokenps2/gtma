#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <string.h>
#include "util/files.h"
#include "util/config.h"

long vtShaderLength;
long frShaderLength;

const char* res(const char* path) {
    const char* resLoc = cfgGetResLoc();
    
    char* fullPath = malloc(strlen(resLoc) + strlen(path) + 1);
    
    if(fullPath) {
        snprintf(fullPath, strlen(resLoc) + strlen(path) + 1, "%s%s", resLoc, path);
    }
    
    return fullPath;
}

char* getVertexShaderSrc() {

    char* buffer = NULL;
    long length;
    FILE* fptr = fopen(res("/shaders/vertexShader.glsl"), "r");

    if (fptr) {
        fseek(fptr, 0, SEEK_END);
        length = ftell(fptr);
        fseek(fptr, 0, SEEK_SET);
        
        buffer = malloc(length + 1);
        if (buffer) {
            size_t readLength = fread(buffer, 1, length, fptr);
            buffer[readLength] = '\0';
        }
        fclose(fptr);
    }

    return buffer;
}

char* getFragmentShaderSrc() {

    char* buffer = NULL;
    long length;
    FILE* fptr = fopen(res("/shaders/fragmentShader.glsl"), "r");

    if (fptr) {
        fseek(fptr, 0, SEEK_END);
        length = ftell(fptr);
        fseek(fptr, 0, SEEK_SET);
        
        buffer = malloc(length + 1);
        if (buffer) {
            size_t readLength = fread(buffer, 1, length, fptr);
            buffer[readLength] = '\0';
        }
        fclose(fptr);
    }

    return buffer;
}

char* getFileSrc(const char* path) {

    char* buffer = NULL;
    long length;
    FILE* fptr = fopen(path, "r");

    if (fptr) {
        fseek(fptr, 0, SEEK_END);
        length = ftell(fptr);
        fseek(fptr, 0, SEEK_SET);
        
        buffer = malloc(length + 1);
        if (buffer) {
            size_t readLength = fread(buffer, 1, length, fptr);
            buffer[readLength] = '\0';
        }
        fclose(fptr);
    }

    return buffer;
}

long getFileSize(const char* path) {
    long length;
    FILE* fptr = fopen(path, "r");

    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    fclose(fptr);
    return length;
}
