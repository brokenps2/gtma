#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "WindowManager.h"

double mouseX;
double mouseY;

bool keys[GLFW_KEY_LAST];
bool keysPressed[GLFW_KEY_LAST];

int initialized = 0;

double getMouseX() {
    if(initialized == 0) {
        printf("Mouse used without being initialized\n");
        exit(1);
    } else {
        return mouseX;
    }
}

double getMouseY() {
    if(initialized == 0) {
        printf("err: Mouse used without being initialized\n");
        exit(1);
    } else {
        return mouseY;
    }
}

void cursorCallback(GLFWwindow* window, double xpos, double ypos) {
    mouseX = xpos;
    mouseY = ypos;
}

bool isKeyDown(int keycode){
    if (keycode < 0 || keycode >= GLFW_KEY_LAST) {
        return false;
    }
    return keys[keycode];
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key < 0 || key >= GLFW_KEY_LAST) {
        return;
    }

    if (action == GLFW_PRESS) {
        if (!keys[key]) {
            keysPressed[key] = true;
        }
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }

}


void initInput() {
    glfwSetKeyCallback(getWindow(), keyCallback);
    glfwSetCursorPosCallback(getWindow(), cursorCallback);
    initialized = 1;
}
