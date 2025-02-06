#pragma once
#include "Camera.h"
#include "Models.h"
#include "Shader.h"

void gtmaInitRenderer();
void gtmaSetRenderCamera(Camera* cam);
void gtmaAddObject(Object* obj);
Object* gtmaCreateAndAddObject(const char* path, const char* name, float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz);
void gtmaAddLight(PointLight* light);
void gtmaRemoveObject(Object* obj);
void gtmaRemoveLight(PointLight* light);
void gtmaCloseRenderer();
Shader* gtmaGetShader();
void gtmaRender();
ObjectPack* getObjPack();
int getFrameWidth();
int getFrameHeight();
void gtmaSetClearColor(float r, float g, float b);
void gtmaSetFogLevel(float level);
