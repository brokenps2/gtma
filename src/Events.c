#include "Events.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_mouse.h>
#include <stdbool.h>
#include "Config.h"
#include "WindowManager.h"

double mouseX;
double mouseY;

double mouseToYaw;

int windowPosX = 800, windowPosY = 200;
int windowSizeX = 800, windowSizeY = 600;
bool dimensionsUpdated = false;

bool mouse1;
bool mouse2;

bool running = true;

bool keys[SDL_NUM_SCANCODES];
bool keysPressed[SDL_NUM_SCANCODES];

double getMouseX() {
    return mouseX;
}

double getMouseY() {
    return mouseY;
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

bool isRightPressed() {
    bool pressed = mouse2;
    mouse2 = false;
    return pressed;
}

int getWindowWidth() {
    if(!dimensionsUpdated) {
        return cfgGetResX();
    }
    return windowSizeX;
}

int getWindowHeight() {
    if(!dimensionsUpdated) {
        return cfgGetResY();
    }
    return windowSizeY;
}

int getWindowPosX() {
    return windowPosX;
}

int getWindowPosY() {
    return windowPosY;
}

bool isKeyDown(int keycode) {
    if (keycode < 0 || keycode >= SDL_NUM_SCANCODES) {
        return false;
    }
    return keys[keycode];
}

bool isKeyPressed(int keycode) {
    if (keycode < 0 || keycode >= SDL_NUM_SCANCODES) {
        return false;
    }
    bool pressed = keysPressed[keycode];
    keysPressed[keycode] = false;
    return pressed;
}

void keyDownCallback(SDL_KeyboardEvent keyEvent) {
    if (keyEvent.keysym.scancode < 0 || keyEvent.keysym.scancode >= SDL_NUM_SCANCODES) {
        return;
    }
    if (!keys[keyEvent.keysym.scancode]) {
        keysPressed[keyEvent.keysym.scancode] = true;
    }
    keys[keyEvent.keysym.scancode] = true;
}

void keyUpCallback(SDL_KeyboardEvent keyEvent) {
    if (keyEvent.keysym.scancode < 0 || keyEvent.keysym.scancode >= SDL_NUM_SCANCODES) {
        return;
    }
    keys[keyEvent.keysym.scancode] = false;
}

void mouseDownCallback(SDL_MouseButtonEvent mouseEvent) {
    if (mouseEvent.button == SDL_BUTTON_LEFT) {
        mouse1 = true;
    }
    if (mouseEvent.button == SDL_BUTTON_RIGHT) {
        mouse2 = true;
    }
}

void mouseUpCallback(SDL_MouseButtonEvent mouseEvent) {
    if (mouseEvent.button == SDL_BUTTON_LEFT) {
        mouse1 = false;
    }
    if (mouseEvent.button == SDL_BUTTON_RIGHT) {
        mouse2 = false;
    }
}

void cursorCallback(int xpos, int ypos) {
    int dx, dy;
    SDL_GetRelativeMouseState(&dx, &dy);
    mouseX += dx;
    mouseY += dy;
}

void windowResizeCallback(int width, int height) {
    if(dimensionsUpdated != true) {
        dimensionsUpdated = true;
    }
    windowSizeX = width;
    windowSizeY = height;
    glViewport(0, 0, windowSizeX, windowSizeY);
}

void windowMoveCallback(int xpos, int ypos) {
    windowPosX = xpos;
    windowPosY = ypos;
}

void gtmaInitInput() {
    windowSizeX = cfgGetResX();
    windowSizeY = cfgGetResY();
}

bool gtmaIsRunning() {
    return running;
}

void gtmaUpdateEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                    windowResizeCallback(e.window.data1, e.window.data2);
                }
                if (e.window.event == SDL_WINDOWEVENT_MOVED) {
                    windowMoveCallback(e.window.data1, e.window.data2);
                }
                break;
            case SDL_KEYDOWN:
                keyDownCallback(e.key);
                break;
            case SDL_KEYUP:
                keyUpCallback(e.key);
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouseDownCallback(e.button);
                break;
            case SDL_MOUSEBUTTONUP:
                mouseUpCallback(e.button);
                break;
            case SDL_MOUSEMOTION:
                cursorCallback(e.motion.x, e.motion.y);
                break;
        }
    }
}
