#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>
#include "Config.h"
#include "Math.h"

GLFWwindow* window;
float currentTime = 0;
float lastTime;
float deltaTime;

void initWindow() {

    glfwInit();
 
    std::string title = cfgGetTitle();
    const char* ctitle = title.c_str();

    window = glfwCreateWindow(cfgGetResX(), cfgGetResY(), ctitle, NULL, NULL);
    if(window == NULL) {
        std::cerr << "Window create failed" << std::endl;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwMakeContextCurrent(window);
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, false);
    glfwSetWindowPos(window, 800, 200);

    const GLenum err = glewInit();
    glewExperimental = GL_TRUE;
    if (GLEW_OK != err) {
        std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
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
    glClearColor(glc(0), glc(0), glc(0), 1);
}

