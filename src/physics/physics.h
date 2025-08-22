#pragma once

#include <stdbool.h>

typedef struct AABB {
	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;
} AABB;

typedef struct GameObjectPack GameObjectPack;
typedef struct Mesh Mesh;
typedef struct Camera Camera;

void calculateMeshAABB(Mesh* mesh, float objScale[3], float objPosition[3]);
bool updateCameraPhysics(GameObjectPack* objPack, Camera* cam);
