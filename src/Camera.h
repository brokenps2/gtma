#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include <GLFW/glfw3.h>

using namespace glm;

class Camera {

public:
  vec3 pos;
  vec3 front = vec3(0.0f, 0.0f, -1.0f);
  vec3 up = vec3(0.0f, 1.0f, 0.0f);
  vec3 direction;

  float pitch = 0;
  float yaw = 90.0f;
  float roll = 0;

  int width, height;

  float speed = 0.01f;
  float sensitivity = 100.0f;

  Camera(int width, int height, vec3 pos);

  void matrix(float fov, float nearPlane, float farPlane, Shader &shader, const char* uniform);
  void move();
  void setPosition(vec3 npos);
  void incPosition(vec3 inc);

private:
  void look();
};
