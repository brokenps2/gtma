#include <SDL2/SDL_timer.h>
#include <cglm/cam.h>
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include <cglm/common.h>
#include <cglm/vec3.h>
#include <math.h>
#include "util/config.h"
#include "graphics/shader.h"
#include "window/windowManager.h"
#include <SDL2/SDL.h>
#include "window/events.h"
#include "graphics/camera.h"
#include "physics/physics.h"
#include "graphics/renderer.h"
#include <GLFW/glfw3.h>

double oldMouseX = 0, oldMouseY = 0, newMouseX = 0, newMouseY = 0;

float fov = 80.0f;

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
}

void gtmaResizeCamera(Camera* cam, int width, int height) {
    cam->width = width;
    cam->height = height;
}

void gtmaCameraMatrix(Camera* cam, float nearPlane, float farPlane, Shader* shader) {

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

    gtmaSetMatrix(shader, "camCross", camCross);
    gtmaSetMatrix(shader, "viewMatrix", view);
    gtmaSetMatrix(shader, "projMatrix", proj);
}

void gtmaCameraLook(Camera* cam) {

    if(SDL_GetRelativeMouseMode()) {
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

        if(isKeyDown(SDL_SCANCODE_LEFT)) {
            cam->yaw -= cam->sensitivity * getDeltaTime() * 600;
        }
        if(isKeyDown(SDL_SCANCODE_RIGHT)) {
            cam->yaw += cam->sensitivity * getDeltaTime() * 600;
        }
        if(isKeyDown(SDL_SCANCODE_UP)) {
            cam->pitch += cam->sensitivity * getDeltaTime() * 600;
        }
        if(isKeyDown(SDL_SCANCODE_DOWN)) {
            cam->pitch -= cam->sensitivity * getDeltaTime() * 600;
        }   
    }

}

float maxSpeed = 12.0f;
float accel = 56.0f;
float forwardVelocity = 0.0f;
float backwardVelocity = 0.0f;
float leftVelocity = 0.0f;
float rightVelocity = 0.0f;
float upVelocity = 0.0f;
float downVelocity = 0.0f;
float verticalSpeed = 0.0f;

void gtmaCameraMove(Camera* cam, bool spectating) {

    vec3 proposedPosition;
    proposedPosition[0] = cam->position[0];
    proposedPosition[1] = cam->position[1];
    proposedPosition[2] = cam->position[2];

    if (isKeyDown(SDL_SCANCODE_W)) {
        forwardVelocity += accel * getDeltaTime();
        if (forwardVelocity > maxSpeed) forwardVelocity = maxSpeed;
    }
    if (isKeyDown(SDL_SCANCODE_S)) {
        backwardVelocity += accel * getDeltaTime();
        if (backwardVelocity > maxSpeed) backwardVelocity = maxSpeed;
    }
    if (!isKeyDown(SDL_SCANCODE_W)) {
        forwardVelocity -= accel * getDeltaTime();
        if (forwardVelocity < 0) forwardVelocity = 0;
    }
    if (!isKeyDown(SDL_SCANCODE_S)) {
        backwardVelocity -= accel * getDeltaTime();
        if (backwardVelocity < 0) backwardVelocity = 0;
    }

    if(SDL_GetRelativeMouseMode()) {
        proposedPosition[0] -= (-cos(glm_rad(cam->yaw)) * forwardVelocity) * getDeltaTime();
        proposedPosition[2] += (sin(glm_rad(cam->yaw)) * forwardVelocity) * getDeltaTime();

        proposedPosition[0] += (-cos(glm_rad(cam->yaw)) * backwardVelocity) * getDeltaTime();
        proposedPosition[2] -= (sin(glm_rad(cam->yaw)) * backwardVelocity) * getDeltaTime();
    }

    if (isKeyDown(SDL_SCANCODE_A)) {
        leftVelocity += accel * getDeltaTime();
        if (leftVelocity > maxSpeed) leftVelocity = maxSpeed;
    }
    if (isKeyDown(SDL_SCANCODE_D)) {
        rightVelocity += accel * getDeltaTime();
        if (rightVelocity > maxSpeed) rightVelocity = maxSpeed;
    }
    if (!isKeyDown(SDL_SCANCODE_A)) {
        leftVelocity -= accel * getDeltaTime();
        if (leftVelocity < 0) leftVelocity = 0;
    }
    if (!isKeyDown(SDL_SCANCODE_D)) {
        rightVelocity -= accel * getDeltaTime();
        if (rightVelocity < 0) rightVelocity = 0;
    }

    if(SDL_GetRelativeMouseMode()) {
        proposedPosition[0] += (sin(glm_rad(cam->yaw)) * leftVelocity) * getDeltaTime();
        proposedPosition[2] -= (cos(glm_rad(cam->yaw)) * leftVelocity) * getDeltaTime();

        proposedPosition[0] -= (sin(glm_rad(cam->yaw)) * rightVelocity) * getDeltaTime();
        proposedPosition[2] += (cos(glm_rad(cam->yaw)) * rightVelocity) * getDeltaTime();

        proposedPosition[1] += upVelocity * getDeltaTime();
        proposedPosition[1] -= downVelocity * getDeltaTime();
    }
    
    if(!spectating) {
        //gravity
        if (isKeyDown(SDL_SCANCODE_SPACE) && verticalSpeed == 0) { 
            verticalSpeed = -20.0f;
        }

        if(SDL_GetRelativeMouseMode()) {
            proposedPosition[1] -= verticalSpeed * getDeltaTime();
        }

        vec3 tempPosition;
        tempPosition[0] = cam->position[0];
        tempPosition[1] = cam->position[1];
        tempPosition[2] = cam->position[2];

        const float maxSlopeHeight = 0.15f;
        const float slopeStep = 0.05f;

        tempPosition[0] = proposedPosition[0];
        if (!handleCamPhysics(&tempPosition, getObjPack(), 2.5f, 5.0f)) {
            cam->position[0] = tempPosition[0];
        } else {
            for (float step = slopeStep; step <= maxSlopeHeight; step += slopeStep) {
                tempPosition[1] = cam->position[1] + step;
                if (!handleCamPhysics(&tempPosition, getObjPack(), 2.5f, 5.0f)) {
                    cam->position[0] = tempPosition[0];
                    cam->position[1] = tempPosition[1];
                    break;
                }
            }
        }

        tempPosition[0] = cam->position[0];
        tempPosition[1] = cam->position[1];
        tempPosition[2] = cam->position[2];
        tempPosition[2] = proposedPosition[2];
        if (!handleCamPhysics(&tempPosition, getObjPack(), 2.5f, 5.0f)) {
            cam->position[2] = tempPosition[2];
        } else {
            for (float step = slopeStep; step <= maxSlopeHeight; step += slopeStep) {
                tempPosition[1] = cam->position[1] + step; // Adjust upward
                if (!handleCamPhysics(&tempPosition, getObjPack(), 2.5f, 5.0f)) {
                    cam->position[2] = tempPosition[2];
                    cam->position[1] = tempPosition[1];
                    break;
                }
            }
        }
        

        tempPosition[0] = cam->position[0];
        tempPosition[1] = proposedPosition[1];
        tempPosition[2] = cam->position[2];
        if (!handleCamPhysics(&tempPosition, getObjPack(), 2.5f, 5.0f)) {
            cam->position[1] = tempPosition[1];
            verticalSpeed += (9.81f * 4) * getDeltaTime();
        } else {
            verticalSpeed = 0.0f;
        }

    } else {
        if (isKeyDown(SDL_SCANCODE_SPACE)) {
            upVelocity += accel * getDeltaTime();
            if (upVelocity > maxSpeed) upVelocity = maxSpeed;
        }
        if (isKeyDown(SDL_SCANCODE_LCTRL)) {
            downVelocity += accel * getDeltaTime();
            if (downVelocity > maxSpeed) downVelocity = maxSpeed;
        }
        if (!isKeyDown(SDL_SCANCODE_SPACE)) {
            upVelocity -= accel * getDeltaTime();
            if (upVelocity < 0) upVelocity = 0;
        }
        if (!isKeyDown(SDL_SCANCODE_LCTRL)) {
            downVelocity -= accel * getDeltaTime();
            if (downVelocity < 0) downVelocity = 0;
        }

        glm_vec3_copy(proposedPosition, cam->position);
    }

    float maxFov = 95.5;

    if(isKeyDown(SDL_SCANCODE_LSHIFT)) {
        maxSpeed = 21;
        fov += 64 * getDeltaTime();
        if(fov > maxFov) fov = maxFov;
    } else {
        maxSpeed = 9;
        fov -= 64 * getDeltaTime();
        if(fov <= 90) fov = 90;
    }

    cam->position[0] = roundf(cam->position[0] * 100) / 100;
    cam->position[1] = roundf(cam->position[1] * 1000) / 1000;
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
