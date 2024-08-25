#pragma once

typedef struct GLFWwindow GLFWwindow;

void gtmaInitWindow();
void gtmaUpdateWindow();
float getTime();
float getDeltaTime();
GLFWwindow* getWindow();
