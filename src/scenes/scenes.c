#include "scenes/scenes.h"
#include <SDL2/SDL.h>
#include "graphics/camera.h"
#include "graphics/shader.h"
#include "physics/physics.h"
#include "scenes/objects.h"
#include "window/events.h"
#include "graphics/renderer.h"
#include <stdbool.h>

extern Scene* currentScene;
extern Scene titleScreen;

bool paused = false;

int editMode = 1;
int objIndex = 0;
int lightIndex = 0;

bool gtmaCheckPauseAndSelect(ScreenObject* pauseScreen, GameObjectPack* objPack, PointLightPack* lightPack) {
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


    if(editMode == 1) {
        gtmaSetOrthographic(true);
        objPack->objects[objIndex]->selected = true;
        if(isKeyPressed(SDL_SCANCODE_UP)) {
            objPack->objects[objIndex]->selected = false;
            if(objIndex != objPack->objectCount) {
                objIndex++;
            }
        } else if(isKeyPressed(SDL_SCANCODE_DOWN)) {
            objPack->objects[objIndex]->selected = false;
            if(objIndex != 0) {
                objIndex--;
            }
        }
    } else if(editMode == 2) {
        
    } else {
        gtmaSetOrthographic(false);
        objPack->objects[objIndex]->selected = false;

    }


}

void gtmaSetEditMode(int set) {
    editMode = set;
}

void switchScene(Scene* newScene) {
    if (currentScene && currentScene->dispose)
        currentScene->dispose();
    currentScene = newScene;
    if (currentScene && currentScene->init)
        currentScene->init();
}
