#pragma once
#include <stdbool.h>

typedef struct SDL_Window SDL_Window;

void gtmaInitWindow();
void gtmaUpdateWindow();
float getTime();
float getDeltaTime();
SDL_Window* getWindow();
void gtmaCloseWindow();
bool isMouseCaptured();
float gtmaGetFramerate();
