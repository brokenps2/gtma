#pragma once
#include "../graphics/camera.h"
#include "../physics/physics.h"
#include "objects.h"

typedef struct Player {
    Camera* camera;
    vec3 position;
    int health;
    AABB aabb;
    GameObject** currentCollisions;
    int currentCollisionCount;
} Player;

void gtmaCreatePlayer(Player* player, Camera *camera, int health, float collisionRadius, float collisionLength);
void gtmaSetPlayerFallingSpeed(float speed);
void gtmaPlayerMove(Player* player, GameObjectPack* objPack, bool flying);
