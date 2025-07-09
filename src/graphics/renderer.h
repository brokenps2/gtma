#pragma once
#include "graphics/camera.h"
#include "graphics/shader.h"
#include "scenes/objects.h"

void gtmaInitRenderer();
void gtmaSetRenderCamera(Camera* cam);
void gtmaCloseRenderer();
Shader* gtmaGetShader();
void gtmaRender();
int getFrameWidth();
int getFrameHeight();
void gtmaLoadGameObjectPack(GameObjectPack* pack);
void gtmaLoadPointLightPack(PointLightPack* pack);
void gtmaSetClearColor(float r, float g, float b);
void gtmaSetFogLevel(float level);
