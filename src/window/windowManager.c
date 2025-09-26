#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <unistd.h>
#include "util/config.h"
#include "graphics/renderer.h"
#include "window/events.h"

int fullscreen = 0;

SDL_Window* window;
float currentTime = 0;
float lastTime = 0;
float deltaTime;

float monitorWidth, monitorHeight;

double frameTime;

bool mouseCaptured = true;

int posX, posY;

void gtmaInitWindow() {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL init failed\n");
        exit(1);
    }
    
    frameTime = 1.0f / 144.0f;
 
    const char* title = cfgLookupString("title");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, cfgLookupInt("width"), cfgLookupInt("height"), SDL_WINDOW_OPENGL);

    SDL_SetWindowResizable(window, true);

    SDL_GL_CreateContext(window);
    
    const GLenum err = glewInit();
    glewExperimental = GL_TRUE;
    if (GLEW_OK != err) {
        printf("GLEW Error: %s", glewGetErrorString(err));
        exit(1);
    }

    glViewport(0, 0, cfgLookupInt("width"), cfgLookupInt("height"));
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    SDL_GL_SetSwapInterval(1);
}

SDL_Window* getWindow() {
    return window;
}

float getDeltaTime() {
    return deltaTime;
}

float getTime() {
    return currentTime;
}

bool isMouseCaptured() {
    return mouseCaptured;
}

void gtmaUpdateWindow() {
    double currentTime = (double)SDL_GetTicks() / 1000;
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    /*
    if(isKeyPressed(GLFW_KEY_ESCAPE) && glfwGetInputMode(getWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    */

    if(isKeyPressed(SDL_SCANCODE_ESCAPE)) {
        SDL_SetRelativeMouseMode(false);
        mouseCaptured = false;
    }

    if(!mouseCaptured) {
        if(isLeftPressed()) {
            SDL_SetRelativeMouseMode(true);
            mouseCaptured = true;
        }
    }

    gtmaRender();

    double timeToSleep = frameTime - (((double)SDL_GetTicks() / 1000) - lastTime);

    if (timeToSleep > 0) {
        usleep(timeToSleep * 1e6);
    }

    //printf("\r%lf", 1 / deltaTime);
    //fflush(stdout);

    SDL_GL_SwapWindow(window);

}

void gtmaCloseWindow() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}
