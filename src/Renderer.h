#pragma once
#include "Camera.h"
#include "Models.h"
#include "Shader.h"

void gtmaInitRenderer();
void gtmaSetRenderCamera(Camera* cam);
void gtmaAddObject(Object* obj);
void gtmaAddLight(PointLight* light);
void gtmaRemoveObject(Object* obj);
void gtmaRemoveLight(PointLight* light);
void gtmaCloseRenderer();
Shader* gtmaGetShader();
void gtmaRender();
