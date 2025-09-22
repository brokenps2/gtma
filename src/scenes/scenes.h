#pragma once
#include <stdbool.h>
#include "scenes/objects.h"

typedef struct Scene {
    void (*init)();
    void (*update)();
    void (*dispose)();
} Scene;

bool checkPaused(ScreenObject* pauseScreen);
void switchScene(Scene* newScene);
