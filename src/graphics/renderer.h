#pragma once
#include "camera.h"
#include "shader.h"
#include "../scenes/objects.h"

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
void gtmaSetAmbientLightLevel(float ambient);
void gtmaSetFBOBrightness(float level);
void gtmaSetFogLevel(float level);
void gtmaRenderFBO();
void gtmaSetFBOImage(unsigned char** data);
void gtmaLoadScreenObjectPack(ScreenObjectPack* pack);
