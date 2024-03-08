#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include <GLFW/glfw3.h>

using namespace glm;

class Camera {

public:
  vec3 pos;
  vec3 target = vec3(0.0f, 0.0f, 1.0f);
  vec3 worldUp = vec3(0.0f, 1.0f, 0.0f);
  vec3 direction = normalize(pos - target);
  vec3 right = normalize(cross(worldUp, direction));
  vec3 up = cross(direction, right);

  int width, height;

  //for later use
  //double mouseX, mouseY;

  float speed = 0.1f;
  float sensitivity = 100.0f;

  Camera(int width, int height, vec3 pos);

  void matrix(float fov, float nearPlane, float farPlane, Shader &shader, const char* uniform);
  void getInputs(GLFWwindow* win);
  void setPosition(vec3 npos);
  void incPosition(vec3 inc);
  void rotateBy(vec3 rot);

};
