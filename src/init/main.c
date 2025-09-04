#include <stdio.h>
#include <stdlib.h>
#include "graphics/renderer.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "audio/audio.h"
#include "scenes/scenes.h"
#include "window/windowManager.h"
#include "window/events.h"
#include "util/config.h"

Scene* currentScene = NULL;

int main(int argc, char* argv[]) {

    extern Scene outdoorScene;
    currentScene = &outdoorScene;

    if(argc < 2 || argc > 2) {
        printf("Usage: gtma [config file path]\n");
        printf("this will be handled by the launcher eventually\n");
        exit(1);
    }

    printf("\n");

    gtmaInitConfig(argv[1]);
    gtmaInitWindow();
    gtmaInitAudio();
    gtmaInitRenderer();
    currentScene->init();

    while(gtmaIsRunning()) {
        gtmaUpdateEvents();
        currentScene->update();
        gtmaUpdateWindow();
    }

    currentScene->dispose();
    gtmaCloseRenderer();
    gtmaCloseAudio();
    gtmaDestroyConfig();
    gtmaCloseWindow();

    printf("\n");
    return 0;
}
