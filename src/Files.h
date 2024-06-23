#pragma once

const char* res(const char* path);

char* getVertexShaderSrc();
char* getFragmentShaderSrc();

char* getFileSrc(const char* path);

long getFileSize(const char* path);

long getVtShaderLen();
long getFrShaderLen();
