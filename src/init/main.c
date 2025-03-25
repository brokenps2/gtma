#include <stdio.h>
#include <stdlib.h>
#include "graphics/renderer.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "audio/audio.h"
#include "scene/scene.h"
#include "window/windowManager.h"
#include "window/events.h"
#include "util/config.h"

int main(int argc, char* argv[]) {

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
    initScene();

    while(gtmaIsRunning()) {
        gtmaUpdateEvents();
        updateScene();
        gtmaUpdateWindow();
    }

    gtmaCloseRenderer();
    gtmaCloseAudio();
    disposeScene();

    printf("\n");
    return 0;
}
