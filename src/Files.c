#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "Files.h"
#include "Config.h"

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

