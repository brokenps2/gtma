#include "player.h"
#include "../window/events.h"
#include "../audio/audio.h"
#include "../window/windowManager.h"
#include "graphics/camera.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <cglm/vec3.h>
#include <math.h>

float maxSpeed = 120.0f;
float accel = 220.0f;
float forwardVelocity = 0.0f;
float backwardVelocity = 0.0f;
float leftVelocity = 0.0f;
float rightVelocity = 0.0f;
float upVelocity = 0.0f;
float downVelocity = 0.0f;
float fallingSpeed = 0.0f;

float maxSlopeHeight = 0.15f;
float slopeStep = 0.05f;

vec3 proposedPosition;

bool crouched = false;
float camLengthHalf = 0;

static float length, radius;

static bool moving = false;

Sound footstep;
Sound footstepFast;
Sound jump;

AABB calculatePlayerAABB(vec3 position, float sizeXZ, float sizeY) {
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

void gtmaSetPlayerFallingSpeed(float speed) {
    fallingSpeed = speed;
}

void gtmaPlayerCollide(Player* player, GameObjectPack* objPack) {

    if(SDL_GetRelativeMouseMode()) {
        proposedPosition[1] -= fallingSpeed * getDeltaTime();
    }

    vec3 tempPosition;
    tempPosition[1] = player->position[1];
    tempPosition[2] = player->position[2];
    tempPosition[0] = proposedPosition[0];

    player->aabb = calculatePlayerAABB(tempPosition, radius, length);

    if (!updatePlayerPhysics(objPack, player)) {
        player->position[0] = tempPosition[0];
    } else {
        for (float step = slopeStep; step <= maxSlopeHeight; step += slopeStep) {
            tempPosition[1] = player->position[1] + step;
            player->aabb = calculatePlayerAABB(tempPosition, radius, length);

            if (!updatePlayerPhysics(objPack, player)) {
                player->position[0] = tempPosition[0];
                player->position[1] = tempPosition[1];
                break;
            }
        }
    }

    tempPosition[0] = player->position[0];
    tempPosition[1] = player->position[1];
    tempPosition[2] = proposedPosition[2];

    player->aabb = calculatePlayerAABB(tempPosition, radius, length);

    if (!updatePlayerPhysics(objPack, player)) {
        player->position[2] = tempPosition[2];
    } else {
        for (float step = slopeStep; step <= maxSlopeHeight; step += slopeStep) {
            tempPosition[1] = player->position[1] + step;
            player->aabb = calculatePlayerAABB(tempPosition, radius, length);

            if (!updatePlayerPhysics(objPack, player)) {
                player->position[2] = tempPosition[2];
                player->position[1] = tempPosition[1];
                break;
            }
        }
    }
        

    tempPosition[1] = proposedPosition[1];
    tempPosition[2] = player->position[2];

    if (isKeyDown(SDL_SCANCODE_SPACE) && fallingSpeed == 0) { 
        gtmaSetSoundPosition(&jump, player->position);
        gtmaPlaySound(&jump);
        fallingSpeed = -20.0f;
    }

    player->aabb = calculatePlayerAABB(tempPosition, radius, length);

    if (!updatePlayerPhysics(objPack, player)) {
        player->position[1] = tempPosition[1];
        fallingSpeed += (9.81f * 4) * getDeltaTime();
    } else {
        fallingSpeed = 0.0f;
    }


}

void gtmaCreatePlayer(Player* player, Camera *camera, int health, float collisionRadius, float collisionLength) {
    glm_vec3_copy(camera->position, player->position);
    player->health = 100;
    player->camera = camera;
    player->aabb = calculatePlayerAABB(player->position, collisionRadius, collisionLength);
    radius = collisionRadius;
    length = collisionLength;
    forwardVelocity = 0;
    backwardVelocity = 0;
    fallingSpeed = 0;
    upVelocity = 0;
    downVelocity = 0;
    leftVelocity = 0;
    rightVelocity = 0;

    gtmaCreateSound(&footstep, "audio/footstep.wav", true, 1, player->position);
    gtmaCreateSound(&footstepFast, "audio/footstepfast.wav", true, 1, player->position);
    alSourcef(footstep.sourceID, AL_GAIN, 0.5);
    alSourcef(footstepFast.sourceID, AL_GAIN, 0.5);
    gtmaCreateSound(&jump, "audio/jump.wav", 0, 1, player->position);
}

int counter = 0;

void gtmaPlayerMove(Player *player, GameObjectPack *objPack, bool flying) {

    bool bob = false;

    proposedPosition[0] = player->position[0];
    proposedPosition[1] = player->position[1];
    proposedPosition[2] = player->position[2];

    if(flying) maxSpeed = 60;

    if (isKeyDown(SDL_SCANCODE_W)) {
        forwardVelocity += accel * getDeltaTime();
        if (forwardVelocity > maxSpeed) forwardVelocity = maxSpeed;
        bob = true;
    } else {
        forwardVelocity -= (accel / 1.5) * getDeltaTime();
        if (forwardVelocity < 0) forwardVelocity = 0;
    }

    if (isKeyDown(SDL_SCANCODE_S)) {
        backwardVelocity += accel * getDeltaTime();
        if (backwardVelocity > maxSpeed) backwardVelocity = maxSpeed;
        bob = true;
    } else {
        backwardVelocity -= (accel / 1.5) * getDeltaTime();
        if (backwardVelocity < 0) backwardVelocity = 0;
    }

    if (isKeyDown(SDL_SCANCODE_A)) {
        leftVelocity += accel * getDeltaTime();
        if (leftVelocity > maxSpeed) leftVelocity = maxSpeed;
        bob = true;
    } else {
        leftVelocity -= (accel / 1.5) * getDeltaTime();
        if (leftVelocity < 0) leftVelocity = 0;
    }

    if (isKeyDown(SDL_SCANCODE_D)) {
        rightVelocity += accel * getDeltaTime();
        if (rightVelocity > maxSpeed) rightVelocity = maxSpeed;
        bob = true;
    } else {
        rightVelocity -= (accel / 1.5) * getDeltaTime();
        if (rightVelocity < 0) rightVelocity = 0;
    }

    bool playedFootstepSound = false;

    if((forwardVelocity > 0 || backwardVelocity > 0 || leftVelocity > 0 || rightVelocity > 0)) {
        gtmaSetSoundPosition(&footstep, player->position);
        gtmaSetSoundPosition(&footstepFast, player->position);
        moving = true;
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
        moving = false;
    }

    Camera* cam = player->camera;

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
        glm_vec3_copy(proposedPosition, player->position);
        glm_vec3_copy(player->position, cam->position);
        gtmaCameraLook(cam);
        return;
    }

    if(isKeyDown(SDL_SCANCODE_LCTRL)) {
        crouched = true;
    } else {
        crouched = false;
    }

    gtmaPlayerCollide(player, objPack);


    if(isKeyDown(SDL_SCANCODE_LSHIFT)) {
        maxSpeed = 32;
    } else {
        maxSpeed = 20;
    }

    player->position[0] = roundf(player->position[0] * 100) / 100;
    player->position[1] = roundf(player->position[1] * 1000) / 1000;
    player->position[2] = roundf(player->position[2] * 100) / 100;

    glm_vec3_copy(player->position, player->camera->position);

    gtmaCameraLook(cam);
    
}
