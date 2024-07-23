#pragma once
#include "Camera.h"
#include "Models.h"
#include "Shader.h"

void initRenderer();
void setCamera(Camera* cam);
void addObject(Object* obj);
void removeObject(Object* obj);
Shader* getShader();
void render();
