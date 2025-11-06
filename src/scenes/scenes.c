#include "scenes.h"
#include <SDL2/SDL.h>
#include "../physics/physics.h"
#include "player.h"
#include "objects.h"
#include "../window/events.h"
#include "../graphics/renderer.h"
#include "../window/windowManager.h"
#include <SDL2/SDL_mouse.h>
#include <cglm/vec3.h>
#include <stdbool.h>

extern Scene* currentScene;
extern Scene titleScreen;

static ScreenObject crosshair;
static ScreenObject loadingScreen;
static ScreenObject pauseScreen;

bool paused = false;

int editMode = 1;
int objIndex = 0;
int lightIndex = 0;

void gtmaInitScene(Scene* scene, Player* player, GameObjectPack* objectPack, ScreenObjectPack* screenObjPack, vec3 spawnpoint) {
    scene->player = player;
    scene->screenPack = screenObjPack;
    scene->objPack = objectPack;

    gtmaCreateScreenObject(&crosshair, "models/uitest.glb", "uitest", (vec2){((float)getWindowWidth() / 2), ((float)getWindowHeight() / 2)}, (vec2){8, 8}, 0);
    gtmaChangeScreenObjectTexture(&crosshair, "images/crosshair.png");

    gtmaCreateScreenObject(&pauseScreen, "models/uitest.glb", "pause", (vec2){((float)getWindowWidth()/2), ((float)getWindowHeight()/2)}, (vec2){400, 80}, 0);
    gtmaChangeScreenObjectTexture(&pauseScreen, "images/paused.png");
    pauseScreen.visible = false;

    gtmaCreateScreenObject(&loadingScreen, "models/uitest.glb", "loading", (vec2){(float)getWindowWidth()/2, (float)getWindowHeight() / 2}, (vec2){400, 60}, 0);
    gtmaChangeScreenObjectTexture(&loadingScreen, "images/loading.png");
    loadingScreen.visible = false;

    gtmaAddScreenObject(&crosshair, scene->screenPack);
    gtmaAddScreenObject(&loadingScreen, scene->screenPack);
    gtmaAddScreenObject(&pauseScreen, scene->screenPack);

    glm_vec3_copy(spawnpoint, scene->spawnpoint);
}

void gtmaSetEditMode(int set) {
    editMode = set;
}


static float transitionTimer = 0.0f;
static float transitionDuration = 1.0f; // seconds
static bool transitioning = false;
Scene* nextScene;

void switchScene(Scene* newScene) {
    transitioning = true;
    transitionTimer = transitionDuration;
    gtmaToggleControls(false);
    loadingScreen.visible = true;
    nextScene = newScene;
}

bool checkWarp() {
    if (transitioning) {
        transitionTimer -= getDeltaTime();
        if (transitionTimer <= 0.0f) {
            transitioning = false;
            loadingScreen.visible = false;
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

bool gtmaUpdateScene(Scene* scene, Player* player) {

    if (paused) {
        if (isKeyPressed(SDL_SCANCODE_ESCAPE)) {
            paused = false;
            pauseScreen.visible = false;
            SDL_SetRelativeMouseMode(true);
            gtmaSetFBOBrightness(1);
        }
        if (isKeyPressed(SDL_SCANCODE_Q)) {
            gtmaSetRunning(false);
        }
        if (isKeyPressed(SDL_SCANCODE_T)) {
            paused = false;
            pauseScreen.visible = false;
            SDL_SetRelativeMouseMode(true);
            gtmaSetFBOBrightness(1);
            switchScene(&titleScreen);
        }
    } else {
        if (isKeyPressed(SDL_SCANCODE_ESCAPE)) {
            pauseScreen.visible = true;
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
