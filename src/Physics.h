#pragma once

#include <cglm/vec3.h>
#include "Models.h"
#include "Camera.h"

typedef struct AABB {
    vec3 min;
    vec3 max;
} AABB;

AABB calculateAABB(Object* obj);
bool checkAABBCollision(AABB* a, AABB* b);
bool checkMeshCollision(Object* objA, Object* objB);
bool handleCameraPhysics(Camera* camera, ObjectPack* pack, float boundingBoxSize);
bool handleVecBoxPhysics(vec3* position, ObjectPack* pack, float boundingBoxSize);
