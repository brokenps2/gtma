#include "events.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_mouse.h>
#include <stdbool.h>
#include "../util/config.h"

double mouseX;
double mouseY;

int windowPosX = 800, windowPosY = 200;
int windowSizeX = 1280, windowSizeY = 960;
bool dimensionsUpdated = false;

bool mouse1, mouse1Pressed;
bool mouse2, mouse2Pressed;

bool running = true;

bool keys[SDL_NUM_SCANCODES];
bool keysPressed[SDL_NUM_SCANCODES];

static bool useFixedBorder = false;

int blockAllEvents(void* userdata, SDL_Event* event) {
    return 0; // block everything
}

void blockInput() {
    SDL_SetEventFilter(blockAllEvents, NULL);
}

void unblockInput() {
    SDL_SetEventFilter(NULL, NULL); // restore default
}

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
    return mouse1Pressed;
}

bool isRightDown() {
    return mouse2;
}

bool isRightPressed() {
    return mouse2Pressed;
}

int getWindowWidth() {
    if(!dimensionsUpdated) {
        return cfgLookupInt("width");
    }
    return windowSizeX;
}

int getWindowHeight() {
    if(!dimensionsUpdated) {
        return cfgLookupInt("height");
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
        mouse1Pressed = true;
    }
    if (mouseEvent.button == SDL_BUTTON_RIGHT) {
        mouse2 = true;
        mouse2Pressed = true;
    }
}

void mouseUpCallback(SDL_MouseButtonEvent mouseEvent) {
    if (mouseEvent.button == SDL_BUTTON_LEFT) {
        mouse1 = false;
        mouse1Pressed = false;
    }
    if (mouseEvent.button == SDL_BUTTON_RIGHT) {
        mouse2 = false;
        mouse1Pressed = false;
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
    useFixedBorder = cfgLookupBool("useFixedBorder");
    windowSizeX = cfgLookupInt("width");
    windowSizeY = cfgLookupInt("height");
}

bool gtmaIsRunning() {
    return running;
}

void gtmaSetRunning(bool run) {
    running = run;
}

bool controlsEnabled = true;

void gtmaClearKeyInputs() {
    for(int i = 0; i < SDL_NUM_SCANCODES; i++) {
        keys[i] = false;
        keysPressed[i] = false;
    }
}

void gtmaToggleControls(bool onoff) {
    if(onoff == false) SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    controlsEnabled = onoff;
    gtmaClearKeyInputs();
}

void gtmaEndInputFrame() {
    mouse1Pressed = false;
    mouse2Pressed = false;
}

void gtmaUpdateEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        
        if(!controlsEnabled) {
            SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
            continue;
        }

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
