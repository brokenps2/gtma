#pragma once
#include "graphics/shader.h"
#include "scenes/objects.h"

typedef struct Level {
    GameObject mapObject;
    GameObjectPack objPack;
    ScreenObjectPack screenPack;
    PointLightPack lightPack;
    //init script
    //update script
    //dispose script
    vec3 cameraPos;
    vec3 clearColor;
} Level;
