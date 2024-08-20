#pragma once
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

void initUI();
void updateUI();
ImGuiIO* getIOPtr();
void disposeUI();
