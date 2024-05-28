#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "Config.h"
#include "Input.h"
#include "Math.h"

GLFWwindow* window;
float currentTime = 0;
float lastTime;
float deltaTime;

int posX, posY;

void initWindow() {

    const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    posX = (videoMode->width / 2) - (cfgGetResX() / 2);
    posY = (videoMode->height / 2) - (cfgGetResY() / 2);
 
    const char* ctitle = cfgGetTitle();

    window = glfwCreateWindow(cfgGetResX(), cfgGetResY(), ctitle, NULL, NULL);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwMakeContextCurrent(window);
    glfwSetWindowPos(window, posX, posY);

    const GLenum err = glewInit();
    glewExperimental = GL_TRUE;
    if (GLEW_OK != err) {
        printf("GLEW Error: %s", glewGetErrorString(err));
        exit(1);
    }

    glViewport(0, 0, cfgGetResX(), cfgGetResY());
    glEnable(GL_DEPTH_TEST);
}

GLFWwindow* getWindow() {
    return window;
}

float getDeltaTime() {
    return deltaTime;
}

float getTime() {
    return currentTime;
}

void updateWindow() {

    if(isKeyPressed(GLFW_KEY_ESCAPE) && glfwGetInputMode(getWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if(isLeftPressed() && glfwGetInputMode(getWindow(), GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
        glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    lastTime = currentTime;
    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;

    glfwSwapBuffers(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(glc(9), glc(8), glc(22), 1);
}

