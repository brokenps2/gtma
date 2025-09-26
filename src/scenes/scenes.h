#pragma once
#include <stdbool.h>
#include "scenes/objects.h"
#include "graphics/shader.h"

typedef struct Scene {
    void (*init)();
    void (*update)();
    void (*dispose)();
} Scene;

bool gtmaCheckPauseAndSelect(ScreenObject* pauseScreen, GameObjectPack* objPack, PointLightPack* lightPack);
void gtmaSetEditMode(int set);
void switchScene(Scene* newScene);
