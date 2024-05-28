#include <cglm/cam.h>
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include <cglm/common.h>
#include <math.h>
#include "WindowManager.h"
#include "Input.h"
#include "Camera.h"
#include <GLFW/glfw3.h>


double oldMouseX = 0, oldMouseY = 0, newMouseX = 0, newMouseY = 0;

void createCamera(Camera* cam, int width, int height, vec3 pos) {
    cam->width = width;
    cam->height = height;

    cam->front[0] = 0.0f;
    cam->front[1] = 0.0f;
    cam->front[2] = -1.0f;

    cam->up[0] = 0.0f;
    cam->up[1] = 1.0f;
    cam->up[2] = 0.0f;

    cam->pos[0] = pos[0];
    cam->pos[1] = pos[1];
    cam->pos[2] = pos[2];

    cam->pitch = 90.0f;
    cam->yaw = 0.0f;
    cam->roll = 0.0f;
    cam->speed = 3.0f;
    cam->sensitivity = 20.0f;
}

void resizeCamera(Camera* cam, int width, int height) {
    cam->width = width;
    cam->height = height;
}

void cameraMatrix(Camera* cam, float fov, float nearPlane, float farPlane, Shader* shader, const char* uniform) {

    if(cam->width != getWindowWidth()) {
        cam->width = getWindowWidth();
    }
    if(cam->height != getWindowHeight()) {
        cam->height = getWindowHeight();
    }

    mat4 view;
    mat4 proj;

    glm_mat4_identity(view);
    glm_mat4_identity(proj);

    cam->direction[0] = cos(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));
    cam->direction[1] = sin(glm_rad(cam->pitch));
    cam->direction[2] = sin(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));
    glm_normalize_to(cam->direction, cam->front);

    vec3 cent;
    glm_vec3_add(cam->pos, cam->front, cent);

    glm_lookat(cam->pos, cent, cam->up, view);
    glm_perspective(glm_rad(fov), ((float)cam->width / (float)cam->height), nearPlane, farPlane, proj);

    mat4 camCross;
    glm_mat4_mul(proj, view, camCross);

    glUniformMatrix4fv(glGetUniformLocation(shader->id, uniform), 1, GL_FALSE, (GLfloat*)camCross);
}

void cameraLook(Camera* cam) {

    if(glfwGetInputMode(getWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        newMouseX = getMouseX();
        newMouseY = getMouseY();

        float dx = (float) (newMouseX - oldMouseX) * getDeltaTime();
        float dy = (float) (newMouseY - oldMouseY) * getDeltaTime();

        cam->yaw += dx * cam->sensitivity;
        cam->pitch -= dy * cam->sensitivity;

        oldMouseX = newMouseX;
        oldMouseY = newMouseY;

        if(cam->pitch > 89.9f) cam->pitch = 89.9f;
        if(cam->pitch < -89.9f) cam->pitch = -89.9f;
    }

}

void cameraMove(Camera* cam) {

    if(isKeyDown(GLFW_KEY_S)) {
        cam->pos[0] += (-cos(glm_rad(cam->yaw)) * cam->speed) * getDeltaTime();
        cam->pos[2] -= (sin(glm_rad(cam->yaw)) * cam->speed) * getDeltaTime();
    }

    if(isKeyDown(GLFW_KEY_A)) {
        cam->pos[0] += (sin(glm_rad(cam->yaw)) * cam->speed) * getDeltaTime();
        cam->pos[2] -= (cos(glm_rad(cam->yaw)) * cam->speed) * getDeltaTime();
    }

    if(isKeyDown(GLFW_KEY_D)) {
        cam->pos[0] -= (sin(glm_rad(cam->yaw)) * cam->speed) * getDeltaTime();
        cam->pos[2] += (cos(glm_rad(cam->yaw)) * cam->speed) * getDeltaTime();
    }

    if(isKeyDown(GLFW_KEY_W)) {
        cam->pos[0] -= (-cos(glm_rad(cam->yaw)) * cam->speed) * getDeltaTime();
        cam->pos[2] += (sin(glm_rad(cam->yaw)) * cam->speed) * getDeltaTime();
    }

    if(isKeyDown(GLFW_KEY_LEFT_SHIFT) || cam->speed == 1) {
        cam->speed = 8;
    } else {
        cam->speed = 3;
    }

    if(isKeyDown(GLFW_KEY_LEFT_CONTROL)){
        cam->pos[1] -= cam->speed * getDeltaTime();
    }

    if(isKeyDown(GLFW_KEY_SPACE)) {
        cam->pos[1] += cam->speed * getDeltaTime();
    }

    if(isKeyDown(GLFW_KEY_E)) {
        printf("wahoo!\n");
    }

    cameraLook(cam);

}

void cameraSetPosition(Camera* cam, vec3 npos) {
    cam->pos[0] = npos[0];
    cam->pos[1] = npos[1];
    cam->pos[2] = npos[2];
}

void cameraIncPosition(Camera* cam, vec3 inc) {
    cam->pos[0] += inc[0];
    cam->pos[1] += inc[1];
    cam->pos[2] += inc[2];   
}
