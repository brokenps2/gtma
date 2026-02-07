#pragma once
#include <stdbool.h>
#include "../objects/objects.h"
#include "../physics/physics.h"
#include "graphics/shader.h"

typedef struct Scene {
    void (*init)();
    void (*update)();
    void (*dispose)();
    vec3 spawnpoint;
    Player* player;
    ScreenObjectPack* screenPack;
    GameObjectPack* objPack;
    EntityPack* entPack;
    bool debugInfo;
} Scene;


void gtmaLoadInitialScene(Scene* scene);
Scene* gtmaGetCurrentScene();
void gtmaInitScene(Scene* scene, Player* player, GameObjectPack* objectPack, ScreenObjectPack* screenObjPack, EntityPack* entityPack, vec3 spawnpoint, bool debug);
void gtmaToggleCrosshair(Scene* scene, bool toggle);
bool gtmaUpdateScene(Scene* scene, Player* player);
void gtmaPlayDoorSound();
void gtmaBeep();
void gtmaSpawnLightGrid(PointLightPack* pack, float brightness, int lightCount, float maxPos, float minPos);
void switchScene(Scene* newScene);
