#include <stdio.h>
#include <stdlib.h>
#include "Interface.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "Audio.h"
#include "Scene.h"
#include "WindowManager.h"
#include "Events.h"
#include "Config.h"

int main(int argc, char* argv[]) {

    if(argc < 2 || argc > 2) {
        printf("Usage: gtma [config file path]\n");
        printf("this will be handled by the launcher eventually\n");
        exit(1);
    }

    printf("\n");

    cfgSetPath(argv[1]);

    gtmaInitConfig();
    gtmaInitWindow();
    gtmaInitAudio();
    gtmaInitRenderer();
    initNuklear();
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
