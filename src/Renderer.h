#pragma once
#include "Camera.h"
#include "Models.h"
#include "Shader.h"

void gtmaInitRenderer();
void gtmaSetRenderCamera(Camera* cam);
void gtmaAddGameObject(GameObject* obj);
GameObject* gtmaCreateAndAddGameObject(const char* path, const char* name, float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz);
void gtmaAddLight(PointLight* light);
void gtmaRemoveGameObject(GameObject* obj);
void gtmaRemoveLight(PointLight* light);
void gtmaCloseRenderer();
Shader* gtmaGetShader();
void gtmaRender();
GameObjectPack* getObjPack();
PointLightPack* getLightPack();
int getFrameWidth();
int getFrameHeight();
void gtmaSetClearColor(float r, float g, float b);
void gtmaSetFogLevel(float level);
