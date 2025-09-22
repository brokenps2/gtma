#include "scenes/scenes.h"
#include <SDL2/SDL.h>
#include "scenes/objects.h"
#include "window/events.h"
#include "graphics/renderer.h"
#include <stdbool.h>

extern Scene* currentScene;
extern Scene titleScreen;

bool paused = false;;

bool checkPaused(ScreenObject* pauseScreen) {
    if(paused) {
        if(isKeyPressed(SDL_SCANCODE_ESCAPE)) {
            paused = false;
            pauseScreen->visible = false;
            SDL_SetRelativeMouseMode(true);
            gtmaSetFBOBrightness(1);
        }
        if(isKeyPressed(SDL_SCANCODE_Q)) {
            gtmaSetRunning(false);
        }
        if(isKeyPressed(SDL_SCANCODE_T)) {
            paused = false;
            pauseScreen->visible = false;
            SDL_SetRelativeMouseMode(true);
            gtmaSetFBOBrightness(1);
            switchScene(&titleScreen);
        }
        return paused;
    } else {
        if(isKeyPressed(SDL_SCANCODE_ESCAPE)) {
            pauseScreen->visible = true;
            SDL_SetRelativeMouseMode(false);
            gtmaSetFBOBrightness(0.5);
            paused = true;
        }
        return paused;
    }
}

void switchScene(Scene* newScene) {
    if (currentScene && currentScene->dispose)
        currentScene->dispose();
    currentScene = newScene;
    if (currentScene && currentScene->init)
        currentScene->init();
}
