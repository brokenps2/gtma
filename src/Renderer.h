#pragma once
#include "Camera.h"
#include "Models.h"
#include "Shader.h"

void initRenderer();
void setCamera(Camera* cam);
void addObject(Object* obj);
void addLight(PointLight* light);
void removeObject(Object* obj);
void removeLight(PointLight* light);
Shader* getShader();
void render();
