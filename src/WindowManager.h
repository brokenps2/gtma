#pragma once

typedef struct GLFWwindow GLFWwindow;

void initWindow();
void updateWindow();
float getTime();
float getDeltaTime();
GLFWwindow* getWindow();
