#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "WindowManager.h"
#include "Renderer.h"
#include "Input.h"
#include "Config.h"
#include "Files.h"

Model* model;

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
    initRenderer();
    initInput();

    createModel(model, "untitled.obj");
 
    //initial setup finished, run start tasks here

    while(!glfwWindowShouldClose(getWindow())) {
        updateWindow();

        render();

        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
