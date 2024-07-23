#include <stdio.h>
#include <stdlib.h>
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "Audio.h"
#include "Scene.h"
#include "WindowManager.h"
#include "Input.h"
#include "Config.h"

int main(int argc, char* argv[]) {

    if(argc < 2 || argc > 2) {
        printf("Usage: gtma [config file path]\n");
        printf("this will be handled by the launcher eventually\n");
        exit(1);
    }

    printf("\n");

    cfgSetPath(argv[1]);

    initConfig();
    initWindow();
    initInput();
    initAudio();
    initRenderer();
    initScene();

    while(!glfwWindowShouldClose(getWindow())) {
        render();
        updateScene();
        updateWindow();
        glfwPollEvents();
    }

    glfwTerminate();
    printf("\n");
    return 0;
}
