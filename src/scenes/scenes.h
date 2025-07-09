#pragma once

typedef struct Scene {
    void (*init)();
    void (*update)();
    void (*dispose)();
} Scene;

void switchScene(Scene* newScene);
