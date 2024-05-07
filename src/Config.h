#pragma once

void initConfig();

void cfgSetPath(const char* newPath);

int cfgGetResX();
int cfgGetResY();
const char* cfgGetTitle();
const char* cfgGetVertexShaderPath();
const char* cfgGetFragmentShaderPath();
