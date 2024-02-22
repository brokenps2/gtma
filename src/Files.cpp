#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <string>
#include <fstream>
#include "Config.h"
#include "Files.h"

using namespace std;

string getVertexShaderSrc() {
  
  ifstream in(cfgGetVertexShaderPath());
  string contents((istreambuf_iterator<char>(in)), 
    istreambuf_iterator<char>());

  return contents;

}

string getFragmentShaderSrc() {

  ifstream in(cfgGetFragmentShaderPath());
  string contents((istreambuf_iterator<char>(in)), 
    istreambuf_iterator<char>());

  return contents;

}

Texture::Texture(char* path) {
  data = stbi_load(path, &w, &h, &channels, 0);
}

void Texture::dispose() {
  stbi_image_free(data);
}
