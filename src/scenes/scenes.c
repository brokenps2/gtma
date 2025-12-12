#include "scenes.h"
#include <SDL2/SDL.h>
#include "../physics/physics.h"
#include "../graphics/texture.h"
#include "player.h"
#include "objects.h"
#include "../window/events.h"
#include "../graphics/renderer.h"
#include "../window/windowManager.h"
#include "../audio/audio.h"
#include <SDL2/SDL_mouse.h>
#include <cglm/vec3.h>
#include <stdbool.h>
#include <string.h>

extern Scene* currentScene;
extern Scene titleScreen;

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

void gtmaInitScene(Scene* scene, Player* player, GameObjectPack* objectPack, ScreenObjectPack* screenObjPack, vec3 spawnpoint) {
    scene->player = player;
    scene->screenPack = screenObjPack;
    scene->objPack = objectPack;

    gtmaCreateScreenObject(&crosshair, "images/crosshair.png", "uitest", (vec2){((float)getWindowWidth() / 2), ((float)getWindowHeight() / 2)}, (vec2){10, 10}, 0, GTMA_FLAG_NONE);
    gtmaCreateTexture(&regularCrosshair, "images/crosshair.png");
    gtmaCreateTexture(&highlightedCrosshair, "images/crosshairselected.png");

    gtmaCreateScreenObject(&pauseScreen, "images/paused.png", "pause", (vec2){((float)getWindowWidth()/2), ((float)getWindowHeight()/2)}, (vec2){400, 80}, 0, GTMA_FLAG_INVISIBLE);

    gtmaCreateScreenObject(&loadingScreen, "images/loading.png", "loading", (vec2){(float)getWindowWidth()/2, (float)getWindowHeight() / 2}, (vec2){400, 60}, 0, GTMA_FLAG_INVISIBLE);

    gtmaAddScreenObject(&crosshair, scene->screenPack);
    gtmaAddScreenObject(&loadingScreen, scene->screenPack);
    gtmaAddScreenObject(&pauseScreen, scene->screenPack);

    gtmaCreateSound(&beep, "audio/beep.wav", false, 1, player->position);
    gtmaStopSound(&beep);

    gtmaCreateSound(&doorSound, "audio/door.wav", false, 1, player->position);
    gtmaStopSound(&doorSound);

    glm_vec3_copy(spawnpoint, scene->spawnpoint);
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
    loadingScreen.flags &= ~GTMA_FLAG_INVISIBLE;
    nextScene = newScene;
}

bool checkWarp() {
    if (transitioning) {
        transitionTimer -= getDeltaTime();
        if (transitionTimer <= 0.0f) {
            transitioning = false;
            loadingScreen.flags &= ~GTMA_FLAG_INVISIBLE;
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

bool gtmaUpdateScene(Scene* scene, Player* player) {

    if (paused) {
        if (isKeyPressed(SDL_SCANCODE_ESCAPE)) {
            paused = false;
            pauseScreen.flags |= GTMA_FLAG_INVISIBLE;
            SDL_SetRelativeMouseMode(true);
            gtmaSetFBOBrightness(1);
        }
        if (isKeyPressed(SDL_SCANCODE_Q)) {
            gtmaSetRunning(false);
        }
        if (isKeyPressed(SDL_SCANCODE_T)) {
            paused = false;
            pauseScreen.flags &= ~GTMA_FLAG_INVISIBLE;
            SDL_SetRelativeMouseMode(true);
            gtmaSetFBOBrightness(1);
            switchScene(&titleScreen);
        }
    } else {
        if (isKeyPressed(SDL_SCANCODE_ESCAPE)) {
            pauseScreen.flags &= ~GTMA_FLAG_INVISIBLE;
            SDL_SetRelativeMouseMode(false);
            gtmaSetFBOBrightness(0.5);
            paused = true;
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

    bool isHighlighted = strcmp(pickObject(scene->objPack, player->camera), "none") != 0;

    if (isHighlighted != lastState) {
        if (isHighlighted) {
            crosshair.model.meshes[0].texture.id = highlightedCrosshair.id;
        } else {
            crosshair.model.meshes[0].texture.id = regularCrosshair.id;
        }
        lastState = isHighlighted;
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
