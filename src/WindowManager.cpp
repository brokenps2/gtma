#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>
#include "Config.h"
#include "Math.h"

GLFWwindow* window;

void initWindow() {
 
  char* ctitle;

  if(cfgGetResX() == 1 || cfgGetResY() == 1 || cfgGetTitle() == "1") {
    std::cerr << "Unknown error in configuration file" << std::endl;
    exit(1);
  } else {
    const int length = cfgGetTitle().length();
    ctitle = new char[length+1];
    strcpy(ctitle, cfgGetTitle().c_str());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(cfgGetResX(), cfgGetResY(), ctitle, NULL, NULL);
    glfwMakeContextCurrent(window);
  }

  const GLenum err = glewInit();
  glewExperimental = GL_TRUE;
  if (GLEW_OK != err) {
    std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
    exit(1);
  }

  //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  glViewport(0, 0, cfgGetResX(), cfgGetResY());
  glEnable(GL_DEPTH_TEST);
}

GLFWwindow* getWindow() {
  return window;
}

void updateWindow() {
  glfwSwapBuffers(window);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, (GLint)cfgGetResX(), (GLint)cfgGetResY());
  glClearColor(glc(135), glc(206), glc(235), 1);
}

