#pragma once
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

void gtmaInitUI();
void gtmaUpdateUI();
ImGuiIO* getIOPtr();
void gtmaCloseUI();
