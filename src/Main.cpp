#include <iostream>
#include <string>
#include <GLFW/glfw3.h>
#include "WindowManager.h"
#include "Renderer.h"
#include "Mouse.h"
#include "Config.h"

int main(int argc, char* argv[]) {

  if (!glfwInit()) {
    std::cout << "GLFW Init Failed!" << std::endl;
  }

  if(argc < 2 || argc > 2) {
    std::cout << "Usage: gtma [config file path]" << std::endl;
    std::cout << "this will be handled by the launcher eventually" << std::endl;
    exit(1);
  }

  cfgSetPath(argv[1]);

  initWindow();
  initRenderer();
  initMouse();
 
  //initial setup finished, run start tasks here

  while(!glfwWindowShouldClose(getWindow())) {

    updateWindow();

    render();

    glfwPollEvents();

  }
 
  glfwTerminate();
  return 0;

}
