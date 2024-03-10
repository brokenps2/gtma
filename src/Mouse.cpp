#include <iostream>
#include <GLFW/glfw3.h>
#include "WindowManager.h"

double mouseX;
double mouseY;
int initialized = 0;

double getMouseX() {
  if(initialized == 0) {
    std::cout << "err: Mouse used without being initialized" << std::endl;
    exit(1);
  } else {
    return mouseX;
  }
}

double getMouseY() {
  if(initialized == 0) {
    std::cout << "err: Mouse used without being initialized" << std::endl;
    exit(1);
  } else {
    return mouseY;
  }
}

void cursorCallback(GLFWwindow* window, double xpos, double ypos) {
  mouseX = xpos;
  mouseY = ypos;
}

void initMouse() {
  glfwSetCursorPosCallback(getWindow(), cursorCallback);
  initialized = 1;
}
