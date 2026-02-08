#include "scenes.h"
#include <SDL2/SDL.h>
#include "../physics/physics.h"
#include "../graphics/texture.h"
#include "../objects/player.h"
#include "../objects/objects.h"
#include "../window/events.h"
#include "../graphics/renderer.h"
#include "../window/windowManager.h"
#include "../audio/audio.h"
#include "objects/entities.h"
#include <SDL2/SDL_mouse.h>
#include <cglm/vec3.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

Scene* currentScene = NULL;

static ScreenObject crosshair;
static Texture regularCrosshair;
static Texture highlightedCrosshair;
static ScreenObject loadingScreen;
static ScreenObject pauseScreen;

static Sound beep;
static Sound doorSound;

bool paused = false;

int editMode = 1;
int objIndex = 0;
int lightIndex = 0;

void gtmaToggleCrosshair(Scene* scene, bool toggle) {
    if(toggle) {
        if(crosshair.inPack) {
            return;
        } else {
            gtmaAddScreenObject(&crosshair, scene->screenPack);
        }
    } else {
        if(crosshair.inPack) {
            gtmaRemoveScreenObjectID(scene->screenPack, crosshair.packID);
        } else {
            return;
        }
    }
}

void gtmaLoadInitialScene(Scene* scene) {
    currentScene = scene;
    currentScene->init();
}

Scene* gtmaGetCurrentScene() {
    return currentScene;
}

void gtmaInitScene(Scene* scene, Player* player, GameObjectPack* objectPack, ScreenObjectPack* screenObjPack, EntityPack* entityPack, vec3 spawnpoint, bool debug) {
    scene->player = player;
    scene->screenPack = screenObjPack;
    scene->objPack = objectPack;
    scene->entPack = entityPack;

    gtmaCreateScreenObject(&crosshair, "images/crosshair.png", "uitest", (vec2){((float)getWindowWidth() / 2), ((float)getWindowHeight() / 2)}, (vec2){10, 10}, 0, GTMA_NONE);
    gtmaCreateTexture(&regularCrosshair, "images/crosshair.png");
    gtmaCreateTexture(&highlightedCrosshair, "images/crosshairselected.png");

    gtmaCreateScreenObject(&pauseScreen, "images/paused.png", "pause", (vec2){((float)getWindowWidth()/2), ((float)getWindowHeight()/2)}, (vec2){400, 80}, 0, GTMA_INVISIBLE);

    gtmaCreateScreenObject(&loadingScreen, "images/loading.png", "loading", (vec2){(float)getWindowWidth()/2, (float)getWindowHeight() / 2}, (vec2){400, 60}, 0, GTMA_INVISIBLE);

    gtmaAddScreenObject(&crosshair, scene->screenPack);
    gtmaAddScreenObject(&loadingScreen, scene->screenPack);
    gtmaAddScreenObject(&pauseScreen, scene->screenPack);

    gtmaCreateSound(&beep, "audio/beep.wav", false, 1, player->position);
    gtmaStopSound(&beep);

    gtmaCreateSound(&doorSound, "audio/door.wav", false, 1, player->position);
    gtmaStopSound(&doorSound);

    glm_vec3_copy(spawnpoint, scene->spawnpoint);

    scene->debugInfo = debug;
}

void gtmaSetEditMode(int set) {
    editMode = set;
}

void gtmaPlayDoorSound() {
    gtmaStopSound(&doorSound);
    gtmaPlaySoundFrom(&doorSound, 0);
}

void gtmaBeep() {
    gtmaStopSound(&beep);
    gtmaPlaySoundFrom(&beep, 0);
}

static float transitionTimer = 0.0f;
static float transitionDuration = 0.5f; // seconds
static bool transitioning = false;
Scene* nextScene;

void switchScene(Scene* newScene) {
    transitioning = true;
    transitionTimer = transitionDuration;
    gtmaToggleControls(false);
    loadingScreen.flags &= ~GTMA_INVISIBLE;
    nextScene = newScene;
}

bool checkWarp() {
    if (transitioning) {
        transitionTimer -= getDeltaTime();
        if (transitionTimer <= 0.0f) {
            transitioning = false;
            loadingScreen.flags &= ~GTMA_INVISIBLE;
            if (currentScene && currentScene->dispose)
                currentScene->dispose();
            currentScene = nextScene;
            if (currentScene && currentScene->init)
                currentScene->init();
            gtmaToggleControls(true);
        }
        return true;
    }
    return false;
}

static bool lastState = false;

bool gtmaIsPaused() {
    return paused;
}

bool gtmaUpdateScene(Scene* scene, Player* player) {

    if (paused) {
        if (isKeyPressed(SDL_SCANCODE_ESCAPE)) {
            paused = false;
            pauseScreen.flags |= GTMA_INVISIBLE;
            SDL_SetRelativeMouseMode(true);
            gtmaSetFBOBrightness(1);
        }
        if (isKeyPressed(SDL_SCANCODE_Q)) {
            gtmaSetRunning(false);
        }
        /*
        if (isKeyPressed(SDL_SCANCODE_T)) {
            paused = false;
            pauseScreen.flags &= ~GTMA_INVISIBLE;
            SDL_SetRelativeMouseMode(true);
            gtmaSetFBOBrightness(1);
            //switchScene(&titleScreen);
        }
        */
    } else {
        if (isKeyPressed(SDL_SCANCODE_ESCAPE)) {
            pauseScreen.flags &= ~GTMA_INVISIBLE;
            SDL_SetRelativeMouseMode(false);
            gtmaSetFBOBrightness(0.5);
            paused = true;
        }
    }

    for(int i = 0; i < scene->entPack->entityCount; i++) {
        gtmaUpdateEntity(scene->entPack->entities[i], scene->entPack, player->camera);
    }

    if (isLeftPressed()) {
        Entity* picked = pickEntity(scene->entPack, player->camera);

        if (picked && !(picked->flags & GTMA_INVISIBLE)) {
            picked->health -= 10;
            picked->damaged = true;
        }
    }

    if(player->health < 0) {
        glm_vec3_copy(scene->spawnpoint, player->position);
        player->health = 100;
    }

    if(player->camera->position[1] < -200) {
        player->health -= 200 * getDeltaTime();
    }

    glm_vec3_copy(player->position, doorSound.position);
    glm_vec3_copy(player->position, beep.position);

    gtmaUpdateAudio(player->position, player->camera->direction);

    bool isHighlighted;
    if(pickObject(scene->objPack, scene->player->camera) != NULL) {
        isHighlighted = true;
    } else {
        isHighlighted = false;
    }

    if (isHighlighted != lastState) {
        if (isHighlighted) {
            crosshair.model.meshes[0].texture.id = highlightedCrosshair.id;
        } else {
            crosshair.model.meshes[0].texture.id = regularCrosshair.id;
        }
        lastState = isHighlighted;
    }

    if(scene->debugInfo) {
        printf("\rx:%f y:%f z:%f    pitch:%f yaw:%f    %ffps", player->position[0], player->position[1], player->position[2], player->camera->pitch, player->camera->yaw, gtmaGetFramerate());
        fflush(stdout);
    }

    // Always update screen positions
    crosshair.position[0] = (float)getWindowWidth() / 2;
    crosshair.position[1] = (float)getWindowHeight() / 2;
    loadingScreen.position[0] = (float)getWindowWidth() / 2;
    loadingScreen.position[1] = (float)getWindowHeight() / 2;

    if (checkWarp()) {
        return true;
    }

    return paused;
}

void gtmaSpawnLightGrid(PointLightPack* pack, float brightness, int lightCount, float maxPos, float minPos) {
    float step = (maxPos - minPos) / (lightCount - 1);

    for (int x = 0; x < lightCount; x++) {
        for (int z = 0; z < lightCount; z++) {
            vec3 position = {
                minPos + x * step,
                5.0f,                     // Y height of lights
                minPos + z * step
            };

            vec3 color = { brightness, brightness, brightness};  // white light
            unsigned int flags = 0;

            gtmaCreateAndAddPointLight(pack, (vec3){position[0], 23, position[2]}, color, flags);
            gtmaCreateAndAddPointLight(pack, (vec3){position[0], 6, position[2]}, color, flags);
            pack->lights[pack->lightCount - 1]->range = 0.3;
        }
    }
}
