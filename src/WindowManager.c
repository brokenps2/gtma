#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <unistd.h>
#include "Config.h"
#include "Renderer.h"
#include "Input.h"

GLFWwindow* window;
float currentTime = 0;
float lastTime = 0;
float deltaTime;

double frameTime;

int posX, posY;

void gtmaInitWindow() {

    if (!glfwInit()) {
        printf("GLFW init failed\n");
        exit(1);
    }

    const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    posX = (videoMode->width / 2) - (cfgGetResX() / 2);
    posY = (videoMode->height / 2) - (cfgGetResY() / 2);
    
    frameTime = 1.0f / 144.0f;
 
    const char* ctitle = cfgGetTitle();

    window = glfwCreateWindow(cfgGetResX(), cfgGetResY(), ctitle, NULL, NULL);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
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
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
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

void gtmaUpdateWindow() {
    double currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    glfwSwapBuffers(window);

    if(isKeyPressed(GLFW_KEY_ESCAPE) && glfwGetInputMode(getWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    gtmaRender();

    double timeToSleep = frameTime - (glfwGetTime() - lastTime);

    if (timeToSleep > 0) {
        usleep(timeToSleep * 1e6);
    }
}
