#pragma once
#include <string>

void cfgSetPath(std::string newPath);

int cfgGetResX();
int cfgGetResY();
std::string cfgGetTitle();
std::string cfgGetVertexShaderPath();
std::string cfgGetFragmentShaderPath();
void cfgInitValues(std::string path);
