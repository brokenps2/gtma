#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include "WindowManager.h"
#include "Input.h"
#include "Camera.h"

using namespace glm;

double oldMouseX = 0, oldMouseY = 0, newMouseX = 0, newMouseY = 0;

Camera::Camera(int width, int height, vec3 pos) {
    Camera::width = width;
    Camera::height = height;
    Camera::pos = pos;
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

void Camera::look() {
 
    newMouseX = getMouseX();
    newMouseY = getMouseY();

    float dx = (float) (newMouseX - oldMouseX) * getDeltaTime();
    float dy = (float) (newMouseY - oldMouseY) * getDeltaTime();

    if(pitch > 90.0f) pitch = 90.0f;
    if(pitch < -90.0f) pitch = -90.0f;

    yaw += dx * sensitivity;
    pitch -= dy * sensitivity;

    oldMouseX = newMouseX;
    oldMouseY = newMouseY;

    printf("%f %f %f    %f     \n", pitch, yaw, pos.z, up.y);

    //printf("nmx:  %lf       omx  %lf       dx  %f       yaw  %f        pos  %f %f %f\n", newMouseX, oldMouseX, dx, pitch, pos.x, pos.y, pos.z);

}

void Camera::move() {

    if(isKeyDown(GLFW_KEY_S)) {
        pos.x += (-cos(radians(yaw)) * speed) * getDeltaTime();
        pos.z -= (sin(radians(yaw)) * speed) * getDeltaTime();
    }

    if(isKeyDown(GLFW_KEY_A)) {
        pos.x += (sin(radians(yaw)) * speed) * getDeltaTime();
        pos.z -= (cos(radians(yaw)) * speed) * getDeltaTime();
    }

    if(isKeyDown(GLFW_KEY_D)) {
        pos.x -= (sin(radians(yaw)) * speed) * getDeltaTime();
        pos.z += (cos(radians(yaw)) * speed) * getDeltaTime();
    }

    if(isKeyDown(GLFW_KEY_W)) {
        pos.x -= (-cos(radians(yaw)) * speed) * getDeltaTime();
        pos.z += (sin(radians(yaw)) * speed) * getDeltaTime();
    }

    if(isKeyDown(GLFW_KEY_LEFT_SHIFT)){
        pos.y -= speed * getDeltaTime();
    }

    if(isKeyDown(GLFW_KEY_SPACE)) {
        pos.y += speed * getDeltaTime();
    }

    if(isKeyDown(GLFW_KEY_E)) {
        printf("wahoo!\n");
    }

    look();

}

void Camera::setPosition(vec3 npos) {
    pos = npos;
}

void Camera::incPosition(vec3 inc) {
    pos += inc;
}
