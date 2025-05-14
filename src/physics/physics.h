#pragma once
#include <cglm/vec3.h>
#include "graphics/models.h"

AABB calculateAABB(GameObject* obj);
bool checkAABBCollision(AABB* a, AABB* b);
bool checkMeshCollision(GameObject* objA, GameObject* objB);
bool handleCamPhysics(vec3* position, GameObjectPack* pack, float boundingBoxSizeXZ, float boundingBoxSizeY);
