#include <stdbool.h>
#pragma once

typedef struct SDL_Window SDL_Window;

double getMouseX();
double getMouseY();
bool isLeftDown();
bool isLeftPressed();
bool isRightDown();
int getWindowWidth();
int getWindowHeight();
int getWindowPosX();
int getWindowPosY();
bool isKeyDown(int keycode);
bool isKeyPressed(int keycode);
void gtmaSetRunning(bool run);
bool gtmaIsRunning();
void gtmaUpdateEvents();
void gtmaToggleControls(bool onoff);
void gtmaClearKeyInputs();
void gtmaEndInputFrame();

void gtmaInitWindow();
void gtmaUpdateWindow();
float getTime();
float getDeltaTime();
SDL_Window* getWindow();
void gtmaCloseWindow();
bool isMouseCaptured();
float gtmaGetFramerate();
