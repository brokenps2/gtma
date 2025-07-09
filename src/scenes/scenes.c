#include "scenes/scenes.h"

extern Scene* currentScene;

void switchScene(Scene* newScene) {
    if (currentScene && currentScene->dispose)
        currentScene->dispose();
    currentScene = newScene;
    if (currentScene && currentScene->init)
        currentScene->init();
}
