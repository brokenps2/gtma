#pragma once
#include <stdbool.h>
#include "objects.h"
#include "../physics/physics.h"

typedef struct Scene {
    void (*init)();
    void (*update)();
    void (*dispose)();
    vec3 spawnpoint;
    Player* player;
    ScreenObjectPack* screenPack;
    GameObjectPack* objPack;
} Scene;


void gtmaInitScene(Scene* scene, Player* player, GameObjectPack* objectPack, ScreenObjectPack* screenObjPack, vec3 spawnpoint);
void gtmaToggleCrosshair(Scene* scene, bool toggle);
bool gtmaUpdateScene(Scene* scene, Player* player);
void gtmaSetEditMode(int set);
void switchScene(Scene* newScene);
