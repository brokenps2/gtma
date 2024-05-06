#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "Config.h"
#include "Math.h"

GLFWwindow* window;
float currentTime = 0;
float lastTime;
float deltaTime;

void initWindow() {
 
    const char* ctitle = cfgGetTitle();

    window = glfwCreateWindow(cfgGetResX(), cfgGetResY(), ctitle, NULL, NULL);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwMakeContextCurrent(window);
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, false);
    glfwSetWindowPos(window, 800, 200);

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

void updateWindow() {

    lastTime = currentTime;
    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;

    glfwSwapBuffers(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, (GLint)cfgGetResX(), (GLint)cfgGetResY());
    glClearColor(glc(0), glc(128), glc(128), 1);
}

