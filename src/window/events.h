#pragma once
#include <stdbool.h>

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
bool gtmaIsRunning();
void gtmaUpdateEvents();
