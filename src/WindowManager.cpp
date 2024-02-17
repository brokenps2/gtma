#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <cstring>
#include "Config.h"
#include "Math.h"

SDL_Window* window;

void initWindow() {
 
  char* ctitle;

  if(cfgGetResX() == 1 || cfgGetResY() == 1 || cfgGetTitle() == "1") {
    std::cerr << "Unknown error in configuration file" << std::endl;
    exit(1);
  } else {
    const int length = cfgGetTitle().length();
    ctitle = new char[length+1];
    strcpy(ctitle, cfgGetTitle().c_str());
    window = SDL_CreateWindow(ctitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, cfgGetResX(), cfgGetResY(), SDL_WINDOW_OPENGL);
  }

  SDL_GL_CreateContext(window);

  const GLenum err = glewInit();
  glewExperimental = GL_TRUE;
  if (GLEW_OK != err) {
    std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
    exit(1);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  glViewport(0, 0, cfgGetResX(), cfgGetResY());
}

void updateWindow() {
  SDL_UpdateWindowSurface(window);
  SDL_GL_SwapWindow(window);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(glc(0), glc(156), glc(150), 1);
}

SDL_Window* getWindow() {
  return window;
} 
