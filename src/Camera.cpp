#include <GL/glew.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include "WindowManager.h"
#include "Camera.h"

using namespace glm;

static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {

}

Camera::Camera(int width, int height, vec3 pos) {
  Camera::width = width;
  Camera::height = height;
  pos = pos;
  glfwSetCursorPosCallback(getWindow(), cursorPosCallback);
}

void Camera::matrix(float fov, float nearPlane, float farPlane, Shader &shader, const char* uniform) {

  mat4 view = mat4(1.0f);
  mat4 proj = mat4(1.0f);

  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = normalize(direction);
  
  view = lookAt(pos, pos + front, up);
  proj = perspective(radians(fov), ((float)width / (float)height), nearPlane, farPlane);

  glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, uniform), 1, GL_FALSE, glm::value_ptr(proj * view));

}

void Camera::move() {

  if(glfwGetKey(getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
    pos.x += (sin(radians(yaw)) * speed);
    pos.z -= (cos(radians(yaw)) * speed);
  }

  if(glfwGetKey(getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
    pos.x -= (-cos(radians(yaw)) * speed);
    pos.z += (sin(radians(yaw)) * speed);
  }

  if(glfwGetKey(getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
    pos.x += (-cos(radians(yaw)) * speed);
    pos.z -= (sin(radians(yaw)) * speed);
  }

  if(glfwGetKey(getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
    pos.x -= (sin(radians(yaw)) * speed);
    pos.z += (cos(radians(yaw)) * speed);
  }


  float newMouseX, newMouseY, oldMouseX, oldMouseY;

  

}

void Camera::setPosition(vec3 npos) {
  pos = npos;
}

void Camera::incPosition(vec3 inc) {
  pos += inc;
}
