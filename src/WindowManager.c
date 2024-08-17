#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "Config.h"
#include "Renderer.h"
#include "Input.h"

GLFWwindow* window;
float currentTime = 0;
float lastTime = 0;
float deltaTime;

int posX, posY;

void initWindow() {

    if (!glfwInit()) {
        printf("GLFW init failed\n");
        exit(1);
    }

    const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    posX = (videoMode->width / 2) - (cfgGetResX() / 2);
    posY = (videoMode->height / 2) - (cfgGetResY() / 2);
 
    const char* ctitle = cfgGetTitle();

    window = glfwCreateWindow(cfgGetResX(), cfgGetResY(), ctitle, NULL, NULL);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwMakeContextCurrent(window);
    glfwSetWindowPos(window, posX, posY);
    //glfwSwapInterval(0);
    
    #ifdef linux

    #endif

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

void updateWindow() {

    deltaTime = glfwGetTime() - lastTime;
    lastTime = glfwGetTime();
    printf("\rFPS: %f      %f", 1/deltaTime, deltaTime);
    fflush(stdout);

    if(isKeyPressed(GLFW_KEY_ESCAPE) && glfwGetInputMode(getWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if(isLeftPressed() && glfwGetInputMode(getWindow(), GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
        glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    render();
    glfwSwapBuffers(window);
    
}

