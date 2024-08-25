#include <cglm/cam.h>
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include <cglm/common.h>
#include <math.h>
#include "Config.h"
#include "Shader.h"
#include "WindowManager.h"
#include "Input.h"
#include "Camera.h"
#include <GLFW/glfw3.h>

double oldMouseX = 0, oldMouseY = 0, newMouseX = 0, newMouseY = 0;

void gtmaCreateCamera(Camera* cam, int width, int height, vec3 pos) {
    cam->width = width;
    cam->height = height;

    cam->front[0] = 0.0f;
    cam->front[1] = 0.0f;
    cam->front[2] = -1.0f;

    cam->up[0] = 0.0f;
    cam->up[1] = 1.0f;
    cam->up[2] = 0.0f;

    cam->position[0] = pos[0];
    cam->position[1] = pos[1];
    cam->position[2] = pos[2];

    cam->pitch = 0.0f;
    cam->yaw = 0.0f;
    cam->roll = 0.0f;
    cam->sensitivity = (float)cfgLookupInt("mouseSensitivity") / 100;

    cam->radius = 1;
}

void gtmaResizeCamera(Camera* cam, int width, int height) {
    cam->width = width;
    cam->height = height;
}

void gtmaCameraMatrix(Camera* cam, float fov, float nearPlane, float farPlane, Shader* shader, const char* uniform) {

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
    glm_vec3_add(cam->position, cam->front, cent);

    glm_lookat(cam->position, cent, cam->up, view);
    glm_perspective(glm_rad(fov), ((float)cam->width / (float)cam->height), nearPlane, farPlane, proj);

    mat4 camCross;
    glm_mat4_mul(proj, view, camCross);

    gtmaSetMatrix(shader, uniform, camCross);
}

void gtmaCameraLook(Camera* cam) {

    if(glfwGetInputMode(getWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        newMouseX = getMouseX();
        newMouseY = getMouseY();

        float dx = (float)(newMouseX - oldMouseX);
        float dy = (float)(newMouseY - oldMouseY);

        cam->yaw += dx * cam->sensitivity;
        cam->pitch -= dy * cam->sensitivity;

        oldMouseX = newMouseX;
        oldMouseY = newMouseY;

        if(cam->pitch > 89.99f) cam->pitch = 89.99f;
        if(cam->pitch < -89.99f) cam->pitch = -89.99f;

        if(cam->yaw < 0) cam->yaw = 360 + cam->yaw;
        if(cam->roll < 0) cam->roll = 360 + cam->roll;

        if(cam->yaw >= 360) cam->yaw = cam->yaw - 360;
        if(cam->yaw <= -360) cam->yaw = cam->yaw + 360;

        if(cam->roll >= 360) cam->roll = cam->roll - 360;
        if(cam->roll <= -360) cam->roll = cam->roll + 360;
    }

}

float maxSpeed = 12;
float accel = 42;
float forwardVelocity = 0;
float backwardVelocity = 0;
float leftVelocity = 0;
float rightVelocity = 0;

void gtmaCameraMove(Camera* cam) {

    if(isKeyDown(GLFW_KEY_W)) {
        forwardVelocity += accel * getDeltaTime();
        if(forwardVelocity > maxSpeed) {
            forwardVelocity = maxSpeed;
        }
    } 
    if(isKeyDown(GLFW_KEY_S)) {
        backwardVelocity += accel * getDeltaTime();
        if(backwardVelocity > maxSpeed) {
            backwardVelocity = maxSpeed;
        }
    }
    if(isKeyDown(GLFW_KEY_A)) {
        leftVelocity += accel * getDeltaTime();
        if(leftVelocity > maxSpeed) {
            leftVelocity = maxSpeed;
        }
    }
    if(isKeyDown(GLFW_KEY_D)) {
        rightVelocity += accel * getDeltaTime();
        if(rightVelocity > maxSpeed) {
            rightVelocity = maxSpeed;
        }
    }



    if(!isKeyDown(GLFW_KEY_W)) {
        if(forwardVelocity != 0) {
            forwardVelocity -= accel * getDeltaTime();
            if(forwardVelocity < 0) {
                forwardVelocity = 0;
            }
        }
    }
    if(!isKeyDown(GLFW_KEY_S)) {
        if(backwardVelocity != 0) {
            backwardVelocity -= accel * getDeltaTime();
            if(backwardVelocity < 0) {
                backwardVelocity = 0;
            }
        }
    }
    if(!isKeyDown(GLFW_KEY_A)) {
        if(leftVelocity != 0) {
            leftVelocity -= accel * getDeltaTime();
            if(leftVelocity < 0) {
                leftVelocity = 0;
            }
        }
    }
    if(!isKeyDown(GLFW_KEY_D)) {
         if(rightVelocity != 0) {
            rightVelocity -= accel * getDeltaTime();
            if(rightVelocity < 0) {
                rightVelocity = 0;
            }
        }
    }

    cam->position[0] -= (-cos(glm_rad(cam->yaw)) * forwardVelocity) * getDeltaTime();
    cam->position[2] += (sin(glm_rad(cam->yaw)) * forwardVelocity) * getDeltaTime();

    cam->position[0] += (-cos(glm_rad(cam->yaw)) * backwardVelocity) * getDeltaTime();
    cam->position[2] -= (sin(glm_rad(cam->yaw)) * backwardVelocity) * getDeltaTime();

    cam->position[0] += (sin(glm_rad(cam->yaw)) * leftVelocity) * getDeltaTime();
    cam->position[2] -= (cos(glm_rad(cam->yaw)) * leftVelocity) * getDeltaTime();

    cam->position[0] -= (sin(glm_rad(cam->yaw)) * rightVelocity) * getDeltaTime();
    cam->position[2] += (cos(glm_rad(cam->yaw)) * rightVelocity) * getDeltaTime();

    if(isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
        maxSpeed = 18;
    } else {
        maxSpeed = 12;
    }

    if(isKeyDown(GLFW_KEY_LEFT_CONTROL)){
        cam->position[1] -= 8 * getDeltaTime();
    }

    if(isKeyDown(GLFW_KEY_SPACE)) {
        cam->position[1] += 8 * getDeltaTime();
    }

    cam->position[0] = roundf(cam->position[0] * 100) / 100;
    cam->position[1] = roundf(cam->position[1] * 100) / 100;
    cam->position[2] = roundf(cam->position[2] * 100) / 100;

    gtmaCameraLook(cam);
}

void gtmaCameraSetPosition(Camera* cam, vec3 npos) {
    cam->position[0] = npos[0];
    cam->position[1] = npos[1];
    cam->position[2] = npos[2];
}

void gtmaCameraIncPosition(Camera* cam, vec3 inc) {
    cam->position[0] += inc[0];
    cam->position[1] += inc[1];
    cam->position[2] += inc[2];   
}
