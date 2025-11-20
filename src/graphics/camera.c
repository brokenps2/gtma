#include <AL/al.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <cglm/cam.h>
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include <cglm/common.h>
#include <cglm/vec3.h>
#include <math.h>
#include "../physics/physics.h"
#include "../util/config.h"
#include "shader.h"
#include "../window/windowManager.h"
#include <SDL2/SDL.h>
#include "../window/events.h"
#include "camera.h"
#include "../audio/audio.h"

double oldMouseX = 0, oldMouseY = 0, newMouseX = 0, newMouseY = 0;

bool orthographic = false;

vec2 camPosLast;

float zoom = 16.0f/14.0f;

void gtmaCreateCamera(Camera* cam, vec3 pos) {
    cam->width = getWindowWidth();
    cam->height = getWindowHeight();

    cam->front[0] = 0.0f;
    cam->front[1] = 0.0f;
    cam->front[2] = -1.0f;

    cam->up[0] = 0.0f;
    cam->up[1] = 1.0f;
    cam->up[2] = 0.0f;

    cam->position[0] = pos[0];
    cam->position[1] = pos[1];
    cam->position[2] = pos[2];

    cam->fov = 60.0f;

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

    glm_vec3_copy(cam->position, cam->renderPos);

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
    glm_vec3_add(cam->renderPos, cam->front, cent);

    glm_lookat(cam->renderPos, cent, cam->up, view);
    if(orthographic) {
        float left = -(float)getWindowWidth() / 240, right = (float)getWindowWidth() / 240, down = -(float)getWindowWidth() / 240, up = (float)getWindowHeight() / 240;
        glm_ortho(left / zoom, right / zoom, down / zoom, up / zoom, 1.0f, 1000.0f, proj);
    } else {
        glm_perspective(glm_rad(cam->fov), ((float)cam->width / (float)cam->height), nearPlane, farPlane, proj);
    }

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
    }

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
