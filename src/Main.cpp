#include <iostream>
#include <string>
#include <GLFW/glfw3.h>
#include "WindowManager.h"
#include "Renderer.h"
#include "Input.h"
#include "Config.h"

int main(int argc, char* argv[]) {

    if(argc < 2 || argc > 2) {
        std::cout << "Usage: gtma [config file path]" << std::endl;
        std::cout << "this will be handled by the launcher eventually" << std::endl;
        exit(1);
    }

    cfgInitValues(argv[1]);

    initWindow();
    initRenderer();
    initInput();
 

    while(!glfwWindowShouldClose(getWindow())) {
        updateWindow();

        render();

        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
