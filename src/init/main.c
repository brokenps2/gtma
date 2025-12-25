#include <stdio.h>
#include <stdlib.h>
#include "../graphics/renderer.h"
#include <stdbool.h>
#include "../audio/audio.h"
#include "../scenes/scenes.h"
#include "../window/windowManager.h"
#include "../window/events.h"
#include "../util/config.h"
#include "../util/diagnostics.h"
#include <time.h>
#include <unistd.h>

Scene* currentScene = NULL;

int main(int argc, char* argv[]) {

    extern Scene titleScreen;
    currentScene = &titleScreen;

    srand(time(NULL));

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
    gtmaPrintSystemInfo();

    printf("\n");

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
