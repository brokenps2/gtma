#pragma once
#include <stdbool.h>
#include <cglm/vec3.h>

typedef struct AABB {
	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;
} AABB;

typedef struct GameObjectPack GameObjectPack;
typedef struct Mesh Mesh;
typedef struct Camera Camera;
typedef struct Player Player;

void calculateMeshAABB(Mesh* mesh, float objScale[3], float objPosition[3]);
bool updatePlayerPhysics(GameObjectPack* objPack, Player* player);
const char* pickObject(GameObjectPack* pack, Camera* cam);

