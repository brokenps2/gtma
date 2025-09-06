#include <AL/al.h>
#include <SDL2/SDL_timer.h>
#include <cglm/cam.h>
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include <cglm/common.h>
#include <cglm/vec3.h>
#include <math.h>
#include "physics/physics.h"
#include "util/config.h"
#include "graphics/shader.h"
#include "window/windowManager.h"
#include <SDL2/SDL.h>
#include "window/events.h"
#include "graphics/camera.h"
#include "audio/audio.h"
#include <GLFW/glfw3.h>

double oldMouseX = 0, oldMouseY = 0, newMouseX = 0, newMouseY = 0;

float fov = 75.0f;

static float camLength, camRadius;

Sound footstep;
Sound footstepFast;
Sound jump;

AABB calculateCameraAABB(vec3 position, float sizeXZ, float sizeY) {
    vec3 halfSize = {sizeXZ / 2, sizeY / 2, sizeXZ / 2};
    AABB box;

    box.minX = position[0] - halfSize[0];
    box.minY = position[1] - halfSize[1] * 2;
    box.minZ = position[2] - halfSize[2];

    box.maxX = position[0] + halfSize[0];
    box.maxY = position[1] + halfSize[1];
    box.maxZ = position[2] + halfSize[2];

    return box;
}

void gtmaCreateCamera(Camera* cam, float length, float radius, vec3 pos) {
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

    cam->pitch = 0.0f;
    cam->yaw = 0.0f;
    cam->roll = 0.0f;
    cam->sensitivity = (float)cfgLookupInt("mouseSensitivity") / 100;

    cam->aabb = calculateCameraAABB(cam->position, radius, length);
    camLength = length;
    camRadius = radius;

    gtmaCreateSound(&footstep, "audio/footstep.wav", true, 1, cam->position);
    gtmaCreateSound(&footstepFast, "audio/footstepfast.wav", true, 1, cam->position);
    alSourcef(footstep.sourceID, AL_GAIN, 0.5);
    alSourcef(footstepFast.sourceID, AL_GAIN, 0.5);
    gtmaCreateSound(&jump, "audio/jump.wav", 0, 1, cam->position);

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

    cam->aabb = calculateCameraAABB(cam->position, camRadius, camLength);

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

float maxSpeed = 120.0f;
float accel = 76.0f;
float forwardVelocity = 0.0f;
float backwardVelocity = 0.0f;
float leftVelocity = 0.0f;
float rightVelocity = 0.0f;
float upVelocity = 0.0f;
float downVelocity = 0.0f;
float fallingSpeed = 0.0f;
float lastFallingSpeed = 0.0f;

float maxSlopeHeight = 0.15f;
float slopeStep = 0.05f;

vec3 proposedPosition;

void cameraCollide(Camera* cam, GameObjectPack* objPack) {
    if (isKeyDown(SDL_SCANCODE_SPACE) && fallingSpeed == 0) { 
        gtmaSetSoundPosition(&jump, cam->position);
        gtmaPlaySound(&jump);
        fallingSpeed = -20.0f;
    }

    if(SDL_GetRelativeMouseMode()) {
        proposedPosition[1] -= fallingSpeed * getDeltaTime();
    }

    vec3 tempPosition;
    tempPosition[1] = cam->position[1];
    tempPosition[2] = cam->position[2];
    tempPosition[0] = proposedPosition[0];

    cam->aabb = calculateCameraAABB(tempPosition, camRadius, camLength);

    if (!updateCameraPhysics(objPack, cam)) {
        cam->position[0] = tempPosition[0];
    } else {
        for (float step = slopeStep; step <= maxSlopeHeight; step += slopeStep) {
            tempPosition[1] = cam->position[1] + step;
            cam->aabb = calculateCameraAABB(tempPosition, camRadius, camLength);

            if (!updateCameraPhysics(objPack, cam)) {
                cam->position[0] = tempPosition[0];
                cam->position[1] = tempPosition[1];
                break;
            }
        }
    }

    tempPosition[0] = cam->position[0];
    tempPosition[1] = cam->position[1];
    tempPosition[2] = proposedPosition[2];

    cam->aabb = calculateCameraAABB(tempPosition, camRadius, camLength);

    if (!updateCameraPhysics(objPack, cam)) {
        cam->position[2] = tempPosition[2];
    } else {
        for (float step = slopeStep; step <= maxSlopeHeight; step += slopeStep) {
            tempPosition[1] = cam->position[1] + step;
            cam->aabb = calculateCameraAABB(tempPosition, camRadius, camLength);

            if (!updateCameraPhysics(objPack, cam)) {
                cam->position[2] = tempPosition[2];
                cam->position[1] = tempPosition[1];
                break;
            }
        }
    }
        

    tempPosition[1] = proposedPosition[1];
    tempPosition[2] = cam->position[2];

    cam->aabb = calculateCameraAABB(tempPosition, camRadius, camLength);

    lastFallingSpeed = fallingSpeed;

    if (!updateCameraPhysics(objPack, cam)) {
        cam->position[1] = tempPosition[1];
        fallingSpeed += (9.81f * 4) * getDeltaTime();
    } else {
        fallingSpeed = 0.0f;
    }
}

float fallingSpeedRounded = 0;

void gtmaCameraMove(Camera* cam, GameObjectPack* objPack, bool flying) {

    proposedPosition[0] = cam->position[0];
    proposedPosition[1] = cam->position[1];
    proposedPosition[2] = cam->position[2];

    if(flying) maxSpeed = 60;

    if (isKeyDown(SDL_SCANCODE_W)) {
        forwardVelocity += accel * getDeltaTime();
        if (forwardVelocity > maxSpeed) forwardVelocity = maxSpeed;
    } else {
        forwardVelocity -= accel * getDeltaTime();
        if (forwardVelocity < 0) forwardVelocity = 0;
    }

    if (isKeyDown(SDL_SCANCODE_S)) {
        backwardVelocity += accel * getDeltaTime();
        if (backwardVelocity > maxSpeed) backwardVelocity = maxSpeed;
    } else {
        backwardVelocity -= accel * getDeltaTime();
        if (backwardVelocity < 0) backwardVelocity = 0;
    }

    if (isKeyDown(SDL_SCANCODE_A)) {
        leftVelocity += accel * getDeltaTime();
        if (leftVelocity > maxSpeed) leftVelocity = maxSpeed;
    } else {
        leftVelocity -= accel * getDeltaTime();
        if (leftVelocity < 0) leftVelocity = 0;
    }

    if (isKeyDown(SDL_SCANCODE_D)) {
        rightVelocity += accel * getDeltaTime();
        if (rightVelocity > maxSpeed) rightVelocity = maxSpeed;
    } else {
        rightVelocity -= accel * getDeltaTime();
        if (rightVelocity < 0) rightVelocity = 0;
    }

    bool playedFootstepSound = false;

    if((forwardVelocity > 0 || backwardVelocity > 0 || leftVelocity > 0 || rightVelocity > 0)) {
        gtmaSetSoundPosition(&footstep, cam->position);
        gtmaSetSoundPosition(&footstepFast, cam->position);
        if(!playedFootstepSound) {
            playedFootstepSound = true;
            if(isKeyDown(SDL_SCANCODE_LSHIFT)) {
                gtmaChangePitch(&footstepFast, ((float)(rand() % (12 + 1 - 6) + 6)) / 10);
                gtmaStopSound(&footstep);
                gtmaPlaySound(&footstepFast);
            } else {
                gtmaChangePitch(&footstep, ((float)(rand() % (12 + 1 - 6) + 6)) / 10);
                gtmaStopSound(&footstepFast);
                gtmaPlaySound(&footstep);
            }
        }
    } else {
        gtmaStopSound(&footstep);
        gtmaStopSound(&footstepFast);
        playedFootstepSound = false;
    }

    if(SDL_GetRelativeMouseMode()) {
        proposedPosition[0] -= (-cos(glm_rad(cam->yaw)) * forwardVelocity) * getDeltaTime();
        proposedPosition[2] += (sin(glm_rad(cam->yaw)) * forwardVelocity) * getDeltaTime();

        proposedPosition[0] += (-cos(glm_rad(cam->yaw)) * backwardVelocity) * getDeltaTime();
        proposedPosition[2] -= (sin(glm_rad(cam->yaw)) * backwardVelocity) * getDeltaTime();

        proposedPosition[0] += (sin(glm_rad(cam->yaw)) * leftVelocity) * getDeltaTime();
        proposedPosition[2] -= (cos(glm_rad(cam->yaw)) * leftVelocity) * getDeltaTime();

        proposedPosition[0] -= (sin(glm_rad(cam->yaw)) * rightVelocity) * getDeltaTime();
        proposedPosition[2] += (cos(glm_rad(cam->yaw)) * rightVelocity) * getDeltaTime();

        proposedPosition[1] += upVelocity * getDeltaTime();
        proposedPosition[1] -= downVelocity * getDeltaTime();
    }
    
    if(flying) {
        if (isKeyDown(SDL_SCANCODE_SPACE)) {
            upVelocity += accel * getDeltaTime();
            if (upVelocity > 64) upVelocity = 64;
        }
        if (isKeyDown(SDL_SCANCODE_LCTRL)) {
            downVelocity += accel * getDeltaTime();
            if (downVelocity > 64) downVelocity = 64;
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
        gtmaCameraLook(cam);
        return;
    }

    cameraCollide(cam, objPack);

    float maxFov = 80.5;

    if(isKeyDown(SDL_SCANCODE_LSHIFT)) {
        maxSpeed = 32;
        fov += 64 * getDeltaTime();
        if(fov > maxFov) fov = maxFov;
    } else {
        maxSpeed = 20;
        fov -= 64 * getDeltaTime();
        if(fov <= 75) fov = 75;
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
