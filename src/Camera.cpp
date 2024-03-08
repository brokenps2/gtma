#include <GL/glew.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include "Camera.h"

using namespace glm;

Camera::Camera(int width, int height, vec3 pos) {
  Camera::width = width;
  Camera::height = height;
  pos = pos;
}

void Camera::matrix(float fov, float nearPlane, float farPlane, Shader &shader, const char* uniform) {

  mat4 view = mat4(1.0f);
  mat4 proj = mat4(1.0f);
  
  view = lookAt(pos, target, worldUp);
  proj = perspective(radians(fov), (float)(width / height), nearPlane, farPlane);

  glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, uniform), 1, GL_FALSE, glm::value_ptr(proj * view));

}

void Camera::setPosition(vec3 npos) {
  pos = npos;
}

void Camera::incPosition(vec3 inc) {
  pos += inc;
}

void Camera::rotateBy(vec3 rot) {
  //i'm doing this later im going to bed

  //orientation = rotate(orientation, radians(-rot.x), )
}
