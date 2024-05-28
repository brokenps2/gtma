#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "WindowManager.h"

double mouseX;
double mouseY;

int windowPosX = 800, windowPosY = 200;
int windowSizeX = 800, windowSizeY = 600;

bool mouse1;
bool mouse2;

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

bool isLeftDown() {
    return mouse1;
}

bool isLeftPressed() {
    bool pressed = mouse1;
    mouse1 = false;
    return pressed;
}

bool isRightDown() {
    return mouse2;
}

void cursorCallback(GLFWwindow* window, double xpos, double ypos) {
    mouseX = xpos;
    mouseY = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        mouse1 = true;
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        mouse2 = true;
    }
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mouse1 = false;
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        mouse2 = false;
    }
}

int getWindowWidth() {
    return windowSizeX;
}

int getWindowHeight() {
    return windowSizeY;
}

int getWindowPosX() {
    return windowPosX;
}

int getWindowPosY() {
    return windowPosY;
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
    windowSizeX = width;
    windowSizeY = height;
    //glViewport(windowPosX, windowPosY, width, height);
}

void windowMoveCallback(GLFWwindow* window, int xpos, int ypos) {
    windowPosX = xpos;
    windowPosY = ypos;
}


bool isKeyDown(int keycode){
    if (keycode < 0 || keycode >= GLFW_KEY_LAST) {
        return false;
    }
    return keys[keycode];
}

bool isKeyPressed(int keycode){
    if (keycode < 0 || keycode >= GLFW_KEY_LAST) {
        return false;
    }
    bool pressed = keysPressed[keycode];
    keysPressed[keycode] = false;
    return pressed;
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
    glfwSetMouseButtonCallback(getWindow(), mouseButtonCallback);
    glfwSetWindowSizeCallback(getWindow(), windowResizeCallback);
    glfwSetWindowPosCallback(getWindow(), windowMoveCallback);
    initialized = 1;
}
