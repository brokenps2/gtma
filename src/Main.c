#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "WindowManager.h"
#include "Renderer.h"
#include "Input.h"
#include "Config.h"

int main(int argc, char* argv[]) {

    if (!glfwInit()) {
        printf("GLFW Init Failed!\n");
    }

    if(argc < 2 || argc > 2) {
        printf("Usage: gtma [config file path]\n");
        printf("this will be handled by the launcher eventually\n");
        exit(1);
    }

    cfgSetPath(argv[1]);

    initConfig();
    initWindow();
    initInput();
    initRenderer();

    while(!glfwWindowShouldClose(getWindow())) {
        updateWindow();

        render();

        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
